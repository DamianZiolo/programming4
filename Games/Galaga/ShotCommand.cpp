#include "ShotCommand.h"

#include "GameObject.h"
#include "PlayerShootingComponent.h"

dae::ShotCommand::ShotCommand(GameObject* player)
	: m_pTarget{ player }
{
}

void dae::ShotCommand::Execute()
{
	if (!m_pTarget)
		return;

	if (auto* shooting = m_pTarget->GetComponent<PlayerShootingComponent>())
	{
		shooting->Shoot();
	}
}