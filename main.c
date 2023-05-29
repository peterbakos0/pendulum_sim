#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#define LENM 0.03
#define G 1.6
#define IFIR 30 * M_PI / 180
#define FPS 30
#define DT 1.0 / FPS
#define WW 640
#define WH 480
#define PPM 5400
#define MCRM 0.0015

struct state_t
{
	float fir, beta, omega;
};

unsigned lenp, mcrp;
struct state_t s;
SDL_Renderer *r;

void draw_circle(unsigned cr, int x, int y);
void update(void);
void render(void);
void tick(void);

void draw_circle(unsigned cr, int x, int y)
{
	char s;
	int cy;
	unsigned dx, dy;

	for(dx = 0U; dx < cr; dx++)
	{
		dy = sqrt(pow(cr, 2) - pow(dx, 2));

		for(s = -1; s <= 1; s += 2)
		{
			for(cy = y - dy + 1; cy < y + dy; cy++)
				SDL_RenderDrawPoint(r, x + dx * s, cy);
		}
	}
}

void update(void)
{
	s.beta = sin(s.fir) * G / LENM;
	s.omega += s.beta * DT;
	s.fir -= s.omega * DT;
}

void render(void)
{
	int x, y, dx, dy;
	float rt;
	unsigned i;

	dx = sin(s.fir) * lenp;
	dy = cos(s.fir) * lenp;

	for(i = 0U; i < lenp; i++)
	{
		rt = ((float) i) / lenp;

		x = WW / 2 + rt * dx;
		y = WH / 2 + rt * dy;

		SDL_RenderDrawPoint(r, x, y);
	}

	draw_circle(mcrp, x, y);
}

void tick(void)
{
	render();
	update();
}

int main()
{
	bool ir;
	useconds_t p;
	SDL_Window *w;
	SDL_Event e;

	lenp = LENM * PPM;
	mcrp = MCRM * PPM;

	s.fir = IFIR;

	p = 1000000 / FPS;

	SDL_Init(SDL_INIT_VIDEO);

	w = SDL_CreateWindow(
		"pendulum",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WW, WH,
		0
	);

	r = SDL_CreateRenderer(w, -1, 0);

	ir = true;

	while(ir)
	{
		while(SDL_PollEvent(&e))
			ir = e.type != SDL_QUIT;

		SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
		SDL_RenderClear(r);

		SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
		tick();

		SDL_RenderPresent(r);

		usleep(p);
	}

	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);

	SDL_Quit();

	return 0;
}

