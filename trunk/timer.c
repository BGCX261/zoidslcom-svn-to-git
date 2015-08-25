#include "timer.h"

void timer_init(int timer, int mode)
{

outportb(TIMER_CTRL, (timer<<6) | mode);

}

void timer_load(int timer, int value)
{

outportb(TIMER_0 | timer, LSB(value));
outportb(TIMER_0 | timer, MSB(value));

}

void mili_sleep(int mili)
{
	int time_max = time_tick + mili;
	while(time_max >= time_tick);
}

void sec_sleep(int secs)
{
	mili_sleep(secs*1000);
}
