#pragma once
#include "Command.h"


namespace dae {

	class GameObject;
	class MenuComponent;

	class ConfirmCommand : public Command
	{
	public:
		explicit ConfirmCommand(MenuComponent* menu);
		void Execute() override;
	private:
		dae::MenuComponent* m_pMenu{};
	};

}