#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include "obj.c"
#include "util.h"
#include "error.h"




static OBJ sdl_init_flag = {.name="sdl-init-flag", .type=T_NUM, .num=0};
typedef struct 
{
	int type;
	SDL_Window* win;
}SDL_Win_wrapper;

OBJ* sdl_create_win(int argc, OBJ* o)
{

	// if (sdl_init_flag.type != T_TRUE || sdl_init_flag.num == 0) {
		// sdl_init_flag.num = SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
		// if (sdl_init_flag.num != 0) {
			// LOBOTOMY_ERROR_S(ERR_RUNTIME, "Failed to initialize SDL");
		// }
	// }
	OBJ* ret = empty_obj_t(T_VOID_PTR);
	SDL_Window* win = SDL_CreateWindow(
		  "test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		  1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|SDL_WINDOW_ALLOW_HIGHDPI
		  );

	// SDL_Win_wrapper wrapper = malloc(sizeof(SDL_Win_wrapper));
	// wrapper->win = win;
	// wrapper->type = 
	ret->data = win;
	return ret;
}


OBJ* sdl_destroy_win(int argc, OBJ* o)
{
	// SDL_DestroyWindow(o->data);
	SDL_Quit();
	return NIL;
}



void l_sdl_load(ENV* env)
{
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	env_add(env, create_cfn("SDL-create-win", sdl_create_win));
	env_add(env, create_cfn("SDL-destroy-win", sdl_destroy_win));
	env_add(env, &sdl_init_flag);
}