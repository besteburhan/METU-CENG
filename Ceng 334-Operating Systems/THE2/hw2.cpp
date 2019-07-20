#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>

extern "C"{
#include "writeOutput.h"
	#include "writeOutput.c"
}
using namespace std;

class MinerInput {
	public:
		unsigned int ID;
		unsigned int duration;
		unsigned int capacity;
	    OreType oreType;
	    unsigned int total_extract; 
	    unsigned int current_count; // initially 0
	    bool active;
	    bool created;
};

class TransporterInput {
	public:
	    unsigned int ID;
	    unsigned int duration;
	    OreType *carry; // what tranports? initially null
	    bool created;
};

class SmelterInput {
	public:
	    unsigned int ID;
	    unsigned int duration;
	    OreType oreType; // it can be iron or copper
	    unsigned int loading_capacity; //storage capacity
	    unsigned int waiting_ore_count; //initially 0
	    unsigned int total_produce; // initially 0
	    bool active;
	    bool created;
};

class FoundryInput {
	public:
	    unsigned int ID;
	    unsigned int duration;
	    unsigned int loading_capacity;
	    unsigned int waiting_iron; // initially 0
	    unsigned int waiting_coal; // initially 0
	    unsigned int total_produce; // initially 0
	    bool active;
	    bool created;
};

std::vector<MinerInput> minersVec;
std::vector<TransporterInput> transportersVec;
std::vector<FoundryInput> foundriesVec;
std::vector<SmelterInput> smeltersVec;

std::vector<sem_t*> minerWaitCanProduce;
std::vector<sem_t*> minerProduced;
std::vector<sem_t*> minerStopped;
sem_t *aMinerProduced;
sem_t * signalFromProducers;

std::vector<sem_t*> smelterMutex;
std::vector<sem_t*> smelterBarrier;
std::vector<sem_t*> smelterProduced;
std::vector<sem_t*> smelterStopped;

std::vector<sem_t*> foundryMutex;
std::vector<sem_t*> foundryBarrier;
std::vector<sem_t*> foundryProduced;
std::vector<sem_t*> foundryStopped;

int numberMiner, numberTransporter, numberSmelter, numberFoundry;



void *minerThreadRoutine(void *i){
	int minerInd = *((int*) i);
	minersVec[minerInd].created = true;
	MinerInfo minerInfo;
	FillMinerInfo(&minerInfo,(minersVec[minerInd]).ID, minersVec[minerInd].oreType , minersVec[minerInd].capacity , minersVec[minerInd].current_count);
	WriteOutput(&minerInfo,NULL,NULL,NULL,MINER_CREATED);
	while(minersVec[minerInd].total_extract != 0){
		sem_wait(minerWaitCanProduce[minerInd]);
		FillMinerInfo(&minerInfo,minersVec[minerInd].ID, minersVec[minerInd].oreType , minersVec[minerInd].capacity , minersVec[minerInd].current_count);
		WriteOutput(&minerInfo,NULL,NULL,NULL,MINER_STARTED);
		usleep(minersVec[minerInd].duration - (minersVec[minerInd].duration*0.01) + (rand()%(int)(minersVec[minerInd].duration*0.02)));
		sem_wait(minerProduced[minerInd]);
			minersVec[minerInd].current_count =  minersVec[minerInd].current_count +1;
			minersVec[minerInd].total_extract =  minersVec[minerInd].total_extract -1;
			if(minersVec[minerInd].total_extract == 0){
				sem_wait(minerStopped[minerInd]);
					minersVec[minerInd].active = false;
				sem_post(minerStopped[minerInd]);
			}
		sem_post(aMinerProduced);//inform transporter
		sem_post(minerProduced[minerInd]);
		FillMinerInfo(&minerInfo,minersVec[minerInd].ID, minersVec[minerInd].oreType , minersVec[minerInd].capacity , minersVec[minerInd].current_count);
		WriteOutput(&minerInfo,NULL,NULL,NULL,MINER_FINISHED);
		usleep(minersVec[minerInd].duration - (minersVec[minerInd].duration*0.01) + (rand()%(int)(minersVec[minerInd].duration*0.02)));

	}
	
	FillMinerInfo(&minerInfo,minersVec[minerInd].ID, minersVec[minerInd].oreType , minersVec[minerInd].capacity , minersVec[minerInd].current_count);
	WriteOutput(&minerInfo,NULL,NULL,NULL,MINER_STOPPED);
	return 0;

}
 
void *smelterThreadRoutine(void *i){
	int smelterInd = *((int*) i);
	smeltersVec[smelterInd].created = true;
	SmelterInfo smelterInfo;
	FillSmelterInfo(&smelterInfo,smeltersVec[smelterInd].ID,smeltersVec[smelterInd].oreType,smeltersVec[smelterInd].loading_capacity,smeltersVec[smelterInd].waiting_ore_count,smeltersVec[smelterInd].total_produce);
	WriteOutput(NULL,NULL,&smelterInfo,NULL,SMELTER_CREATED);
	while(true){

		if (smeltersVec[smelterInd].waiting_ore_count >= 2){
			sem_wait(smelterMutex[smelterInd]);
				smeltersVec[smelterInd].waiting_ore_count -=2;
				smeltersVec[smelterInd].total_produce +=1;
				
				FillSmelterInfo(&smelterInfo,smeltersVec[smelterInd].ID,smeltersVec[smelterInd].oreType,smeltersVec[smelterInd].loading_capacity,smeltersVec[smelterInd].waiting_ore_count,smeltersVec[smelterInd].total_produce);
				WriteOutput(NULL,NULL,&smelterInfo,NULL,SMELTER_STARTED);
				
			sem_post(smelterMutex[smelterInd]);
			usleep(smeltersVec[smelterInd].duration - (smeltersVec[smelterInd].duration*0.01) + (rand()%(int)(smeltersVec[smelterInd].duration*0.02)));
				
			FillSmelterInfo(&smelterInfo,smeltersVec[smelterInd].ID,smeltersVec[smelterInd].oreType,smeltersVec[smelterInd].loading_capacity,smeltersVec[smelterInd].waiting_ore_count,smeltersVec[smelterInd].total_produce);
			WriteOutput(NULL,NULL,&smelterInfo,NULL,SMELTER_FINISHED);
			sem_post(signalFromProducers);
		} 
		else {
			struct timespec ts;
			clock_gettime(CLOCK_REALTIME, &ts);
			ts.tv_sec += 5;
			if(sem_timedwait(smelterBarrier[smelterInd],&ts)==-1 && errno == ETIMEDOUT){
				break;
			}
		}
	}
	
	FillSmelterInfo(&smelterInfo,smeltersVec[smelterInd].ID,smeltersVec[smelterInd].oreType,smeltersVec[smelterInd].loading_capacity,smeltersVec[smelterInd].waiting_ore_count,smeltersVec[smelterInd].total_produce);
	WriteOutput(NULL,NULL,&smelterInfo,NULL,SMELTER_STOPPED);
	sem_wait(smelterStopped[smelterInd]);
		smeltersVec[smelterInd].active = false;
	sem_post(smelterStopped[smelterInd]);
	return 0;
}
void *foundryThreadRoutine(void *i){
	int foundryInd = *((int*) i);
	foundriesVec[foundryInd].created = true;
	FoundryInfo foundryInfo;
	FillFoundryInfo(&foundryInfo,foundriesVec[foundryInd].ID, foundriesVec[foundryInd].loading_capacity,foundriesVec[foundryInd].waiting_iron ,foundriesVec[foundryInd].waiting_coal,foundriesVec[foundryInd].total_produce);
	WriteOutput(NULL,NULL,NULL,&foundryInfo,FOUNDRY_CREATED);
	while(true){
		if(foundriesVec[foundryInd].waiting_coal>=1 && foundriesVec[foundryInd].waiting_iron>=1){
			sem_wait(foundryMutex[foundryInd]);
				foundriesVec[foundryInd].waiting_iron -=1;
				foundriesVec[foundryInd].waiting_coal -=1;
				foundriesVec[foundryInd].total_produce +=1;
			
			FillFoundryInfo(&foundryInfo,foundriesVec[foundryInd].ID, foundriesVec[foundryInd].loading_capacity,foundriesVec[foundryInd].waiting_iron ,foundriesVec[foundryInd].waiting_coal,foundriesVec[foundryInd].total_produce);
			WriteOutput(NULL,NULL,NULL,&foundryInfo,FOUNDRY_STARTED);
			sem_post(foundryMutex[foundryInd]);
			usleep(foundriesVec[foundryInd].duration - (foundriesVec[foundryInd].duration*0.01) + (rand()%(int)(foundriesVec[foundryInd].duration*0.02)));
				
				
			FillFoundryInfo(&foundryInfo,foundriesVec[foundryInd].ID, foundriesVec[foundryInd].loading_capacity,foundriesVec[foundryInd].waiting_iron ,foundriesVec[foundryInd].waiting_coal,foundriesVec[foundryInd].total_produce);
			WriteOutput(NULL,NULL,NULL,&foundryInfo,FOUNDRY_FINISHED);
			sem_post(signalFromProducers);
		}
		
		else{
			struct timespec ts;
			clock_gettime(CLOCK_REALTIME, &ts);
			ts.tv_sec += 5;
			if(sem_timedwait(foundryBarrier[foundryInd],&ts)==-1 && errno == ETIMEDOUT){
				break;
			}
		}
	}
	
	FillFoundryInfo(&foundryInfo,foundriesVec[foundryInd].ID, foundriesVec[foundryInd].loading_capacity,foundriesVec[foundryInd].waiting_iron ,foundriesVec[foundryInd].waiting_coal,foundriesVec[foundryInd].total_produce);
	WriteOutput(NULL,NULL,NULL,&foundryInfo,FOUNDRY_STOPPED);
	sem_wait(foundryStopped[foundryInd]);
		foundriesVec[foundryInd].active = false;
	sem_post(foundryStopped[foundryInd]);
	return 0;
}
void * transporterThreadRoutine(void *i){
	int transporterInd = *((int *)i);
	TransporterInfo transporterInfo;
	FillTransporterInfo(&transporterInfo,transportersVec[transporterInd].ID,transportersVec[transporterInd].carry);
	WriteOutput(NULL,&transporterInfo,NULL,NULL,TRANSPORTER_CREATED);
	bool allMinersPassive = false;
	allMinersPassive = true;
	for(int k=0; k<numberMiner ;k++){
		if(minersVec[k].created==false) continue;
		bool minActive;
		sem_wait(minerStopped[k]);
			minActive = minersVec[k].active ;
		if(k==0)  allMinersPassive = true;
		if(minActive == true){
			allMinersPassive = false;
		}
		sem_post(minerStopped[k]);
		sem_wait(minerProduced[k]);
		int currCount = minersVec[k].current_count;
		if(k==(numberMiner-1) && minersVec[k].current_count == 0){
			sem_post(minerProduced[k]);
			if(allMinersPassive)
				break;
			sem_wait(aMinerProduced); //wait to be informed by a miner
			k=-1;
			continue;
		}
		
		if(currCount==0) sem_post(minerProduced[k]);
		if( currCount != 0){
			//miner routine
			MinerInfo minerInfo;
			FillMinerInfo(&minerInfo,(minersVec[k]).ID, minersVec[k].oreType ,minersVec[k].capacity , minersVec[k].current_count);
			FillTransporterInfo(&transporterInfo,transportersVec[transporterInd].ID,transportersVec[transporterInd].carry);
			WriteOutput(&minerInfo,&transporterInfo,NULL,NULL,TRANSPORTER_TRAVEL);
			usleep(transportersVec[transporterInd].duration - (transportersVec[transporterInd].duration*0.01) + (rand()%(int)(transportersVec[transporterInd].duration*0.02)));

			
				minersVec[k].current_count =  minersVec[k].current_count -1;
			sem_post(minerProduced[k]);
			
			(transportersVec[transporterInd].carry) = &(minersVec[k].oreType);
			FillMinerInfo(&minerInfo,(minersVec[k]).ID, minersVec[k].oreType , minersVec[k].capacity , minersVec[k].current_count);
			FillTransporterInfo(&transporterInfo,transportersVec[transporterInd].ID,transportersVec[transporterInd].carry);
			WriteOutput(&minerInfo,&transporterInfo,NULL,NULL,TRANSPORTER_TAKE_ORE);
			usleep(transportersVec[transporterInd].duration - (transportersVec[transporterInd].duration*0.01) + (rand()%(int)(transportersVec[transporterInd].duration*0.02)));
			sem_post(minerWaitCanProduce[k]);
			//end of miner routine
			while(true) {
							
							int smelterProducer =-1;
							int foundryProducer =-1;
							OreType trOreType =  *(transportersVec[transporterInd].carry);
							
							for(int s=0,f=0; (s<numberSmelter || f<numberFoundry) ;s++,f++ ){
									if(s<numberSmelter) sem_wait(smelterMutex[s]);
									if(f<numberFoundry) sem_wait(foundryMutex[f]);
									if(s<numberSmelter  && smeltersVec[s].created && smeltersVec[s].oreType == trOreType && smeltersVec[s].waiting_ore_count == 1 ){
										smelterProducer=s;
										if(f<numberFoundry) sem_post(foundryMutex[f]);
										break;
									}
									else if(s<numberSmelter)sem_post(smelterMutex[s]);
									if(f <numberFoundry &&  foundriesVec[f].created && trOreType == IRON  && foundriesVec[f].waiting_coal>=1 && foundriesVec[f].waiting_iron==0){
										foundryProducer = f;
										break;
									}
									else if(f <numberFoundry  && foundriesVec[f].created && trOreType == COAL  && foundriesVec[f].waiting_coal==0 && foundriesVec[f].waiting_iron>=1){
										foundryProducer = f;
										break;
									}
									else if(f<numberFoundry)sem_post(foundryMutex[f]);
									if(s >=numberSmelter && f>=numberFoundry)
										break;
							}
							
							
							if(smelterProducer==-1 && foundryProducer==-1){
								for(int s=0,f=0; (s<numberSmelter || f<numberFoundry) ;s++,f++ ){
									if(s<numberSmelter) sem_wait(smelterMutex[s]);
									if(f<numberFoundry) sem_wait(foundryMutex[f]);
									if(s<numberSmelter  && smeltersVec[s].created && smeltersVec[s].oreType == trOreType && smeltersVec[s].waiting_ore_count != smeltersVec[s].loading_capacity ){
										smelterProducer=s;
										if(f<numberFoundry)sem_post(foundryMutex[f]);
										break;
									}  
									else if(s<numberSmelter) sem_post(smelterMutex[s]);
									if(f<numberFoundry   && foundriesVec[f].created &&(trOreType == IRON ) && (foundriesVec[f].waiting_iron != foundriesVec[f].loading_capacity)){
										foundryProducer = f;
										break;
									}
									else if(f<numberFoundry   && foundriesVec[f].created &&(trOreType == COAL ) && (foundriesVec[f].waiting_coal != foundriesVec[f].loading_capacity )){
										foundryProducer = f;
										break;
									}
									else if(f<numberFoundry) sem_post(foundryMutex[f]);
									if(s>=numberSmelter && f>=numberFoundry)
										break;
								}
								if(smelterProducer==-1 && foundryProducer==-1){
									sem_wait(signalFromProducers);
									continue;
									
								}
							}
							if(smelterProducer>=0){
								//smelterroutine

								SmelterInfo smelterInfo;
								FillSmelterInfo(&smelterInfo,smeltersVec[smelterProducer].ID,smeltersVec[smelterProducer].oreType, 0,0,0);
								FillTransporterInfo(&transporterInfo,transportersVec[transporterInd].ID,transportersVec[transporterInd].carry);
								WriteOutput(NULL,&transporterInfo,&smelterInfo,NULL,TRANSPORTER_TRAVEL);
								usleep(transportersVec[transporterInd].duration - (transportersVec[transporterInd].duration*0.01) + (rand()%(int)(transportersVec[transporterInd].duration*0.02)));
								
									smeltersVec[smelterProducer].waiting_ore_count +=1;
								sem_post(smelterMutex[smelterProducer]);
								FillSmelterInfo(&smelterInfo,smeltersVec[smelterProducer].ID,smeltersVec[smelterProducer].oreType, smeltersVec[smelterProducer].loading_capacity,smeltersVec[smelterProducer].waiting_ore_count,smeltersVec[smelterProducer].total_produce);
								FillTransporterInfo(&transporterInfo,transportersVec[transporterInd].ID,transportersVec[transporterInd].carry);
								WriteOutput(NULL,&transporterInfo,&smelterInfo,NULL,TRANSPORTER_DROP_ORE);
								usleep(transportersVec[transporterInd].duration - (transportersVec[transporterInd].duration*0.01) + (rand()%(int)(transportersVec[transporterInd].duration*0.02)));

								if(smeltersVec[smelterProducer].waiting_ore_count>=2) {
									sem_post(smelterBarrier[smelterProducer]);
								}
								if(k==(numberMiner-1)) k=-1;
								break;
							}
							if(foundryProducer>=0){
								//foundryRoutine
								FoundryInfo foundryInfo;
								FillFoundryInfo(&foundryInfo, foundriesVec[foundryProducer].ID, 0,0,0,0);
								FillTransporterInfo(&transporterInfo,transportersVec[transporterInd].ID,transportersVec[transporterInd].carry);
								WriteOutput(NULL,&transporterInfo,NULL,&foundryInfo,TRANSPORTER_TRAVEL);
								usleep(transportersVec[transporterInd].duration - (transportersVec[transporterInd].duration*0.01) + (rand()%(int)(transportersVec[transporterInd].duration*0.02)));
								if(*(transportersVec[transporterInd].carry) == IRON){
									foundriesVec[foundryProducer].waiting_iron +=1;
								}
								else{	
									foundriesVec[foundryProducer].waiting_coal +=1;
								}
								sem_post(foundryMutex[foundryProducer]);
								FillFoundryInfo(&foundryInfo, foundriesVec[foundryProducer].ID, foundriesVec[foundryProducer].loading_capacity,foundriesVec[foundryProducer].waiting_iron,foundriesVec[foundryProducer].waiting_coal,foundriesVec[foundryProducer].total_produce);
								FillTransporterInfo(&transporterInfo,transportersVec[transporterInd].ID,transportersVec[transporterInd].carry);
								WriteOutput(NULL,&transporterInfo,NULL,&foundryInfo,TRANSPORTER_DROP_ORE);
								usleep(transportersVec[transporterInd].duration - (transportersVec[transporterInd].duration*0.01) + (rand()%(int)(transportersVec[transporterInd].duration*0.02)));
								if(foundriesVec[foundryProducer].waiting_iron >=1  && foundriesVec[foundryProducer].waiting_coal>=1) 
									sem_post(foundryBarrier[foundryProducer]);
								if(k==(numberMiner-1)) k=-1;
								break;
							}
			}
		}

	}
	FillTransporterInfo(&transporterInfo,transportersVec[transporterInd].ID,transportersVec[transporterInd].carry);
	WriteOutput(NULL,&transporterInfo,NULL,NULL,TRANSPORTER_STOPPED);
	return 0;
}

int main(){

	
	cin >> numberMiner ;
	for(int i=1; i<=numberMiner ; i++){
		MinerInput m;
		m.ID = i;
		cin >> m.duration;
		cin >> m.capacity;
		int oretype;
		cin >> oretype;
		if(oretype == 0) m.oreType = IRON;
		else if(oretype == 1) m.oreType = COPPER;
		else m.oreType = COAL;
		m.created = false;
		cin >> m.total_extract;
		m.current_count =0;
		m.active =true;
		minersVec.push_back(m);
	}

	cin >> numberTransporter ;
	for(int i=1; i<=numberTransporter ;i++){
		TransporterInput t;
		t.ID=i;
		cin >> t.duration;
		t.carry = NULL;
		t.created = false;
		transportersVec.push_back(t);
	}
	cin >> numberSmelter;
	for(int i=1; i<=numberSmelter ; i++){
		SmelterInput s;
		s.ID=i;
		cin >> s.duration;
		cin >> s.loading_capacity;
		int oretype;
		cin >> oretype;
		if(oretype == 0) s.oreType = IRON;
		else if(oretype == 1) s.oreType = COPPER;
		s.waiting_ore_count =0;
		s.total_produce =0;
		s.created = false;
		smeltersVec.push_back(s);

	}
	cin >> numberFoundry;
	for(int i=1; i<=numberFoundry ; i++){
		FoundryInput f;
		f.ID=i;
		cin>> f.duration;
		cin >> f.loading_capacity;
		f.waiting_coal=0;
		f.waiting_iron=0;
		f.total_produce=0;
		f.created = false;
		foundriesVec.push_back(f);
	}
	/////end getting input
	std::vector<pthread_t>  threadsMiner(numberMiner);
	std::vector<pthread_t>  threadsTransporter(numberTransporter);
	std::vector<pthread_t>  threadsSmelter(numberSmelter);
	std::vector<pthread_t>  threadsFoundry(numberFoundry);

		//semaphore things
	const char * s="aMinerProduced";
	sem_unlink(s);
	aMinerProduced = sem_open(s,O_CREAT,0600,0);

	const char * s1="signalFromProducers";
	sem_unlink(s1);
	signalFromProducers = sem_open(s1,O_CREAT,0600,0);
	sem_t *waitCanProduce,*m_mutex,*m2_mutex;
	for (int i=0;i < numberMiner; i++) {

		const char * s="minerWaitCanProduce"+i+1;
		const char * s2="minerProduced"+i+1;
		const char * s3="minerStopped"+i+1;
		
		sem_unlink(s);
		sem_unlink(s2);
		sem_unlink(s3);
		waitCanProduce = sem_open(s,O_CREAT,0600,minersVec[i].capacity);
		m_mutex = sem_open(s2,O_CREAT,0600,1);
		m2_mutex = sem_open(s3,O_CREAT,0600,1);
		minerWaitCanProduce.push_back(waitCanProduce);
		minerProduced.push_back(m_mutex);
		minerStopped.push_back(m2_mutex);
        
    }
    sem_t *smelterWait,*smelterBar;
    for (int i=0;i < numberSmelter; i++) {
    	const char * s="smelterMutex"+i+1;
    	const char * s1="smelterBarrier"+i+1;
		const char * s2="smelterProduced"+i+1;
		const char * s3="smelterStopped"+i+1;
		
		sem_unlink(s);
		sem_unlink(s1);
		sem_unlink(s2);
		sem_unlink(s3);
		smelterWait = sem_open(s,O_CREAT,0600,1);
		smelterBar = sem_open(s1,O_CREAT,0600,0);
		m_mutex = sem_open(s2,O_CREAT,0600,1);
		m2_mutex = sem_open(s3,O_CREAT,0600,1);
		smelterMutex.push_back(smelterWait);
		smelterBarrier.push_back(smelterBar);
		smelterProduced.push_back(m_mutex);
		smelterStopped.push_back(m2_mutex);
        
	}
        
	sem_t *foundryWait,*foundryBar;
    for (int i=0;i < numberFoundry; i++) {
    	const char * s="foundryMutex"+i+1;
    	const char * s1="foundryBarrier"+i+1;
		const char * s2="foundryProduced"+i+1;
		const char * s3="foundryStopped"+i+1;
		
		sem_unlink(s);
		sem_unlink(s1);
		sem_unlink(s2);
		sem_unlink(s3);
		foundryWait = sem_open(s,O_CREAT,0600,1);
		foundryBar = sem_open(s1,O_CREAT,0600,0);
		m_mutex = sem_open(s2,O_CREAT,0600,1);
		m2_mutex = sem_open(s3,O_CREAT,0600,1);
		foundryMutex.push_back(foundryWait);
		foundryBarrier.push_back(foundryBar);
		foundryProduced.push_back(m_mutex);
		foundryStopped.push_back(m2_mutex);
        
	}
	int d[numberMiner];
    for (int i=0;i < numberMiner; i++) {
    	d[i]=i;
    	pthread_create(&threadsMiner[i], NULL, minerThreadRoutine, (void *) &d[i]);

    }
    int c[numberSmelter];
    for (int i=0;i < numberSmelter; i++){
    	c[i]=i;
		pthread_create(&threadsSmelter[i], NULL, smelterThreadRoutine, (void *) &c[i]);
	}
	int a[numberFoundry];
	for (int i=0;i < numberFoundry; i++){
		a[i]=i;
    	pthread_create(&threadsFoundry[i], NULL, foundryThreadRoutine, (void *) &a[i]);
    }
    int b[numberTransporter];
	for (int i=0;i < numberTransporter; i++) {
		b[i]=i;
        pthread_create(&threadsTransporter[i], NULL, transporterThreadRoutine, (void *) &b[i]);
	}
	
    for (int i=0;i < numberMiner; i++) {
        pthread_join(threadsMiner[i], NULL);
    }
    
	for (int i=0;i < numberSmelter; i++){
    	pthread_join(threadsSmelter[i], NULL);
	}
	for (int i=0;i < numberFoundry; i++){
        pthread_join(threadsFoundry[i], NULL);
    }
	for (int i=0;i < numberTransporter; i++) {
        pthread_join(threadsTransporter[i], NULL);
	}
    
	return 0;
}



