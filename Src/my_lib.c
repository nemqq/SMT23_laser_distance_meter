/*
 * my_bib.c
 *
 *  Created on: May 25, 2021
 *      Author: Kamil
 */

#include "my_lib.h"
//#include "stm32f1xx_hal.h"


void digit_con(int conv_val, int tab1[]){
    /* Function to convert one number for individual digits */

    int temp_val, number1;
    int i = 0, sub = 0, p = 10, a = 1000;
    for(i=0;i<4;i++){
        number1 = conv_val - sub;
        temp_val = number1 / a;
        tab1[i] = temp_val;
        temp_val *= a;
        sub += temp_val;
        a = a / p;
        }
    }


void char_con(int val_to_con[], char *val_con){
    /* Function to convert data form int char */
    
    char charValue;
    int i = 0;
    int num;
    val_to_con[4] = val_to_con[3];
    for(i=0; i<5; i++){
        if (i == 3){
        	val_con[i] = ',';
        }
        else{
        	num = val_to_con[i];
        	charValue = num + '0';
        	val_con[i] = charValue;
        }
    }
}


void my_buzzer(){
    /* Function which activate buzzer */

	HAL_Delay(300);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_Delay(300);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	//*my_time += 200;
	//set_flag = 0;
}


void calc_average(int *data, int *outcome){
    /* Function to calcualte average */

	int sr = 0, c = 0;
	for(c=0;c<5;c++){
		sr+=data[c];
	}
	*outcome = sr/5;
	sr=0;
}