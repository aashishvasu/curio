#pragma once

#include "IWindow.h"

struct SDL_Window;
namespace CuCore
{
	class WindowSDL final : public IWindow
	{
	public:
		~WindowSDL() override;

		// IWindow Interface
		void Create(const WindowProperties& inProps) override;
		void Close() override;
		bool ShouldClose() const override;
		void PollEvents() override;
		// IWindow Interface
	private:
		// TODO: Replace with custom allocator
		SDL_Window* Window = nullptr;

		bool CanClose = false;
	};
}
