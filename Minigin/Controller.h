#pragra once
#include <Xinput.h>

namespace dae {
	class Controller final
	{
	public:
		explicit Controller(int controllerIndex);
		void Update();
		bool IsPressed(int button) const;
		bool IsDownThisFrame(int button) const;
		bool IsUpThisFrame(int button) const;

	private:
		int m_ControllerIndex;
		XINPUT_STATE m_PreviousState{};
		XINPUT_STATE m_CurrentState{};

		int m_ButtonsPressedThisFrame{};
		int m_ButtonsReleasedThisFrame{};
		bool m_IsConnected{ false };

	};

}