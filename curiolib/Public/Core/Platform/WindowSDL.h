#pragma once

#include "IWindow.h"

struct SDL_Window;
namespace CuCore
{
	class CURIO_API WindowSDL final : public IWindow
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
		SDL_Window* Window = nullptr;

		bool CanClose = false;
	};
}
