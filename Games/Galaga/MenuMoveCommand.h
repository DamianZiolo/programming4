#pragma once
#include "Command.h"


namespace dae {

	class GameObject;
	class MenuComponent;

	class MenuMoveCommand : public Command
	{
	public:
		explicit MenuMoveCommand(MenuComponent* menu, int direction);
		void Execute() override;
	private:
		dae::MenuComponent* m_pMenu{};
		int m_Direction{};
	};

}