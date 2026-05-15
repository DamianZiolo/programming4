#include "ConfirmCommand.h"
#include "MenuComponent.h"

dae::ConfirmCommand::ConfirmCommand(MenuComponent* menu):
	m_pMenu{menu}
{
}

void dae::ConfirmCommand::Execute()
{
	if (!m_pMenu)
		return;

	m_pMenu->Confirm();
}
