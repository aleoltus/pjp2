#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <list>
#include <cstdlib>
#include <ctime>
#include <string>

#include "Menu.h"
#include "Gra.h"
#include "Scores.h"

using namespace std;

#define PI 3.14

#define SCREEN_WIDTH 30
#define SCREEN_HEIGHT 30

#define BOX_SIZE 20
#define HIGH_RAM 20

struct pozycja_t
{
	int x;
	int y;
};

bool operator==(pozycja_t a, pozycja_t b)
{
	return a.x == b.x && a.y == b.y;
}

bool operator!=(pozycja_t a, pozycja_t b)
{
	return !(a == b);
}

enum kierunek_t
{
	UP, DOWN, LEFT, RIGHT
};

kierunek_t w_jakim_kierunku(pozycja_t a, pozycja_t b)
{
	if (a.x == b.x - 1)
		return RIGHT;
	if (a.x == b.x + 1)
		return LEFT;
	if (a.y == b.y - 1)
		return DOWN;
	if (a.y == b.y + 1)
		return UP;

	throw "Nie da sie okreœliæ pozycji";
}

float angle(kierunek_t k)
{
	switch (k)
	{
	case UP:
		return 3 * PI / 2;
	case DOWN:
		return PI / 2;
	case LEFT:
		return PI;
	case RIGHT:
		return 0;
	}

	return 0;
}

pozycja_t przesun(pozycja_t a, kierunek_t k)
{
	pozycja_t out = a;
	switch (k)
	{
	case UP:
		out.y--;
		break;
	case DOWN:
		out.y++;
		break;
	case LEFT:
		out.x--;
		break;
	case RIGHT:
		out.x++;
		break;
	}

	return out;
}

struct snake_t
{
	kierunek_t kierunek;
	list<pozycja_t> body;

} snake;

void init_snake()
{
	snake.body.clear();
	snake.kierunek = RIGHT;

	pozycja_t p;

	p.x = SCREEN_WIDTH / 2;
	p.y = SCREEN_HEIGHT / 2;

	snake.body.push_back(p);

	p.x--;
	snake.body.push_back(p);

	p.x--;
	snake.body.push_back(p);
}

pozycja_t kawalek(int n)
{
	list<pozycja_t>::iterator it = snake.body.begin();

	while (n--)
		it++;

	return *it;
}

pozycja_t kawalek_od_konca(int n)
{
	list<pozycja_t>::reverse_iterator it = snake.body.rbegin();

	while (n--)
		it++;

	return *it;
}

bool czy_w_wezu(pozycja_t p)
{
	for (list<pozycja_t>::iterator it = snake.body.begin(); it != snake.body.end(); ++it)
	{
		if (*it == p)
			return true;
	}
	return false;
}

bool czy_w_wezu(int x, int y)
{
	pozycja_t p;
	p.x = x;
	p.y = y;

	return czy_w_wezu(p);
}

enum element_t
{
	FREE, WALL, FOOD, COIN
};

struct plansza_t
{
	element_t plansza[SCREEN_WIDTH][SCREEN_HEIGHT];
	int punkty;

	pozycja_t coin;
	pozycja_t food;
	pozycja_t mine;

	bool gameOver;

	int typ_owocu;

	string name;

} plansza;

pozycja_t rand_position()
{
	pozycja_t r;
	do
	{
		r.x = rand() % SCREEN_WIDTH;
		r.y = rand() % SCREEN_HEIGHT;
	} while (czy_w_wezu(r) || plansza.plansza[r.x][r.y] != FREE);

	return r;
}

void new_coin()
{
	plansza.plansza[plansza.coin.x][plansza.coin.y] = FREE;

	pozycja_t p = rand_position();

	plansza.plansza[p.x][p.y] = COIN;
	plansza.coin = p;
}

void new_food()
{
	plansza.typ_owocu = rand() % 5;

	plansza.plansza[plansza.food.x][plansza.food.y] = FREE;

	pozycja_t p = rand_position();

	plansza.plansza[p.x][p.y] = FOOD;
	plansza.food = p;
}

void new_mine()
{
	plansza.plansza[plansza.mine.x][plansza.mine.y] = FREE;

	pozycja_t p = rand_position();

	plansza.plansza[p.x][p.y] = WALL;
	plansza.mine = p;
}

void init_plansza(string name)
{
	for (int i = 0; i < SCREEN_WIDTH; ++i)
	for (int j = 0; j < SCREEN_HEIGHT; ++j)
		plansza.plansza[i][j] = FREE;

	plansza.punkty = 0;

	pozycja_t p = rand_position();

	plansza.plansza[p.x][p.y] = FOOD;
	plansza.food = p;

	p = rand_position();

	plansza.plansza[p.x][p.y] = COIN;
	plansza.coin = p;

	p = rand_position();

	plansza.plansza[p.x][p.y] = WALL;
	plansza.mine = p;

	plansza.gameOver = false;

	plansza.name = name;
}

struct grafiki_t
{
	ALLEGRO_BITMAP *image_snake;
	ALLEGRO_BITMAP *image_snake_tail;
	ALLEGRO_BITMAP *image_snake_head;
	ALLEGRO_BITMAP *food[5];
	ALLEGRO_BITMAP *coin;
	ALLEGRO_BITMAP *mine;
	ALLEGRO_BITMAP* tlo;

	ALLEGRO_FONT *font;

} grafiki;

void load_graph(string color)
{
	//grafiki.tlo = al_load_bitmap("Resources/tlo.jpg");
	grafiki.image_snake = al_load_bitmap(("Resources/snake_body_"+color+".png").c_str());
	grafiki.image_snake_tail = al_load_bitmap(("Resources/snake_tail_"+color+".png").c_str());
	grafiki.image_snake_head = al_load_bitmap(("Resources/snake_head_"+color+".png").c_str());

//	grafiki.food = al_load_bitmap("Resources/food.png");

	grafiki.food[0] = al_load_bitmap("Resources/apple.png");
	grafiki.food[1] = al_load_bitmap("Resources/pear.png");
	grafiki.food[2] = al_load_bitmap("Resources/kiwi.png");
	grafiki.food[3] = al_load_bitmap("Resources/cupcake.png");
	grafiki.food[4] = al_load_bitmap("Resources/strawberry.png");

	grafiki.coin = al_load_bitmap("Resources/coin.png");
	grafiki.mine = al_load_bitmap("Resources/monster.png");

	al_convert_mask_to_alpha(grafiki.food[0], al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(grafiki.food[1], al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(grafiki.food[2], al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(grafiki.food[3], al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(grafiki.food[4], al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(grafiki.coin, al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(grafiki.mine, al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(grafiki.image_snake_tail, al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(grafiki.image_snake_head, al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(grafiki.image_snake, al_map_rgb(0, 0, 0));

	grafiki.font = al_load_ttf_font("Resources/font.otf", HIGH_RAM * 0.8, 0);

}

void draw(snake_t& s, plansza_t& p)
{
	kierunek_t ustawienie_ogona = w_jakim_kierunku(kawalek_od_konca(0), kawalek_od_konca(1));

	list<pozycja_t>::iterator it = s.body.begin();
	al_draw_rotated_bitmap(grafiki.image_snake_head, BOX_SIZE / 2, BOX_SIZE / 2, (it->x + 0.5)*BOX_SIZE, (it->y + 0.5)*BOX_SIZE + HIGH_RAM, angle(snake.kierunek), snake.kierunek == LEFT ? ALLEGRO_FLIP_VERTICAL : 0);

	for (it++; *it != s.body.back(); it++)
	{
		al_draw_bitmap(grafiki.image_snake, it->x*BOX_SIZE, it->y*BOX_SIZE + HIGH_RAM, 0);
	}

	al_draw_rotated_bitmap(grafiki.image_snake_tail, BOX_SIZE / 2, BOX_SIZE / 2, (it->x + 0.5)*BOX_SIZE, (it->y + 0.5)*BOX_SIZE + HIGH_RAM, angle(ustawienie_ogona), 0);


	for (int i = 0; i < SCREEN_WIDTH; ++i)
	for (int j = 0; j < SCREEN_HEIGHT; ++j)
	{
		switch (plansza.plansza[i][j])
		{
		case FOOD:
			al_draw_bitmap(grafiki.food[plansza.typ_owocu], i*BOX_SIZE, j*BOX_SIZE + HIGH_RAM, 0);
			break;
		case COIN:
			al_draw_bitmap(grafiki.coin, i*BOX_SIZE, j*BOX_SIZE + HIGH_RAM, 0);
			break;
		case WALL:
			al_draw_bitmap(grafiki.mine, i*BOX_SIZE, j*BOX_SIZE + HIGH_RAM, 0);
			break;
		}
	}

	al_draw_textf(grafiki.font, al_map_rgb(0, 0, 0), 0, 0, 0, " %s %d", plansza.name.c_str(), plansza.punkty);

	if (plansza.gameOver)
	{
		al_draw_filled_rectangle(0, 0, (BOX_SIZE * SCREEN_HEIGHT), (BOX_SIZE * SCREEN_HEIGHT + HIGH_RAM), al_map_rgba(0, 0, 0, 200));

		al_draw_textf(grafiki.font, al_map_rgb(255, 0, 0), BOX_SIZE * SCREEN_WIDTH / 2, (BOX_SIZE * SCREEN_HEIGHT + HIGH_RAM) / 2, ALLEGRO_ALIGN_CENTRE, "-----KONIEC GRY-----");
	}

}

void gra(options op)
{
	srand(time(NULL));

	bool done = false;
	bool pause = true;
	bool move = false;

	//setup
	init_snake();
	init_plansza(op.name);

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;

	if (!al_init())
		return;

	display = al_create_display(SCREEN_WIDTH * BOX_SIZE, SCREEN_HEIGHT * BOX_SIZE + HIGH_RAM);
	
	al_clear_to_color(al_map_rgb(255, 233, 0));
	//al_draw_bitmap(grafiki.tlo, 0, 0, 0);
	al_draw_line(0, 20, 600, 20, al_map_rgb(0, 0, 0), 1);

	if (!display)
		return;

	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();

	al_set_window_title(display, "Snake - Gra");

	load_graph(op.color);

	event_queue = al_create_event_queue();
	timer = al_create_timer(0.4);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);

	//do obs³ugi miny
	int counter = 0;
	int hackCounter = 0;
	//pêtla
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (!plansza.gameOver)
				pause = false;

			kierunek_t ustawienie_glowy = w_jakim_kierunku(snake.body.front(), kawalek(1));

			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
			{
					done = true;
					Score s;
					s.name = op.name;
					s.score = plansza.punkty;
					AddScore(s);
			}
				break;
			case ALLEGRO_KEY_LEFT:
				//ignorujemy skrêty "wstecz"
				if (ustawienie_glowy != LEFT && !plansza.gameOver)
					snake.kierunek = LEFT;
				move = true;
				hackCounter = 0;

				break;
			case ALLEGRO_KEY_RIGHT:
				if (ustawienie_glowy != RIGHT && !plansza.gameOver)
					snake.kierunek = RIGHT;
				move = true;
				hackCounter = 0;

				break;
			case ALLEGRO_KEY_UP:
				if (ustawienie_glowy != UP && !plansza.gameOver)
					snake.kierunek = UP;
				move = true;
				hackCounter = 0;

				break;

			case ALLEGRO_KEY_DOWN:
				if (ustawienie_glowy != DOWN && !plansza.gameOver)
					snake.kierunek = DOWN;
				move = true;
				hackCounter = 0;
				break;
			case ALLEGRO_KEY_2:
				++hackCounter;
				if (hackCounter > 5)
				{
					hackCounter = 0;
					plansza.punkty += 50;
				}
			case ALLEGRO_KEY_1:
				++hackCounter;
				if (hackCounter > 5)
				{
					hackCounter = 0;
					plansza.punkty += 200;
				}
				break;
			default:
				hackCounter = 0;
			}
		}
		if ((move || ev.type == ALLEGRO_EVENT_TIMER) && !pause)
		{
			move = false;
			pozycja_t newHead = przesun(snake.body.front(), snake.kierunek);

			if (newHead.x < 0 || newHead.y < 0 || newHead.x >= SCREEN_WIDTH || newHead.y >= SCREEN_HEIGHT || czy_w_wezu(newHead))
			{
				plansza.gameOver = pause = true;
			}
			else
			{
				switch (plansza.plansza[newHead.x][newHead.y])
				{
				case COIN:
					new_coin();
					plansza.punkty++;
				case FREE:
					snake.body.pop_back();
					snake.body.push_front(newHead);
					break;
				case WALL:
					plansza.gameOver = pause = true;
					break;
				case FOOD:
					snake.body.push_front(newHead);
					plansza.punkty += 2;
					new_food();
					break;
				}
			}

			++counter;
			if (counter == 10)
			{
				counter = 0;
				new_mine();
			}
		}
		draw(snake, plansza);
		al_flip_display();
		//al_draw_bitmap(grafiki.tlo, 0, 0, 0);
		al_clear_to_color(al_map_rgb(255, 233, 0));
		al_draw_line(0, 20, 600, 20, al_map_rgb(0, 0, 0), 1);

	}
	
	al_destroy_display(display);
	//al_destroy_bitmap(grafiki.tlo);
}
