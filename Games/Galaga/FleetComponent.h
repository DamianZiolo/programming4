#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <vector>

namespace dae
{
	class EnemyFly;
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

		void SetSlot(int row, int col, GameObject* slot);

		GameObject* GetSlot(int row, int col);

		glm::vec3 GetSlotWorldPosition(int row, int col);

		float GetSpacingX() const { return m_SpacingX; }
		float GetSpacingY() const { return m_SpacingY; }

	private:
		struct Slot
		{
			GameObject* object{};
			bool occupied{};
		};

		int m_Rows{};
		int m_Cols{};

		float m_SpacingX{};
		float m_SpacingY{};

		std::vector<Slot> m_Slots;

		glm::vec3 m_StartPos{};
		float m_Time{};

		float m_AttackTimer{};
		float m_AttackInterval{ 3.f };
		int m_MaxAttackers{ 2 };
	};
}