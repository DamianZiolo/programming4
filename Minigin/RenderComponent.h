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

    private:
        std::shared_ptr<Texture2D> m_Texture{};
		Transform* m_Transform{ nullptr };

    };
}