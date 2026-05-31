#pragma once
#include "Command.h"


namespace dae {

	class GameObject;
	class MenuComponent;

	enum class MenuMoveDirection
	{
		Up,
		Down,
		Left,
		Right
	};

	class MenuMoveCommand : public Command
	{
	public:
		explicit MenuMoveCommand(MenuComponent* menu, MenuMoveDirection direction);
		void Execute() override;
	private:
		dae::MenuComponent* m_pMenu{};
		MenuMoveDirection m_Direction{};
	};

}