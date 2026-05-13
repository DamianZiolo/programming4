#include "ShotCommand.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "ServiceLocator.h"
#include <RenderComponent.h>
#include <ProjectileComponent.h>
#include "SceneManager.h"
#include "BoxCollider.h"

dae::ShotCommand::ShotCommand(GameObject* player)
	: m_pTarget(player)
{
}

void dae::ShotCommand::Execute()
{
	if (!m_pTarget) return;
	ServiceLocator::GetSoundSystem().Play(1, 1.f);\

		auto projectile = std::make_unique<dae::GameObject>();

	auto renderComponent = projectile->AddComponent<dae::RenderComponent>("Bullet.png");
	renderComponent->SetSize(10.f, 10.f);
	projectile->AddComponent<dae::BoxCollider>(glm::vec2{ 10.f,10.f });
	projectile->AddComponent<dae::ProjectileComponent>();

	projectile->SetLocalPosition(
		m_pTarget->GetWorldPosition().x,
		m_pTarget->GetWorldPosition().y,
		0.f
	);

	auto& activeScene = dae::SceneManager::GetInstance().GetActiveScene();
	activeScene.Add(std::move(projectile));
	

	/*auto health = m_pTarget->GetComponent<dae::HealthComponent>();

	if (health)
	{
		health->DealDamage();
		
	}*/
}