#include "GameTime.h"
#include <SDL3/SDL.h>
#include <algorithm>

dae::GameTime& dae::GameTime::GetIntance()
{
	static Time instance;
	return instance;
}

void dae::GameTime::Reset()
{
	const std::uint64_t frequency = SDL_GetPerformanceFrequency(); //how much ticks per second
	m_invFrequency = 1.0 / static_cast<double>(frequency);

	m_prevCounter = SDL_GetPerformanceCounter();
	m_deltaTime = 0.f;
	m_totalTime = 0.f;
}

void dae::GameTime::Tick()
{
	const std::uint64_t now = SDL_GetPerformanceCounter();

	// Compute in double for better precision when converting large timer values,
	// then cast to float since gameplay systems usually operate on floats.
	const double dt = (now - m_prevCounter) * m_invFrequency; //time in seconds since last frame)
	m_prevCounter = now;

	m_deltaTime = static_cast<float>(dt);
	m_totalTime += m_deltaTime;

}

float dae::GameTime::GetDeltaTime() const
{
	return m_deltaTime;
}

float dae::GameTime::GetTotalTime() const
{
	return m_totalTime;
}