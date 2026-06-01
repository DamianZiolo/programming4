#pragma once

#include "Command.h"
#include "BossAttackType.h"

namespace dae
{
	class LevelManagerComponent;

	class TriggerBossAttackCommand final : public Command
	{
	public:
		TriggerBossAttackCommand(
			LevelManagerComponent* levelManager,
			BossAttackType attackType);

		void Execute() override;

	private:
		LevelManagerComponent* m_pLevelManager{};
		BossAttackType m_AttackType{};
	};
}