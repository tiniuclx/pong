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

PD0 makes paddle go left
PD1 makes paddle go right

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
int rectCollision(animated_box box1, animated_box box2);

int main() {

	pictorInit(0);

	//ball initialization
	animated_box ball = {{100  , 10    },
			            { 100+5, 10 + 5},1,1};
	//red paddle initialization
	animated_box paddle1 = { {120-20, 300  },
							 {120+20, 300+5},0,0};

	//PD0 and PD1 are configured as active-low inputs, they are connected to
	//buttons which pull their value down to ground
	DDRD  = 0x00;
	PORTD = 0xFF;

	for(;;_delay_ms(4)) {

		//paddle movement logic
		if      ( !(PIND & _BV(PD0)) && paddle1.TOP.X > UNIT_POINT.X)
			paddle1.VEL_X = -1;
		else if ( !(PIND & _BV(PD1)) && paddle1.BOT.X < PICTOR_MAX.X)
			paddle1.VEL_X = 1;
		else
			paddle1.VEL_X = 0;


		//ball collision logic
		if(ball.TOP.X == UNIT_POINT.X || ball.BOT.X == PICTOR_MAX.X)
			ball.VEL_X *= -1;
		if(ball.TOP.Y == UNIT_POINT.Y || ball.BOT.Y == PICTOR_MAX.Y || rectCollision(ball,paddle1))
			ball.VEL_Y *= -1;
		//bug: if it hits the sides of the paddle it gets stuck inside of it until
		//it leaves out the other side

		//graphic logic
		drawAniBox(&ball, WHITE);
		drawAniBox(&paddle1, BLUE);
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

int rectCollision(animated_box box1, animated_box box2){
	if(box1.BOT.X > box2.TOP.X &&
	   box1.TOP.X < box2.BOT.X &&
	   box1.TOP.Y < box2.BOT.Y &&
	   box1.BOT.Y > box2.TOP.Y  )
		return 1;
	else
		return 0;
}
