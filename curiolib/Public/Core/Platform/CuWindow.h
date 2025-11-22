#pragma once
#include "Core/Engine.h"

namespace CuCore
{
	class CuWindow
	{
	public:
		virtual ~CuWindow() = default;
		CuWindow(const CuWindow&) = delete;
		CuWindow& operator=(const CuWindow&) = delete;

		// Interface
		// TODO: Replace const char with custom string when ready?
		virtual void Create(const char* Title, int Width, int Height) = 0;
		virtual void Close() = 0;
		virtual bool ShouldClose() const = 0;
		virtual void PollEvents() = 0;
		
	protected:
		CuWindow() = default;
	};
}
