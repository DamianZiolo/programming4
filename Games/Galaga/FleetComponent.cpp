#include "FleetComponent.h"
#include <memory>
#include "GameObject.h"

dae::FleetComponent::FleetComponent(GameObject* owner, int rows, int cols, float spacingX, float spacingY):
Component(owner),
m_Rows{rows},
m_Cols{cols}
{
	for (int i = 0; i < m_Rows; ++i)
	{
		for (int j = 0; j < m_Cols; ++j)
		{
			auto slot = std::make_unique<GameObject>();
			slot->SetParent(owner, false);
			slot->SetLocalPosition(
				j * spacingX,
				i * spacingY,
				0.f);

			m_Slots.push_back({
		   slot.get(),
		   false
			});

		}
	}

}

void dae::FleetComponent::Update()
{
}
