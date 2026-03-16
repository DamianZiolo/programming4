//#pragma once
//#include "Component.h"
//#include "Observer.h"
//#include <string>
//#include <functional>
//
//namespace dae
//{
//	class GameActor;
//	class TextComponent;
//
//	class DisplayComponent final : public Component, public Observer
//	{
//	public:
//		using ValueGetter = std::function<int()>;
//
//		DisplayComponent(
//			GameObject* owner,
//			GameActor* actor,
//			const std::string& displayText,
//			ValueGetter getter
//			);
//
//		void Update() override {};
//		void Notify(dae::Event event, GameActor* actor) override;
//	private:
//		GameActor* m_pActor{};
//		TextComponent* m_pTextComponent{};
//		std::string m_DisplayText{};
//		ValueGetter m_GetValue{};
//	};
//
//}