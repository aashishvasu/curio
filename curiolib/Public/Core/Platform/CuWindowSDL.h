#pragma once
#include "CuWindow.h"

struct SDL_Window;
namespace CuCore
{
	class CuWindowSDL final : public CuWindow
	{
	public:
		CuWindowSDL() = default;
		~CuWindowSDL() override;

		// CuWindow Interface
		void Create(const char* Title, int Width, int Height) override;
		void Close() override;
		bool ShouldClose() const override;
		void PollEvents() override;
		// CuWindow Interface
	private:
		// TODO: Replace with custom allocator
		SDL_Window* Window = nullptr;

		bool CanClose = false;
	};
}
