#include <stdio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_ttf.h>

#include "Menu.h"
#include "Scores.h"

static bool wybor(options&, ALLEGRO_EVENT_QUEUE*);
void highScore(ALLEGRO_EVENT_QUEUE* q);
ALLEGRO_FONT *font;
ALLEGRO_BITMAP *obrazek;

int szer = 600, wys = 600;

#pragma region menu

const string pozycje[] = { "Nowa Gra", "Ranking", "Koniec" };

void draw_menu(int pos)
{
	al_clear_to_color(al_map_rgb(0, 0, 100));
	obrazek = al_load_bitmap("Resources/snake.png");
	al_draw_bitmap(obrazek, 190, 190, 0);

	auto white = al_map_rgb(255, 255, 255);
	auto red = al_map_rgb(255, 0, 0);
	//al_draw_text(font, al_map_rgb(255, 255, 255), 140, 100, 0, "Snake - Gra");

	al_draw_rounded_rectangle(205, 290, 385, 340, 20, 20, al_map_rgba(0, 255, 0, 128), 2);
	al_draw_text(font, pos == 0 ? red : white, 255, 305, 0, "Nowa Gra");

	al_draw_rounded_rectangle(205, 350, 385, 400, 20, 20, al_map_rgba(0, 255, 0, 128), 2);
	al_draw_text(font, pos == 1 ? red : white, 260, 365, 0, "Ranking");

	al_draw_rounded_rectangle(205, 410, 385, 460, 20, 20, al_map_rgba(0, 255, 0, 128), 2);
	al_draw_text(font, pos == 2 ? red : white, 265, 425, 0, "Koniec");
	al_flip_display();
}

options menu()
{
	bool done = false;

	options op;
	op.scale = 1;
	op.exit = false;
	//op.sound = true;
	op.name = "";
	op.color = "green"; //blue, red, sweet...

	al_init();
	al_init_primitives_addon();
	al_init_font_addon();
	al_install_keyboard();
	al_init_ttf_addon();
	al_init_image_addon();

	ALLEGRO_DISPLAY *display = al_create_display(szer, wys);
	al_set_window_title(display, "Snake - Gra");
	font = al_load_ttf_font("Resources/font.otf", 15, 0);
	ALLEGRO_KEYBOARD_STATE keyboard;

	auto event_queue = al_create_event_queue();

	al_get_keyboard_state(&keyboard);
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));

	int selected_option = 0;

	draw_menu(selected_option);

	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				--selected_option;
				if (selected_option < 0)
					selected_option = 2;
				break;

			case ALLEGRO_KEY_DOWN:
				++selected_option;
				if (selected_option > 2)
					selected_option = 0;
				break;

			case ALLEGRO_KEY_ENTER:
				if (selected_option == 0)
				{
					if (wybor(op, event_queue))
					{
						op.exit = false;
						done = true;
					}
				}
				else if (selected_option == 1)
				{
					highScore(event_queue);
				}
				else if (selected_option == 2)
				{
					op.exit = true;
					done = true;
				}
				
				break;

			case ALLEGRO_KEY_ESCAPE:
				done = true;
				op.exit = true;

			}

			draw_menu(selected_option);
		}
	}

	al_destroy_display(display);

	return op;
}

#pragma endregion

#pragma region Wybor

struct grafiki
{
	ALLEGRO_BITMAP *x;

} grafiki;
void draw_wybor(int pos, string imie){

	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_clear_to_color(al_map_rgb(0, 0, 100));

	al_draw_text(font, al_map_rgb(255, 255, 255), 110, 200, 0, "Wpisz imie: ");
	al_draw_rectangle(210, 200, 430, 220, al_map_rgba(0, 255, 0, 128), 2);

	al_draw_text(font, al_map_rgb(255, 255, 255), 240, 260, 0, "Wybierz weza: ");
	auto red = al_map_rgb(255, 0, 0);
	auto green = al_map_rgb(0, 255, 0);

	al_draw_text(font, al_map_rgb(255, 255, 255), 210, 200, 0, imie.c_str());

	al_draw_rectangle(205, 290, 245, 330, pos==0?red:green, 2);
	grafiki.x =  al_load_bitmap("Resources/green.png");
	al_draw_bitmap(grafiki.x, 205, 290, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	al_draw_rectangle(255, 290, 295, 330, pos==1?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/red.png");
	al_draw_bitmap(grafiki.x, 255, 290, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	al_draw_rectangle(305, 290, 345, 330, pos==2?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/blue.png");
	al_draw_bitmap(grafiki.x, 305, 290, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	al_draw_rectangle(355, 290, 395, 330, pos==3?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/sweet.png");
	al_draw_bitmap(grafiki.x, 355, 290, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	al_draw_rectangle(205, 340, 245, 380, pos==4?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/bluedot.png");
	al_draw_bitmap(grafiki.x, 205, 340, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	al_draw_rectangle(255, 340, 295, 380, pos==5?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/greendot.png");
	al_draw_bitmap(grafiki.x, 255, 340, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	/*al_draw_rectangle(305, 340, 345, 380, pos==6?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/sweetdot.png");
	al_draw_bitmap(grafiki.x, 305, 340, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	/*al_draw_rectangle(355, 340, 395, 380, pos==7?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/reddot.png");
	al_draw_bitmap(grafiki.x, 355, 340, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	/*al_draw_rectangle(205, 390, 245, 430, pos==8?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/classic.png");
	al_draw_bitmap(grafiki.x, 205, 390, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));*/

	al_draw_rectangle(355, 340, 395, 380, pos==7?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/classic2.png");
	al_draw_bitmap(grafiki.x, 355, 340, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	al_draw_rectangle(305, 340, 345, 380, pos == 8 ? red : green, 2);
	grafiki.x = al_load_bitmap("Resources/nyan.png");
	al_draw_bitmap(grafiki.x, 305, 340, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));


	al_flip_display();
}

static bool wybor(options& op, ALLEGRO_EVENT_QUEUE* q)
{
	string kolory[] = {"green", "red", "blue", "sweet", "bluedot","greendot", "nyan","classic2" };
	int pos = 0;

	while (true)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(q, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_LEFT:
				pos--;
				if (pos < 0)
					pos = 8;
				op.color = kolory[pos];
				break;
			case ALLEGRO_KEY_RIGHT:
				pos++;
				if (pos > 8)
					pos = 0;
				op.color = kolory[pos];
				break;
			case ALLEGRO_KEY_DOWN:
				pos += 4;
				pos %= 12;
				if (pos > 8)
					pos %= 4;
				op.color = kolory[pos];
				break;
			case ALLEGRO_KEY_UP:
				pos -= 4;
				if (pos < 0)
					pos += 12;
				if (pos >8)
					pos -= 4;
				op.color = kolory[pos];
				break;
		
			case ALLEGRO_KEY_ENTER:
				return true;
			case ALLEGRO_KEY_ESCAPE:
				return false;
			case ALLEGRO_KEY_BACKSPACE:
				if (op.name.size() > 0)
					op.name.resize(op.name.size() - 1);
			}
			if (ev.keyboard.keycode >= ALLEGRO_KEY_A && ev.keyboard.keycode <= ALLEGRO_KEY_Z && op.name.size() <=20)
			{
				char litera = 'A' + ev.keyboard.keycode - ALLEGRO_KEY_A;
				op.name += litera;
			}
			if (ev.keyboard.keycode >= ALLEGRO_KEY_0 && ev.keyboard.keycode <= ALLEGRO_KEY_9 && op.name.size() <= 20)
			{
				char litera = '0' + ev.keyboard.keycode - ALLEGRO_KEY_0;
				op.name += litera;
			}
			

		}
		draw_wybor(pos, op.name);
	}
}

#pragma endregion

#pragma region HighScore

void draw_highScore()
{
	al_clear_to_color(al_map_rgb(0, 0, 100));

	al_draw_text(font, al_map_rgb(255, 255, 255), 270, 185, 0, "Ranking: ");
	al_draw_rounded_rectangle(180, 170, 430, 220, 20, 20, al_map_rgba(0, 255, 0, 128), 2);

	wyniki w = ReadScores();

	int pos = 0;
	for (auto it = w.begin(); it != w.end(); it++)
	{
		char napis[50];
		sprintf(napis, "%s %d", it->name.c_str(), it->score);
		al_draw_text(font, al_map_rgb(255, 255, 255), 245, 220 + pos*20, 0, napis);
		++pos;
	}

	al_flip_display();
}

void highScore(ALLEGRO_EVENT_QUEUE* q)
{

	draw_highScore();
	while (true)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(q, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			return;
	}
}

#pragma endregion