#ifndef DS3WIIBT_H
#define DS3WIIBT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <gccore.h>
#include "l2cap.h"

enum ds3wiibt_status {
	DS3WIIBT_STATUS_DISCONNECTED,
	DS3WIIBT_STATUS_LISTENING,
	DS3WIIBT_STATUS_CONNECTING,
	DS3WIIBT_STATUS_CONNECTED,
	DS3WIIBT_STATUS_DISCONNECTING
};

struct ds3wiibt_input {
	unsigned char HID_data;
	unsigned char unk0;
	
	struct {
		unsigned char left     : 1;
		unsigned char down     : 1;
		unsigned char right    : 1;
		unsigned char up       : 1;
		unsigned char start    : 1;
		unsigned char R3       : 1;
		unsigned char L3       : 1;
		unsigned char select   : 1;

		unsigned char square   : 1;
		unsigned char cross    : 1;
		unsigned char circle   : 1;
		unsigned char triangle : 1;
		unsigned char R1       : 1;
		unsigned char L1       : 1;
		unsigned char R2       : 1;
		unsigned char L2       : 1;

		unsigned char not_used : 7;
		unsigned char PS       : 1;
	};
	
	unsigned char unk1;
	
	unsigned char leftX;
	unsigned char leftY;
	unsigned char rightX;
	unsigned char rightY;

	unsigned int unk2;

	struct {
		unsigned char up_sens;
		unsigned char right_sens;
		unsigned char down_sens;
		unsigned char left_sens;
	};

	struct {
		unsigned char L2_sens;
		unsigned char R2_sens;
		unsigned char L1_sens;
		unsigned char R1_sens;
	};

	struct {
		unsigned char triangle_sens;
		unsigned char circle_sens;
		unsigned char cross_sens;
		unsigned char square_sens;
	};

	unsigned short unk3;
	unsigned char  unk4;

	unsigned char status;
	unsigned char power_rating;
	unsigned char comm_status;
	unsigned int  unk5;
	unsigned int  unk6;
	unsigned char unk7;
	
	struct {
		unsigned short accelX;
		unsigned short accelY;
		unsigned short accelZ;
		union {
			unsigned short gyroZ;
			unsigned short roll;
		};
	};
} __attribute__((packed, aligned(32)));

typedef void (*ds3wiibt_cb)(void *usrdata);

struct ds3wiibt_context {
	struct l2cap_pcb *ctrl_pcb;
	struct l2cap_pcb *data_pcb;
	struct bd_addr	  bdaddr;
	struct ds3wiibt_input input;
	long long timestamp;
	unsigned char led;
	struct {
		unsigned char duration_right;
		unsigned char power_right;
		unsigned char duration_left;
		unsigned char power_left;
	} rumble;
	void *usrdata;
	ds3wiibt_cb connect_cb;
	ds3wiibt_cb disconnect_cb;
	enum ds3wiibt_status status;
};

void ds3wiibt_initialize(struct ds3wiibt_context *ctx, struct bd_addr *addr);
void ds3wiibt_set_userdata(struct ds3wiibt_context *ctx, void *data);
void ds3wiibt_set_connect_cb(struct ds3wiibt_context *ctx, ds3wiibt_cb cb);
void ds3wiibt_set_disconnect_cb(struct ds3wiibt_context *ctx, ds3wiibt_cb cb);
void ds3wiibt_set_led(struct ds3wiibt_context *ctx, u8 led);
void ds3wiibt_set_rumble(struct ds3wiibt_context *ctx, u8 duration_right, u8 power_right, u8 duration_left, u8 power_left);
void ds3wiibt_send_ledsrumble(struct ds3wiibt_context *ctx);
void ds3wiibt_listen(struct ds3wiibt_context *ctx);
void ds3wiibt_disconnect(struct ds3wiibt_context *ctx);

#define ds3wiibt_is_connected(ctxp) \
	((ctxp)->status == DS3WIIBT_STATUS_CONNECTED)

#define ds3wiibt_is_disconnected(ctxp) \
	((ctxp)->status == DS3WIIBT_STATUS_DISCONNECTED)

#ifdef __cplusplus
}
#endif

#endif
