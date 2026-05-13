#include "FleetComponent.h"
#include "GameObject.h"

dae::FleetComponent::FleetComponent(GameObject* owner,int rows,int cols,float spacingX,float spacingY)
	: Component(owner)
	, m_Rows(rows)
	, m_Cols(cols)
	, m_SpacingX(spacingX)
	, m_SpacingY(spacingY)
{
	m_Slots.resize(rows * cols);
}

void dae::FleetComponent::Update()
{
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