#include "ToggleMuteCommand.h"

#include "ServiceLocator.h"

void dae::ToggleMuteCommand::Execute()
{
	ServiceLocator::GetSoundSystem().ToggleMute();
}