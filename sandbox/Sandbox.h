#pragma once

#include "Curio.h"

DECLARE_LOGGER(LogSandbox, Trace);

class Sandbox : public CuCore::Application
{
public:
	Sandbox(const CuCore::ApplicationSpec& Spec);
	void Initialize() override;
	void Update() override;
	void Shutdown() override;
};

inline CuCore::Application* CuCore::CreateApplication(int argc, char** argv)
{
	// TODO: Use custom allocator when ready
	ApplicationSpec spec = {"Sandbox", 640, 480, {argc, argv}};
	return new Sandbox(spec);
}