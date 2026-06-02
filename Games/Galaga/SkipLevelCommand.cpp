#include "SkipLevelCommand.h"

#include "LevelManagerComponent.h"

dae::SkipLevelCommand::SkipLevelCommand(
	LevelManagerComponent* levelManager)
	: m_pLevelManager{ levelManager }
{
}

void dae::SkipLevelCommand::Execute()
{
	if (!m_pLevelManager)
		return;

	m_pLevelManager->SkipLevel();
}