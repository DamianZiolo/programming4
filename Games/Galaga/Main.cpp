#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>) //If we're in debug mode and leak detector is available(File vlh.h exist), include it
#include <vld.h>
#endif

#if _DEBUG
	#if __has_include(<vld.h>)
	#pragma message(">>> VLD HEADER FOUND - including vld.h")
	#include <vld.h>
	#else
	#pragma message(">>> VLD HEADER NOT FOUND")
	#endif
#endif


#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "RotationComponent.h"
#include "TrashcacheComponent.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "HealthComponent.h"
#include "HealthDisplayComponent.h"
#include "ShotCommand.h"
#include "ScoreDisplayComponent.h"
#include "ScoreComponent.h"
#include "CollectedPointsCommand.h"
#include "SteamManager.h"
#include "AchievementSystemComponent.h"


#include <filesystem>
#include "ControllerButton.h"
#include "GameActor.h"
#include "BoxCollider.h"
#include <EnemyFly.h>
#include <ServiceLocator.h>
#include "EnemyBoss.h"
#include "FleetComponent.h"

namespace fs = std::filesystem;

void CreateBackground(dae::Scene& scene)
{
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("background.png");
	scene.Add(std::move(go));
}

void CreateLogo(dae::Scene& scene, const glm::vec3& screenCenter)
{
	auto go = std::make_unique<dae::GameObject>();
	go->SetLocalPosition(screenCenter);
	go->AddComponent<dae::RenderComponent>("logo.png");
	scene.Add(std::move(go));
}

dae::GameActor* CreateKeyboardPlayer(dae::Scene& scene, const glm::vec3& screenCenter)
{
	auto player = std::make_unique<dae::GameObject>();

	player->SetLocalPosition(screenCenter);
	player->AddComponent<dae::RenderComponent>("Player.png");
	player->GetComponent<dae::RenderComponent>()->SetSize(20, 20);
	auto actor = player->AddComponent<dae::GameActor>();
	player->AddComponent<dae::ScoreComponent>();
	player->AddComponent<dae::HealthComponent>(3);
	auto playerCollider = player->AddComponent<dae::BoxCollider>(glm::vec2(20,20));
	playerCollider->SetDrawDebug(true);
	
	//turn on debug 


	auto& input = dae::InputManager::GetInstance();

	input.BindKeyboardCommand(SDL_SCANCODE_SPACE, dae::InputState::Down,
		std::make_unique<dae::ShotCommand>(player.get()));

	input.BindKeyboardCommand(SDL_SCANCODE_W, dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ 0,-1,0 }, player.get()));

	input.BindKeyboardCommand(SDL_SCANCODE_S, dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ 0,1,0 }, player.get()));

	input.BindKeyboardCommand(SDL_SCANCODE_A, dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ -1,0,0 }, player.get()));

	input.BindKeyboardCommand(SDL_SCANCODE_D, dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ 1,0,0 }, player.get()));

	input.BindKeyboardCommand(
		SDL_SCANCODE_P,
		dae::InputState::Down,
		std::make_unique<dae::CollectedPointsCommand>(player.get(), 100)
	);

	scene.Add(std::move(player));

	return actor;
}

std::unique_ptr<dae::GameObject> CreateFly()
{
	auto enemy = std::make_unique<dae::GameObject>();

	enemy->AddComponent<dae::RenderComponent>("Enemy1.png");
	enemy->GetComponent<dae::RenderComponent>()->SetSize(30, 30);

	auto collider = enemy->AddComponent<dae::BoxCollider>(glm::vec2(30, 30));
	collider->SetDrawDebug(true);

	enemy->AddComponent<dae::EnemyFly>();

	return enemy;
}

std::unique_ptr<dae::GameObject> CreateBoss()
{
	auto boss = std::make_unique<dae::GameObject>();

	boss->AddComponent<dae::RenderComponent>("Boss.png");
	boss->GetComponent<dae::RenderComponent>()->SetSize(50, 50);

	auto collider = boss->AddComponent<dae::BoxCollider>(glm::vec2(50, 50));
	collider->SetDrawDebug(true);

	boss->AddComponent<dae::EnemyBoss>();

	return boss;
}

std::unique_ptr<dae::GameObject> CreateFleet(dae::Scene& scene)
{
	const int rows = 4;
	const int cols = 10;

	float spacingX = 100.f;
	float spacingY = 50.f;

	auto fleet = std::make_unique<dae::GameObject>();
	fleet->SetLocalPosition(glm::vec3(50.f, 0.f, 0.f));

	auto* fleetRaw = fleet.get();

	auto fleetComponent = fleet->AddComponent<dae::FleetComponent>(
			rows,
			cols,
			spacingX,
			spacingY);

	for (int row = 0; row < rows; ++row)
	{
		for (int col = 0; col < cols; ++col)
		{
			auto slot = std::make_unique<dae::GameObject>();

			slot->SetParent(fleetRaw, false);

			slot->SetLocalPosition(
				col * spacingX,
				row * spacingY,
				0.f);

			auto* slotRaw = slot.get();

			scene.Add(std::move(slot));

			fleetComponent->SetSlot(row, col, slotRaw);

			if (row == 0)
			{
				auto boss = CreateBoss();

				boss->SetParent(slotRaw, false);
				auto enemyComponent = boss->GetComponent<dae::Enemy>();
				enemyComponent->SetSlot(fleetComponent->GetSlot(row, col));

				scene.Add(std::move(boss));
			}
			else
			{
				auto enemy = CreateFly();

				enemy->SetParent(slotRaw, false);
				auto enemyComponent = enemy->GetComponent<dae::Enemy>();
				enemyComponent->SetSlot(fleetComponent->GetSlot(row, col));

				scene.Add(std::move(enemy));
			}
		}
	}

	return fleet;
}



dae::GameActor* CreateControllerPlayer(dae::Scene& scene, const glm::vec3& screenCenter)
{
	auto parent = std::make_unique<dae::GameObject>();

	parent->SetLocalPosition(screenCenter);
	parent->AddComponent<dae::RenderComponent>("Player.png");
	parent->GetComponent<dae::RenderComponent>()->SetSize(30, 30);
	auto actor = parent->AddComponent<dae::GameActor>();
	parent->AddComponent<dae::ScoreComponent>();

	
	parent->AddComponent<dae::HealthComponent>(3);

	auto& input = dae::InputManager::GetInstance();

	input.BindControllerCommand(
		dae::ControllerButton::B,
		dae::InputState::Down,
		std::make_unique<dae::ShotCommand>(parent.get()),
		0);

	input.BindControllerCommand(
		dae::ControllerButton::DPadUp,
		dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ 0,-1,0 }, parent.get()),
		0);

	input.BindControllerCommand(
		dae::ControllerButton::DPadDown,
		dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ 0,1,0 }, parent.get()),
		0);

	input.BindControllerCommand(
		dae::ControllerButton::DPadLeft,
		dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ -1,0,0 }, parent.get()),
		0);

	input.BindControllerCommand(
		dae::ControllerButton::DPadRight,
		dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ 1,0,0 }, parent.get()),
		0);

	input.BindControllerCommand(
		dae::ControllerButton::A,
		dae::InputState::Down,
		std::make_unique<dae::CollectedPointsCommand>(parent.get(), 100)
	);

	scene.Add(std::move(parent));

	return actor;
}

void CreateLivesUI(
	dae::Scene& scene,
	dae::GameActor* actor,
	std::shared_ptr<dae::Font> font,
	const glm::vec3& position,
	const std::string& label)
{
	auto ui = std::make_unique<dae::GameObject>();

	ui->SetLocalPosition(position);

	ui->AddComponent<dae::TextComponent>(
		font,
		label + "3",
		SDL_Color{ 255,255,255,255 });

	ui->AddComponent<dae::HealthDisplayComponent>(actor);

	scene.Add(std::move(ui));
}

void CreateScoreUI(
	dae::Scene& scene,
	dae::GameActor* actor,
	std::shared_ptr<dae::Font> font,
	const glm::vec3& position,
	const std::string& label)
{
	auto ui = std::make_unique<dae::GameObject>();

	ui->SetLocalPosition(position);

	ui->AddComponent<dae::TextComponent>(
		font,
		label + "0",
		SDL_Color{ 255,255,255,255 });

	ui->AddComponent<dae::ScoreDisplayComponent>(actor);

	scene.Add(std::move(ui));
}

void CreateTexts(dae::Scene& scene, std::shared_ptr<dae::Font> font)
{
	auto goText = std::make_unique<dae::GameObject>();

	goText->SetLocalPosition(292, 20);
	goText->AddComponent<dae::TextComponent>(
		font,
		"Programming 4 Assignment",
		SDL_Color{ 255,255,0,255 });

	scene.Add(std::move(goText));


	goText = std::make_unique<dae::GameObject>();

	goText->SetLocalPosition(292, 80);
	goText->AddComponent<dae::TextComponent>(
		font,
		"0 FPS",
		SDL_Color{ 255,255,0,255 });

	goText->AddComponent<dae::FPSComponent>();

	scene.Add(std::move(goText));
}

void CreateControlsUI(
	dae::Scene& scene,
	std::shared_ptr<dae::Font> font,
	const glm::vec3& position)
{
	auto ui = std::make_unique<dae::GameObject>();

	ui->SetLocalPosition(position);

	std::string controlsText =
		"Keyboard:"
		"SPACE - Shoot (THIS PLAY SOUND)"
		;

	ui->AddComponent<dae::TextComponent>(
		font,
		controlsText,
		SDL_Color{ 255,255,255,255 }
	);

	scene.Add(std::move(ui));
}



static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	const glm::vec3 screenCenter{ 512.f,288.f,0.f };
	dae::ServiceLocator::GetSoundSystem().RegisterSound(1, "Data/sound1.mp3");

	CreateBackground(scene);
	//CreateLogo(scene, screenCenter);

	auto player1 = CreateKeyboardPlayer(scene, screenCenter);
	auto player2 = CreateControllerPlayer(scene, screenCenter);

	scene.Add(CreateFleet(scene));
	

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	CreateLivesUI(scene, player1, font, { 20,20,0 }, "Health: ");
	CreateScoreUI(scene, player1, font, { 20,60,0 }, "Score: ");
	CreateLivesUI(scene, player2, font, { 20,100,0 }, "Health: ");
	CreateScoreUI(scene, player2, font, { 20,140,0 }, "Score: ");
	CreateControlsUI(
		scene,
		font,
		glm::vec3{ 20.f, 400.f, 0.f }
	);


	auto achievementGO = std::make_unique<dae::GameObject>();
	auto achievement = achievementGO->AddComponent<dae::AchievementSystemComponent>();

	achievement->AddActor(player1);
	achievement->AddActor(player2);

	scene.Add(std::move(achievementGO));

	dae::ServiceLocator::GetSoundSystem().Play(1, 1.0f);

}

int main(int, char*[]) {
	/*int* leak = new int[100];
	leak[0] = 123;  */ 
	// leak on purpose to check if vld is working

#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::SteamManager::Init();
	dae::Minigin engine(data_location);
	engine.Run(load);
	dae::SteamManager::Shutdown();
    return 0;
}

