/*
 * fsm.c
 *
 *  Created on: Nov 18, 2023
 *      Author: Admin
 */

#include "main.h"
#include "stdio.h"
#include "stdint.h"
#define RX_BUFFER_SIZE 30
#define MAX_BUFFER_SIZE  30
// Buffer to store received data
ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;

char rx_buffer[RX_BUFFER_SIZE];
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
			if(buffer[index_buffer -1] == '#'){
				if(buffer[index_buffer -2] == 'T' &&buffer[index_buffer -3] == 'S' && buffer[index_buffer -4] == 'R' && buffer[index_buffer -2] == '!' ){
					command_flag = 1;
					command_data = HAL_ADC_GetValue(&hadc1);
					STATE = IDLE;
				}else if (buffer[index_buffer -2] == 'K' && buffer[index_buffer -3] == 'O' && buffer[index_buffer -4] == '!' ){
					command_flag = 2;
					command_data = HAL_ADC_GetValue(&hadc1);
					STATE = IDLE;
				}
			}
			break;
		default:
			STATE =IDLE;
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

uint8_t receiveBufferIndexTail;
uint8_t receiveBufferIndexHead;
uint8_t Uart1_Received_Buffer_Available(void){
	if(receiveBufferIndexTail != receiveBufferIndexHead) return 1;
	return 0;
}

// We already have UART transmit and receive functions
void uart_communication_fsm(){
	static uint8_t state = 0;
	switch(state){
		case 0:
			if(command_flag == 1)	state = 1;
			else if(command_flag == 2) state =2;

			break;
		case 1:
			command_flag =0;
			// Process data

//			uint8_t response[20];
			printf("HelloWorld!");
			break;
		case 2:
			command_flag =0;
			//Process data
//			uint8_t response[20];
			printf("HelloWorld!");
			break;
		default:
			break;
	}
}

