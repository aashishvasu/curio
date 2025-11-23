#include "Core/Platform/WindowSDL.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>

CuCore::WindowSDL::~WindowSDL()
{
	
}

void CuCore::WindowSDL::Create(const WindowProperties& inProps)
{
	Window = SDL_CreateWindow(inProps.Title, inProps.Width, inProps.Height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	CU_ASSERT(Window, "Could not create SDL window: %s", SDL_GetError());
}

void CuCore::WindowSDL::Close()
{
	if(Window)
	{
		SDL_DestroyWindow(Window);
		Window = nullptr;
	}
}

bool CuCore::WindowSDL::ShouldClose() const
{
	return CanClose;
}

void CuCore::WindowSDL::PollEvents()
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		if (e.type == SDL_EVENT_QUIT)
			CanClose = true;
	}
}
