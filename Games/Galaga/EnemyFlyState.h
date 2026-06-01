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

	};
}