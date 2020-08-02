#include "breakpoints.h"

volatile char special = 4;

void init_complete(){}
void adc_complete(){}
void rb4_handled(){}
void latjh_update_complete(){}
void latcde_update_complete(){}
void correct_guess(){}
void hs_passed(){}
void game_over(){}
void restart(){}
char special_number(){return special;}
