#include "AchievementSystemComponent.h"
#include "GameObject.h"
#include "GameActor.h"
#include "ScoreComponent.h"
#include "SteamManager.h"

dae::AchievementSystemComponent::AchievementSystemComponent(GameObject* owner) :Component(owner)
{
}

void dae::AchievementSystemComponent::Notify(Event event)
{
	if (event == Event::ScoreReached500 && !m_ScoreReached500Unclocked)
	{
		m_ScoreReached500Unclocked = true;
		dae::SteamManager::UnlockAchievement("ACH_WIN_ONE_GAME");
	}

}

void dae::AchievementSystemComponent::AddActor(GameActor* actor)
{
	if (!actor) return;

	actor->AddObserver(this);
}


