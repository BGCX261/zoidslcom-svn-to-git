
#include "kbc.h"
#include "ints.h"

int  write_kbc_data( unsigned data) {
	return -1;
}

int  read_kbc(void) {
	int time = 0;
	while (time < KBC_TIMEOUT) {
		int status = inportb(STAT_REG);
		if ((status & OBF) == 1) {
			if ((status & (TO_ERR | PAR_ERR)) != 0)
				return -1;
			return inportb(DATA_REG);
		}
		time++;
		mili_sleep(1);
	}
	return -1;
}

int  write_kbc(unsigned adr, unsigned data) {
	int time = 0;
	while (time < KBC_TIMEOUT) {
		int status = inportb(STAT_REG);
		if ((status & IBF) == 0) {
			outportb(adr, data);
			return 0;
		}
		time++;
		mili_sleep(1);
	}
	return -1;
}

void blink_leds(void) {
	blink_led(1);
	blink_led(2);
	blink_led(3);
}

void blink_led(uchar led) {
	int ledOn = 0;
	switch(led){
		case 1:
			ledOn = NUM_LOCK;
			break;
		case 2:
			ledOn = CAPS_LOCK;
			break;
		case 3:
			ledOn = SCROLL_LOCK;
			break;
	}

	disable_irq(KBD_IRQ);
	write_kbc(DATA_REG, WriteLEDS);
	write_kbc(DATA_REG, ledOn);
	mili_sleep(400);
	write_kbc(DATA_REG, WriteLEDS);
	write_kbc(DATA_REG, 0);
	enable_irq(KBD_IRQ);
}
