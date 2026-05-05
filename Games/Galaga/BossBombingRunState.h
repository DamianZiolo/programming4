#pragma once
#include "BossState.h"

namespace dae
{
	class BossBombingRunState final : public BossState
	{
		void OnEnter(EnemyBoss& boss) override;
		std::unique_ptr<dae::BossState> Update(EnemyBoss& boss) override;

	private:
		float m_Timer{};
	};
}