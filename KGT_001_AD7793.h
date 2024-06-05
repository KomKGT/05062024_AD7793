/*
 * KGT_001_AD7793.h
 *
 *  Created on: Jun 1, 2024
 *      Author: kkk
 */

#ifndef INC_KGT_001_AD7793_H_
#define INC_KGT_001_AD7793_H_

#include "main.h"
#include "KGT_002_UART.h"
extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart1;


uint8_t 	KGT_AD7793_Init();
void 		KGT_AD7793_Reset();

uint8_t 	KGT_AD7793_Read_ID_reg();

void 		KGT_AD7793_Write_Config_reg();
void 		KGT_AD7793_Read_Config_reg();

void 		KGT_AD7793_Write_Mode_reg();
void 		KGT_AD7793_Read_Mode_reg();

void 		KGT_AD7793_WriteMode_Single_Conversion();
uint32_t 	KGT_AD7793_Read_Single_Conversion_Data_reg();

uint32_t 	KGT_AD7793_Read_Continuous_Conversion_Data_reg();

#endif /* INC_KGT_001_AD7793_H_ */
