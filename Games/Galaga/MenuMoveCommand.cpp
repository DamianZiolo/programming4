#include "MenuMoveCommand.h"
#include "MenuComponent.h"

dae::MenuMoveCommand::MenuMoveCommand(MenuComponent* menu, MenuMoveDirection direction)
	: m_pMenu{ menu }
	, m_Direction{ direction }
{
}

void dae::MenuMoveCommand::Execute()
{
	if (!m_pMenu)
		return;

	switch (m_Direction)
	{
	case MenuMoveDirection::Up:
		m_pMenu->MoveUp();
		break;

	case MenuMoveDirection::Down:
		m_pMenu->MoveDown();
		break;

	case MenuMoveDirection::Left:
		m_pMenu->MoveLeft();
		break;

	case MenuMoveDirection::Right:
		m_pMenu->MoveRight();
		break;
	}
}