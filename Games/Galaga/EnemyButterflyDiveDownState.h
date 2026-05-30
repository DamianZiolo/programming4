#pragma once
#include "EnemyButterflyState.h"

namespace dae
{
	class EnemyButterflyDiveDownState final : public EnemyButterflyState
	{
	public:
		void OnEnter(EnemyButterfly& enemy) override;
		std::unique_ptr<EnemyButterflyState> Update(EnemyButterfly& enemy) override;

	private:
		float m_DownSpeed{ 180.f };
		float m_HorizontalSpeed{};
	};
}