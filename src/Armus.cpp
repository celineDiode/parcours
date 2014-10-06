/*
============================================================================
 Name : Avancer.cpp
 Author :
 Version :
 Description : Demo - Exe source file
============================================================================
*/

// Include Files
#include <libarmus.h>
#include <stdint.h>

const short PAUSE = 500;
const short TIMEOUT = 50;//25;
const short SUBDIVISIONS = 10;//20;
const short TICK_PER_ROTATION = 64;
const short CM_PER_ROTATION = 24;
const float PI = 3.1415926535897932384626;
const float CM_BETWEEN = 14.0;//13.8;

void move_cm(short, short);
void move_ticks(short, short);
void turn_degrees(short, short);

int main()
{
	short move_speed = 75;
	short turn_speed = 40;

	THREAD_MSleep(500);

	move_cm(220, move_speed);		THREAD_MSleep(PAUSE);

	turn_degrees(90, -turn_speed);	THREAD_MSleep(PAUSE);

	move_cm(50, move_speed);		THREAD_MSleep(PAUSE);

	turn_degrees(90, turn_speed);	THREAD_MSleep(PAUSE);

	move_cm(45, move_speed);		THREAD_MSleep(PAUSE);

	turn_degrees(90, turn_speed);	THREAD_MSleep(PAUSE);

	move_cm(50, move_speed);		THREAD_MSleep(PAUSE);

	turn_degrees(90, -turn_speed);	THREAD_MSleep(PAUSE);

	move_cm(30, move_speed);		THREAD_MSleep(PAUSE);

	turn_degrees(45, turn_speed);	THREAD_MSleep(PAUSE);

	move_cm(45, move_speed);		THREAD_MSleep(PAUSE);

	turn_degrees(90, -turn_speed);	THREAD_MSleep(PAUSE);

	move_cm(70, move_speed);		THREAD_MSleep(PAUSE);//Moins loin!

	turn_degrees(45, turn_speed);	THREAD_MSleep(PAUSE);

	move_cm(40, move_speed);		THREAD_MSleep(PAUSE);

	turn_degrees(22, turn_speed);	THREAD_MSleep(PAUSE);

	move_cm(100, move_speed);

	return 0;
}

void move_cm(short cm, short ticks_per_second)
{
	move_ticks((cm * TICK_PER_ROTATION) / CM_PER_ROTATION, ticks_per_second);
}

void move_ticks(short ticks, short ticks_per_second)
{
	short ticks_per_second_wanted = (ticks_per_second * TIMEOUT * SUBDIVISIONS) / 1000;

	short total_left = 0;
	short total_right = 0;

	short speed_left = 50;//ticks_per_second;
	short speed_right = 50;//ticks_per_second;

	ENCODER_Read(ENCODER_LEFT);
	ENCODER_Read(ENCODER_RIGHT);

	while(1)
	{
		MOTOR_SetSpeed(MOTOR_LEFT,  speed_left);
		MOTOR_SetSpeed(MOTOR_RIGHT, speed_right);

		short progression_left = 0;
		short progression_right = 0;

		for(short i = 0; i < SUBDIVISIONS; i++)
		{
			THREAD_MSleep(TIMEOUT);

			int temp_left  = ENCODER_Read(ENCODER_LEFT);
			int temp_right = ENCODER_Read(ENCODER_RIGHT);

			progression_left  += temp_left;
			progression_right += temp_right;

			total_left  += temp_left;
			total_right += temp_right;

			/*LCD_ClearAndPrint("speed : %3d   %3d\n", speed_left, speed_right);
				   LCD_Printf("total : %3d   %3d\n", total_left, total_right);
				   LCD_Printf("ticks : %3d\n", ticks);*/

			if(total_left >= ticks || total_right >= ticks)
			{
				MOTOR_SetSpeed(MOTOR_LEFT, 	0);
				MOTOR_SetSpeed(MOTOR_RIGHT, 0);

				return;
			}
		}

		speed_left  += (ticks_per_second_wanted - progression_left);
		speed_right += (ticks_per_second_wanted - progression_right);
	}
}

void turn_degrees(short degrees, short speed)
{
	short ticks_wanted = (degrees * CM_BETWEEN * PI * TICK_PER_ROTATION) / (CM_PER_ROTATION * 360);

	short ticks_left = 0;
	short ticks_right = 0;

	ENCODER_Read(ENCODER_LEFT);
	ENCODER_Read(ENCODER_RIGHT);

	MOTOR_SetSpeed(MOTOR_LEFT,  speed);
	MOTOR_SetSpeed(MOTOR_RIGHT, -speed);

	do
	{
		THREAD_MSleep(TIMEOUT);

		ticks_left  += ENCODER_Read(ENCODER_LEFT);
		ticks_right += ENCODER_Read(ENCODER_RIGHT);

	} while(ticks_left < ticks_wanted && ticks_right < ticks_wanted);

	MOTOR_SetSpeed(MOTOR_LEFT, 	0);
	MOTOR_SetSpeed(MOTOR_RIGHT, 0);
}
