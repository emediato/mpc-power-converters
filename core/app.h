//
// Created by Mônica Emediato on 16/06/2024.
//

#ifndef APP_H
#define APP_H

#include <stdio.h>
#include <stddef.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

extern uint16_t adcRawValues[2];


extern volatile int kindex;
extern int ialfa[200];
extern int ibeta[200];
extern int switch_all[200];


extern void App();
extern unsigned int selectBestCombination();

#define NumberOfStates 7

extern uint16_t adcRawValues[2];

extern matrix_t K;
extern matrix_t xk;
extern matrix_t uk;
extern matrix_t x_ref;

extern matrix_type xkdata[2 * 1];



#endif //APP_H
