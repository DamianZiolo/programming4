#include "FleetComponent.h"
#include <memory>
#include "GameObject.h"

dae::FleetComponent::FleetComponent(GameObject* owner, int rows, int cols, float spacingX, float spacingY)
    : Component(owner)
    , m_Rows(rows)
    , m_Cols(cols)
{
    m_Slots.resize(rows * cols);

    for (int i = 0; i < m_Rows; ++i)
    {
        for (int j = 0; j < m_Cols; ++j)
        {
            //yes, I use raw pointer because other way slots would be deleted when they are out of scope, now I will delete them whem fleet is deleted
            auto* slot = new GameObject();

            slot->SetParent(owner, false);
            slot->SetLocalPosition(
                j * spacingX,
                i * spacingY,
                0.f);

            int index = i * m_Cols + j;

            m_Slots[index].object = slot;
            m_Slots[index].occupied = false;
        }
    }
}

void dae::FleetComponent::Update()
{
}

dae::GameObject* dae::FleetComponent::GetSlot(int row, int col)
{
    return m_Slots[row * m_Cols + col].object;
}

glm::vec3 dae::FleetComponent::GetSlotWorldPosition(int row, int col)
{
    return m_Slots[row * m_Cols + col].object->GetWorldPosition();
}
