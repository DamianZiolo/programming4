#include "BossBombingRunState.h"
#include <EnemyBoss.h>
#include <BossFormationState.h>

void dae::BossBombingRunState::OnEnter(EnemyBoss& boss)
{
	(void)boss;
}

std::unique_ptr<dae::BossState> dae::BossBombingRunState::Update(dae::EnemyBoss& boss)
{
	(void)boss;
	return nullptr;
}
