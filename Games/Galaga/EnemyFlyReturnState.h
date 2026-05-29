#pragma once
#include "EnemyFlyState.h"

namespace dae
{
	class EnemyFlyReturnState final : public EnemyFlyState
	{
	public:
		EnemyFlyReturnState() = default;
		~EnemyFlyReturnState() override = default;

		void OnEnter(EnemyFly& enemy) override;
		std::unique_ptr<EnemyFlyState> Update(EnemyFly& enemy) override;

	private:
		float m_ReturnSpeed{ 250.f };
	};
}