#include "rtc.h"
#include <dos.h>
#include <pc.h>

static Bool bcd_flag = 0;

static _go32_dpmi_seginfo old_irc_irq;

void rtc_int_enable()
{
	write_rtcv(RTC_STAT_B, read_rtcv(RTC_STAT_B) | RTC_PIE | RTC_AIE | RTC_UIE);
	unmask_pic(RTC_IRQ);
}

void rtc_int_disable()
{
	mask_pic(RTC_IRQ);
	write_rtcv(RTC_STAT_B, read_rtcv(RTC_STAT_B) & ~(RTC_PIE | RTC_AIE | RTC_UIE));
}

Byte bcd2dec(Byte i)
{
	if (bcd_flag)
		return (i & 0x0F) + 10 * ((i >> 4) & 0x0F);
	else return i;
}

Byte dec2bcd(Byte i)
{
    Byte bcd = 0;

    bcd = i/10;
    bcd <<= 4;       

    bcd += i%10;
	
	if (bcd_flag)
		return bcd;
	else return i;
}

void rtc_valid()
{
	Byte old_pic1 = inportb(PIC1_MASK);
	Byte old_pic2 = inportb(PIC2_MASK);
	
	outportb(PIC1_MASK, 0xFF);
	outportb(PIC2_MASK, 0xFF);
	
    outportb(RTC_ADDR_REG, RTC_STAT_A);

    while (inportb(RTC_DATA_REG) & RTC_UIP)
        ;

    outportb(PIC1_MASK, old_pic1);
	outportb(PIC2_MASK, old_pic2);
}

Byte read_rtc(Byte add)
{
    outportb(RTC_ADDR_REG, add);
    Byte value = inportb(RTC_DATA_REG);
    return value;
}

Byte read_rtcv(Byte add)
{
    rtc_valid();
    return read_rtc(add);
}

void write_rtc(Byte add, Byte value)
{
    outportb(RTC_ADDR_REG, add);
    outportb(RTC_DATA_REG, value);
}

void write_rtcv(Byte add, Byte value)
{
    rtc_valid();
    write_rtc(add, value);
}

void rtc_read_time(RTC_TIME *rt)
{
    rt->sec = bcd2dec(read_rtcv(SEC));
    rt->min = bcd2dec(read_rtcv(MIN));
    rt->hour = bcd2dec(read_rtcv(HOUR));
}

void rtc_read_date(RTC_DATE *rd)
{
    rd->day = bcd2dec(read_rtcv(MONTH_DAY));
    rd->month = bcd2dec(read_rtcv(MONTH));
    rd->year = bcd2dec(read_rtcv(YEAR));
}

void rtc_read_alarm(RTC_TIME *rt)
{
    rt->sec = bcd2dec(read_rtcv(SEC_ALARM));
    rt->min = bcd2dec(read_rtcv(MIN_ALARM));
    rt->hour = bcd2dec(read_rtcv(HOUR_ALARM));
}
void rtc_write_alarm(RTC_TIME *rt)
{
    write_rtcv(SEC_ALARM, dec2bcd(rt->sec));
    write_rtcv(MIN_ALARM, dec2bcd(rt->min));
    write_rtcv(HOUR_ALARM, dec2bcd(rt->hour));
} 

void install_rtc(void (*irq_func)(void))
{
	install_c_irq_handler(RTC_IRQ, irq_func, &old_irc_irq);	
	
	//rtc_init();
	rtc_int_enable();
	write_rtc(RTC_STAT_B, (read_rtc(RTC_STAT_B) & 0x8F) | RTC_PIE);
	read_rtc(RTC_STAT_C);
}

void reinstall_rtc()
{
	mask_pic(RTC_IRQ);
	
	Byte stat_b = read_rtcv(RTC_STAT_B);
	stat_b = stat_b & ~(RTC_PIE);
	write_rtcv(RTC_STAT_B, stat_b);
	
	reinstall_c_irq_handler(RTC_IRQ, &old_irc_irq);
}
