#include "EnemyButterflyLoopDiveState.h"

#include "EnemyButterfly.h"
#include "EnemyButterflyReturnState.h"
#include "GameObject.h"
#include "GameTime.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace dae
{
	void EnemyButterflyLoopDiveState::OnEnter(EnemyButterfly& enemy)
	{
		m_Timer = 0.f;
		m_IsLooping = false;
		m_HasFinishedLoop = false;

		m_HorizontalSpeed =
			static_cast<float>((rand() % 2 == 0) ? -35.f : 35.f);

		m_LoopCenter = enemy.GetOwner()->GetWorldPosition();
	}

	std::unique_ptr<EnemyButterflyState> EnemyButterflyLoopDiveState::Update(EnemyButterfly& enemy)
	{
		auto* owner = enemy.GetOwner();

		const auto dt = GameTime::GetIntance().GetDeltaTime();

		auto pos = owner->GetWorldPosition();

		if (!m_IsLooping && !m_HasFinishedLoop && pos.y >= loopStartLine)
		{
			m_IsLooping = true;
			m_Timer = 0.f;
			m_LoopCenter = pos;
		}

		if (m_IsLooping)
		{
			m_Timer += dt;

			const auto progress =
				std::clamp(m_Timer / m_LoopDuration, 0.f, 1.f);

			const auto angle = progress * fullCircle;

			pos.x = m_LoopCenter.x + std::sin(angle) * radiusX;
			pos.y = m_LoopCenter.y + (1.f - std::cos(angle)) * radiusY;

			if (progress >= 1.f)
			{
				m_IsLooping = false;
				m_HasFinishedLoop = true;

				pos = m_LoopCenter;
			}
		}
		else
		{
			pos.x += m_HorizontalSpeed * dt;
			pos.y += m_DownSpeed * dt;
		}

		owner->SetWorldPosition(pos);

		if (enemy.TeleportToTopIfBelowScreen())
		{
			return std::make_unique<EnemyButterflyReturnState>();
		}

		return nullptr;
	}
}