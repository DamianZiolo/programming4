#include "FPSComponent.h"

#include "GameObject.h"
#include "TextComponent.h"
#include "GameTime.h" 
#include <cmath>
#include <sstream>
#include <iomanip>

namespace dae
{
    FPSComponent::FPSComponent(GameObject* owner)
        : Component(owner)
    {
        
    }

    void FPSComponent::Update()
    {
        if (!IsActive())
            return;

        const float dt = GameTime::GetIntance().GetDeltaTime();
        if (dt <= 0.0f)
        {
            return;
        }
           

        m_frames++;
        m_accumulatedTime += dt;

        if (m_accumulatedTime >= m_updateInterval)
        {
            m_fps = static_cast<float>(m_frames) / m_accumulatedTime;

            m_frames = 0;
            m_accumulatedTime = 0.0f;

            UpdateText();
        }
    }

    void FPSComponent::UpdateText()
    {
        if (!m_text)
        {
            m_text = GetOwner()->GetComponent<TextComponent>();
            if (!m_text)
                return; // GO doesn't have a TextComponent, so we don't update text
        }
		//ostringstream is better than string because it allow us to format the string in a more efficient way, and it's more readable than using string concatenation, and it also allow us to use manipulators like fixed and setprecision to format the number of decimal places for the FPS value
        std::ostringstream ss;
        ss << "FPS: " << std::fixed << std::setprecision(1) << m_fps;
        m_text->SetText(ss.str());
    }
}