#include "Core/Platform/CuWindowSDL.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>

CuCore::CuWindowSDL::~CuWindowSDL()
{
	
}

void CuCore::CuWindowSDL::Create(const char* Title, const int Width, const int Height)
{
	Window = SDL_CreateWindow(Title, Width, Height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	CU_ASSERT(Window, "Could not create SDL window: %s", SDL_GetError());
}

void CuCore::CuWindowSDL::Close()
{
	if(Window)
	{
		SDL_DestroyWindow(Window);
		Window = nullptr;
	}
}

bool CuCore::CuWindowSDL::ShouldClose() const
{
	return CanClose;
}

void CuCore::CuWindowSDL::PollEvents()
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		if (e.type == SDL_EVENT_QUIT)
			CanClose = true;
	}
}
