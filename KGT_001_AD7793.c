/*
 * KGT_001_AD7793.c
 *
 *  Created on: Jun 1, 2024
 *      Author: kkk
 */

/*
 * CS : PA4
 * RDY/DOUT/MISO : PA6
 * DIN/MOSI : PA7
 * SCLK : PA5
 */
#include "KGT_001_AD7793.h"

static void KGT_AD7793_CS_LOW()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}
static void KGT_AD7793_CS_HIGH()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}
static void KGT_AD7793_waitReadyGotoLow()
{
	while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))
	{
		;
	}
}
uint8_t KGT_AD7793_Init()
{
	if((KGT_AD7793_Read_ID_reg() & 0x0F) == 0x0B)
	{
		return 1;
	}
	return 0;
}
void KGT_AD7793_Reset()
{
	uint8_t ui8_reset[4] = {0xff, 0xff, 0xff, 0xff};
	KGT_AD7793_CS_LOW();
	HAL_SPI_Transmit(&hspi1, ui8_reset, sizeof(ui8_reset), 10);
	KGT_AD7793_CS_HIGH();
}

uint8_t KGT_AD7793_Read_ID_reg()
{
	uint8_t ui8_ID_reg = 0x04;
	uint8_t ui8_Buffer;
	uint8_t ui8_request = (1<<6) | (ui8_ID_reg << 3);
	KGT_AD7793_CS_LOW();
	KGT_AD7793_waitReadyGotoLow();
	HAL_SPI_Transmit(&hspi1, &ui8_request, 1, 10);
	HAL_SPI_Receive(&hspi1, &ui8_Buffer, 1, 10);
	KGT_AD7793_CS_HIGH();
	/*send UART*/
	return ui8_Buffer;
}
void KGT_AD7793_Write_Config_reg()
{
	uint8_t ui8_Config_reg = 0x02;
	uint8_t ui8_Config_command[2] = {0x16,0x90};// Unipolar,Gain 64,Internal Ref,Enable Buffer,CH0
	uint8_t ui8_Comunicate_reg = (0<<6) | (ui8_Config_reg << 3);
	KGT_AD7793_CS_LOW();
	HAL_SPI_Transmit(&hspi1,&ui8_Comunicate_reg , 1, 10);
	HAL_SPI_Transmit(&hspi1, ui8_Config_command, 2, 10);
	KGT_AD7793_CS_HIGH();
}
void KGT_AD7793_Read_Config_reg()
{
	uint8_t ui8_Config_reg = 0x02;
	uint8_t ui8_Buffer[2];
	uint8_t ui8_request = (1<<6) | (ui8_Config_reg << 3);
	KGT_AD7793_CS_LOW();
	KGT_AD7793_waitReadyGotoLow();
	HAL_SPI_Transmit(&hspi1, &ui8_request, 1, 10);
	HAL_SPI_Receive(&hspi1, ui8_Buffer, 2, 10);
	KGT_AD7793_CS_HIGH();
	/*send UART*/
	uint16_t ui16_sum_Buffer = 0;
	ui16_sum_Buffer |= (ui8_Buffer[0]<<8);
	ui16_sum_Buffer |= (ui8_Buffer[1]<<0);
	UART_SEND_TXT(&huart1, "Configuration :", 0);
	UART_SEND_HEX(&huart1, ui16_sum_Buffer, 1);
}
void KGT_AD7793_Write_Mode_reg()
{
	uint8_t ui8_Mode_reg = 0x01;
	uint8_t ui8_write_mode_command_Internal_Scale_Zero_Calibrate[2] = {0x80,0x0f};//8008
	uint8_t ui8_write_mode_command_Internal_Full_Scale_Calibrate[2] = {0xA0,0x0f};//A008
	uint8_t ui8_Comunicate_reg = (0<<6) | (ui8_Mode_reg << 3);
	for(int cycle = 0;cycle<2;cycle++)
	{
		KGT_AD7793_CS_LOW();
		HAL_SPI_Transmit(&hspi1,&ui8_Comunicate_reg , 1, 10);
		HAL_SPI_Transmit(&hspi1, ui8_write_mode_command_Internal_Scale_Zero_Calibrate, 2, 10);
		KGT_AD7793_waitReadyGotoLow();
		KGT_AD7793_CS_HIGH();
	}

	for(int cycle = 0;cycle<4;cycle++)
	{
		KGT_AD7793_CS_LOW();
		HAL_SPI_Transmit(&hspi1,&ui8_Comunicate_reg , 1, 10);
		HAL_SPI_Transmit(&hspi1, ui8_write_mode_command_Internal_Full_Scale_Calibrate, 2, 10);
		KGT_AD7793_waitReadyGotoLow();
		KGT_AD7793_CS_HIGH();
	}
}
void KGT_AD7793_Read_Mode_reg()
{
	uint8_t ui8_Mode_reg = 0x01;
	uint8_t ui8_Buffer[2];
	uint8_t ui8_request = (1<<6) | (ui8_Mode_reg << 3);
	KGT_AD7793_CS_LOW();
	KGT_AD7793_waitReadyGotoLow();
	HAL_SPI_Transmit(&hspi1, &ui8_request, 1, 10);
	HAL_SPI_Receive(&hspi1, ui8_Buffer, 2, 10);
	KGT_AD7793_CS_HIGH();
	/*send UART*/
	uint16_t ui16_sum_Buffer = 0;
	ui16_sum_Buffer |= (ui8_Buffer[0]<<8);
	ui16_sum_Buffer |= (ui8_Buffer[1]<<0);
	UART_SEND_TXT(&huart1, "Mode :", 0);
	UART_SEND_HEX(&huart1, ui16_sum_Buffer, 1);
}
void KGT_AD7793_WriteMode_Single_Conversion()
{
	uint8_t ui8_Mode_reg = 0x01;
	uint8_t ui8_write_mode_command_SingleConv[2] = {0x20,0x0f};
	uint8_t ui8_Comunicate_reg = (0<<6) | (ui8_Mode_reg << 3);
	KGT_AD7793_CS_LOW();
	HAL_SPI_Transmit(&hspi1, &ui8_Comunicate_reg, 1, 10);
	HAL_SPI_Transmit(&hspi1, ui8_write_mode_command_SingleConv, 2, 10);
}
uint32_t KGT_AD7793_Read_Single_Conversion_Data_reg()
{
	uint8_t ui8_Data_reg = 0x03;
	uint8_t ui8_Buffer[3] = {0,0,0};
	uint8_t ui8_request = (1<<6) | (ui8_Data_reg << 3);
	KGT_AD7793_waitReadyGotoLow();
	HAL_SPI_Transmit(&hspi1, &ui8_request, 1, 10);
	HAL_SPI_Receive(&hspi1, ui8_Buffer, 3, HAL_MAX_DELAY);
	KGT_AD7793_CS_HIGH();

	uint32_t ui32_sum_Buffer = 0;
	ui32_sum_Buffer |= (ui8_Buffer[0]<<16);
	ui32_sum_Buffer |= (ui8_Buffer[1]<<8);
	ui32_sum_Buffer |= (ui8_Buffer[2]<<0);
	return ui32_sum_Buffer;
}
uint32_t KGT_AD7793_Read_Continuous_Conversion_Data_reg()
{
	uint8_t ui8_Data_reg = 0x03;
	uint8_t ui8_Buffer[3] = {0,0,0};
	uint8_t ui8_request = (1<<6) | (ui8_Data_reg << 3);
	KGT_AD7793_CS_LOW();
	KGT_AD7793_waitReadyGotoLow();
	HAL_SPI_Transmit(&hspi1, &ui8_request, 1, 10);
	HAL_SPI_Receive(&hspi1, ui8_Buffer, 3, HAL_MAX_DELAY);

	uint32_t ui32_sum_Buffer = 0;
	ui32_sum_Buffer |= (ui8_Buffer[0]<<16);
	ui32_sum_Buffer |= (ui8_Buffer[1]<<8);
	ui32_sum_Buffer |= (ui8_Buffer[2]<<0);
	return ui32_sum_Buffer;
}




