/*
 * fsm.c
 *
 *  Created on: Nov 18, 2023
 *      Author: Admin
 */

#include "fsm.h"

//#include "main.h"
//#include "stdio.h"
//#include "softwaretimer.h"

#define MAX_BUFFER_SIZE  30
// Buffer to store received data
ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;

char response[20];
uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

//enum CommandState{
//	IDLE,
//	RECEIVING,
//};

int command_flag = 0;
uint32_t command_data = 0;
uint32_t ADC_Value =0;
//enum CommandState STATE = IDLE;

uint32_t previous=0; 	// to store the previous value when user type OK
int state = START;
//void command_parser_fsm(){
//	switch(STATE){
//		case IDLE:
//			if(buffer_flag == 1){
//				STATE = RECEIVING;
//			}
//			break;
//		case RECEIVING:
//			if(buffer[index_buffer-1] == '!') STATE = 2;
//			break;
////			if(buffer[index_buffer-1] == '#'){
////				if(buffer[index_buffer -2] == 'T' && buffer[index_buffer -3] == 'S' && buffer[index_buffer -4] == 'R' && buffer[index_buffer -5] == '!' ){
////					command_flag = 1;
//////					command_data = GetCommandData();
////					command_data =HAL_ADC_GetValue(&hadc1);
////					STATE = IDLE;
////				}else if (buffer[index_buffer -2] == 'K' && buffer[index_buffer -3] == 'O' && buffer[index_buffer -4] == '!' ){
////					command_flag = 0;
//////					HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
//////					command_data = GetCommandData();
////					command_data =HAL_ADC_GetValue(&hadc1);
////					STATE = IDLE;
////				}else STATE = IDLE;
////			}else STATE = IDLE;
////			break;
//		case 2:
//			// 'R' or 'O' character
//			if(buffer[index_buffer-1] == 'R') STATE = 3;
//			else if(buffer[index_buffer-1] == 'O') STATE = 4;
//			else STATE = IDLE;
//			break;
//		case 3:
//			if(buffer[index_buffer-1] == 'S') STATE = 5;
//			else STATE = IDLE;
//			break;
//		case 5:
//			if(buffer[index_buffer-1] == 'T') STATE = 6;
//			else STATE = IDLE;
//		case 6:
//			if(buffer[index_buffer-1] == '#'){
//				command_flag = 1;
//				STATE = IDLE;
//			}else STATE = IDLE;
//			break;
//		case 4:
//			if(buffer[index_buffer-1] == 'K') STATE = 7;
//			else STATE = IDLE;
//			break;
//		case 7:
//			if(buffer[index_buffer-1] == '#'){
//				// OK = stop sending
//				command_flag = 0;
//				STATE = IDLE;
//			}else STATE = IDLE;
//			break;
//		default:
//			STATE =IDLE;
//			break;
//	}
//}

void command_parser_fsm(){
	switch(state){
		case START:
			if(buffer[index_buffer - 1] == START_STRING) state = FIRST_CMD;
			break;
		case FIRST_CMD:
			if(buffer[index_buffer - 1] == 'R'){
				state = GOT_S;
			}
			else if(buffer[index_buffer - 1] == 'O'){
				state = GOT_K;
			}
			else state = START;
			break;
		case GOT_S:
			if(buffer[index_buffer - 1] == 'S'){
				state = GOT_T;
			}
			else state = START;
			break;
		case GOT_T:
			if(buffer[index_buffer - 1] == 'T'){
				state = SEND;
			}
			else state = START;
			break;
		case GOT_K:
			if(buffer[index_buffer - 1] == 'K'){
				state = HALT;
			}
			else state = START;
			break;
		case SEND:
			if(buffer[index_buffer - 1] == END_STRING){
				command_flag = SENDING;
				state = START;
			}
			else state = START;
			break;
		case HALT:
			// !OK#
			if(buffer[index_buffer - 1] == END_STRING){
				command_flag = STOP_SEND;
				setTimer1(300);
				state = START;
			}
			break;

		default:
			state = START;
			break;
		}
}
void uart_communication_fsm(){
//	if(timerflag2 == 1){
//		if(command_flag == SENDING){
////			HAL_UART_Transmit(&huart2, (void *)response, sprintf(response, "!ADC= %ld# \r\n", command_data), 1000);
//			HAL_UART_Transmit(&huart2, (void *)response, sprintf(response, "!ADC= %ld# \r\n", ADC_Value), 1000);
//			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
//			setTimer2(300);
//		}
//		if(command_flag == STOP_SEND){
//			HAL_UART_Transmit(&huart2, (void *)response, sprintf(response, "!OK_Value= %ld# \r\n", ADC_Value), 1000);
//			setTimer2(300);
//		}
//	}
	switch(command_flag){
	case SENDING:
		if(timerflag2 == 1){
			previous = ADC_Value;
			HAL_UART_Transmit(&huart2, (void *)response, sprintf(response, "!ADC= %ld# \r\n", ADC_Value), 1000);
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			setTimer2(100);
		}
		break;
	case STOP_SEND:
		if(timerflag1 == 1){
			HAL_UART_Transmit(&huart2, (void *)response, sprintf(response, "!OK_Value= %ld# \r\n", previous), 1000);
			command_flag = IDLE;
		}
		break;
	default:
		break;
	}
}

