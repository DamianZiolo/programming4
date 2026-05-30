#pragma once
#include "EnemyButterflyState.h"

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
		float m_LoopDuration{ 1.0f };
		static constexpr float loopStartLine = ScreenHeight - 20.f;
		float m_DownSpeed{ 160.f };
		float m_HorizontalSpeed{};

		bool m_IsLooping{ false };
		static constexpr float radiusX = 45.f;
		static constexpr float radiusY = 35.f;
		static constexpr float fullCircle = 6.2831853f;
	};
}