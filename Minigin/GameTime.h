#pragma once
#include <cstdint>

namespace dae
{
	class GameTime final
	{
	public:
		static GameTime& GetIntance();

		//Called once at start of the game
		void Reset();

		//Called once every frame from Minigin RunOneFrame
		void Tick();

		float GetDeltaTime() const;
		float GetTotalTime() const;

	private:
		//It's not obligatory to make default and delete but it's more readable and it makes it clear that we don't want to copy or move this class, because it's a singleton, I only want one instance of it
		GameTime() = default;
		~GameTime() = default;
		GameTime(const GameTime&) = delete;
		GameTime(GameTime&&) = delete;
		GameTime& operator=(const GameTime&) = delete;
		GameTime& operator=(GameTime&&) = delete;

		float m_deltaTime{0.f}; //Time of previous frame in seconds
		float m_totalTime{ 0.f }; // Total time since the start of the game in seconds

		//uint64_t because SDLK_GetPerformanceCounter returns a 64 bit integer, and I want to store it in the same type to avoid any issues with overflow or loss of precision
		std::uint64_t m_prevCounter{ 0 }; // Time of previous frame in ticks
		//double because the frequency can be a very small number, and I want to store it in a type that can handle small numbers without losing precision, and also because I will be dividing by the frequency, so I want to avoid any issues with integer division
		//It's inverted Frequency because I will be multiplying by it, and it's more efficient to multiply than to divide
		//so insted of doing dt = (now - prev) / frequency every frame I can do dt = (now - prev) * m_invFrequency every frame, and it's the same result but more efficient
		double m_invFrequency{ 0.0 };

	};

}