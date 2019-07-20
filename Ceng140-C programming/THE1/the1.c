#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define PI 3.14159265
long double getrandom(long double pro_of_letter[26][103],char output,int letter,int n);
void shunting(char formula[200],long double pro_of_letter[26][103],int letter,int n,long int experiment);
void calculate(char output[200],long double pro_of_letter[26][103],int out_j,int letter,int n,long int experiment);


void calculate(char output[200],long double pro_of_letter[26][103],int out_j,int letter,int n,long int experiment){
	long double stackn[200];
	long double min=0,max=0,intervalLength,randomVal;
	long double end[103],randomValues[52]={0};
	long int i,j=0,t,ex,randomCounter1=0,tempInd;
	char temp[15];


	for(tempInd=0;tempInd<52;tempInd++) randomValues[tempInd]='\0';



	for(i=0,t=0;i<out_j;i++){
		t=0;
		if(output[i]=='n'){
			while(1){
				i++;
				if(output[i]=='.'|| (output[i]<='9' && output[i]>='0')){
					temp[t]=output[i];t++;
				}else{
					stackn[j]=atof(temp);
					for(t=0;t<15;t++) temp[t]=0;
					j++;
					i--;
					break;	
				}
			}
		}else if(output[i]<='Z' && output[i]>='A'){
			randomVal=getrandom(pro_of_letter,output[i],letter,n);

			for(tempInd=0;tempInd<52;tempInd++){
				if(randomValues[tempInd]==(float)output[i]){
					randomCounter1++;
					break;
				}
			}if(randomCounter1==0) {
				stackn[j]=randomVal;
				j++;
				for(tempInd=0;tempInd<52;tempInd++){
					if(randomValues[tempInd]=='\0'){
						randomValues[tempInd]=(float)output[i];
						randomValues[tempInd+1]=randomVal;

						break;
					}
				}
			}else{
				stackn[j]=randomValues[tempInd+1];
				randomCounter1=0;
				j++;

			}
		}else if(output[i]=='~') {stackn[j-1]=-(stackn[j-1]);}
		else if(output[i]=='i') {stackn[j-1]= sin((stackn[j-1]));}
		else if(output[i]=='c') {stackn[j-1]= cos((stackn[j-1]));}
		else if(output[i]=='l') {stackn[j-1]= log(stackn[j-1]);}
		else if(output[i]=='q'){ stackn[j-1]= sqrt(stackn[j-1]);}
		else if(output[i]=='+'){
			stackn[j-2]=stackn[j-2] + stackn[j-1];j--;}
		else if(output[i]=='-'){
			stackn[j-2]=stackn[j-2] - stackn[j-1];j--;}
		else if(output[i]=='*'){
			stackn[j-2]=stackn[j-2] * stackn[j-1];j--;}
		else if(output[i]=='/'){
			stackn[j-2]=stackn[j-2] / stackn[j-1];j--;}
		else if(output[i]=='^'){
			stackn[j-2]=pow(stackn[j-2],stackn[j-1]);j--;}		
	}

	min=stackn[0];
	max=stackn[0];


	for(ex=1;ex<experiment;ex++){
		for(tempInd=0;tempInd<52;tempInd++) randomValues[tempInd]='\0';

		for(i=0,t=0,j=0;i<out_j;i++){
			t=0;
			if(output[i]=='n'){
				while(1){
					i++;
					if(output[i]=='.'|| (output[i]<='9' && output[i]>='0')){
						temp[t]=output[i];t++;
					}else{
						stackn[j]=atof(temp);
						for(t=0;t<15;t++) temp[t]=0;
						j++;
						i--;
						break;	
					}
				}
			}else if(output[i]<='Z' && output[i]>='A'){
			randomVal=getrandom(pro_of_letter,output[i],letter,n);

			for(tempInd=0;tempInd<52;tempInd++){
				if(randomValues[tempInd]==(float)output[i]){
					randomCounter1++;
					break;
				}
			}if(randomCounter1==0) {
				stackn[j]=randomVal;
				for(tempInd=0;tempInd<52;tempInd++){
					if(randomValues[tempInd]=='\0'){
						randomValues[tempInd]=(float)output[i];
						randomValues[tempInd+1]=randomVal;
						break;
					}
				}
			}else{
				stackn[j]=randomValues[tempInd+1];
				randomCounter1=0;
			}
			j++;
			}else if(output[i]=='~') {stackn[j-1]=-(stackn[j-1]);}
			else if(output[i]=='i') {stackn[j-1]= sin((stackn[j-1]));}
			else if(output[i]=='c') {stackn[j-1]= cos((stackn[j-1]));}
			else if(output[i]=='l') {stackn[j-1]= log(stackn[j-1]);}
			else if(output[i]=='q') {stackn[j-1]= sqrt(stackn[j-1]);}
			else if(output[i]=='+'){
				stackn[j-2]=stackn[j-2] + stackn[j-1];j--;}
			else if(output[i]=='-'){
				stackn[j-2]=stackn[j-2] - stackn[j-1]; j--;}
			else if(output[i]=='*'){
				stackn[j-2]=stackn[j-2] * stackn[j-1]; j--;}
			else if(output[i]=='/'){
				stackn[j-2]=stackn[j-2] / stackn[j-1];j--;}
			else if(output[i]=='^'){
				stackn[j-2]=pow(stackn[j-2],stackn[j-1]);j--;}

		}
		if(stackn[0]<=min) min=stackn[0];
		if(stackn[0]>=max) max=stackn[0];		
	}





	end[0]=min;end[1]=max;
	intervalLength=(max-min)/n;
	





	for(t=2;t<(n+2);t++) end[t]=0;




	for(ex=0;ex<experiment;ex++){
		for(tempInd=0;tempInd<52;tempInd++) randomValues[tempInd]='\0';
	


		for(i=0,j=0,t=0;i<out_j;i++){
			t=0;
			if(output[i]=='n'){
				while(1){
					i++;
					if(output[i]=='.'|| (output[i]<='9' && output[i]>='0')){
						temp[t]=output[i];t++;
					}else{
						stackn[j]=atof(temp);
						for(t=0;t<15;t++) temp[t]=0;
						j++;
						i--;
						break;	
					}
				}
			}else if(output[i]<='Z' && output[i]>='A'){
			randomVal=getrandom(pro_of_letter,output[i],letter,n);

			for(tempInd=0;tempInd<52;tempInd++){
				if(randomValues[tempInd]==(float)output[i]){
					randomCounter1++;
					break;
				}
			}if(randomCounter1==0) {
				stackn[j]=randomVal;
				for(tempInd=0;tempInd<52;tempInd++){
					if(randomValues[tempInd]=='\0'){
						randomValues[tempInd]=(float)output[i];
						randomValues[tempInd+1]=randomVal;
						break;
					}
				}
			}else{
				stackn[j]=randomValues[tempInd+1];
				randomCounter1=0;
			}
			j++;
			}else if(output[i]=='~') {stackn[j-1]=-(stackn[j-1]);}
			else if(output[i]=='i') {stackn[j-1]= sin((stackn[j-1]));}
			else if(output[i]=='c') {stackn[j-1]= cos((stackn[j-1]));}
			else if(output[i]=='l') {stackn[j-1]= log(stackn[j-1]);}
			else if(output[i]=='q') {stackn[j-1]= sqrt(stackn[j-1]);}
			else if(output[i]=='+'){
				stackn[j-2]=stackn[j-2] + stackn[j-1]; j--;}
			else if(output[i]=='-'){
				stackn[j-2]=stackn[j-2] - stackn[j-1]; j--;}
			else if(output[i]=='*'){
				stackn[j-2]=stackn[j-2] * stackn[j-1]; j--;}
			else if(output[i]=='/'){
				stackn[j-2]=stackn[j-2] / stackn[j-1];j--;}
			else if(output[i]=='^'){
				stackn[j-2]=pow(stackn[j-2],stackn[j-1]);j--;}		
		}
		for(t=0;t<n;t++){
			if(stackn[0]>=min+(intervalLength*t) && stackn[0]<=min+(intervalLength*(t+1))){
				end[t+2]=end[t+2]+1;
			}else if(stackn[0]>max) {end[n+1]=end[n+1]+1;}
			else if(stackn[0]<min) {end[2]=end[2]+1;}
		}

	}


	for(t=2;t<n+2;t++){
		end[t]=end[t]/experiment;
	}







	for(t=0;t<n+2;t++){
		printf("%Lf ",end[t]);
	}
}
long double getrandom(long double pro_of_letter[26][103],char output,int letter,int n){

	int i,j,temp1=0,temp2,randomNumber,intervalNumber;
	float intervalLength,randomFloat;
	int array[1000];
	for(i=0;i<letter;i++){
		if(pro_of_letter[i][0]==(float)(output)){
			intervalLength=(pro_of_letter[i][2]-pro_of_letter[i][1])/n;
			for(j=3;j<n+3;j++){
				temp2=pro_of_letter[i][j]*1000+temp1;
				for(;temp1<temp2;temp1++){
					array[temp1]=j;
				}
			}
			randomNumber=rand()%1000;
			intervalNumber=array[randomNumber]-2;
			randomFloat=((float)rand()/(float)(RAND_MAX)) * intervalLength;
			break;
		}
	}
	return pro_of_letter[i][1]+(intervalNumber-1)*intervalLength+randomFloat;
	
}
void shunting(char formula[200],long double pro_of_letter[26][103],int letter,int n,long int experiment){
	char array[200];
	int i,out_j=0,sta_j=0;
	char stack[200];
	char output[200];
	strcpy(array,formula);
	while (array[0]){
		
		/* space*/
		if(array[0]==' ') memmove(&array[0],&array[1],strlen(array));
		/*A B C .. Z*/
		else if (array[0]<=90 && array[0]>=65){
			output[out_j]=array[0];
			out_j++;
			memmove(&array[0],&array[1],strlen(array));
		}
		/* number*/
		else if((array[0]>=48 && array[0]<=57) || array[0]=='.' ){
			output[out_j]='n'; out_j++;
			output[out_j]=array[0]; out_j++;
			memmove(&array[0],&array[1],strlen(array));
			while((array[0]>=48 && array[0]<=57) || array[0]=='.') {
				output[out_j]=array[0];out_j++;
				memmove(&array[0],&array[1],strlen(array));
			}		
		}
		
		/* ~ function */
		else if(array[0]=='~'){
			stack[sta_j]=array[0];sta_j++;
			memmove(&array[0],&array[1],strlen(array));
		}
		/* ln function*/
		else if(array[0]=='l'){
			stack[sta_j]='l';sta_j++;
			memmove(&array[0],&array[1],strlen(array));
			memmove(&array[0],&array[1],strlen(array));
		}
		/* cos function */
		else if(array[0]=='c'){
			stack[sta_j]='c';sta_j++;
			memmove(&array[0],&array[1],strlen(array)); memmove(&array[0],&array[1],strlen(array)); memmove(&array[0],&array[1],strlen(array));
		}
		/* sin */ 
		else if(array[0]=='s' && array[1]=='i'){
			stack[sta_j]='i';sta_j++;
			memmove(&array[0],&array[1],strlen(array)); memmove(&array[0],&array[1],strlen(array)); memmove(&array[0],&array[1],strlen(array));
		}
		/*sqrt*/
		else if(array[0]=='s' && array[1]=='q'){
			stack[sta_j]='q';sta_j++;
			memmove(&array[0],&array[1],strlen(array));memmove(&array[0],&array[1],strlen(array));
			memmove(&array[0],&array[1],strlen(array));memmove(&array[0],&array[1],strlen(array));
		}
		/* ( left paranthesis*/
		else if(array[0]=='('){
			stack[sta_j]='(';sta_j++;
			memmove(&array[0],&array[1],strlen(array));
		}
		/* ) right paranthesis */
		else if(array[0]==')'){
			for(i=(sta_j-1);;i--){
				if (stack[i] !='('){
					sta_j--;
					output[out_j]=stack[sta_j];out_j++;	
				}
				else if(stack[i]=='('){
				sta_j--;break;}
				}
			if(stack[sta_j-1]=='~' || stack[sta_j-1]=='l' || stack[sta_j-1]=='i' || stack[sta_j-1]=='c' || stack[sta_j-1]=='q' ){
				sta_j--;
				output[out_j]=stack[sta_j];out_j++;
			}
			memmove(&array[0],&array[1],strlen(array));
			}	
		
		/* ^ operator*/
		else if(array[0]=='^'){
			stack[sta_j]=array[0];sta_j++;
			memmove(&array[0],&array[1],strlen(array));
		}
		/* / ve * operator*/
		else if (array[0]=='*' || array[0]=='/'){
			while(stack[(sta_j-1)]=='^'|| stack[(sta_j-1)]=='*'||  stack[(sta_j-1)]=='/' ){
				output[out_j]=stack[(sta_j-1)];out_j++;sta_j--;
				
			}
			stack[sta_j]=array[0];sta_j++;
			memmove(&array[0],&array[1],strlen(array));
		}
		/* + - operator*/
		else if (array[0]=='+' || array[0]=='-'){
			while(stack[(sta_j-1)]=='^' || stack[(sta_j-1)]=='*' || stack[(sta_j-1)]=='/' || stack[(sta_j-1)]=='+' || stack[(sta_j-1)]=='-'){
				output[out_j]=stack[(sta_j-1)];out_j++;sta_j--;
			}
			stack[sta_j]=array[0];sta_j++;
			memmove(&array[0],&array[1],strlen(array));
		}
	}
	while(sta_j>0){
		output[out_j]=stack[(sta_j-1)]; out_j++; sta_j--;
	}
	

	calculate(output,pro_of_letter,out_j,letter,n,experiment);
	
}

int main() 
{	

	char formula[200];
	int letter=0,i,j;
	int n;
	char s;

	long int experiment;
	long double temp;
	long double pro_of_letter[26][103];
	scanf("%[^\n]%*c",formula);
	srand(time(NULL));
	/* letter */
	for(i=0;formula[i]!='\0';i++){
		if (formula[i]<=90 && formula[i]>=65){
			letter++;
			for(j=i+1;formula[j]!='\0';j++){
				if(formula[j]==formula[i]) letter--;
			}
		}
	}
	
	scanf("%d %ld",&n,&experiment);
	
	
	i=0;
	while(i<letter){
		
		scanf(" %c ",&s);
		pro_of_letter[i][0]=(int) s;
		
		for(j=1;j<(n+3);j++){
			scanf("%Lf",&temp);
			pro_of_letter[i][j]=temp;		
		}
		i++;	
	}
	shunting(formula,pro_of_letter,letter,n,experiment);
	
	return 0;}
