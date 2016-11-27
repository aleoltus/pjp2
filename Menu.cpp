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

int szer = 400, wys = 400;

#pragma region menu

const string pozycje[] = { "Nowa Gra", "Ranking", "Koniec" };

void draw_menu(int pos)
{
	al_clear_to_color(al_map_rgb(0, 0, 100));
	obrazek = al_load_bitmap("Resources/snake.png");
	al_draw_bitmap(obrazek, 90, 90, 0);

	auto white = al_map_rgb(255, 255, 255);
	auto red = al_map_rgb(255, 0, 0);
	//al_draw_text(font, al_map_rgb(255, 255, 255), 140, 100, 0, "Snake - Gra");

	al_draw_rounded_rectangle(105, 190, 285, 240, 20, 20, al_map_rgba(0, 255, 0, 128), 2);
	al_draw_text(font, pos == 0 ? red : white, 155, 205, 0, "Nowa Gra");

	al_draw_rounded_rectangle(105, 250, 285, 300, 20, 20, al_map_rgba(0, 255, 0, 128), 2);
	al_draw_text(font, pos == 1 ? red : white, 160, 265, 0, "Ranking");

	al_draw_rounded_rectangle(105, 310, 285, 360, 20, 20, al_map_rgba(0, 255, 0, 128), 2);
	al_draw_text(font, pos == 2 ? red : white, 165, 325, 0, "Koniec");
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

	al_draw_text(font, al_map_rgb(255, 255, 255), 10, 100, 0, "Wpisz imie: ");
	al_draw_rectangle(110, 100, 330, 120, al_map_rgba(0, 255, 0, 128), 2);

	al_draw_text(font, al_map_rgb(255, 255, 255), 140, 160, 0, "Wybierz weza: ");
	auto red = al_map_rgb(255, 0, 0);
	auto green = al_map_rgb(0, 255, 0);

	al_draw_text(font, al_map_rgb(255, 255, 255), 110, 100, 0, imie.c_str());

	al_draw_rectangle(105, 190, 145, 230, pos==0?red:green, 2);
	grafiki.x =  al_load_bitmap("Resources/green.png");
	al_draw_bitmap(grafiki.x, 105, 190, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	al_draw_rectangle(155, 190, 195, 230, pos==1?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/red.png");
	al_draw_bitmap(grafiki.x, 155, 190, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	al_draw_rectangle(205, 190, 245, 230, pos==2?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/blue.png");
	al_draw_bitmap(grafiki.x, 205, 190, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	al_draw_rectangle(255, 190, 295, 230, pos==3?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/sweet.png");
	al_draw_bitmap(grafiki.x, 255, 190, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	al_draw_rectangle(105, 240, 145, 280, pos==4?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/bluedot.png");
	al_draw_bitmap(grafiki.x, 105, 240, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	al_draw_rectangle(155, 240, 195, 280, pos==5?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/greendot.png");
	al_draw_bitmap(grafiki.x, 155, 240, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	al_draw_rectangle(205, 240, 245, 280, pos==6?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/sweetdot.png");
	al_draw_bitmap(grafiki.x, 205, 240, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	al_draw_rectangle(255, 240, 295, 280, pos==7?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/reddot.png");
	al_draw_bitmap(grafiki.x, 255, 240, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	al_draw_rectangle(105, 290, 145, 330, pos==8?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/classic.png");
	al_draw_bitmap(grafiki.x, 105, 290, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	al_draw_rectangle(155, 290, 195, 330, pos==9?red:green, 2);
	grafiki.x = al_load_bitmap("Resources/classic2.png");
	al_draw_bitmap(grafiki.x, 155, 290, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));

	al_draw_rectangle(205, 290, 245, 330, pos == 10 ? red : green, 2);
	grafiki.x = al_load_bitmap("Resources/nyan.png");
	al_draw_bitmap(grafiki.x, 205, 290, 0);
	al_convert_mask_to_alpha(grafiki.x, al_map_rgb(225, 225, 225));


	al_flip_display();
}

static bool wybor(options& op, ALLEGRO_EVENT_QUEUE* q)
{
	string kolory[] = {"green", "red", "blue", "sweet", "bluedot", "greendot", "sweetdot", "reddot", "classic", "classic2", "nyan"};
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
					pos = 10;
				op.color = kolory[pos];
				break;
			case ALLEGRO_KEY_RIGHT:
				pos++;
				if (pos > 10)
					pos = 0;
				op.color = kolory[pos];
				break;
			case ALLEGRO_KEY_DOWN:
				pos += 4;
				pos %= 12;
				if (pos > 10)
					pos %= 4;
				op.color = kolory[pos];
				break;
			case ALLEGRO_KEY_UP:
				pos -= 4;
				if (pos < 0)
					pos += 12;
				if (pos >9)
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

	al_draw_text(font, al_map_rgb(255, 255, 255), 170, 85, 0, "Ranking: ");
	al_draw_rounded_rectangle(80, 70, 330, 120, 20, 20, al_map_rgba(0, 255, 0, 128), 2);

	wyniki w = ReadScores();

	int pos = 0;
	for (auto it = w.begin(); it != w.end(); it++)
	{
		char napis[50];
		sprintf(napis, "%s %d", it->name.c_str(), it->score);
		al_draw_text(font, al_map_rgb(255, 255, 255), 145, 120 + pos*20, 0, napis);
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