#pragma once
#include <memory>

namespace dae
{
	class EnemyFly;

	class EnemyFlyState
	{
	public:
		virtual ~EnemyFlyState() = default;

		virtual void OnEnter(EnemyFly&) {}
		virtual void OnExit(EnemyFly&) {}

		virtual std::unique_ptr<EnemyFlyState> Update(EnemyFly& enemy) = 0;

	protected:
		static constexpr float ScreenWidth{ 1024.f };
		static constexpr float ScreenHeight{ 576.f };
	};
}