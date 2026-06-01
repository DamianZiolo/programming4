#include "ShotCommand.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "ServiceLocator.h"
#include <RenderComponent.h>
#include <ProjectileComponent.h>
#include "SceneManager.h"
#include "BoxCollider.h"
#include "ProjectilePoolComponent.h"

dae::ShotCommand::ShotCommand(GameObject* player, ProjectilePoolComponent* projectilePool)
	: m_pTarget(player),
	m_pProjectilePool{ projectilePool }
{
}

void dae::ShotCommand::Execute()
{
    if (!m_pTarget)
        return;

    if (!m_pProjectilePool)
        return;
    auto* health = m_pTarget->GetComponent<HealthComponent>();
    if (health->GetHealth() <= 0)
        return;

    ServiceLocator::GetSoundSystem().Play(1, 1.f);

    const auto targetPos = m_pTarget->GetWorldPosition();

    m_pProjectilePool->SpawnProjectile(
        glm::vec3{
            targetPos.x,
            targetPos.y,
            0.f
        },
        -100.f,
        dae::ProjectileOwner::Player
    );
}