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
#include <MenuComponent.h>
#include <MenuMoveCommand.h>
#include <ConfirmCommand.h>
#include <ProjectilePoolComponent.h>
#include <ctime>
#include <cstdlib>
#include "EnemyButterfly.h"

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

dae::GameActor* CreateKeyboardPlayer(dae::Scene& scene, const glm::vec3& screenCenter, dae::ProjectilePoolComponent* projectilePool)
{
	auto player = std::make_unique<dae::GameObject>();

	player->SetLocalPosition(screenCenter);
	player->AddComponent<dae::RenderComponent>("Player.png");
	player->GetComponent<dae::RenderComponent>()->SetSize(30, 30);
	auto actor = player->AddComponent<dae::GameActor>();
	player->AddComponent<dae::ScoreComponent>();
	player->AddComponent<dae::HealthComponent>(3);
	auto playerCollider = player->AddComponent<dae::BoxCollider>(glm::vec2(30,30));
	playerCollider->SetDrawDebug(true);
	
	//turn on debug 


	auto& input = dae::InputManager::GetInstance();

	input.BindKeyboardCommand(SDL_SCANCODE_SPACE, dae::InputState::Down,
		std::make_unique<dae::ShotCommand>(player.get(), projectilePool));

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

std::unique_ptr<dae::GameObject> CreateFly(dae::ProjectilePoolComponent* projectilePool)
{
	auto enemy = std::make_unique<dae::GameObject>();

	enemy->AddComponent<dae::RenderComponent>("Enemy1.png");
	enemy->GetComponent<dae::RenderComponent>()->SetSize(30, 30);

	auto collider = enemy->AddComponent<dae::BoxCollider>(glm::vec2(30, 30));
	collider->SetDrawDebug(true);

	enemy->AddComponent<dae::EnemyFly>(*projectilePool);

	return enemy;
}
std::unique_ptr<dae::GameObject> CreateButterfly(
	dae::ProjectilePoolComponent* projectilePool)
{
	auto enemy = std::make_unique<dae::GameObject>();

	enemy->AddComponent<dae::RenderComponent>("Enemy2.png");
	enemy->GetComponent<dae::RenderComponent>()->SetSize(30, 30);

	auto collider = enemy->AddComponent<dae::BoxCollider>(glm::vec2(30, 30));
	collider->SetDrawDebug(true);

	enemy->AddComponent<dae::EnemyButterfly>(*projectilePool);

	return enemy;
}


std::unique_ptr<dae::GameObject> CreateBoss(
	dae::Scene& scene,
	dae::ProjectilePoolComponent* projectilePool)
{
	auto boss = std::make_unique<dae::GameObject>();

	auto* render = boss->AddComponent<dae::RenderComponent>("Boss.png");
	render->SetSize(50, 50);

	auto collider = boss->AddComponent<dae::BoxCollider>(glm::vec2(50, 50));
	collider->SetDrawDebug(true);

	auto* bossComponent = boss->AddComponent<dae::EnemyBoss>(*projectilePool);
	bossComponent->CreateTractorBeam(scene);

	return boss;
}

std::unique_ptr<dae::GameObject> CreateFleet(
	dae::Scene& scene,
	dae::ProjectilePoolComponent* projectilePool)
{
	const int rows = 5;
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
				auto boss = CreateBoss(scene, projectilePool);

				boss->SetParent(slotRaw, false);

				auto enemyComponent = boss->GetComponent<dae::Enemy>();
				enemyComponent->SetSlot(fleetComponent->GetSlot(row, col));

				scene.Add(std::move(boss));
			}
			else if (row == 1 || row == 2)
			{
				auto butterfly = CreateButterfly(projectilePool);

				butterfly->SetParent(slotRaw, false);

				auto enemyComponent = butterfly->GetComponent<dae::Enemy>();
				enemyComponent->SetSlot(fleetComponent->GetSlot(row, col));

				scene.Add(std::move(butterfly));
			}
			else
			{
				auto enemy = CreateFly(projectilePool);

				enemy->SetParent(slotRaw, false);

				auto enemyComponent = enemy->GetComponent<dae::Enemy>();
				enemyComponent->SetSlot(fleetComponent->GetSlot(row, col));

				scene.Add(std::move(enemy));
			}
		}
	}

	return fleet;
}



dae::GameActor* CreateControllerPlayer(dae::Scene& scene, const glm::vec3& screenCenter, dae::ProjectilePoolComponent* projectilePool)
{
	auto parent = std::make_unique<dae::GameObject>();

	parent->SetLocalPosition(screenCenter);
	parent->AddComponent<dae::RenderComponent>("Player.png");
	parent->GetComponent<dae::RenderComponent>()->SetSize(30, 30);
	auto actor = parent->AddComponent<dae::GameActor>();
	parent->AddComponent<dae::ScoreComponent>();
	auto playerCollider = parent->AddComponent<dae::BoxCollider>(glm::vec2(30, 30));
	playerCollider->SetDrawDebug(true);
	
	parent->AddComponent<dae::HealthComponent>(3);

	auto& input = dae::InputManager::GetInstance();

	input.BindControllerCommand(
		dae::ControllerButton::B,
		dae::InputState::Down,
		std::make_unique<dae::ShotCommand>(parent.get(), projectilePool),
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

std::unique_ptr<dae::GameObject> CreatePooling(dae::Scene& scene)
{
	auto pooling = std::make_unique<dae::GameObject>();

	pooling->AddComponent<dae::ProjectilePoolComponent>(
		200,
		scene
	);

	return pooling;
}

static void LoadGameScene(dae::Scene& mainScene)
{
	const glm::vec3 screenCenter{ 512.f,288.f,0.f };
	dae::ServiceLocator::GetSoundSystem().RegisterSound(1, "Data/sound1.mp3");

	CreateBackground(mainScene);
	//CreateLogo(scene, screenCenter);
	auto bulletPooling = CreatePooling(mainScene);
	auto* projectilePool =
		bulletPooling->GetComponent<dae::ProjectilePoolComponent>();

	mainScene.Add(std::move(bulletPooling));
	
	auto player1 = CreateKeyboardPlayer(mainScene, screenCenter, projectilePool);
	auto player2 = CreateControllerPlayer(mainScene, screenCenter, projectilePool);
	mainScene.Add(CreateFleet(mainScene, projectilePool));

	

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	CreateLivesUI(mainScene, player1, font, { 20,20,0 }, "Health: ");
	CreateScoreUI(mainScene, player1, font, { 20,60,0 }, "Score: ");
	CreateLivesUI(mainScene, player2, font, { 20,100,0 }, "Health: ");
	CreateScoreUI(mainScene, player2, font, { 20,140,0 }, "Score: ");
	CreateControlsUI(
		mainScene,
		font,
		glm::vec3{ 20.f, 400.f, 0.f }
	);


	auto achievementGO = std::make_unique<dae::GameObject>();
	auto achievement = achievementGO->AddComponent<dae::AchievementSystemComponent>();

	achievement->AddActor(player1);
	achievement->AddActor(player2);

	mainScene.Add(std::move(achievementGO));

	//dae::ServiceLocator::GetSoundSystem().Play(1, 1.0f);

}

static void LoadMenuScene(dae::Scene& menuScene)
{
	auto fontTitle = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 64);
	auto fontMenu = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	
	auto menuController = std::make_unique<dae::GameObject>();
	auto* menu = menuController->AddComponent<dae::MenuComponent>();
	//CreateLogo(menuScene, glm::vec3{ 512.f, 120.f, 0.f });

	auto single = std::make_unique<dae::GameObject>();
	single->SetLocalPosition(420.f, 260.f, 0.f);
	auto singleText = single->AddComponent<dae::TextComponent>(
		fontMenu,
		"Single mode",
		SDL_Color{ 255,255,0,255 });

	menu->AddOption(singleText);
	menuScene.Add(std::move(single));

	auto coop = std::make_unique<dae::GameObject>();
	coop->SetLocalPosition(420.f, 320.f, 0.f);
	auto coopText = coop->AddComponent<dae::TextComponent>(
		fontMenu,
		"Co-op mode",
		SDL_Color{ 255,255,255,255 });
	menu->AddOption(coopText);
	menuScene.Add(std::move(coop));

	auto versus = std::make_unique<dae::GameObject>();
	versus->SetLocalPosition(420.f, 380.f, 0.f);
	auto versusText = versus->AddComponent<dae::TextComponent>(
		fontMenu,
		"Versus mode",
		SDL_Color{ 255,255,255,255 });
	menu->AddOption(versusText);

	auto& input = dae::InputManager::GetInstance();
	//W and S
	input.BindKeyboardCommand(SDL_SCANCODE_W, dae::InputState::Down, 
		std::make_unique<dae::MenuMoveCommand>(menu, -1));
	input.BindKeyboardCommand(SDL_SCANCODE_S, dae::InputState::Down,
		std::make_unique<dae::MenuMoveCommand>(menu, 1));

	//Arrows
	input.BindKeyboardCommand(SDL_SCANCODE_UP, dae::InputState::Down,
		std::make_unique<dae::MenuMoveCommand>(menu, -1));
	input.BindKeyboardCommand(SDL_SCANCODE_DOWN, dae::InputState::Down,
		std::make_unique<dae::MenuMoveCommand>(menu, 1));

	//keyboard cofirmation
	input.BindKeyboardCommand(SDL_SCANCODE_RETURN, dae::InputState::Down,
		std::make_unique<dae::ConfirmCommand>(menu));

	//Pad
	input.BindControllerCommand(dae::ControllerButton::DPadUp, dae::InputState::Down,
		std::make_unique<dae::MenuMoveCommand>(menu, -1), 0);

	input.BindControllerCommand(dae::ControllerButton::DPadDown, dae::InputState::Down,
		std::make_unique<dae::MenuMoveCommand>(menu, 1), 0);

	input.BindControllerCommand(dae::ControllerButton::A, dae::InputState::Down,
		std::make_unique<dae::ConfirmCommand>(menu), 0);


	menuScene.Add(std::move(versus));
	menuScene.Add(std::move(menuController));
}

static void LoadScoreScene(dae::Scene& scoreScene)
{
	auto fontTitle = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 64);
	auto fontMenu = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto single = std::make_unique<dae::GameObject>();
	single->SetLocalPosition(420.f, 260.f, 0.f);
	single->AddComponent<dae::TextComponent>(
		fontMenu,
		"Score",
		SDL_Color{ 255,255,0,255 });
	scoreScene.Add(std::move(single));

	
}

static void load()
{
	auto& menuScene = dae::SceneManager::GetInstance().CreateScene();
	auto& mainScene = dae::SceneManager::GetInstance().CreateScene();
	auto& ScoreScene = dae::SceneManager::GetInstance().CreateScene();
	
	//0 menu
	//1 main
	//2 score

	dae::SceneManager::GetInstance().SetActiveScene(0);
	

	LoadGameScene(mainScene);
	LoadMenuScene(menuScene);
	LoadScoreScene(ScoreScene);
	

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
	srand(static_cast<unsigned>(time(nullptr)));
    return 0;
}

