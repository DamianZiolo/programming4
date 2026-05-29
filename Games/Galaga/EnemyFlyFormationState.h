#pragma once
#include "EnemyFlyState.h"

namespace dae
{
	class EnemyFlyFormationState final : public EnemyFlyState
	{
	public:
		EnemyFlyFormationState() = default;
		~EnemyFlyFormationState() override = default;

		void OnEnter(EnemyFly& enemy) override;
		std::unique_ptr<EnemyFlyState> Update(EnemyFly& enemy) override;

	private:
		float m_Timer{};
		float m_TimeUntilAttack{ 3.f };
	};
}