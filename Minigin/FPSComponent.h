#pragma once
#include "Component.h"
#include <string>

namespace dae
{
    class TextComponent;

    class FPSComponent final : public Component
    {
    public:
        explicit FPSComponent(GameObject* owner);

        void Update() override;

        float GetFPS() const { return m_fps; }

    private:
        void UpdateText();

        TextComponent* m_text{};
        float m_fps{ 0.0f };

        
        int m_frames{ 0 };
        float m_accumulatedTime{ 0.0f };
        //To avoid updating texture every frame, we will update it every 0.25seconds its fast anyway
        float m_updateInterval{ 0.25f };
    };
}