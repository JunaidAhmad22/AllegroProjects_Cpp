#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <iostream>
#include <string>

bool collision(float &x, float &y, float ex, float ey, int w, int h)
{
	if (x + w < ex || x>ex + w || y + h<ey || y> ey + h)
	{
		return false;
	}
	return true;
}





		
int main()
{
	
	
	ALLEGRO_DISPLAY *display;
	enum Direction { DOWN, LEFT, RIGHT, UP };
	const float SCREEN_W = 800;
	const float SCREEN_H = 600;
	const float FPS = 60;
	const float WALKSPEED = 15;
	int MOVESPEED = 2;
	bool coins = true;
	//bool walkA = true;
	//bool walkB = false;
	int random_x[100];
	int random_y[100];
	int coinAmount;
	int score = 0;
	bool end = false;
	int player_x = 50;
	int player_y = 50;
	bool win = false;

	bool c[10] = { false , false , false , false , false,false,false , false , false , false  };
	bool a[10] = { false , false , false , false , false,false,false , false , false , false };



	if (!al_init()) {
		al_show_native_message_box(NULL, NULL, NULL, "Could not initialize allegro", NULL, NULL);
		//return -1;

	}

	al_set_new_display_flags(ALLEGRO_RESIZABLE);
	//al_set_new_display_flags(ALLEGRO_NOFRAME);
	display = al_create_display(SCREEN_W, SCREEN_H);
	al_set_window_position(display, 500, 100);
	al_set_window_title(display, "ProtoType");
	//ALLEGRO_BITMAP *image;
	if (!display) {
		al_show_native_message_box(display, "Message box windpw", "Error", "Display window could not be shown", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	}

	bool done = false, draw = true;
	float x_position = 10, y_position = 50, moveSpeed = 15;
	float x1_position = 0, y1_position = 0;
	float f = 200; 
	float g = 420;
	float h = 500;
	float m = 200;


	//moveSpeed = 0;

	int dir = DOWN;
	// variable for spritesheet
	bool active = false;
	int x_source = 0, y_source = 0;
	al_install_keyboard();
	al_init_image_addon();
	al_init_acodec_addon();
	al_install_audio();
	al_init_font_addon();
	al_init_ttf_addon();
	
	ALLEGRO_FONT *font = al_load_font("OpenSans-Regular.ttf", 20, NULL);
	ALLEGRO_FONT *font1 = al_load_font("OpenSans-Regular.ttf", 50, NULL);

	ALLEGRO_BITMAP *bg = al_load_bitmap("bg.jpg");
	ALLEGRO_KEYBOARD_STATE keystate;
	ALLEGRO_BITMAP *player = al_load_bitmap("player.png");
	ALLEGRO_BITMAP *enemy = al_load_bitmap("ant.png");
	ALLEGRO_BITMAP *enemy1 = al_load_bitmap("ant1.png");
	al_convert_mask_to_alpha(enemy1, al_map_rgb(255, 255, 255));

	ALLEGRO_BITMAP *enemy2 = al_load_bitmap("ant2.png");
	al_convert_mask_to_alpha(enemy2, al_map_rgb(255, 255, 255));

	

	ALLEGRO_SAMPLE *sound = al_load_sample("sample.wav");
	al_reserve_samples(1);
	al_convert_mask_to_alpha(enemy, al_map_rgb(255, 255, 255));
	
	ALLEGRO_TIMER *timer = al_create_timer(5.0 / 60);
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	ALLEGRO_BITMAP *coin = al_load_bitmap("coin1.bmp");
	coin = al_load_bitmap("coin1.png");
	al_convert_mask_to_alpha(coin, al_map_rgb(0, 0, 0));
	ALLEGRO_BITMAP *bricks = al_load_bitmap("brick.png");
	ALLEGRO_BITMAP *bricks1 = al_load_bitmap("brick1.png");
	ALLEGRO_BITMAP *bricks2 = al_load_bitmap("brick2.png");
	ALLEGRO_BITMAP *bricks3 = al_load_bitmap("brick3.png");

	
	al_start_timer(timer);
	int sec = 30;
	while (sec != 0)
	{
		if (sec > 20)
		{
			al_draw_text(font1, al_map_rgb(250, 65, 45), 400, 230, ALLEGRO_ALIGN_CENTER, "WELCOME TO PROTO-TYPE");
			Sleep(200);
		}
		else if (sec > 10)
		{
			al_draw_text(font1, al_map_rgb(250, 25, 45), 400, 230, ALLEGRO_ALIGN_CENTER, "Press Enter To Continue ");
			Sleep(200);
		}
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
		sec--;
	}

	
	// game loop
	while (!done) {
		
		al_draw_scaled_bitmap(bg, 0, 0 , al_get_bitmap_width(bg) , al_get_bitmap_height(bg) , 0, 0,800,600, NULL);
		

		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);

		if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}

		else if (events.type == ALLEGRO_EVENT_TIMER)
		{
			active = true;
			al_get_keyboard_state(&keystate);
			if (al_key_down(&keystate, ALLEGRO_KEY_DOWN)) {
				y_position += moveSpeed;
				dir = DOWN;
			}
			else if (al_key_down(&keystate, ALLEGRO_KEY_UP)) {
				y_position -= moveSpeed;
				dir = UP;
			}
			else if (al_key_down(&keystate, ALLEGRO_KEY_RIGHT)) {
				x_position += moveSpeed;
				dir = RIGHT;
			}
			else if (al_key_down(&keystate, ALLEGRO_KEY_LEFT)) {
				x_position -= moveSpeed;
				dir = LEFT;
			}
			else
				active = false;

			//active = false;
			if (collision(x_position, y_position, 0, 0, 800, 5)) // up
			{
				if (dir == UP)
				{
					y_position += moveSpeed;
				}
				else if (dir == RIGHT)
				{
					x_position -= moveSpeed;
				}
				else if (dir == DOWN)
				{
					y_position -= moveSpeed;
				}
				else if (dir == LEFT)
				{
					x_position += moveSpeed;
				}
			}

			if (collision(x_position, y_position, 0, 540, 800, 0))//down
			{
				if (dir == UP)
				{
					y_position += moveSpeed;
				}
				else if (dir == RIGHT)
				{
					x_position -= moveSpeed;
				}
				else if (dir == DOWN)
				{
					y_position -= moveSpeed;
				}
				else if (dir == LEFT)
				{
					x_position += moveSpeed;
				}
			}


			if (collision(x_position, y_position, 0, 0, 6, 600))//left
			{
				if (dir == UP)
				{
					y_position += moveSpeed;
				}
				else if (dir == RIGHT)
				{
					x_position -= moveSpeed;
				}
				else if (dir == DOWN)
				{
					y_position -= moveSpeed;
				}
				else if (dir == LEFT)
				{
					x_position += moveSpeed;
				}
			}

			if (collision(x_position, y_position, 770, 0, 10, 600))
			{
				if (dir == UP)
				{
					y_position += moveSpeed;
				}
				else if (dir == RIGHT)
				{
					x_position -= moveSpeed;
				}
				else if (dir == DOWN)
				{
					y_position -= moveSpeed;
				}
				else if (dir == LEFT)
				{
					x_position += moveSpeed;
				}
			}
			if (collision(x_position, y_position, 310, 558, al_get_bitmap_width(bricks)+200, al_get_bitmap_height(bricks)))
			{
				if (dir == UP)
				{
					y_position += moveSpeed;
				}
				else if (dir == RIGHT)
				{
					x_position -= moveSpeed;
				}
				else if (dir == DOWN)
				{
					y_position -= moveSpeed;
				}
				else if (dir == LEFT)
				{
					x_position += moveSpeed;
				}
			}
			//wall collision
			if (collision(x_position, y_position, 790, 275, al_get_bitmap_width(bricks), al_get_bitmap_height(bricks)))
			{
				if (dir == UP)
				{
					y_position += moveSpeed;
				}
				else if (dir == RIGHT)
				{
					x_position -= moveSpeed;
				}
				else if (dir == DOWN)
				{
					y_position -= moveSpeed;
				}
				else if (dir == LEFT)
				{
					x_position += moveSpeed;
				}
			}

			if (collision(x_position, y_position, 390, 500, al_get_bitmap_width(bricks3), al_get_bitmap_height(bricks3)))
			{
				if (dir == UP)
				{
					y_position += moveSpeed;
				}
				else if (dir == RIGHT)
				{
					x_position -= moveSpeed;
				}
				else if (dir == DOWN)
				{
					y_position -= moveSpeed;
				}
				else if (dir == LEFT)
				{
					x_position += moveSpeed;
				}
			}


			if (collision(x_position, y_position, 380, 0, al_get_bitmap_width(bricks3), al_get_bitmap_height(bricks3)))
			{
				if (dir == UP)
				{
					y_position += moveSpeed;
				}
				else if (dir == RIGHT)
				{
					x_position -= moveSpeed;
				}
				else if (dir == DOWN)
				{
					y_position -= moveSpeed;
				}
				else if (dir == LEFT)
				{
					x_position += moveSpeed;
				}
			}
			//3
			if (collision(x_position, y_position, 0, 290, al_get_bitmap_width(bricks)-20, al_get_bitmap_height(bricks)+10))
			{
				if (dir == UP)
				{
					y_position += moveSpeed;
				}
				else if (dir == RIGHT)
				{
					x_position -= moveSpeed;
				}
				else if (dir == DOWN)
				{
					y_position -= moveSpeed;
				}
				else if (dir == LEFT)
				{
					x_position += moveSpeed;
				}
			}

			if (collision(x_position, y_position, 550, 200, al_get_bitmap_width(bricks1)-170 , al_get_bitmap_height(bricks1)+7))
			{
				if (dir == UP)
				{
					y_position += moveSpeed;
				}
				else if (dir == RIGHT)
				{
					x_position -= moveSpeed;
				}
				else if (dir == DOWN)
				{
					y_position -= moveSpeed;
				}
				else if (dir == LEFT)
				{
					x_position += moveSpeed;
				}
			}

			//coins collisin
			if (collision(x_position, y_position, 450-10, 200, 20, 30))
			{
				
				c[0] = true;
				
			}

			if (collision(x_position, y_position, 90, 550, 20, 30))
			{
				
				c[1] = true;
				
			}

			if (collision(x_position, y_position, 650, 50, 20, 30))
			{
				
				c[2] = true;
			
			}

			if (collision(x_position, y_position, 250, 150, 20, 30))
			{
				
				c[3] = true;
			
			}

			if (collision(x_position, y_position, 700, 500, 20, 30))
			{
				
				c[4] = true;
			
			}

			if (collision(x_position, y_position, 600, 400, 20, 30))
			{
				
				c[5] = true;
			

			}

			if (collision(x_position, y_position, 450, 550, 20, 30))
			{
				
				c[6] = true;
			
			}

			if (collision(x_position, y_position, 300, 500, 20, 30))
			{
				
				c[7] = true;
			
			}

			if (collision(x_position, y_position, 300, 500, 20, 30))
			{
				
				c[8] = true;
			
			}

			if (collision(x_position, y_position, 70, 200, 20, 30))
			{
				
				c[9] = true;
			
			}
			
			if (collision(x_position, y_position, 200, 350, 20, 30))
			{
				
				c[10] = true;
			
			}
			

			if (active) {
				x_source += al_get_bitmap_width(player) / 4;
				
			}
			else
				x_source = al_get_bitmap_width(player) / 4;

			if (x_source >= al_get_bitmap_width(player))
				x_source = 0;


			y_source = dir * al_get_bitmap_height(player) / 4;
			draw = true;
		}
		//enemy collision 
		if (collision(x_position, y_position, f-10 , 100, 30, 30))
		{
			end = true;
		}
		
		if (collision(x_position, y_position, g, 80, 30, 30))
		{
			end = true;
		}

		if (collision(x_position, y_position, h, 400, 30, 30))
		{
			end = true;
		}

		if (collision(x_position, y_position, m, 400, 30, 30))
		{
			end = true;
		}
		//draw_again:
		if (draw) {
			draw = false;
			al_draw_bitmap_region(player, x_source, y_source, al_get_bitmap_width(player) / 4, al_get_bitmap_height(player) / 4, x_position, y_position, NULL);
			al_draw_bitmap_region(enemy, 0, 0, al_get_bitmap_width(enemy) , al_get_bitmap_height(enemy) , f, 100, NULL);
			al_draw_bitmap_region(enemy1, 0, 0, al_get_bitmap_width(enemy1), al_get_bitmap_height(enemy1), g, 80, NULL);
			al_draw_bitmap_region(enemy1, 0, 0, al_get_bitmap_width(enemy1), al_get_bitmap_height(enemy1), h, 400, NULL);
			al_draw_bitmap_region(enemy, 0, 0, al_get_bitmap_width(enemy), al_get_bitmap_height(enemy), m, 400, NULL);

			
			if (f > 30)
			{
				f = f - 8;
			}
			else
			{
				f = 300;
			}

			if (g < 700)
			{
				g = g + 8;
			}
			else
			{
				g = 420;
			}

			if (h < 700)
			{
				h = h + 8;
			}
			else
			{
				h = 410;
			}

			if (m > 30)
			{
				m = m - 8;
			}
			else
			{
				m = 300;
			}

			if (!c[0])
			{
				al_draw_bitmap_region(coin, 0, 0, al_get_bitmap_width(coin), al_get_bitmap_height(coin), 450, 200, NULL);
			}
			else if(!a[0])
			{
				score++;
				a[0] = true;
				al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				
			}
			if (!c[1])
			{
				al_draw_bitmap_region(coin, 0, 0, al_get_bitmap_width(coin), al_get_bitmap_height(coin), 90, 550, NULL);
			}
			else if (!a[1])
			{
				score++;
				a[1] = true;
				al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				
			}
			if (!c[2])
			{
				al_draw_bitmap_region(coin, 0, 0, al_get_bitmap_width(coin), al_get_bitmap_height(coin), 650, 50, NULL);
			}
			else if (!a[2])
			{
				score++;
				a[2] = true;
				al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				
			}
			if (!c[3])
			{
				al_draw_bitmap_region(coin, 0, 0, al_get_bitmap_width(coin), al_get_bitmap_height(coin), 250, 150, NULL);
			}
			else if (!a[3])
			{
				score++;
				a[3] = true;
				al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				
			}
			if (!c[4])
			{
				al_draw_bitmap_region(coin, 0, 0, al_get_bitmap_width(coin), al_get_bitmap_height(coin), 700, 550, NULL);
			}
			else if (!a[4])
			{
				score++;
				a[4] = true;
				al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				
			}
			if (!c[5])
			{
				al_draw_bitmap_region(coin, 0, 0, al_get_bitmap_width(coin), al_get_bitmap_height(coin), 600,400, NULL);
			}
			else if (!a[5])
			{
				score++;
				a[5] = true;
				al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				
			}
			if (!c[6])
			{
				al_draw_bitmap_region(coin, 0, 0, al_get_bitmap_width(coin), al_get_bitmap_height(coin), 450, 550, NULL);
			}
			else if (!a[6])
			{
				score++;
				a[6] = true;
				al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				
			}
			if (!c[7])
			{
				al_draw_bitmap_region(coin, 0, 0, al_get_bitmap_width(coin), al_get_bitmap_height(coin), 300, 500, NULL);
			}
			else if (!a[7])
			{
				score++;
				a[7] = true;
				al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			}
			if (!c[8])
			{
				al_draw_bitmap_region(coin, 0, 0, al_get_bitmap_width(coin), al_get_bitmap_height(coin), 300, 500, NULL);
			}
			else if (!a[8])
				
			{
				score++;
				a[8] = true;
				al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			}
			if (!c[9])
			{
				al_draw_bitmap_region(coin, 0, 0, al_get_bitmap_width(coin), al_get_bitmap_height(coin), 70, 200, NULL);
			}
			else if (!a[9])
			{
				score++;
				a[9] = true;
				al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				
			}
			if (!c[10])
			{
				al_draw_bitmap_region(coin, 0, 0, al_get_bitmap_width(coin), al_get_bitmap_height(coin), 200, 350, NULL);
			}
			else if (!a[10])
			{
				score++;
				a[10] = true;
				al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				
			}
			al_draw_bitmap_region(bricks, 0, 0, al_get_bitmap_width(bricks), al_get_bitmap_height(bricks), 0, 0, NULL);
			al_draw_bitmap_region(bricks, 0, 0, al_get_bitmap_width(bricks), al_get_bitmap_height(bricks), 310, 0, NULL);
			al_draw_bitmap_region(bricks, 0, 0, al_get_bitmap_width(bricks), al_get_bitmap_height(bricks), 597, 0, NULL);
			al_draw_bitmap_region(bricks, 0, 0, al_get_bitmap_width(bricks), al_get_bitmap_height(bricks), 0, 585, NULL);
			al_draw_bitmap_region(bricks, 0, 0, al_get_bitmap_width(bricks), al_get_bitmap_height(bricks), 310, 585, NULL);
			al_draw_bitmap_region(bricks, 0, 0, al_get_bitmap_width(bricks), al_get_bitmap_height(bricks), 597, 585, NULL);

			al_draw_bitmap_region(bricks2, 0, 0, al_get_bitmap_width(bricks2), al_get_bitmap_height(bricks2), 0, 0, NULL);
			al_draw_bitmap_region(bricks2, 0, 0, al_get_bitmap_width(bricks2), al_get_bitmap_height(bricks2), 0, 310, NULL);
			al_draw_bitmap_region(bricks2, 0, 0, al_get_bitmap_width(bricks2), al_get_bitmap_height(bricks2), 785, 0, NULL);
			al_draw_bitmap_region(bricks2, 0, 0, al_get_bitmap_width(bricks2), al_get_bitmap_height(bricks2), 785, 310, NULL);
			
			al_draw_bitmap_region(bricks3, 0, 0, al_get_bitmap_width(bricks3), al_get_bitmap_height(bricks3), 400, 15, NULL);
			al_draw_bitmap_region(bricks3, 0, 0, al_get_bitmap_width(bricks3), al_get_bitmap_height(bricks3), 400, 368, NULL);
			
			
			al_draw_bitmap_region(bricks, 0, 0, al_get_bitmap_width(bricks1), al_get_bitmap_height(bricks1), 500, 300, NULL);
			al_draw_bitmap_region(bricks, 0, 0, al_get_bitmap_width(bricks1), al_get_bitmap_height(bricks1), 400,225, NULL);
			al_draw_bitmap_region(bricks, 0, 0, al_get_bitmap_width(bricks1), al_get_bitmap_height(bricks1), 0,300, NULL);
			
			if (score==1)
			{
				al_draw_text(font, al_map_rgb(250, 25, 45), 100, 10, ALLEGRO_ALIGN_CENTER, "Coin : 1 ");
				
			}
			if (score == 2)
			{
				al_draw_text(font, al_map_rgb(250, 25, 45), 100, 10, ALLEGRO_ALIGN_CENTER, "Coin : 2 ");
				
			}
			if (score == 3)
			{
				al_draw_text(font, al_map_rgb(250, 25, 45), 100, 10, ALLEGRO_ALIGN_CENTER, "Coin : 3 ");
				
			}
			if (score == 4)
			{
				al_draw_text(font, al_map_rgb(250, 25, 45), 100, 10, ALLEGRO_ALIGN_CENTER, "Coin : 4 ");
				
			}
			if (score == 5)
			{
				al_draw_text(font, al_map_rgb(250, 25, 45), 100, 10, ALLEGRO_ALIGN_CENTER, "Coin : 5 ");
				
			}
			if (score == 6)
			{
				al_draw_text(font, al_map_rgb(250, 25, 45), 100, 10, ALLEGRO_ALIGN_CENTER, "Coin : 6 ");
				
			}
			if (score == 7)
			{
				al_draw_text(font, al_map_rgb(250, 25, 45), 100, 10, ALLEGRO_ALIGN_CENTER, "Coin : 7 ");
				
			}
			if (score == 8)
			{
				al_draw_text(font, al_map_rgb(250, 25, 45), 100, 10, ALLEGRO_ALIGN_CENTER, "Coin : 8 ");
				
			}
			if (score == 9)
			{
				al_draw_text(font, al_map_rgb(250, 25, 45), 100, 10, ALLEGRO_ALIGN_CENTER, "Coin : 9 ");
				
			}
			if (score == 10)
			{
				al_draw_text(font, al_map_rgb(250, 25, 45), 100, 10, ALLEGRO_ALIGN_CENTER, "Coin : 10 ");
				
			}
			if (end)
			{
				al_draw_text(font1, al_map_rgb(0, 0, 0), 380, 240, ALLEGRO_ALIGN_CENTER, "GAME OVER ");
				done = true;
			}
			if (score == 10)
			{
				win = true;
				al_draw_text(font1, al_map_rgb(250, 25, 45), 380, 240, ALLEGRO_ALIGN_CENTER, "YOU WON ");
			}
			al_flip_display();


			al_clear_to_color(al_map_rgb(0, 255, 255));
			

		}
	}
	al_rest(5);
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_bitmap(player);
	//al_destroy_sample(sound);
	//al_destroy_font(font);
	al_destroy_bitmap(bg);
	return 0;
}
