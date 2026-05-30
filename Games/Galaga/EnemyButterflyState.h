#pragma once
#include <memory>

namespace dae
{
	class EnemyButterfly;

	class EnemyButterflyState
	{
	public:
		virtual ~EnemyButterflyState() = default;

		virtual void OnEnter(EnemyButterfly&) {}
		virtual void OnExit(EnemyButterfly&) {}

		virtual std::unique_ptr<EnemyButterflyState> Update(EnemyButterfly& enemy) = 0;

	protected:
		static constexpr float ScreenHeight{ 576.f };
	};
}