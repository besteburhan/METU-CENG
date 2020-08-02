#ifndef BREAKPOINTS_H
#define	BREAKPOINTS_H

volatile int adc_value = 0;

void init_complete(void);
void adc_complete(void);
void rb4_handled(void);
void latjh_update_complete(void);
void latcde_update_complete(void);
void correct_guess(void);
void hs_passed(void);
void game_over(void);
void restart(void);
char special_number(void);

#endif /* BREAKPOINTS_H */
