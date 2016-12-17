/*
 * pong.c
 *
 *  Created on: Dec 17, 2016
 *      Author: Ata
 */

/*
To compile & upload in Windows command box:

avr-gcc -mmcu=atmega644p -DF_CPU=12000000 -Wall -Os pong.c -o prog.elf
avr-objcopy -O ihex prog.elf prog.hex
avrdude -c usbasp -p m644p -U flash:w:prog.hex

 */

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "../pictor/pictor.h"
#include "../pictor/fonts/Mash.h"

const uint16_t BACKGROUND = BLACK;

typedef struct {
	point TOP;
	point BOT;
	int8_t VEL_X;
	int8_t VEL_Y;
} animated_box;

//Draws the next frame of the animated box
void drawAniBox(animated_box * box, uint16_t colour);


int main() {

	pictorInit(0);

	//ball initialisation
	animated_box ball = {{100  , 160    },
			            { 100+5, 160 + 5},1,1};

	for(;;_delay_ms(4)) {

		//collision logic
		if(ball.TOP.X == UNIT_POINT.X || ball.BOT.X == PICTOR_MAX.X)
			ball.VEL_X *= -1;
		if(ball.TOP.Y == UNIT_POINT.Y || ball.BOT.Y == PICTOR_MAX.Y)
			ball.VEL_Y *= -1;

		//graphic logic
		drawAniBox(&ball, WHITE);
	}

	return 0;
}

void drawAniBox(animated_box * box, uint16_t colour){
	pictorDrawBox(box->TOP,box->BOT,BACKGROUND);
	box->TOP.X += box->VEL_X;
	box->TOP.Y += box->VEL_Y;

	box->BOT.X += box->VEL_X;
	box->BOT.Y += box->VEL_Y;
	pictorDrawBox(box->TOP,box->BOT,colour);
}
