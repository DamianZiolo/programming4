#pragma once
#include <memory>
#include "ControllerButton.h"
namespace dae
{
	class Controller final
	{
	public:
		explicit Controller(int controllerIndex);
		~Controller();

		void Update();
		bool IsPressed(ControllerButton button) const;
		bool IsDownThisFrame(ControllerButton button) const;
		bool IsUpThisFrame(ControllerButton button) const;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}