/*
 * fsm.h
 *
 *  Created on: Nov 18, 2023
 *      Author: Admin
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#define MAX_BUFFER_SIZE 30

extern uint8_t temp;
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t buffer_flag;

/*State machine to extract command*/
void command_parser_fsm();
void uart_communication_fsm();
#endif /* INC_FSM_H_ */
