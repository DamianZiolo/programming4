#pragma once
#include "Component.h"
#include <string>
#include <memory>
namespace dae
{
    class Transform;
    class Texture2D;
    class RenderComponent final : public dae::Component
    {
    public:
        explicit RenderComponent(GameObject* owner, const std::string& filename);
        void Update() override {};
        void UpdatePosition();
        void Render() const override;
        void SetTexture(const std::string& filename);
		void SetSize(float width, float height) { m_Width = width; m_Height = height; }

    private:
        std::shared_ptr<Texture2D> m_Texture{};
		Transform* m_Transform{ nullptr };
		float m_Width{ 0.f };
		float m_Height{ 0.f };

    };
}