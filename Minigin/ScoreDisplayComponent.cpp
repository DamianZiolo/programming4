#include "ScoreDisplayComponent.h"
#include "GameActor.h"
#include "GameObject.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "Event.h"

dae::ScoreDisplayComponent::ScoreDisplayComponent(GameObject* owner, GameActor* actor) :Component(owner),
m_pActor{ actor }
{
	m_pActor->AddObserver(this);
	m_pScoreComponent = m_pActor->GetOwner()->GetComponent<ScoreComponent>();
	m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
}


void dae::ScoreDisplayComponent::Notify(Event event)
{
	if (event == Event::ScoreChanged)
	{
		int score = m_pScoreComponent->GetScore();
		m_pTextComponent->SetText("Score: " + std::to_string(score));
	}

}
