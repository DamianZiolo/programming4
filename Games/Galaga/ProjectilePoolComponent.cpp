#include "ProjectilePoolComponent.h"
#include "Scene.h"
#include <RenderComponent.h>
#include "BoxCollider.h"

dae::ProjectilePoolComponent::ProjectilePoolComponent(GameObject* owner, int poolSize, Scene& scene):Component(owner)
{
    for (int i = 0; i < poolSize; ++i)
    {
        auto projectileGO = std::make_unique<GameObject>();

        auto renderComponent = projectileGO->AddComponent<RenderComponent>("Bullet.png");
        renderComponent->SetSize(8, 16);
        projectileGO->AddComponent<BoxCollider>(glm::vec2{ 8, 16 });

        auto* projectile = projectileGO->AddComponent<ProjectileComponent>();
        projectile->Deactivate();

        m_Projectiles.push_back(projectile);

        scene.Add(std::move(projectileGO));
    }

}

dae::ProjectileComponent* dae::ProjectilePoolComponent::SpawnProjectile(const glm::vec3& position, float velocity, ProjectileOwner ownerType)
{
    for (auto* projectile : m_Projectiles)
    {
        if (!projectile->IsInUse())
        {
            projectile->Activate(position, velocity, ownerType);
            return projectile;
        }
    }

    return nullptr;
}
