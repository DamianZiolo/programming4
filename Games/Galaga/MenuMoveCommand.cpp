#include "MenuMoveCommand.h"
#include "MenuComponent.h"

dae::MenuMoveCommand::MenuMoveCommand(MenuComponent* menu, int direction):
m_pMenu{menu},
m_Direction{direction}
{

}

void dae::MenuMoveCommand::Execute()
{
	if (!m_pMenu) return;

	if (m_Direction < 0)
	{
		m_pMenu->MoveUp();
	}
	else 
	{
		m_pMenu->MoveDown();
	}
}
