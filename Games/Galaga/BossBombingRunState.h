#pragma once
#include "BossState.h"
#include <memory>
#include <glm/vec3.hpp>

namespace dae
{
	class BossBombingRunState final : public BossState
	{
		void OnEnter(EnemyBoss& boss) override;
		std::unique_ptr<dae::BossState> Update(EnemyBoss& boss) override;

	private:
		float m_Timer{};
		glm::vec3 m_StartPos{};
		bool m_Returning{ false };
	};
}