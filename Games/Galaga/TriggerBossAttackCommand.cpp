#include "TriggerBossAttackCommand.h"

#include "LevelManagerComponent.h"

dae::TriggerBossAttackCommand::TriggerBossAttackCommand(
	LevelManagerComponent* levelManager,
	BossAttackType attackType)
	: m_pLevelManager{ levelManager }
	, m_AttackType{ attackType }
{
}

void dae::TriggerBossAttackCommand::Execute()
{
	if (!m_pLevelManager)
		return;

	m_pLevelManager->SendRandomBossAttack(m_AttackType);
}