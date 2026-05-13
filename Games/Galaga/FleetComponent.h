#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <vector>


namespace dae
{

	class FleetComponent final : public Component
	{
	public:
		FleetComponent(
			GameObject* owner,
			int rows,
			int cols,
			float spacingX,
			float spacingY);
		void Update() override;
		GameObject* GetSlot(int row, int col);
		
		glm::vec3 GetSlotWorldPosition(int row, int col);

	private:
		struct Slot 
		{
			GameObject* object{};
			bool occupied{};
		};

		int m_Rows{};
		int m_Cols{};

		std::vector<Slot> m_Slots;

	};

}