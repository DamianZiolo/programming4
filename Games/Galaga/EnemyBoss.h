#pragma once
#include "Enemy.h"
#include <memory>
#include "BossState.h"
#include <EnemyBoss.h>

namespace dae
{
	class BoxCollider;
	class BossState;
	class RenderComponent;
	class Scene;

	class EnemyBoss final : public Enemy
	{
	public:
		explicit EnemyBoss(GameObject* owner, ProjectilePoolComponent& projectilePool);
		~EnemyBoss() override = default;
		void TakeDamage() override;

		void Update() override;
		EnemyBoss(const EnemyBoss&) = delete;
		EnemyBoss(EnemyBoss&&) = delete;
		EnemyBoss& operator=(const EnemyBoss&) = delete;
		EnemyBoss& operator=(EnemyBoss&&) = delete;

		int GetHealth() { return m_Health; }
		void ChangeState(std::unique_ptr<BossState> newState);

		void CreateTractorBeam(Scene& scene);
		void EnableTractorBeam(bool enabled);
		bool IsTractorBeamEnabled() const { return m_BeamEnabled; }

	private:
		std::unique_ptr<BossState> m_State;
		bool m_IsDamaged{ false };

		GameObject* m_pTractorBeam{};
		bool m_BeamEnabled{ false };
	};

}