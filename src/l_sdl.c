#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

#include "obj.c"
#include "eval.c"
#include "l_std.c"
#include "util.h"
#include "error.h"


typedef struct 
{
	const char* type;
	SDL_Window* win;
	SDL_Renderer* rd;
} SDL_WIN_OBJ;

OBJ* sdl_create_win(int argc, OBJ* o)
{
	
	OBJ* ret = empty_obj_t(T_OTHER);
	SDL_Window* win = SDL_CreateWindow(
	  "test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	  1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|SDL_WINDOW_ALLOW_HIGHDPI
	);
	if (win == NULL)
	{
		LOBOTOMY_ERROR("SDL ERROR: couldn't create new window");
	}

	SDL_Renderer* rd = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (rd == NULL)
	{
		LOBOTOMY_ERROR("SDL ERROR: couldn't create new renderer");
	}

	
	SDL_WIN_OBJ* wrapper = malloc(sizeof(SDL_WIN_OBJ));
	wrapper->type = "SDL_WIN";
	wrapper->win = win;
	wrapper->rd = rd;
	ret->data = wrapper;
	return ret;
}


OBJ* sdl_draw_rect(int argc, OBJ* o)
{
	o = preeval(o);
	// o = __eval(o, 1);
	// SDL_WIN_OBJ* wrapper = (SDL_WIN_OBJ*)o->data;
	// SDL_Window* win = wrapper->win;
	// SDL_Renderer* rd = wrapper->rd;

	// OBJ* x = NT(o);
	// OBJ* y = NT(x);
	// OBJ* w = NT(y);
	// OBJ* h = NT(w);

	// SDL_SetRenderDrawColor(rd, 0, 0 ,0, 255);
	// SDL_RenderClear(rd);
	// SDL_SetRenderDrawColor(rd, 255, 0 ,0, 255);
	// SDL_Rect r = (SDL_Rect){x->num, y->num, w->num, h->num};
	// SDL_RenderDrawRect(rd, &r);

	// SDL_RenderPresent(rd);
	return NIL;
}

OBJ* sdl_destroy_win(int argc, OBJ* o)
{
	if (o) SDL_DestroyWindow(o->data);
	SDL_Quit();
	return NIL;
}



void l_sdl_load(ENV* env)
{
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
	{
		LOBOTOMY_ERROR("SDL ERROR: couldn't initalize SDL2");
	}
	env_add(env, create_cfn("SDL-create-win", sdl_create_win));
	env_add(env, create_cfn("dr", sdl_draw_rect));
	env_add(env, create_cfn("dw", sdl_destroy_win));
}
