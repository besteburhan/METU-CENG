//
// Created by cagriakpak on 05.04.2019.
//

#ifndef HOMEWORK2_WRITEOUTPUT_H
#define HOMEWORK2_WRITEOUTPUT_H

#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

typedef enum Action {
    MINER_CREATED,
    MINER_STARTED,
    MINER_FINISHED,
    MINER_STOPPED,
    TRANSPORTER_CREATED,
    TRANSPORTER_TRAVEL,
    TRANSPORTER_TAKE_ORE,
    TRANSPORTER_DROP_ORE,
    TRANSPORTER_STOPPED,
    SMELTER_CREATED,
    SMELTER_STARTED,
    SMELTER_FINISHED,
    SMELTER_STOPPED,
    FOUNDRY_CREATED,
    FOUNDRY_STARTED,
    FOUNDRY_FINISHED,
    FOUNDRY_STOPPED
}Action;

typedef enum OreType {
    IRON,
    COPPER,
    COAL
} OreType;

typedef struct MinerInfo {
    unsigned int ID;
    OreType oreType;
    unsigned int capacity;
    unsigned int current_count;
} MinerInfo;

typedef struct TransporterInfo {
    unsigned int ID;
    OreType *carry;
} TransporterInfo;

typedef struct SmelterInfo {
    unsigned int ID;
    OreType oreType;
    unsigned int loading_capacity;
    unsigned int waiting_ore_count;
    unsigned int total_produce;
} SmelterInfo;

typedef struct FoundryInfo {
    unsigned int ID;
    unsigned int loading_capacity;
    unsigned int waiting_iron;
    unsigned int waiting_coal;
    unsigned int total_produce;
} FoundryInfo;


void InitWriteOutput();
unsigned long long GetTimestamp();
void PrintThreadId();

const char* ToString(OreType ore);

void FillMinerInfo(MinerInfo *minerInfo, unsigned int ID, OreType oreType, unsigned int capacity,
                   unsigned int current_count);
void FillTransporterInfo(TransporterInfo *transporterInfo, unsigned int ID, OreType *carry);
void FillSmelterInfo(SmelterInfo *smelterInfo, unsigned int ID, OreType oreType, unsigned int loading_capacity, unsigned int waiting_ore_count, unsigned int total_produce);
void FillFoundryInfo(FoundryInfo *foundryInfo, unsigned int ID, unsigned int loading_capacity, unsigned int waiting_iron, unsigned int waiting_coal, unsigned int total_produce);

void WriteOutput(MinerInfo* minerInfo, TransporterInfo* transporterInfo, SmelterInfo* smelterInfo, FoundryInfo* foundryInfo, Action action);

#endif //HOMEWORK2_WRITEOUTPUT_H
