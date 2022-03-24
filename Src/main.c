/*
  =====================================
  * File            : main.c
  * Author			: Kamil Sikora <kamil.sikora@student.po.edu.pl>
  * Date			: May 25, 2021
  =====================================
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
#include "i2c-lcd.h"
#include "my_lib.h"

/* Private variables ---------------------------------------------------------*/
int number[5];
char tab[5];
uint8_t mark, com[20];
uint16_t com_len;

//interfaces 
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;
uint16_t dev=0x52;
int status=0;
volatile int IntCount;
#define isInterrupt 1

//development board 
int temp_val = 1;
int my_time = 300 , average = 0 ;;
int my_data[5];
int freq_disp = 0;
uint8_t set_my_flag = 0, flagaa = 1 , multiplier = 1;
int squeaking_time  = 500, stop_buzzer = 0;


void SystemClock_Config(void);


/**
  * The application entry point.
  * 
  */
int main(void)
{
	 char buffer[16];
	 int flag=0;
	 uint8_t byteData, sensorState=0;
	 uint16_t wordData;
	 uint8_t ToFSensor = 1; // 0=Left, 1=Center(default), 2=Right
	 uint16_t Distance;
	 uint16_t SignalRate;
	 uint16_t AmbientRate;
	 uint16_t SpadNum;
	 uint8_t RangeStatus;
	 uint8_t dataReady;

  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();

  //LCD INICIALIZATION ------------------
  lcd_init();
  HAL_Delay(200);
  lcd_send_string("Technika");
  lcd_put_cur(1, 0);
  HAL_Delay(50);
  lcd_send_string("Mikroprocesorowa");
  HAL_Delay(1000);
  lcd_put_cur(1, 0);
  HAL_Delay(3000);
  lcd_clear();
  lcd_put_cur(0, 0);
  lcd_send_string("Start in 3...");
  HAL_Delay(3000);
  lcd_clear();
  HAL_Delay(50);
  lcd_clear();
  HAL_Delay(50);
  lcd_put_cur(0, 0);
  HAL_Delay(50);
  lcd_send_string("DISTANCE");
  lcd_send_string("Click 1 to standard");
  HAL_Delay(50);
  lcd_put_cur(1, 0);
  HAL_Delay(50);
  lcd_send_string("Click 2 to average");
  HAL_Delay(50);
  //LCD END INICIALIZATION ------------------ 

  XNUCLEO53L1A1_Init();
  ToFSensor = 1; // Select ToFSensor: 0=Left, 1=Center, 2=Right
  status = XNUCLEO53L1A1_ResetId(ToFSensor, 0); // Reset ToF sensor
  HAL_Delay(2);
  status = XNUCLEO53L1A1_ResetId(ToFSensor, 1); // Reset ToF sensor
  HAL_Delay(2);
  HAL_UART_Receive_IT(&huart2, &mark, 1);
  status = VL53L1X_SensorInit(dev);
  status = VL53L1X_SetDistanceMode(dev, 2); /* 1=short, 2=long */
  status = VL53L1X_SetTimingBudgetInMs(dev, 100);
  status = VL53L1X_SetInterMeasurementInMs(dev, 100);
  status = VL53L1X_StartRanging(dev); /* This function has to be called to enable the ranging */


  //read and disp data
  while (1){
	  while(dataReady==0){
		  status = VL53L1X_CheckForDataReady(dev, &dataReady);
		  HAL_Delay(2);
	  }

	  dataReady = 0;
	  status = VL53L1X_GetDistance(dev, &Distance);
	  status = VL53L1X_ClearInterrupt(dev); 					// DEV - sensor address - 0x52
	 
	 //WRITE_DATA(&Distance);
	  digit_con(Distance,number);
	  char_con(number,tab);

	/*
	  HAL_Delay(50);
	  lcd_put_cur(1, 0);
	  HAL_Delay(50);
	  lcd_my_send(tab);
	  HAL_Delay(50);
	  */

	  if (set_my_flag == 2){
		  if (flagaa ==1){
		        flagaa =2;
		        my_data[0]=Distance;
		    }else if (flagaa==2){
		        flagaa=3;
		        my_data[1]=Distance;
		    }else if(flagaa==3){
		        flagaa=4;
		        my_data[2]=Distance;

		    }else if (flagaa==4){
		        flagaa =5;
		        my_data[3]=Distance;
		    }else if (flagaa==5){
		    	my_data[4]=Distance;
		        flagaa=1;
	 		 }
		}


	  switch (set_my_flag){
	  case 1:
		  if (stop_buzzer == 1)
		  {
			  lcd_clear();
			  HAL_Delay(50);
			  lcd_put_cur(0, 0);
			  HAL_Delay(50);
			  my_buzzer();
			  lcd_send_string("DISTANCE");
			  HAL_Delay(50);
			  stop_buzzer = 0 ;
		  }
		  char_con(Distance,number);
		  digit_con(number,tab);
		  lcd_put_cur(1, 0);
		  HAL_Delay(50);
		  lcd_my_send(tab);
		  HAL_Delay(freq_disp);
		  break;

	  case 2:
		  if (stop_buzzer == 1)
		  	  {
			  	  my_buzzer();
			  	lcd_clear();
			  	HAL_Delay(50);
			  	lcd_put_cur(0, 0);
			  	HAL_Delay(50);
			  	lcd_send_string("TRYB SREDNIA");
			  	  stop_buzzer = 0 ;
		  	  }
		  calc_average(my_data, &average);
		  digit_con1(average,number);
		  czar_con(number,tab);
		  lcd_put_cur(1, 0);
		  HAL_Delay(50);
		  lcd_my_send(tab);
		  HAL_Delay(100);
		  break;

	  default:
		  break;
	  }
 	  //dl_odl=sprintf(buffer,"Distance %d",Distance);
	  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sprintf(buffer, "%d\n", Distance), 500);
	  //printf("%u, %u, %u, %u, %u\n", RangeStatus, Distance, SignalRate, AmbientRate,SpadNum);
  }
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	/* Callback - activated by a button */

	if (GPIO_Pin == GPIO_PIN_5){
		// STANDARD MODE
		stop_buzzer =1;
		set_my_flag=1;
		if (freq_disp == 1000){
				freq_disp = 0;
				multiplier =1;
		}
		freq_disp=(freq_disp +100)*multiplier;
		multiplier = 2;
	}
	else{
		// AVERAGE MODE
		stop_buzzer = 1;
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
		set_my_flag = 2;
	}
}