#pragma once
#include <memory>

namespace dae 
{
	class EnemyBoss;

	class BossState 
	{
	public:
		virtual ~BossState() = default;

		virtual void OnEnter(EnemyBoss& boss) { (void)boss; };
		virtual void OnExit(EnemyBoss& boss) { (void)boss; };

		virtual std::unique_ptr<BossState> Update(EnemyBoss& boss) = 0;
		virtual std::unique_ptr<BossState> OnHit(EnemyBoss& boss) { (void)boss; return nullptr; }

	};

}