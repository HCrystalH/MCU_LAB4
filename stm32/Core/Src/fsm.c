/*
 * fsm.c
 *
 *  Created on: Nov 18, 2023
 *      Author: Admin
 */

#include "main.h"
#include "stdio.h"
#include "softwaretimer.h"

#define MAX_BUFFER_SIZE  30
// Buffer to store received data
ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;

char response[20];
uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;
enum CommandState{
	IDLE,
	RECEIVING,
};

uint8_t command_flag = 0;
uint32_t command_data = 0;

enum CommandState STATE = IDLE;
void command_parser_fsm(){
	switch(STATE){
		case IDLE:
			if(buffer_flag == 1){
				STATE = RECEIVING;
			}
			break;
		case RECEIVING:

			if(buffer[index_buffer-1] == '#'){
				if(buffer[index_buffer -2] == 'T' && buffer[index_buffer -3] == 'S' && buffer[index_buffer -4] == 'R' && buffer[index_buffer -5] == '!' ){
					command_flag = 1;
					command_data = GetCommandData();
					STATE = IDLE;
				}else if (buffer[index_buffer -2] == 'K' && buffer[index_buffer -3] == 'O' && buffer[index_buffer -4] == '!' ){
					command_flag = 2;
					HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
					command_data = HAL_ADC_GetValue(&hadc1);

					STATE = IDLE;
				}

			}
			break;
		default:
//			STATE =IDLE;
			break;
	}
}

uint8_t GetStringLength(uint8_t* buffer){
	uint8_t len =0;
	if(*buffer == 0){
		return 0;
	}
	while(*buffer !=0){
		len++;
		if(*buffer =='\n') break;
		buffer++;
	}
	return len;
}

uint8_t state = 0;
void uart_communication_fsm(){

	switch(state){
	//0: IDLE
		case 0:
			if(command_flag == 1)	state = 1;
			else if(command_flag == 2) state =2;

			break;
		case 1:
			// RST
			command_flag =0;
			// Process data

//			uint8_t response[20];
//			printf("HelloWorld!");
			setTimer1(100);
			HAL_UART_Transmit(&huart2, (void *)response, sprintf(response, "!ADC= %ld# \r\n", command_data), 1000);
			state =0;
			break;
		case 2:
			// OK
			command_flag =0;
			//Process data
//			uint8_t response[20];
//			HAL_UART_Transmit(&huart2, (void *)response, sprintf(response, "!ADC= %ld# \r\n", command_data), 1000);
			setTimer2(300);
			if(timerflag2 == 1){
				HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
				setTimer2(100);
			}
			state=0;
			break;
		default:
			break;
	}
}

