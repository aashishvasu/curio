#pragma once

#include "Curio.h"

DECLARE_LOGGER(LogSandbox, Trace);

class Sandbox : public Core::Application
{
public:
	explicit Sandbox(const Core::ApplicationSpec& Spec);
	void Initialize() override;
	void Update() override;
	void Shutdown() override;
};

inline Core::Application* Core::CreateApplication(const int argc, char** argv)
{
	const ApplicationSpec spec = {"Sandbox", 640, 480, {argc, argv}};
	return NewObject<Sandbox>(spec);
}