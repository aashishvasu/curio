#pragma once

namespace Platform
{
	struct WindowProperties
	{
		// TODO: Replace with custom string when ready
		const char* Title;
		int Width;
		int Height;
	};
	
	class CURIO_API IWindow
	{
	public:
		virtual ~IWindow() = default;
		IWindow(const IWindow&) = delete;
		IWindow& operator=(const IWindow&) = delete;

		// Interface
		// TODO: Replace const char with custom string when ready?
		virtual void Create(const WindowProperties& inProps) = 0;
		virtual void Close() = 0;
		virtual bool ShouldClose() const = 0;
		virtual void PollEvents() = 0;

	protected:
		IWindow() = default;
	};
}
