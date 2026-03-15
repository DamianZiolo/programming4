#pragma once
#include "Component.h"
#include <string>

namespace dae
{
    class HealthComponent final : public Component
    {
    public:
        explicit HealthComponent(GameObject* owner, int startHealth = 3);
        void Update() override;
		void DealDamage();
        int GetHealth() const;
		void ResetDamage() { m_CurrentHealth = m_MaxHealth; }

    private:
        int m_MaxHealth{};
		int m_CurrentHealth{};
    };
}