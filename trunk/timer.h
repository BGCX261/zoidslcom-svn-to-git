#ifndef _TIMER_H_
#define _TIMER_H_
#include <pc.h>

/** @defgroup TimerCounter TimerCounter
 * @{
 *
 * Timer-Counter definitions
 */
 
 extern int time_tick;
 extern int sec_tick;

/** @name Timer Address */
/*@{*/
#define TIMER_0      0x40 ///< Adress of Timer0 divider
#define TIMER_1      0x41 ///< Adress of Timer1 divider
#define TIMER_2      0x42 ///< Adress of Timer2 divider
#define TIMER_CTRL   0x43 ///< The address of the timer control register
/*@}*/

/** @name Speaker and Timer2 control and bit meanings */
/*@{*/
#define SPEAKER_CTRL    0x61     ///< The address of a generic PC register, whose bits 0 and 1 control the timer2 clock and output to the speaker
#define TIMER2_ENABLE  (1 << 0) ///< Enable Timer 2 clock
#define SPEAKER_ENABLE (1 << 1) ///< Enable Timer 2 output to speaker
/*@}*/

/** @name Some usefull (and nice) definitions */
/*@{*/
#define TIMER_CLK    1193181 ///< The timers input frequency
#define LCOM_MODE  0x36    ///< Control byte: LSB followed by MSB, mode 3, binary count  
#define LSB(i) ((i) & 0xFF)    ///< The Least Significative Byte of a Word
#define MSB(i) ((i) >> 8)      ///< The Most Significative Byte of a Word
/*@}*/

/** Program timer (0,1,2) with mode
 */
void timer_init(int timer, int mode);

/** Load timer (0,1,2) with value
 */
void timer_load(int timer, int value);

/** Wait mili miliseconds, indirectly counting T0 interrupts
*/
void mili_sleep(int mili);

/** wait secs seconds, indirectly counting T0 interrupts
 */
void sec_sleep(int secs);

/** @} end of TimerCounter */

#endif
