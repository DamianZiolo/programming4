#pragma once
#include "EnemyButterflyState.h"

namespace dae
{
	class EnemyButterflyFormationState final : public EnemyButterflyState
	{
	public:
		void OnEnter(EnemyButterfly& enemy) override;
		std::unique_ptr<EnemyButterflyState> Update(EnemyButterfly& enemy) override;
	};
}