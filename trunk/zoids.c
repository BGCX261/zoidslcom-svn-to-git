#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <dpmi.h>
#include <unistd.h>
#include "rtc.h"
#include "Pixmap.h"
#include "sprite.h"
#include "videog.h"
#include "codepage.h"

#include "list.h"
#include "ints.h"
#include "timer.h"

#include "defines.h"

// TURMA 3    GRUPO 4

#include "queue.h"
#include "music.h"
#include "zoid.h"
#include "ovule.h"
#include "sperm.h"


//Keyboard
Queue keys_queue;
void handler(void);
void kbd_isr(void);
extern int scancode;
char last_sc;

//Gráfica / Sprites
char* base;
char* base2;

list_t *zoids;
list_t *spermicides;
Sprite *ovule;

int espera_zoid = 1000;
int range_min = 2250;
int range_max = 3000;
int last_zoid;

char *zoid_char1 , *zoid_char2;

void cria_zoid();
void cria_spermicide(int x, int y);

//BEEP
Note alarmS = {1000, 5000};
void beep(void)
{
	play_note(&alarmS);
}

//RTC
long int time_count = 0;

void get_time(int* hour, int* minute, int* second);
int get_second_of_day();



//Music
Note shootS = {La5, 35}, colisionS = {Si5, 35}, pillS = {Dos8, 200};

Bool speaker_shoot = false;
long int play_shoot_init;

Bool speaker_colision = false;
long int play_colision_init;

Bool speaker_pill = false;
long int play_pill_init;


//Mouse
#include "kbc.h"
#include "GQueue.h"
void mouse_isr(void);

Queue keys_queue;
GQueue* mouse_queue;

Byte MSByte1 = 0;
Byte MSByte2 = 0;
Byte MSByte3 = 0;
int dx = 0;
int dy = 0;
int xov = 0;
int yov = 0;
int lmb = 0;
int rmb = 0;
int mmb = 0;

int x_pos, y_pos;

Bool has_mouse;

int cor_temp, cor_rato = WHITE;


//**GamePlay**

/**
 * 0 = main menu
 * 1 = playing
 * 2 = game over screen
 * 3 = exiting
 * 4 = leveling up
**/
int state = 0;
int specialWeapon;
int pausa = 0;

/**
 * Main menu options
 * 0 = new game (default)
 * 1 = quit
**/
int selected = 0;

void select_option();
void draw_main_menu();
void draw_game_over_menu();
void draw_board();

//Score
char table[256 * CHR_H];
int score = 0;
int pills = 3;
int kills = 0;
int shots_fired = 0;
int calc_final_score();

Bool won = false;
int level = 1;
int max_zoids = 5;
int n_zoids = 0;

_go32_dpmi_seginfo old_kbd_irq, old_mouse_irq, old_t0_irq, old_rtc_irq;

void parse_mouse_event()
{
	MouseData* package = (MouseData*)(getGQueue(mouse_queue));
	MSByte1 = package->Byte1;
	MSByte2 = package->Byte2;
	MSByte3 = package->Byte3;
	
	dx = MSByte1 & XSGN ? (MSByte2 - 256) : MSByte2;
	dy = MSByte1 & YSGN ? (MSByte3 - 256) : MSByte3;
	
	xov = X_OVERFLOW(MSByte1);
	yov = Y_OVERFLOW(MSByte1);
	
	lmb = LEFT_BUTTON(MSByte1); 
	mmb = MIDDLE_BUTTON(MSByte1);
	rmb = RIGHT_BUTTON(MSByte1);
}


void process_mouse_event()
{
	if(dx || dy || lmb)
	{
		int x_old = x_pos;
		int y_old = y_pos;
		
		//actualizar posicao do rato
		x_pos += dx;
		if(x_pos < 0) x_pos = 0;
		if(x_pos >= HRES) x_pos = HRES - 1;
		
		y_pos -= dy;
		if(y_pos < 0) y_pos = 0;
		if(y_pos >= VRES) y_pos = VRES - 1;
		
		
		switch(state) {
			case 0:
				//paint the last pixel with cor_temp
				set_pixel(x_old, y_old, cor_temp, base2);
				
				//left button pressed on top of highlighted option -> select option
				if(lmb && get_pixel(x_pos+1, y_pos+1, base2) == selected_color ) { //((get_pixel(x_pos+1, y_pos+1, base2) == selected_color) || (get_pixel(x_pos-1, y_pos-1, base2) == selected_color) ) ) {
					state = (selected)? 3 : 1;	
					if (state == 1) {
						clear_screen(0, base2);
						pills = 3;
						score = 0;
						kills = 0;
						shots_fired = 0;
						
						won = false;
						level = 1;
						max_zoids = 5;
						n_zoids = 0;
						
						range_min = 2250;
						range_max = 3000;
						
						zoids = lst_new();
						spermicides = lst_new();
									
						ovule->x = main_frame_w/2;
						ovule->y = VRES-30;
						drawSprite(ovule, base2);							
					}
				}
				//save the color underneath the mouse
				//mouse moved to unselected option -> switch selection
				else if((cor_temp = get_pixel(x_pos, y_pos, base2)) == unselected_color ) {
					set_pixel(x_pos, y_pos, cor_rato, base2);
					selected = selected? 0 : 1;
					select_option();
					cor_temp = selected_color;
				}
				//replace color with the mouse color
				else
					set_pixel(x_pos, y_pos, cor_rato, base2);
				break;
			
			
			case 1:
				
				//move ovule
	/*			if(x_pos >= 2 && x_pos <= main_frame_w - ovule->width)
					ovule->x = x_pos;
				else if(x_pos < 2)
					ovule->x = 2;
				else ovule->x = main_frame_w - ovule->width;*/
				move_ovule_mouse(x_pos, ovule, base2);
						
				//shoot a spermicide if left mouse button pressed
				if (lmb) {
					cria_spermicide(ovule->x+17, ovule->y-9);
					shots_fired++;

					//play shooting note
					timer_init(2,LCOM_MODE);
					timer_load(2,musicDiv(shootS.freq));
					speaker_on();
					play_shoot_init = time_count;
					speaker_shoot = true;
				}
				break;
		}
	}
}

void rtc_isr(void)
{
	outportb(RTC_ADDR_REG, RTC_STAT_C);
	Byte cause = read_rtc(RTC_STAT_C);

	if (cause & RTC_PF)  {
		time_count++;
		
		if(!isEmptyGQueue(mouse_queue) && has_mouse)
		{	
			parse_mouse_event();
			if(state == 0 || state == 1) process_mouse_event();
			// pack_completo = 0;
		}
		
		if(!queueEmpty(&keys_queue)) {
			//obter o proximo elemento da queue e retira-lo
			last_sc = queueGet(&keys_queue);
			
			switch( state ) {
				case 0:
				
					switch(last_sc) {
						//arrow down
						case(0x50):
							if (!selected) selected = 1;
							select_option();
							break;
						//arrow up
						case(0x48):
							if(selected) selected = 0;
							select_option();
							break;
						//enter
						case(0x1c):
							state = (selected)? 3 : 1;	
							if (state == 1) {
								clear_screen(0, base2);
								pills = 3;
								score = 0;
								kills = 0;
								shots_fired = 0;
								
								won = false;
								level = 1;
								max_zoids = 5;
								n_zoids = 0;
								
								range_min = 2250;
								range_max = 3000;
								
								zoids = lst_new();
								spermicides = lst_new();
											
								ovule->x = main_frame_w/2;
								ovule->y = VRES-30;
								drawSprite(ovule, base2);							
							}
							break;
						//esc
						case(0x01):
							state = 3;
							break;
					}
					break;
			
				//playing
				case 1:
					switch(last_sc) {
						//arrow left
						case (0x4B):
							move_ovule('l', ovule, base2);
							break;
						//arrow right
						case (0x4D):
							move_ovule('r', ovule, base2);
							break;
						//space bar
						case(0x39):
							cria_spermicide(ovule->x+17, ovule->y-9);
							shots_fired++;
							
							//play shooting note
							timer_init(2,LCOM_MODE);
							timer_load(2,musicDiv(shootS.freq));
							speaker_on();
							play_shoot_init = time_count;
							speaker_shoot = true;
				
							break;
						//esc
						
						//pausa
						case(0x19):
							clear_screen(0, base2);
							state = 4;
							break;
						
						//special weapon
						case(0x38):
						if(specialWeapon>0){
						lst_iitem_t* curZoid;
						for(curZoid = zoids->first; curZoid != NULL; curZoid = curZoid->next){
						score += (VRES-curZoid->spr->y);
						n_zoids++;
						}
						zoids = lst_new();
						clear_screen(0, base2);
						drawSprite(ovule, base2);
						specialWeapon--;
						}
						break;
						
						case(0x01):
							state = 2;
							
							draw_game_over_menu();
							break;
					}
					break;
				//game over screen
				case 2:
					switch(last_sc) {
						//enter
						case(0x1c):
							state = 0;
							draw_main_menu();
							break;
						//esc
						case(0x01):
							state = 3;
							break;
					}
				case 4:
					switch(last_sc) {
					case(0x19):
							clear_screen(0, base2);
							drawSprite(ovule, base2);
							state = 1;
						break;
						}
					break;
			}
		}
		
		//stop shooting note
		if ((time_count - play_shoot_init >= shootS.dur) && speaker_shoot) {
			speaker_shoot = false;
			speaker_off();
		}
		
		//stop colision note
		if ((time_count - play_colision_init >= shootS.dur) && speaker_colision) {
			speaker_colision = false;
			speaker_off();
		} 
		
		if ((time_count - play_pill_init >= pillS.dur) && speaker_pill) {
			speaker_pill = false;
			speaker_off();
		} 
	}
	
	outportb(PIC1_CMD, EOI);
	outportb(PIC2_CMD, EOI);
}

void init()
{	
	//desabilita rato e teclado
	disable_irq(KBD_IRQ);
	disable_irq(MOUSE_IRQ);	
	
	//verifica se detecta o rato
	has_mouse = kbc_init(0);
	queueInit(&keys_queue);
	mouse_queue = newGQueue(10, sizeof(MouseData));
	
	//instala as rotinas
	install_asm_irq_handler(KBD_IRQ, kbd_isr, &old_kbd_irq);
	install_asm_irq_handler(MOUSE_IRQ, mouse_isr, &old_mouse_irq);
	
	//activa rato e teclado
	enable_irq(KBD_IRQ);
	enable_irq(MOUSE_IRQ);
	
	//Timer
	timer_init(0, LCOM_MODE);
	timer_init(2, LCOM_MODE);
	timer_load(0, divisor(1000.0));
	install_asm_irq_handler(T0_IRQ, handler, &old_t0_irq);
	
	//RTC
	install_c_irq_handler(RTC_IRQ, rtc_isr, &old_rtc_irq);
	rtc_int_enable();	
	//disable_irq(RTC_IRQ);
	unmask_pic(RTC_IRQ);
	
}

void clean() {

	//TIMER
	reinstall_asm_irq_handler(T0_IRQ, &old_t0_irq);
	
	//RTC
	rtc_int_disable();
	reinstall_c_irq_handler(RTC_IRQ, &old_rtc_irq);
	mask_pic(RTC_IRQ);
	
	//teclado e rato
	disable_irq(KBD_IRQ);
    if(has_mouse) disable_irq(MOUSE_IRQ);
    if(has_mouse) reinstall_asm_irq_handler(MOUSE_IRQ, &old_mouse_irq);
    reinstall_asm_irq_handler(KBD_IRQ, &old_kbd_irq);
    if(has_mouse) enable_irq(MOUSE_IRQ);//habilitar as interrupções do rato
    enable_irq(KBD_IRQ); //habilitar interrupcoes do teclado

}




int main() {
	specialWeapon = 3;
	srand((unsigned)time(NULL)); 
	
	__dpmi_meminfo map;
	base = enter_graphics(mode, &map);
	base2 = (char*) malloc(HRES*VRES);
	memcpy(base2, base, HRES*VRES);
	
	
	if(has_mouse) {
		x_pos = HRES / 2;
		y_pos = VRES / 2;
		set_pixel(x_pos, y_pos, cor_rato, base2);
	}
	
	init();
	
	zoids = lst_new();
	spermicides = lst_new();
	
	ovule = createSprite( ovule_speed, 0, 400, VRES-30, ovule_map, base2);
	
	//reads both zoid pixmaps
	int width, height;
	zoid_char1 = read_xpm(zoid_map, &width, &height);
	zoid_char2 = read_xpm(zoid_map2, &width, &height);
	
	Bool wait = false;
	int timeout;

	load_char_table(table);
	
	draw_main_menu();
	
	while(state != 3)
	{

		switch (state) {
			//main menu
			case 0:
				
			
				break;
		
			//playing
			case 1:
				//Main Frame / Scoreboard dividing line
				draw_line(main_frame_w, 2, main_frame_w, VRES-2, 15, base2);
				
				//limit line
				int n_lines = 10;
				int step = ( main_frame_w -2 ) / (n_lines*2-1);
				int xi = 2, xf = xi+step;
				
				for( ; n_lines > 0 ; n_lines--) {
					draw_line(xi, VRES-90+31, xf, VRES-90+31, 15, base2);
					xi += 2*step;
					xf += 2*step;
				}
				

				draw_board();
				
				//create zoids
				if (time_count - last_zoid >= espera_zoid) {
					espera_zoid = (rand() % (range_max - range_min)) + range_min;	//intervalo de 750 ~ 3000 msecs
					last_zoid = time_count;
					cria_zoid();
				}

				animate_spermicides(base2);
				
				lst_iitem_t* curZoid;
				for(curZoid = zoids->first; curZoid != NULL; curZoid = curZoid->next){
				
					int success = animate_zoid(curZoid->spr, base2);
					//Killed a zoid
					if (success == 2) {
						//play shooting note
						timer_init(2,LCOM_MODE);
						timer_load(2,musicDiv(colisionS.freq));
						speaker_on();
						play_colision_init = time_count;
						speaker_colision = true;
						
						//increase score
						score += (VRES-curZoid->spr->y);
						kills++;
						n_zoids++;
						
						//kill zoid
						curZoid = lst_remove(zoids, curZoid->spr);
						if (curZoid == NULL) break;
					}
					//Lost a pill
					else if (success == 1) {
						pills--;
						zoids = lst_new();
						
						clear_screen(0, base2);
						drawSprite(ovule, base2);
						
						//play note
						timer_init(2,LCOM_MODE);
						timer_load(2,musicDiv(pillS.freq));
						speaker_on();
						play_pill_init = time_count;
						speaker_pill = true;
						
						break;
					}
				}
				//Game Over
				if(!pills) {
					state = 2;
					draw_game_over_menu();
				}
				
				//level up
				if(n_zoids >= max_zoids) {
					if(level == 10) {
						state = 2;
						won = true;
						break;			
					}
					state = 4;
					
					//update vars
					pills--;
					zoids = lst_new();
					spermicides = lst_new();
					n_zoids = 0;
					
					//reduces the time between zoids by 12%
					range_min *= 0.88; 
					range_max *= 0.88;
					
					level++;
					max_zoids = level* 15;
						
					clear_screen(0, base2);
					draw_board();
					
					drawStringAt("LEVEL", main_frame_w/2 - 80, 200, 33, 0, 3, base2, table);
					drawIntAt(level, main_frame_w/2 + 70, 200, 33, 0, 3, base2, table);
				
					//sleep for 2 seconds
					int n = time_count;
					memcpy(base, base2, HRES*VRES);
					while((time_count - n) < 2000);
					
					fill_area(main_frame_w/2-85, 200, main_frame_w/2+95, 300, 0, base2);
					drawSprite(ovule, base2);
					
					state = 1;
				}
				mili_sleep(20);
				
				break;
				
			//Game Over
			case 2:
							

				break;
				//pause
			case 4:
				drawStringAt("PAUSE", HRES*0.4, VRES*0.4, 40, 0, 3, base2, table);
				break;
				
		}
		
		if(time_tick > timeout) 
			wait = false;
		
		if(queueFull(&keys_queue)){
			beep();
		}				
				

		memcpy(base, base2, HRES*VRES);

	}
	
	leave_graphics(map);
	clean();
	
	
	return 0;
}


void cria_zoid() {
	int r;
    r = rand()%(main_frame_w-35)+10; 

	Sprite *zoid = createSprite( 0, zoid_speed, r, 0, zoid_map, base2);
	lst_insert(zoids, zoid);
}

void cria_spermicide(int x, int y) {
	Sprite *spermicide = createSprite( 0, spermicide_speed, x, y, spermicide_map, base2);
	drawSprite(spermicide, base2);
	lst_insert(spermicides, spermicide);
}

int calc_final_score() {
	//10 points penalty for every missed shot
	return score - 10*(shots_fired - kills);
}

void draw_main_menu() {

	clear_screen(0, base2);
	selected = 0;
	
	drawStringAt("--> ZOIDS <--", HRES/2 - 205, 80, 40, 0, 4, base2, table);
	
	
	//options
	draw_line(HRES/2 - 90, 190, HRES/2 + 90, 190, 15, base2);
	draw_line(HRES/2 - 90, 240, HRES/2 + 90, 240, 15, base2);
	
	draw_line(HRES/2 - 90, 270, HRES/2 + 90, 270, 15, base2);
	draw_line(HRES/2 - 90, 320, HRES/2 + 90, 320, 15, base2);
	
	select_option();


}

void draw_game_over_menu() {

	//sleep for 30 miliseconds
	//int n = time_count;
	//while((time_count - n) < 30);
	
	clear_screen(0, base2);

	if(won)
		drawStringAt("You WON!", HRES*0.35, 50, 33, 0, 2, base2, table);
	else
		drawStringAt("You LOST!", HRES*0.35, 50, 40, 0, 2, base2, table);
	
	drawStringAt("Score", HRES/2 - 145, 90, 15, 0, 2, base2, table);
	drawStringAt("Missed shots", HRES/2 - 145, 120, 15, 0, 2, base2, table);
	drawStringAt("Penalty", HRES/2 - 145, 150, 15, 0, 2, base2, table);
	drawStringAt("Final Score", HRES/2 - 145, 200, 15, 0, 2, base2, table);

	drawIntAt(score, HRES/2+100, 90, 15, 0, 2, base2, table);
	drawIntAt(shots_fired - kills, HRES/2+100, 120, 15, 0, 2, base2, table);
	drawIntAt((shots_fired - kills) * 10, HRES/2+100, 150, 15, 0, 2, base2, table);
	drawIntAt(calc_final_score(), HRES/2+100, 200, 15, 0, 2, base2, table);

	
	drawStringAt("Press Enter to continue", HRES*0.35, VRES*0.8, 15, 0, 1, base2, table);
}

void select_option() {


	if (selected) {
		fill_area(HRES/2 - 90, 195, HRES/2 + 90, 236, unselected_color, base2);
		fill_area(HRES/2 - 90, 275, HRES/2 + 90, 316, selected_color, base2);
		
		drawStringAt("New Game", HRES/2 - 62, 200, 15, unselected_color, 2, base2, table);
		drawStringAt("Quit", HRES/2 - 32, 280, 15, selected_color, 2, base2, table);
	}
	else {
		fill_area(HRES/2 - 90, 195, HRES/2 + 90, 236, selected_color, base2);
		fill_area(HRES/2 - 90, 275, HRES/2 + 90, 316, unselected_color, base2);
		
		drawStringAt("New Game", HRES/2 - 62, 200, 15, selected_color, 2, base2, table);
		drawStringAt("Quit", HRES/2 - 32, 280, 15, unselected_color, 2, base2, table);
		
	}
	
/*	int xi = 50, xf = 100;
	int yi = 5, yf = 15;
	int i = 30;
	for( ; i < 100; i++) {
			fill_area(xi, yi, xf, yf, i, base2);
			yi+= 15; yf += 15;
	}*/
}


void draw_board() {

	draw_line(2, 2, HRES-2, 2, 15, base2);
	draw_line(2, 2, 2, VRES-2, 15, base2);
	draw_line(2, VRES-2, HRES-2, VRES-2, 15, base2);
	draw_line(HRES-2, 2, HRES-2, VRES-2, 15, base2);

	//Main Frame / Scoreboard dividing line
	draw_line(main_frame_w, 2, main_frame_w, VRES-2, 15, base2);
	
	//Scoreboard labels
	drawStringAt("Level: ", main_frame_w+10, 50, 15, 0, 1, base2, table);
	drawStringAt("Score:", main_frame_w+10, 110, 15, 0, 1,base2, table);
	drawStringAt("Morning after", main_frame_w+10, 170, 15, 0, 1,base2, table);
	drawStringAt("       pills:", main_frame_w+10, 190, 15, 0, 1,base2, table);
	drawStringAt("Time:", main_frame_w+10, 250, 15, 0, 1,base2, table);
	drawStringAt("Special Weapon:", main_frame_w+10, 300, 15, 0, 1,base2, table);

	//Scoreboard info
	drawIntAt(level, main_frame_w+10, 70, 15, 0, 1, base2, table);
	drawIntAt(score, main_frame_w+10, 130, 15, 0, 1,base2, table);
	drawIntAt(pills, main_frame_w+10, 210, 15, 0, 1,base2, table);
	drawIntAt(time_count/1000, main_frame_w+10, 270, 15, 0, 1,base2, table);
	drawIntAt(specialWeapon, main_frame_w+10, 320, 15, 0, 1,base2, table);

}
