#pragma once

#include "Curio.h"

DECLARE_LOGGER(LogSandbox, Trace);

class Sandbox : public CuCore::Application
{
public:
	void Initialize() override;
	void Update() override;
	void Shutdown() override;
};

inline CuCore::Application* CuCore::CreateApplication()
{
	// TODO: Use custom allocator when ready
	return new Sandbox();
}