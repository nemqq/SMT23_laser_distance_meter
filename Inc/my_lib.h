/*
 * my_bib.h
 *
 *  Created on: May 25, 2021
 *      Author: Kamil
 */



#ifndef INC_MY_BIB_H_
#define INC_MY_BIB_H_

void digit_con(int conv_val, int tab1[]);
void con1(int val_to_con[], char *val_con);
void my_buzzer(void);
void calc_average(int *dane, int *wynik);

//macro define
#define hisz 0xC;


#endif /* INC_MY_BIB_H_ */
#include "stm32f4xx_hal.h"
