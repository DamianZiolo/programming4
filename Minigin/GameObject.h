#pragma once

#include <vector>
#include <memory>
#include <type_traits>
#include <algorithm>   // std::find, std::remove
#include <utility>     // std::forward
#include <glm/glm.hpp>

#include "Transform.h"
#include "Component.h"

namespace dae
{
    class GameObject final
    {
    public:
        GameObject() = default;
        ~GameObject();

        GameObject(const GameObject&) = delete;
        GameObject(GameObject&&) = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject& operator=(GameObject&&) = delete;

        // ----- Main loop -----
        void Update();
        void Render() const;

        // ----- Removal -----
        bool IsMarkedForRemoval() const { return m_markedForRemoval; }
        void MarkForRemoval() { m_markedForRemoval = true; }

        // ----- Transform API (Local/World) -----
        // Local (relative to parent)
        const glm::vec3& GetLocalPosition() const { return m_transform.GetLocalPosition(); }
        void SetLocalPosition(const glm::vec3& local);

        // World
        glm::vec3 GetWorldPosition();//TODO - update world position should be here because we need it!
        void SetWorldPosition(const glm::vec3& world);
        void UpdateWorldTransform();

        // Convenience overloads
        void SetLocalPosition(float x, float y, float z = 0.0f) { SetLocalPosition(glm::vec3{ x, y, z }); }
        void SetWorldPosition(float x, float y, float z = 0.0f) { SetWorldPosition(glm::vec3{ x, y, z }); }

        // Dirty flag
        void SetDirty();

        // ----- Scene graph -----
        GameObject* GetParent() const { return m_parent; }
        const std::vector<GameObject*>& GetChildren() const { return m_children; }
        Transform& GetTransform() { return m_transform; }
        const Transform& GetTransform() const { return m_transform; }

		// keepWorld = true -> object don't ,,jump" to new parent, but keeps its world position (local position will be recalculated)
        void SetParent(GameObject* newParent, bool keepWorld = true);

        // Utility
        bool IsDescendantOf(const GameObject* potentialAncestor) const;

        // ----- Component API -----
        template<typename T, typename... Args>
        T* AddComponent(Args&&... args)
        {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            // no duplicates of same type
            if (GetComponent<T>() != nullptr)
                return nullptr;

            auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
            T* raw = component.get();
            m_components.emplace_back(std::move(component));
            return raw;
        }

        template<typename T>
        void RemoveComponent(T* component)
        {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
            if (!component) return;
            if (component->GetOwner() != this) return;
            component->m_markedForRemoval = true;
        }

        template<typename T>
        T* GetComponent()
        {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            for (auto& c : m_components)
            {
                if (auto casted = dynamic_cast<T*>(c.get()))
                    return casted;
            }
            return nullptr;
        }

        template<typename T>
        const T* GetComponent() const
        {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            for (const auto& c : m_components)
            {
                if (auto casted = dynamic_cast<const T*>(c.get()))
                    return casted;
            }
            return nullptr;
        }

        template<typename T>
        bool HasComponent() const
        {
            return GetComponent<T>() != nullptr;
        }

        void CleanupRemovedComponents();

    private:
        // Scene graph
        void AddChild(GameObject* child);
        void RemoveChild(GameObject* child);


        //Members
        Transform m_transform{};
        std::vector<std::unique_ptr<Component>> m_components{};
        bool m_markedForRemoval{ false };
        GameObject* m_parent{ nullptr };
        std::vector<GameObject*> m_children{};
    };
}