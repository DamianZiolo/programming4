#pragma once
#include "EnemyButterflyState.h"

namespace dae
{
	class EnemyButterflyReturnState final : public EnemyButterflyState
	{
	public:
		void OnEnter(EnemyButterfly& enemy) override;
		std::unique_ptr<EnemyButterflyState> Update(EnemyButterfly& enemy) override;

	private:
		float m_ReturnSpeed{ 250.f };
	};
}