/*
 * fsm.h
 *
 *  Created on: Nov 18, 2023
 *      Author: Admin
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_
#include "main.h"
#include "stdio.h"
#include "softwaretimer.h"
#define MAX_BUFFER_SIZE 30

#define START 0
#define FIRST_CMD 1
#define GOT_S 2
#define GOT_T 3
#define GOT_K 4
#define SEND 5
#define HALT 6

#define SENDING 7
#define STOP_SEND 8
#define IDLE 9

#define START_STRING '!'
#define END_STRING '#'


extern uint8_t temp;
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t buffer_flag;

extern int command_flag;
extern uint32_t command_data;

extern uint32_t ADC_Value;
/*State machine to extract command*/
void command_parser_fsm();
void uart_communication_fsm();
#endif /* INC_FSM_H_ */
