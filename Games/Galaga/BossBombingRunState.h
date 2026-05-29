#pragma once
#include "BossState.h"
#include <memory>
#include <glm/vec3.hpp>

namespace dae
{
	class ProjectilePoolComponent;
	class BossBombingRunState final : public BossState
	{
		void OnEnter(EnemyBoss& boss) override;
		std::unique_ptr<dae::BossState> Update(EnemyBoss& boss) override;

	private:
		float m_Timer{};
		const float m_BombingTime{ 4.f };
		glm::vec3 m_StartPos{};
		bool m_Returning{ false };

		float m_ProjectileTimer{ 0.f };
		float m_ProjectileSpeedRate{ 0.5f };

	};
}