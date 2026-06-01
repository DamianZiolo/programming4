#include "MoveCommand.h"
#include "GameTime.h"
#include "GameObject.h"
#include "GameSettings.h"

#include "MoveCommand.h"
#include "GameObject.h"
#include "RenderComponent.h"

dae::MoveCommand::MoveCommand(
	const glm::vec3& direction,
	GameObject* target)
	: m_Speed(100.f)
	, m_Direction(direction)
	, m_pTarget(target)
{
	if (m_pTarget)
	{
		if (auto* renderer =
			m_pTarget->GetComponent<RenderComponent>())
		{
			m_PlayerWidth = renderer->GetWidth();
		}
	}
}

void dae::MoveCommand::Execute()
{
	if (m_pTarget == nullptr)
		return;

	const float dt = GameTime::GetIntance().GetDeltaTime();

	auto position = m_pTarget->GetLocalPosition();
	const auto newPosition =
		position + m_Direction * m_Speed * dt;

	if (newPosition.x < 0.f ||
		newPosition.x > GameSettings::ScreenWidth - m_PlayerWidth)
	{
		return;
	}

	m_pTarget->SetLocalPosition(newPosition);
}

