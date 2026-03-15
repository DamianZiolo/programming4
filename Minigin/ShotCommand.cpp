#include "ShotCommand.h"
#include "GameObject.h"
#include "HealthComponent.h"

dae::ShotCommand::ShotCommand(GameObject* player)
	: m_pTarget(player)
{
}

void dae::ShotCommand::Execute()
{
	if (!m_pTarget) return;

	auto health = m_pTarget->GetComponent<dae::HealthComponent>();

	if (health)
	{
		health->DealDamage();
	}
}