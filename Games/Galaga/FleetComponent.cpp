#include "FleetComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include <windows.h>
#include <sstream>

dae::FleetComponent::FleetComponent(GameObject* owner,int rows,int cols,float spacingX,float spacingY)
	: Component(owner)
	, m_Rows{ rows }
	, m_Cols{ cols }
	, m_SpacingX{ spacingX }
	, m_SpacingY{ spacingY }
	, m_StartPos{ GetOwner()->GetLocalPosition() }
	, m_Time{0.f}
{
	m_Slots.resize(rows * cols);
}

void dae::FleetComponent::Update()
{
	m_Time += dae::GameTime::GetIntance().GetDeltaTime();
	float offsetX = sin(m_Time) * 20.f +40.f;

	GetOwner()->SetLocalPosition(
		glm::vec3(offsetX, 0, 0.f)
	);
}

void dae::FleetComponent::SetSlot(int row, int col, GameObject* slot)
{
	m_Slots[row * m_Cols + col].object = slot;
	m_Slots[row * m_Cols + col].occupied = false;
}

dae::GameObject* dae::FleetComponent::GetSlot(int row, int col)
{
	return m_Slots[row * m_Cols + col].object;
}

glm::vec3 dae::FleetComponent::GetSlotWorldPosition(int row, int col)
{
	return m_Slots[row * m_Cols + col].object->GetWorldPosition();
}