/*
 * softwaretimer.c
 *
 *  Created on: Nov 18, 2023
 *      Author: Admin
 */


int timerflag1 =0;
int timerflag2 =0;

int timer1_counter;
int timer2_counter;
void setTimer1(int duration){
	timer1_counter = duration;
	timerflag1 =0;
}
void setTimer2(int duration){
	timer2_counter = duration;
	timerflag2 =0;
}
void timerRun(){
	timer1_counter--;
	timer2_counter--;
	if(timer1_counter <= 0){
		timerflag1 = 1;
	}
	if(timer2_counter <= 0){
		timerflag2 = 1;
	}
}
