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
unsigned int selectBestCombination();


#endif //APP_H
