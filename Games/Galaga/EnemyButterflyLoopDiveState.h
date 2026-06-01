#pragma once
#include "EnemyButterflyState.h"
#include "GameSettings.h"
#include <glm/glm.hpp>

namespace dae
{
	class EnemyButterflyLoopDiveState final : public EnemyButterflyState
	{
	public:
		void OnEnter(EnemyButterfly& enemy) override;
		std::unique_ptr<EnemyButterflyState> Update(EnemyButterfly& enemy) override;

	private:
		glm::vec3 m_LoopCenter{};

		float m_Timer{};
		float m_LoopDuration{ 2.0f };

		static constexpr float loopStartLine = GameSettings::ScreenHeight - 100.f;

		float m_DownSpeed{ 120.f };
		float m_HorizontalSpeed{};

		bool m_IsLooping{ false };
		bool m_HasFinishedLoop{ false };

		static constexpr float radiusX = 150.f;
		static constexpr float radiusY = 150.f;
		static constexpr float fullCircle = 6.2831853f;
	};
}