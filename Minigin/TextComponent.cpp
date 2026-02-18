#include "TextComponent.h"

#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>

#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"
namespace dae
{
    TextComponent::TextComponent(GameObject* owner, std::shared_ptr<Font> font,const std::string& text, const SDL_Color& color)
        : Component(owner)
        , m_needsUpdate(true)
        , m_text(text)
        , m_color(color)
        , m_font(std::move(font))
        , m_textTexture(nullptr)
    {
        if (!m_font)
            throw std::invalid_argument("TextComponent requires a valid Font");
    }

    void TextComponent::Update()
    {
        if (!IsActive()) return;

        if (m_needsUpdate)
        {
            RebuildTexture();
            m_needsUpdate = false;
        }
    }

    void TextComponent::Render() const
    {
        if (!IsActive()) return;

        if (m_textTexture)
        {
            const auto& pos = GetOwner()->GetTransform().GetPosition();
            Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
        }
    }

    void TextComponent::SetText(const std::string& text)
    {
        if (m_text == text) return;
        m_text = text;
        m_needsUpdate = true;
    }

    void TextComponent::SetColor(const SDL_Color& color)
    {
        m_color = color;
        m_needsUpdate = true;
    }

    void TextComponent::RebuildTexture()
    {
        const auto surf = TTF_RenderText_Blended(m_font->GetFont(),
            m_text.c_str(),
            static_cast<int>(m_text.length()),
            m_color);

        if (!surf)
            throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

        auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
        SDL_DestroySurface(surf);

        if (!texture)
            throw std::runtime_error(std::string("Create text texture failed: ") + SDL_GetError());

        m_textTexture = std::make_shared<Texture2D>(texture);
    }
}