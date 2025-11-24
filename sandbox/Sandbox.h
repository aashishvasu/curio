#pragma once

#include "Curio.h"

DECLARE_LOGGER(LogSandbox, Trace);

class Sandbox : public CuCore::Application
{
public:
	explicit Sandbox(const CuCore::ApplicationSpec& Spec);
	void Initialize() override;
	void Update() override;
	void Shutdown() override;
};

inline CuCore::Application* CuCore::CreateApplication(const int argc, char** argv)
{
	ApplicationSpec spec = {"Sandbox", 640, 480, {argc, argv}};
	return NewObject<Sandbox>(spec);
}