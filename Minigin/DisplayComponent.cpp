//#include "DisplayComponent.h"
//#include "GameActor.h"
//#include "TextComponent.h"
//#include "GameObject.h"
//
//dae::DisplayComponent::DisplayComponent(GameObject* owner, GameActor* actor, const std::string& displayText, ValueGetter getter)
//	:Component(owner),
//	m_pActor{ actor },
//	m_DisplayText{ displayText },
//	m_GetValue{ getter }
//{
//	m_pActor->AddObserver(this);
//	m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
//}
//
//void dae::DisplayComponent::Notify(Event event, GameActor* actor)
//{
//	if (actor != m_pActor)
//		return;
//	int value = m_GetValue();
//	if (event == Event::PlayerDamaged)
//	{
//		m_pTextComponent->SetText(m_DisplayText + ": " + std::to_string(value));
//	}
//
//}
