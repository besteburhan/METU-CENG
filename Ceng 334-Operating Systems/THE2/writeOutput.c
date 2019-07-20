//
// Created by cagriakpak on 05.04.2019.
//

#include "writeOutput.h"
pthread_mutex_t mutexWrite = PTHREAD_MUTEX_INITIALIZER;

struct timeval startTime;

void InitWriteOutput()
{
    gettimeofday(&startTime, NULL);
}

unsigned long long GetTimestamp()
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return (currentTime.tv_sec - startTime.tv_sec) * 1000000 // second
           + (currentTime.tv_usec - startTime.tv_usec); // micro second
}

void PrintThreadId()
{
    pthread_t tid = pthread_self();
    size_t i;
    printf("ThreadID: ");
    for (i=0; i<sizeof(pthread_t); ++i)
        printf("%02x", *(((unsigned char*)&tid) + i));
    printf(", ");
}

const char* ToString(OreType ore) {
    switch (ore) {
        case IRON:
            return "Iron";
        case COPPER:
            return "Copper";
        case COAL:
            return "Coal";
    }
}

void FillMinerInfo(MinerInfo *minerInfo, unsigned int ID, OreType oreType, unsigned int capacity, unsigned int current_count) {
    minerInfo->ID = ID;
    minerInfo->oreType = oreType;
    minerInfo->capacity = capacity;
    minerInfo->current_count = current_count;
}

void FillTransporterInfo(TransporterInfo *transporterInfo, unsigned int ID, OreType *carry) {
    transporterInfo->ID = ID;
    transporterInfo->carry = carry;
}

void FillSmelterInfo(SmelterInfo *smelterInfo, unsigned int ID, OreType oreType, unsigned int loading_capacity, unsigned int waiting_ore_count, unsigned int total_produce) {
    smelterInfo->ID = ID;
    smelterInfo->oreType = oreType;
    smelterInfo->loading_capacity = loading_capacity;
    smelterInfo->waiting_ore_count = waiting_ore_count;
    smelterInfo->total_produce = total_produce;
}

void FillFoundryInfo(FoundryInfo *foundryInfo, unsigned int ID, unsigned int loading_capacity, unsigned int waiting_iron, unsigned int waiting_coal, unsigned int total_produce) {
    foundryInfo->ID = ID;
    foundryInfo->loading_capacity = loading_capacity;
    foundryInfo->waiting_iron = waiting_iron;
    foundryInfo->waiting_coal = waiting_coal;
    foundryInfo->total_produce = total_produce;
}

void WriteOutput(MinerInfo* minerInfo, TransporterInfo* transporterInfo, SmelterInfo* smelterInfo, FoundryInfo* foundryInfo, Action action) {
    unsigned long long time = GetTimestamp();
    unsigned int minerID =0, transporterID=0, smelterID=0, foundryID=0;
    pthread_mutex_lock(&mutexWrite);

    if ( minerInfo )
        minerID = minerInfo->ID;
    if ( transporterInfo )
        transporterID = transporterInfo->ID;
    if ( smelterInfo )
        smelterID = smelterInfo->ID;
    if ( foundryInfo )
        foundryID = foundryInfo->ID;

    PrintThreadId();
    printf("MID: %d, TID: %d, SID: %d, FID: %d, time stamp: %llu, AID: %d, ", minerID, transporterID, smelterID, foundryID, time, (int)action);
    switch (action) {
        case MINER_CREATED:
            printf("Miner created.\n");
            break;
        case MINER_STARTED:
            printf("Miner started mining %s, current capacity: %d/%d.\n", ToString(minerInfo->oreType), minerInfo->current_count, minerInfo->capacity);
            break;
        case MINER_FINISHED:
            printf("Miner finished mining %s, current capacity: %d/%d.\n", ToString(minerInfo->oreType), minerInfo->current_count, minerInfo->capacity);
            break;
        case MINER_STOPPED:
            printf("Miner stopped mining %s, current capacity: %d/%d.\n", ToString(minerInfo->oreType), minerInfo->current_count, minerInfo->capacity);
            break;
        case TRANSPORTER_CREATED:
            printf("Transporter created.\n");
            break;
        case TRANSPORTER_STOPPED:
            printf("Transporter stopped.\n");
            break;
        case TRANSPORTER_TRAVEL:
            if ( minerInfo )
                printf("Transporter is traveling to miner.\n");
            else if ( smelterInfo )
                printf("Transporter is traveling to smelter, carrying: %s.\n", ToString(*transporterInfo->carry));
            else if ( foundryInfo )
                printf("Transporter is traveling to foundry, carrying: %s.\n", ToString(*transporterInfo->carry));
            else
                printf("Wrong argument format.\n");
            break;
        case TRANSPORTER_TAKE_ORE:
            printf("Transporter take %s from miner, current fill %d/%d.\n", ToString(*transporterInfo->carry), minerInfo->current_count, minerInfo->capacity);
            break;
        case TRANSPORTER_DROP_ORE:
            if ( smelterInfo )
                printf("Transporter put %s ore to smelter. Waiting Ore %d/%d.\n", ToString(*transporterInfo->carry), smelterInfo->waiting_ore_count, smelterInfo->loading_capacity);
            else if ( foundryInfo )
                printf("Transporter put %s ore to foundry. Waiting Ore coal: (%d/%d), iron: (%d/%d).\n", ToString(*transporterInfo->carry), foundryInfo->waiting_coal, foundryInfo->loading_capacity,
                       foundryInfo->waiting_iron, foundryInfo->loading_capacity);
            else
                printf("Wrong argument format.\n");
            break;
        case SMELTER_CREATED:
            printf("Smelter created.\n");
            break;
        case SMELTER_STARTED:
            printf("Smelter started producing %s ingot. Waiting Ore %d/%d.\n", ToString(smelterInfo->oreType), smelterInfo->waiting_ore_count, smelterInfo->loading_capacity);
            break;
        case SMELTER_FINISHED:
            printf("Smelter finished producing %s ingot. %d ingots are produced.\n", ToString(smelterInfo->oreType), smelterInfo->total_produce);
            break;
        case SMELTER_STOPPED:
            printf("Smelter stopped. %d %s ingots are produced.\n", smelterInfo->total_produce, ToString(smelterInfo->oreType));
            break;
        case FOUNDRY_CREATED:
            printf("Foundry created.\n");
            break;
        case FOUNDRY_STARTED:
            printf("Foundry started producing steel ingot. Waiting Ore coal: (%d/%d), iron: (%d/%d).\n", foundryInfo->waiting_coal, foundryInfo->loading_capacity,
                    foundryInfo->waiting_iron, foundryInfo->loading_capacity);
            break;
        case FOUNDRY_FINISHED:
            printf("Foundry finished producing steel ingot. %d steel ingots are produced.\n", foundryInfo->total_produce);
            break;
        case FOUNDRY_STOPPED:
            printf("Foundry stopped. %d steel ingots are produced.\n", foundryInfo->total_produce);
            break;
        default:
            printf("Wrong argument format.\n");
            break;
    }
    pthread_mutex_unlock(&mutexWrite);
}
