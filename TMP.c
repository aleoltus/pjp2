#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

int main(void)
{

	int width = 640;
	int height = 480;
	bool done = false;
	bool check[4] = { true, false, false, false };
	enum keys{ UP, DOWN, LEFT, RIGHT };

	int x = 10;
	int y = height / 2;

	const int maxFrame = 6;
	int curFrame = 0;
	int frameCount = 0;
	int frameDelay = 5;

	int frameWidth = 275;
	int frameHeight = 46;

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;
	ALLEGRO_BITMAP *imageup, *imagedown, *imageleft, *imageright;

	if (!al_init())
		return -1;

	display = al_create_display(width, height);

	if (!display)
		return -1;


	al_install_keyboard();
	al_init_image_addon();

	imageup = al_load_bitmap("data/pacmanup.bmp");
	imagedown = al_load_bitmap("data/pacmandown.bmp");
	imageleft = al_load_bitmap("data/pacmanleft.bmp");
	imageright = al_load_bitmap("data/pacmanrigh.bmp");

	al_convert_mask_to_alpha(imageup, al_map_rgb(106, 76, 48));
	al_convert_mask_to_alpha(imagedown, al_map_rgb(106, 76, 48));
	al_convert_mask_to_alpha(imageleft, al_map_rgb(106, 76, 48));
	al_convert_mask_to_alpha(imageright, al_map_rgb(106, 76, 48));



	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);

	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				check[LEFT] = true;
				check[UP] = false;
				check[DOWN] = false;
				check[RIGHT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				check[LEFT] = false;
				check[UP] = false;
				check[DOWN] = false;
				check[RIGHT] = true;
				break;
			case ALLEGRO_KEY_UP:
				check[LEFT] = false;
				check[UP] = true;
				check[DOWN] = false;
				check[RIGHT] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				check[LEFT] = false;
				check[UP] = false;
				check[DOWN] = true;
				check[RIGHT] = false;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			if (++frameCount >= frameDelay)
			{
				if (++curFrame >= maxFrame)
					curFrame = 0;

				frameCount = 0;
			}
			if (check[UP] == true)
			{
				y -= 2;

				if (y < -20)
					y = height;
				al_draw_bitmap_region(imageup, 0, curFrame * frameHeight, frameWidth, frameHeight, x, y, 0);
			}
			else if (check[DOWN] == true)
			{
				y += 2;

				if (y >= height)
					y = -20;
				al_draw_bitmap_region(imagedown, 0, curFrame * frameHeight, frameWidth, frameHeight, x, y, 0);
			}
			else if (check[LEFT] == true)
			{
				x -= 2;

				if (x < -20)
					x = width;
				al_draw_bitmap_region(imageleft, curFrame * frameHeight, 0, frameHeight, frameWidth, x, y, 0);
			}
			else if (check[RIGHT] == true)
			{
				x += 2;

				if (x >= width)
					x = -20;
				al_draw_bitmap_region(imageright, curFrame * frameHeight, 0, frameHeight, frameWidth, x, y, 0);
			}

		}

		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}


	al_destroy_event_queue(event_queue);
	al_destroy_display(display);

	return 0;
}