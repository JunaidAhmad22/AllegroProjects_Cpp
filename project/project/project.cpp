#include <stdio.h>
#include "pch.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdlib.h>
#include <allegro5/allegro_primitives.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
struct Perro {
	float x;
	float y;
	float lx;
	float ly;
};

struct trashp {
	float x;
	float y;
	int index;
	bool living;
};

struct aero_plane {
	float x;
	float y;
	float tempo;
};

struct Parallax {
	float x;
	float y;
	float x2;
};

struct speed_Parallax {
	float f1;
	float f2;
	float f3;
	float f4;
};




int ta = 10, fa = 0, con, v, agachar = false;
int weather_aero_plane = 0;
int cantrash = 0;
int test= 0;
int points = 0;
int lives = 5;
int bote = 0;

float speed_aero_plane = 9;
float speed_trash = 4;
bool increm = true;

//bool menu = true;
bool show_score = false;

int key[] = { 0, 0, 0, 0 };

float velXplayer = 23;
bool jump = false, inicio = false;
bool death = false;
char direction = 'f';
const float gravity = 0.5;

int score = 0;

Perro perro;

aero_plane aero_planep;

speed_Parallax speed_P;

Parallax parallaxPosition[4];

trashp trashp[3];

ALLEGRO_BITMAP *StartImage;

ALLEGRO_BITMAP *perros[4];

ALLEGRO_BITMAP *aero_plane;

ALLEGRO_BITMAP *pie;

ALLEGRO_BITMAP *b1;
ALLEGRO_BITMAP *b2;
ALLEGRO_BITMAP *b3;

ALLEGRO_BITMAP *life;

ALLEGRO_BITMAP *trash[18];
ALLEGRO_BITMAP *parallax[4];

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *timer;
ALLEGRO_SAMPLE *sample = NULL;
ALLEGRO_SAMPLE *cambio = NULL;
ALLEGRO_SAMPLE *error = NULL;
ALLEGRO_SAMPLE *background = NULL;
ALLEGRO_SAMPLE *gameover = NULL;

enum GAME_KEYS
{
	KEY_UP,
	KEY_DOWN,
};

void start_Variables()
{
	ta = 10; fa = 0; agachar = false;
	weather_aero_plane = 0;
	cantrash = 0;
	test= 0;
	points = 0;
	lives = 6;
	bote = 0;
	speed_aero_plane = 7;
	speed_trash = 1;
	aero_planep.tempo = speed_aero_plane;
	increm = true;

	
	show_score = false;

	velXplayer = 3.8;
	jump = false;
	death = false;
	direction = 'f';
	score = 0;

	perro.x = 100;
	perro.y = 250;

}
void Increase_in_speed_()
{
	if (points % 8 == 0)
	{
		if (points != 0)
			if (increm == true)
			{
				speed_aero_plane++;
				speed_trash += 0.4;

				aero_planep.tempo = speed_aero_plane;

				increm = false;
			}
	}
	else
	{
		increm = true;
	}
}

void drawinglives()
{
	for (int i = 0; i < lives; i++)
		al_draw_bitmap(life, 500 + (i * 30), 395, 0);
}

void DrawBoats()
{
	if (bote == 1)
		al_draw_bitmap(b1, 35, 372, 0);
	if (bote == 2)
		al_draw_bitmap(b2, 102, 372, 0);
	if (bote == 3)
	{
		al_draw_bitmap(b3, 170, 372, 0);
	}
}

void check_knock()
{
	for (int i = 0; i < 3; i++)
	{
		if (trashp[i].living == true)
		{
			if (trashp[i].x + 28 >= perro.x + 65 && trashp[i].x + 28 <= perro.x + 126)
			{
				if (trashp[i].y + 28 >= perro.y + 10 && trashp[i].y + 28 <= perro.y + 30)
				{
					if (trashp[i].index < 6)
					{
						if (bote == 2)
						{
							points++;
							al_play_sample(sample, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						}
						else
						{
							al_play_sample(error, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
							lives--;
						}
					}
					else
						if (trashp[i].index < 12)
						{
							if (bote == 1)
							{
								points++;
								al_play_sample(sample, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
							}
							else
							{
								al_play_sample(error, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
								lives--;
							}

						}
						else
							if (trashp[i].index < 18)
							{
								if (bote == 3)
								{
									points++;
									al_play_sample(sample, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
								}
								else
								{
									al_play_sample(error, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
									lives--;
								}
							}

					trashp[i].living = false;
					trashp[i].y = 500;

				}
			}
		}
	}
}

void drawingtrash()
{
	for (int i = 0; i < 3; i++)
	{
		if (trashp[i].living == true)
		{
			al_draw_bitmap(trash[trashp[i].index], trashp[i].x, trashp[i].y, 0);
			trashp[i].y += speed_trash;

			if (trashp[i].y > 290)
			{
				trashp[i].living = false;
				lives--;
				al_play_sample(error, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		}
	}
}

void Generatetrash()
{
	if (cantrash == 3)
		cantrash = 0;

	//SACAR Position X
	if (cantrash == 0)
		trashp[cantrash].x = 100 + rand() % (331 - 100);
	if (cantrash == 1)
		trashp[cantrash].x = 330 + rand() % (531 - 330);
	if (cantrash == 2)
		trashp[cantrash].x = 530 + rand() % (751 - 530);

	//trashp[cantrash].x=100+rand()%(750-100);

	trashp[cantrash].y = 10;
	trashp[cantrash].index = rand() % 18;
	trashp[cantrash].living = false;

	cantrash++;
}

void moveraero_plane()
{
	aero_planep.x -= aero_planep.tempo;
	if (aero_planep.x <= -110)
	{
		aero_planep.x = 800;
		Generatetrash();
	}

	if (trashp[cantrash - 1].living == false)
	{
		if (aero_planep.x <= trashp[cantrash - 1].x && aero_planep.x > 0)
		{
			trashp[cantrash - 1].living = true;
		}
	}
}

void moverPerro(char dir)
{
	if (dir == 'd')
	{
		if (perro.x < 674)
			perro.x += 5;
	}
	else
	{
		if (perro.x > 0)
			perro.x -= 10;
	}
}

void moverPallarax()
{
	//background
	parallaxPosition[0].x -= speed_P.f1;
	parallaxPosition[0].x2 -= speed_P.f1;

	if (parallaxPosition[0].x <= -1600)
		parallaxPosition[0].x = 1600;

	if (parallaxPosition[0].x2 <= -1600)
		parallaxPosition[0].x2 = 1600;

	parallaxPosition[1].x -= speed_P.f2;
	parallaxPosition[1].x2 -= speed_P.f2;

	if (parallaxPosition[1].x <= -1600)
		parallaxPosition[1].x = 1599 - (int)speed_P.f2;

	if (parallaxPosition[1].x2 <= -1600)
		parallaxPosition[1].x2 = 1599 - (int)speed_P.f2;


	parallaxPosition[2].x -= speed_P.f3;
	parallaxPosition[2].x2 -= speed_P.f3;

	if (parallaxPosition[2].x <= -1600)
		parallaxPosition[2].x = 1599 - (int)speed_P.f3;

	if (parallaxPosition[2].x2 <= -1600)
		parallaxPosition[2].x2 = 1599 - (int)speed_P.f3;
	
	
	parallaxPosition[3].x -= speed_P.f4;
	parallaxPosition[3].x2 -= speed_P.f4;

	if (parallaxPosition[3].x <= -1600)
		parallaxPosition[3].x = 1599 - (int)speed_P.f4;

	if (parallaxPosition[3].x2 <= -1600)
		parallaxPosition[3].x2 = 1599 - (int)speed_P.f4;

}

void Running()
{
	if (!agachar)
	{
		con++;
		if (con >= ta)
		{
			fa++;
			con = 0;
		}
		if (fa > 2)
			fa = 1;
		if (perro.y < 250)
			fa = 2;
		if (con < 0)
			fa = 3;
	}
	else
	{
		fa = 3;
		agachar = false;
		con = -12;
	}
}

void start_Parallax()
{
	parallaxPosition[0].x = 0;
	parallaxPosition[0].y = 0;
	parallaxPosition[0].x2 = 1600;

	parallaxPosition[1].x = 0;
	parallaxPosition[1].y = 0;
	parallaxPosition[1].x2 = 1600;

	parallaxPosition[2].x = 0;
	parallaxPosition[2].y = 0;
	parallaxPosition[2].x2 = 1600;

	parallaxPosition[3].x = 0;
	parallaxPosition[3].y = 0;
	parallaxPosition[3].x2 = 1600;
}

void start_perro()
{
	perro.x = 100;
	perro.y = 250;
	perro.lx = 126;
	perro.ly = 97;
}

void start_speed_P()
{
	speed_P.f1 = 2;
	speed_P.f2 = 3;
	speed_P.f3 = 4;
	speed_P.f4 = 5;
}

void start_aero_plane()
{
	aero_planep.x = 820;
	aero_planep.y = 10;
	aero_planep.tempo = speed_aero_plane;
}

int main()
{
	srand(time(NULL));
	float vely, Veljump = 12; //vely is the velocity in Y for gravity veljump is the velocity of each leap with respect to
	//vely = speed Y
	vely = 0; //starts at 0 because it is going to increase with gravity, which will make every second that falls will fall
	int redraw = 1;


	al_init();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_init_primitives_addon();

	al_install_audio();
	al_init_acodec_addon();

	al_reserve_samples(4);


	sample = al_load_sample("point.wav");
	cambio = al_load_sample("cambio.wav");
	error = al_load_sample("error.wav");
	gameover = al_load_sample("gameover.wav");

	background = al_load_sample("safegaurd.wav");

	ALLEGRO_SAMPLE_INSTANCE *songInstance = al_create_sample_instance(background);

	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());


	ALLEGRO_FONT *font = al_load_font("coolvetica.ttf", 20, 0);
	ALLEGRO_FONT *font2 = al_load_font("coolvetica.ttf", 80, 0);
	ALLEGRO_FONT *font3 = al_load_font("coolvetica.ttf", 50, 0);


	start_perro();
	start_Parallax();
	start_speed_P();
	start_aero_plane();

	display = al_create_display(950, 550); 
	event_queue = al_create_event_queue(); 
	timer = al_create_timer(1.0 / 60); 

	perros[0] = al_load_bitmap("Perro1.png");
	perros[1] = al_load_bitmap("Perro1.png");
	perros[2] = al_load_bitmap("Perro1.png");
	perros[3] = al_load_bitmap("Perro1.png");

	aero_plane = al_load_bitmap("aero_plane.png");
	pie = al_load_bitmap("pie.png");

	StartImage = al_load_bitmap("inicio.png");

	b1 = al_load_bitmap("b1.png");
	b2 = al_load_bitmap("b2.png");
	b3 = al_load_bitmap("b3.png");

	life = al_load_bitmap("life.png");

	trash[0] = al_load_bitmap("trash/i1.png");
	trash[1] = al_load_bitmap("trash/i2.png");
	trash[2] = al_load_bitmap("trash/i3.png");
	trash[3] = al_load_bitmap("trash/i4.png");
	trash[4] = al_load_bitmap("trash/i5.png");
	trash[5] = al_load_bitmap("trash/i6.png");

	trash[6] = al_load_bitmap("trash/o1.png");
	trash[7] = al_load_bitmap("trash/o2.png");
	trash[8] = al_load_bitmap("trash/o3.png");
	trash[9] = al_load_bitmap("trash/o4.png");
	trash[10] = al_load_bitmap("trash/o5.png");
	trash[11] = al_load_bitmap("trash/o6.png");

	trash[12] = al_load_bitmap("trash/r1.png");
	trash[13] = al_load_bitmap("trash/r2.png");
	trash[14] = al_load_bitmap("trash/r3.png");
	trash[15] = al_load_bitmap("trash/r4.png");
	trash[16] = al_load_bitmap("trash/r5.png");
	trash[17] = al_load_bitmap("trash/r6.png");

	parallax[0] = al_load_bitmap("f1.png");
	parallax[1] = al_load_bitmap("f2.png");
	parallax[2] = al_load_bitmap("f3.png");
	parallax[3] = al_load_bitmap("f4.png");


	al_register_event_source(event_queue, al_get_display_event_source(display));// recording the display
	al_register_event_source(event_queue, al_get_timer_event_source(timer)); //recording the timer
	al_register_event_source(event_queue, al_get_keyboard_event_source()); //recording the keys

	al_start_timer(timer);



	while (1)
	{

		ALLEGRO_EVENT event; //events allegro
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			break;
		}


		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_UP)
			{
				if (perro.y == 250)
				{
					vely = -Veljump;
					jump = false;
				}
			}
				
			if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
			{
				agachar = true;
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
			{
				direction = 'd';
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
			{
				direction = 'i';
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_A)
			{
				bote = 1;
				al_play_sample(cambio, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			if (event.keyboard.keycode == ALLEGRO_KEY_S)
			{
				bote = 2;
				al_play_sample(cambio, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			
			if (event.keyboard.keycode == ALLEGRO_KEY_D)
			{
				bote = 3;
				al_play_sample(cambio, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				if (inicio)
				{
					inicio = false;
					show_score = false;
					for (int i = 0; i < 3; i++)
					{
						if (trashp[i].living == true)
						{
							trashp[i].living = false;
						}
					}
					al_stop_sample_instance(songInstance);
				}
				else
				{
					break;
				}
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
			{
				if (show_score == true)
				{
					show_score = false;
					inicio = false;
				}
				else
				{
					vely = 0;
					start_Variables();
					inicio = true;
					al_play_sample_instance(songInstance);
				}
			}
		}

		if (event.type == ALLEGRO_EVENT_KEY_UP) //moving up down
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_UP)
				key[KEY_UP] = 0;
			if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
				key[KEY_DOWN] = 0;
			if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
			{
				direction = 'f';
			}
			if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
			{
				direction = 'f';
			}
		}


		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			//making jumps

			if (!death) //continue
				if (inicio) //
				{
					/*gravity jumps*/
					if (!jump) //	speed increases with gravity
						vely += gravity;
					else
						vely = 0; //speed will stop 

					perro.y += vely; //speed will be reversed

					jump = (perro.y >= 250); //jump will continue inside the screen

					if (perro.y > 250)
						perro.y = 250;


					Running();

					moverPallarax();

					weather_aero_plane++;

					moveraero_plane();

					check_knock();

					Increase_in_speed_();

					if (lives == 0)
					{
						al_play_sample(gameover, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						for (int i = 0; i < 3; i++)
						{
							if (trashp[i].living == true)
							{
								trashp[i].living = false;
							}
						}

						al_stop_sample_instance(songInstance);
						inicio = false;
						show_score = true;
					}

					if (direction == 'd')
						moverPerro(direction);
					if (direction == 'i')
						moverPerro(direction);

				}
			redraw = 1;
		}
		if (redraw)
		{
			al_clear_to_color(al_map_rgb(100, 100, 100));

			al_draw_bitmap(parallax[0], parallaxPosition[0].x, parallaxPosition[0].y, 0);
			al_draw_bitmap(parallax[0], parallaxPosition[0].x2, parallaxPosition[0].y, 0);

			al_draw_bitmap(parallax[1], parallaxPosition[1].x, parallaxPosition[1].y, 0);
			al_draw_bitmap(parallax[1], parallaxPosition[1].x2, parallaxPosition[1].y, 0);

			al_draw_bitmap(parallax[2], parallaxPosition[2].x, parallaxPosition[2].y, 0);
			al_draw_bitmap(parallax[2], parallaxPosition[2].x2, parallaxPosition[2].y, 0);

			drawingtrash();

			al_draw_bitmap(parallax[3], parallaxPosition[3].x, parallaxPosition[3].y, 0);
			al_draw_bitmap(parallax[3], parallaxPosition[3].x2, parallaxPosition[3].y, 0);

			al_draw_bitmap(pie, 0, 370, 0);

			DrawBoats();

			al_draw_bitmap(aero_plane, aero_planep.x, aero_planep.y, 0);

			al_draw_bitmap(perros[fa], perro.x, perro.y, 0);

			drawinglives();

			al_draw_textf(font, al_map_rgb(255, 255, 255), 670, 393, 0, "points %d", points);

			if (show_score == false)
			{
				if (!inicio)
				{
					al_draw_bitmap(StartImage, 0, 0, 0);
				}
			}
			else
			{
				//storing score
				ifstream pointsMaxread("info.txt");
				string pun;
				if (pointsMaxread.is_open())
				{
					getline(pointsMaxread, pun);
					pointsMaxread.close();
				}

				int pmax = atoi(pun.c_str());


				if (points > pmax)
				{
					ofstream pointsMaxEsc;

					//char * x_str = new char[2];
					//pun = itoa(points,x_str,10);

					pointsMaxEsc.open("info.txt");
					pointsMaxEsc << points;
					pointsMaxEsc.close();
				}

				al_draw_textf(font2, al_map_rgb(255, 255, 255), 265, 50, 0, "Game over!");
				al_draw_textf(font2, al_map_rgb(255, 255, 255), 180, 175, 0, "Your score is: %d", points);
				al_draw_textf(font3, al_map_rgb(255, 255, 255), 165, 300, 0, "Best score is       :%d", pmax);

			}

			al_flip_display();
			redraw = 0;

		}
	}

	al_destroy_sample(sample);
	al_destroy_sample(background);
	al_destroy_sample_instance(songInstance);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}
