#include "HealthDisplayComponent.h"

#include "GameActor.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "TextComponent.h"
#include "Event.h"
#include "RenderComponent.h"
#include "Scene.h"

#include <algorithm>

dae::HealthDisplayComponent::HealthDisplayComponent(
	GameObject* owner,
	GameActor* actor,
	Scene& scene,
	int maxLives)
	: Component(owner)
	, m_pActor{ actor }
{
	m_pActor->AddObserver(this);

	m_pHealthComponent =
		m_pActor->GetOwner()->GetComponent<HealthComponent>();

	constexpr float spacing = 35.f;

	for (int i = 0; i < maxLives - 1; ++i)
	{
		auto icon = std::make_unique<GameObject>();

		auto* iconRaw = icon.get();

		scene.Add(std::move(icon));

		iconRaw->SetParent(GetOwner(), false);
		iconRaw->SetLocalPosition(i * spacing, 0.f, 0.f);

		auto* render = iconRaw->AddComponent<RenderComponent>("Player.png");
		render->SetSize(30.f, 30.f);

		m_LifeIcons.push_back(iconRaw);
	}

	RefreshLives();
}

void dae::HealthDisplayComponent::Notify(Event event)
{
	if (event == Event::PlayerDamaged || event == Event::PlayerDied)
	{
		RefreshLives();
	}
}

void dae::HealthDisplayComponent::RefreshLives()
{
	if (!m_pHealthComponent)
		return;

	const int health = m_pHealthComponent->GetHealth();

	const int visibleIcons = std::max(0, health - 1);

	for (int i = 0; i < static_cast<int>(m_LifeIcons.size()); ++i)
	{
		auto* icon = m_LifeIcons[i];

		if (!icon)
			continue;

		auto* render = icon->GetComponent<RenderComponent>();

		if (!render)
			continue;

		render->SetActive(i < visibleIcons);
	}
}