#ifndef _MUSIC_H_
#define _MUSIC_H_

#include <pc.h>
#include <stdio.h>

/*
unsigned char inportb(unsigned short port);

void outportb(unsigned short port, unsigned char data);
*/

/** @defgroup Music Music
 * @{
 *
 * Music related functions
 */

/** Note definition
 */
typedef struct {
	int freq; 	/**< note frequency */
	int dur;	/**< note duration in miliseconds */
} Note;

/** Song definition
 */
typedef struct {
  int lenght;	/**< number of notes */
  int pause;	/**< pause between notes (ms) */
  Note *notes;	/**< pointer to array of notes */
}Song;


int divisor(int op1);

/** Turns the speeker on
 */
void speaker_on();

/** Turns the speeker off
 */
void speaker_off();

/** Plays a single note
 */
void play_note(Note *note);

/** Plays a song using busy-waiting
<pre>
  usage example:

  Note notes[] = {{Sol6, 100}, {Mi6,50}, {Sol6, 50}, {Mi6, 25}}; 
  Song s = { sizeof(notes)/sizeof(Note), 10, notes};
 
  play_song(&s);
</pre>
 */
void play_song(Song *s);

/** Natural notes names.
 * 's' means sharp, e.g. Ds is D#
 * The central octave is the fourth, thus C4 is the "central C"
 * 
 */
enum NotesN { C0, Cs0, D0, Ds0, E0, F0, Fs0, G0, Gs0, A0, As0, B0,
	      C1, Cs1, D1, Ds1, E1, F1, Fs1, G1, Gs1, A1, As1, B1,
	      C2, Cs2, D2, Ds2, E2, F2, Fs2, G2, Gs2, A2, As2, B2,
	      C3, Cs3, D3, Ds3, E3, F3, Fs3, G3, Gs3, A3, As3, B3,
	      C4, Cs4, D4, Ds4, E4, F4, Fs4, G4, Gs4, A4, As4, B4,
	      C5, Cs5, D5, Ds5, E5, F5, Fs5, G5, Gs5, A5, As5, B5,
	      C6, Cs6, D6, Ds6, E6, F6, Fs6, G6, Gs6, A6, As6, B6,
	      C7, Cs7, D7, Ds7, E7, F7, Fs7, G7, Gs7, A7, As7, B7,
	      C8, Cs8, D8, Ds8};

/** French/Italian/Spanish/Portuguese notes names. 
 * 's' means sharp, e.g. Ds is D#
 * The central octave is the fourth, thus Do4 is the "central Do"
 */
enum NotesP { Do0, Dos0, Re0, Res0, Mi0, Fa0, Fas0, Sol0, Sols0, La0, Las0, Si0,
	      Do1, Dos1, Re1, Res1, Mi1, Fa1, Fas1, Sol1, Sols1, La1, Las1, Si1,
	      Do2, Dos2, Re2, Res2, Mi2, Fa2, Fas2, Sol2, Sols2, La2, Las2, Si2,
	      Do3, Dos3, Re3, Res3, Mi3, Fa3, Fas3, Sol3, Sols3, La3, Las3, Si3,
	      Do4, Dos4, Re4, Res4, Mi4, Fa4, Fas4, Sol4, Sols4, La4, Las4, Si4,
	      Do5, Dos5, Re5, Res5, Mi5, Fa5, Fas5, Sol5, Sols5, La5, Las5, Si5,
	      Do6, Dos6, Re6, Res6, Mi6, Fa6, Fas6, Sol6, Sols6, La6, Las6, Si6,
	      Do7, Dos7, Re7, Res7, Mi7, Fa7, Fas7, Sol7, Sols7, La7, Las7, Si7,
	      Do8, Dos8, Re8, Res8};

/** Notes frequencies, in Hz, synchronized with arrays NotesN and NotesP.
 * E.G. frequency[Do4] is the frequency of a central Do, 261.63Hz
 *
 * NOTICE: this array is NOT defined in this header file,
 * as variables should not be declared in header files.
 * You have to copy/paste it to a source file (*.c).
 *
float notes_frequency[] = { 16.35, 17.32, 18.35, 19.45, 20.6, 21.83, 23.12, 24.5, 25.96, 27.5, 29.14, 30.87,
	32.7, 34.65, 36.71, 38.89, 41.2, 43.65, 46.25, 49, 51.91, 55, 58.27, 61.74,
	65.41, 69.3, 73.42, 77.78, 82.41, 87.31, 92.5, 98, 103.83, 110, 116.54, 123.47,
	130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185, 196, 207.65, 220, 233.08, 246.94,
	261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392, 415.3, 440, 466.16, 493.88,
	523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99, 783.99, 830.61, 880, 932.33, 987.77,
	1046.5, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 1760, 1864.66, 1975.53,
	2093, 2217.46, 2349.32, 2489.02, 2637.02, 2793.83, 2959.96, 3135.96, 3322.44, 3520, 3729.31, 3951.07,
	4186.01, 4434.92, 4698.64, 4978.03};
 */
extern float notes_frequency[];

/** @} end of music */

int musicDiv( int nota );

#endif
