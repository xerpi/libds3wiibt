#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <ds3wiibt.h>
#include "utils.h"

//Controller's MAC 1 : 00:24:33:63:4B:6B
//Controller's MAC 2 : 00:06:F7:4F:97:5C
struct bd_addr addr = {.addr = {0x5C, 0x97, 0x4F, 0xF7, 0x06, 0x00}};
static void print_data(struct ds3wiibt_input *inp);
static void conn_cb(void *usrdata);
static void discon_cb(void *usrdata);

struct ds3wiibt_context ctx;

int main(int argc, char *argv[])
{
	fatInitDefault();
	init();
	WPAD_Init();
	printf("ds3wiibt sample by xerpi\n");

	ds3wiibt_initialize(&ctx, &addr);
	ds3wiibt_set_userdata(&ctx, NULL);
	ds3wiibt_set_connect_cb(&ctx, conn_cb);
	ds3wiibt_set_disconnect_cb(&ctx, discon_cb);

	printf("Listening to: ");
	print_mac((struct bd_addr*)addr.addr);
	printf("Listening for an incoming connection...\n");
	ds3wiibt_listen(&ctx);

	while (run) {
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if (pressed & WPAD_BUTTON_HOME) run = 0;
		
		if (pressed & WPAD_BUTTON_B) {
			ds3wiibt_disconnect(&ctx);
		}
		
		if (pressed & WPAD_BUTTON_1) {
			ds3wiibt_listen(&ctx);
		}
		
		if (ds3wiibt_is_connected(&ctx)) {
			print_data(&ctx.input);
			if (ctx.input.PS && ctx.input.start) ds3wiibt_disconnect(&ctx);
		}
		
		flip_screen();
	}
	ds3wiibt_disconnect(&ctx);
	return 0;
}

void conn_cb(void *usrdata)
{
	printf("Controller connected.\n");
}

void discon_cb(void *usrdata)
{
	printf("Controller disconnected.\n");
	//ds4wiibt_listen(&ctx); //Listen again?
}

void print_data(struct ds3wiibt_input *inp)
{ 
	printf("\x1b[10;0H");
	printf("\n\nPS: %i   START: %i   SELECT: %i   /\\: %i   []: %i   O: %i   X: %i   L3: %i   R3: %i\n", \
		inp->PS, inp->start, inp->select, inp->triangle, \
		inp->square, inp->circle, inp->cross, inp->L3, inp->R3);

	printf("L1: %i   L2: %i   R1: %i   R2: %i   UP: %i   DOWN: %i   RIGHT: %i   LEFT: %i\n", \
		inp->L1, inp->L2, inp->R1, inp->R2, \
		inp->up, inp->down, inp->right, inp->left);

	printf("LX: 0x%02X   LY: 0x%02X   RX: 0x%02X   RY: 0x%02X\n", \
		inp->leftX, inp->leftY, inp->rightX, inp->rightY);

	printf("aX: 0x%04X   aY: 0x%04X   aZ: 0x%04X   Zgyro: 0x%04X\n", \
		inp->accelX, inp->accelY, inp->accelZ, inp->gyroZ);

	printf("L1 sens: 0x%02X   L2 sens: 0x%02X   R1 sens: 0x%02X   R2 sens: 0x%02X\n", \
		inp->L1_sens, inp->L2_sens, inp->R1_sens, inp->R2_sens);

	printf("/\\ sens: 0x%02X   [] sens: 0x%02X   O sens: 0x%02X   X sens: 0x%02X\n",
		inp->triangle_sens, inp->square_sens, inp->circle_sens, inp->cross_sens);

	printf("UP: 0x%02X   DOWN: 0x%02X   RIGHT: 0x%02X   LEFT: 0x%02X\n", \
		inp->up_sens, inp->down_sens, inp->right_sens, inp->left_sens);
	printf("Timestamp: %llu\n", ctx.timestamp);
}
