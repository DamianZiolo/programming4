#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
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
#include "ControllerButton.h"
#include "GameActor.h"
#include "BoxCollider.h"
#include "ServiceLocator.h"
#include "MenuComponent.h"
#include "MenuMoveCommand.h"
#include "ConfirmCommand.h"
#include "ProjectilePoolComponent.h"
#include "LevelManagerComponent.h"
#include "GameSettings.h"
#include "GameMode.h"
#include "Component.h"
#include "GameFlowController.h"
#include "PlayerDeathComponent.h"

#include <filesystem>
#include <ctime>
#include <cstdlib>

namespace fs = std::filesystem;

namespace
{
	constexpr float PlayerSize{ 30.f };
	constexpr float PlayerYFromBottom{ 80.f };

	constexpr float LifeIconMarginX{ 10.f };
	constexpr float LifeIconMarginBottom{ 10.f };

	constexpr float ScoreMarginY{ 20.f };
	constexpr float ControlsMarginBottom{ 90.f };

	constexpr float MenuOptionSpacing{ 60.f };
	constexpr float MenuNameSpacing{ 40.f };
}

void CreateBackground(dae::Scene& scene)
{
	auto go = std::make_unique<dae::GameObject>();

	auto* render = go->AddComponent<dae::RenderComponent>("background.png");
	render->SetSize(
		dae::GameSettings::ScreenWidth,
		dae::GameSettings::ScreenHeight);

	scene.Add(std::move(go));
}

void BindKeyboardControls(
	dae::GameObject* player,
	dae::ProjectilePoolComponent* projectilePool)
{
	auto& input = dae::InputManager::GetInstance();

	input.BindKeyboardCommand(SDL_SCANCODE_SPACE, dae::InputState::Down,
		std::make_unique<dae::ShotCommand>(player, projectilePool));

	input.BindKeyboardCommand(SDL_SCANCODE_W, dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ 0.f, -1.f, 0.f }, player));

	input.BindKeyboardCommand(SDL_SCANCODE_S, dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ 0.f, 1.f, 0.f }, player));

	input.BindKeyboardCommand(SDL_SCANCODE_A, dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ -1.f, 0.f, 0.f }, player));

	input.BindKeyboardCommand(SDL_SCANCODE_D, dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ 1.f, 0.f, 0.f }, player));

	input.BindKeyboardCommand(SDL_SCANCODE_P, dae::InputState::Down,
		std::make_unique<dae::CollectedPointsCommand>(player, 100));
}

void BindControllerControls(
	dae::GameObject* player,
	dae::ProjectilePoolComponent* projectilePool,
	int controllerIndex)
{
	auto& input = dae::InputManager::GetInstance();

	input.BindControllerCommand(
		dae::ControllerButton::B,
		dae::InputState::Down,
		std::make_unique<dae::ShotCommand>(player, projectilePool),
		controllerIndex);

	input.BindControllerCommand(
		dae::ControllerButton::DPadUp,
		dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ 0.f, -1.f, 0.f }, player),
		controllerIndex);

	input.BindControllerCommand(
		dae::ControllerButton::DPadDown,
		dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ 0.f, 1.f, 0.f }, player),
		controllerIndex);

	input.BindControllerCommand(
		dae::ControllerButton::DPadLeft,
		dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ -1.f, 0.f, 0.f }, player),
		controllerIndex);

	input.BindControllerCommand(
		dae::ControllerButton::DPadRight,
		dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ 1.f, 0.f, 0.f }, player),
		controllerIndex);

	input.BindControllerCommand(
		dae::ControllerButton::A,
		dae::InputState::Down,
		std::make_unique<dae::CollectedPointsCommand>(player, 100),
		controllerIndex);
}

dae::GameActor* CreatePlayer(
	dae::Scene& scene,
	const glm::vec3& position,
	dae::ProjectilePoolComponent* projectilePool,
	bool useKeyboard,
	bool useController,
	int controllerIndex)
{
	auto player = std::make_unique<dae::GameObject>();

	player->SetLocalPosition(position);

	auto* render = player->AddComponent<dae::RenderComponent>("Player.png");
	render->SetSize(PlayerSize, PlayerSize);

	auto* actor = player->AddComponent<dae::GameActor>();
	player->AddComponent<dae::PlayerDeathComponent>(actor);
	player->AddComponent<dae::ScoreComponent>();
	player->AddComponent<dae::HealthComponent>(4);

	auto* collider = player->AddComponent<dae::BoxCollider>(
		glm::vec2{ PlayerSize, PlayerSize });
	collider->SetDrawDebug(true);

	auto* playerRaw = player.get();

	if (useKeyboard)
	{
		BindKeyboardControls(playerRaw, projectilePool);
	}

	if (useController)
	{
		BindControllerControls(playerRaw, projectilePool, controllerIndex);
	}

	scene.Add(std::move(player));

	return actor;
}

void CreateLivesUI(
	dae::Scene& scene,
	dae::GameActor* actor,
	const glm::vec3& position)
{
	auto ui = std::make_unique<dae::GameObject>();

	ui->SetLocalPosition(position);

	ui->AddComponent<dae::HealthDisplayComponent>(
		actor,
		scene,
		4);

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

void CreateControlsUI(
	dae::Scene& scene,
	std::shared_ptr<dae::Font> font,
	const glm::vec3& position)
{
	auto ui = std::make_unique<dae::GameObject>();

	ui->SetLocalPosition(position);

	std::string controlsText =
		"Keyboard: WASD - Move, SPACE - Shoot";

	ui->AddComponent<dae::TextComponent>(
		font,
		controlsText,
		SDL_Color{ 255,255,255,255 });

	scene.Add(std::move(ui));
}

std::unique_ptr<dae::GameObject> CreatePooling(dae::Scene& scene)
{
	auto pooling = std::make_unique<dae::GameObject>();

	pooling->AddComponent<dae::ProjectilePoolComponent>(
		200,
		scene);

	return pooling;
}

static void LoadGameScene(dae::Scene& mainScene)
{
	const auto mode = dae::GameSettings::GetInstance().GetGameMode();

	constexpr float screenWidth = dae::GameSettings::ScreenWidth;
	constexpr float screenHeight = dae::GameSettings::ScreenHeight;

	const float playerY = screenHeight - PlayerYFromBottom;

	dae::ServiceLocator::GetSoundSystem().RegisterSound(1, "Data/sound1.mp3");

	CreateBackground(mainScene);

	auto bulletPooling = CreatePooling(mainScene);
	auto* projectilePool =
		bulletPooling->GetComponent<dae::ProjectilePoolComponent>();

	mainScene.Add(std::move(bulletPooling));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	dae::GameActor* player1{};
	dae::GameActor* player2{};

	if (mode == dae::GameMode::Solo)
	{
		player1 = CreatePlayer(
			mainScene,
			glm::vec3{ screenWidth * 0.5f, playerY, 0.f },
			projectilePool,
			true,
			true,
			0);

		CreateLivesUI(
			mainScene,
			player1,
			{ LifeIconMarginX, screenHeight - LifeIconMarginBottom - PlayerSize, 0.f });

		CreateScoreUI(
			mainScene,
			player1,
			font,
			{ screenWidth * 0.5f - 80.f, ScoreMarginY, 0.f },
			"Score: ");
	}
	else if (mode == dae::GameMode::Duo)
	{
		player1 = CreatePlayer(
			mainScene,
			glm::vec3{ screenWidth * 0.45f, playerY, 0.f },
			projectilePool,
			true,
			true,
			0);

		player2 = CreatePlayer(
			mainScene,
			glm::vec3{ screenWidth * 0.55f, playerY, 0.f },
			projectilePool,
			false,
			true,
			1);

		CreateLivesUI(
			mainScene,
			player1,
			{ LifeIconMarginX, screenHeight - LifeIconMarginBottom - PlayerSize, 0.f });

		CreateLivesUI(
			mainScene,
			player2,
			{ screenWidth - 120.f, screenHeight - LifeIconMarginBottom - PlayerSize, 0.f });

		CreateScoreUI(
			mainScene,
			player1,
			font,
			{ screenWidth * 0.25f - 80.f, ScoreMarginY, 0.f },
			"P1 Score: ");

		CreateScoreUI(
			mainScene,
			player2,
			font,
			{ screenWidth * 0.75f - 80.f, ScoreMarginY, 0.f },
			"P2 Score: ");
	}
	else
	{
		player1 = CreatePlayer(
			mainScene,
			glm::vec3{ screenWidth * 0.5f, playerY, 0.f },
			projectilePool,
			true,
			true,
			0);

		CreateLivesUI(
			mainScene,
			player1,
			{ LifeIconMarginX, screenHeight - LifeIconMarginBottom - PlayerSize, 0.f });

		CreateScoreUI(
			mainScene,
			player1,
			font,
			{ screenWidth * 0.5f - 80.f, ScoreMarginY, 0.f },
			"Score: ");
	}

	auto levelManagerGO = std::make_unique<dae::GameObject>();

	auto* levelManagerComponent =
		levelManagerGO->AddComponent<dae::LevelManagerComponent>(
			mainScene,
			*projectilePool);

	mainScene.Add(std::move(levelManagerGO));

	auto flowGO = std::make_unique<dae::GameObject>();

	auto* flow = flowGO->AddComponent<dae::GameFlowController>(
		levelManagerComponent);

	flow->AddPlayer(player1);

	if (player2)
	{
		flow->AddPlayer(player2);
	}

	mainScene.Add(std::move(flowGO));

	CreateControlsUI(
		mainScene,
		font,
		glm::vec3{ 20.f, screenHeight - ControlsMarginBottom, 0.f });

	auto achievementGO = std::make_unique<dae::GameObject>();
	auto* achievement = achievementGO->AddComponent<dae::AchievementSystemComponent>();

	if (player1)
	{
		achievement->AddActor(player1);
	}

	if (player2)
	{
		achievement->AddActor(player2);
	}

	mainScene.Add(std::move(achievementGO));
}

class GameSceneLoaderComponent final : public dae::Component
{
public:
	GameSceneLoaderComponent(dae::GameObject* owner, dae::Scene& scene)
		: Component(owner)
		, m_Scene{ scene }
	{
	}

	void Update() override
	{
		if (m_HasLoaded)
			return;

		m_HasLoaded = true;

		LoadGameScene(m_Scene);

		GetOwner()->MarkForRemoval();
	}

private:
	dae::Scene& m_Scene;
	bool m_HasLoaded{ false };
};

static void LoadMenuScene(dae::Scene& menuScene)
{
	constexpr float screenWidth = dae::GameSettings::ScreenWidth;
	constexpr float screenHeight = dae::GameSettings::ScreenHeight;

	auto fontMenu = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	const float menuX = screenWidth * 0.5f - 90.f;
	const float firstOptionY = screenHeight * 0.28f;

	auto menuController = std::make_unique<dae::GameObject>();
	auto* menu = menuController->AddComponent<dae::MenuComponent>();

	auto single = std::make_unique<dae::GameObject>();
	single->SetLocalPosition(menuX, firstOptionY, 0.f);
	auto* singleText = single->AddComponent<dae::TextComponent>(
		fontMenu,
		"Single mode",
		SDL_Color{ 255,255,0,255 });

	menu->AddOption(singleText);
	menuScene.Add(std::move(single));

	auto coop = std::make_unique<dae::GameObject>();
	coop->SetLocalPosition(menuX, firstOptionY + MenuOptionSpacing, 0.f);
	auto* coopText = coop->AddComponent<dae::TextComponent>(
		fontMenu,
		"Co-op mode",
		SDL_Color{ 255,255,255,255 });

	menu->AddOption(coopText);
	menuScene.Add(std::move(coop));

	auto versus = std::make_unique<dae::GameObject>();
	versus->SetLocalPosition(menuX, firstOptionY + MenuOptionSpacing * 2.f, 0.f);
	auto* versusText = versus->AddComponent<dae::TextComponent>(
		fontMenu,
		"Versus mode",
		SDL_Color{ 255,255,255,255 });

	menu->AddOption(versusText);
	menuScene.Add(std::move(versus));

	const float nameY = firstOptionY + MenuOptionSpacing * 3.5f;
	const float nameLabelX = screenWidth * 0.5f - 120.f;
	const float firstLetterX = screenWidth * 0.5f;

	auto nameLabel = std::make_unique<dae::GameObject>();
	nameLabel->SetLocalPosition(nameLabelX, nameY, 0.f);
	nameLabel->AddComponent<dae::TextComponent>(
		fontMenu,
		"Name:",
		SDL_Color{ 255,255,255,255 });

	menuScene.Add(std::move(nameLabel));

	for (int i = 0; i < 4; ++i)
	{
		auto letter = std::make_unique<dae::GameObject>();

		letter->SetLocalPosition(
			firstLetterX + i * MenuNameSpacing,
			nameY,
			0.f);

		auto* letterText = letter->AddComponent<dae::TextComponent>(
			fontMenu,
			"A",
			SDL_Color{ 255,255,255,255 });

		menu->AddNameSlot(letterText);

		menuScene.Add(std::move(letter));
	}

	auto& input = dae::InputManager::GetInstance();

	input.BindKeyboardCommand(SDL_SCANCODE_W, dae::InputState::Down,
		std::make_unique<dae::MenuMoveCommand>(
			menu,
			dae::MenuMoveDirection::Up));

	input.BindKeyboardCommand(SDL_SCANCODE_S, dae::InputState::Down,
		std::make_unique<dae::MenuMoveCommand>(
			menu,
			dae::MenuMoveDirection::Down));

	input.BindKeyboardCommand(SDL_SCANCODE_A, dae::InputState::Down,
		std::make_unique<dae::MenuMoveCommand>(
			menu,
			dae::MenuMoveDirection::Left));

	input.BindKeyboardCommand(SDL_SCANCODE_D, dae::InputState::Down,
		std::make_unique<dae::MenuMoveCommand>(
			menu,
			dae::MenuMoveDirection::Right));

	input.BindKeyboardCommand(SDL_SCANCODE_UP, dae::InputState::Down,
		std::make_unique<dae::MenuMoveCommand>(
			menu,
			dae::MenuMoveDirection::Up));

	input.BindKeyboardCommand(SDL_SCANCODE_DOWN, dae::InputState::Down,
		std::make_unique<dae::MenuMoveCommand>(
			menu,
			dae::MenuMoveDirection::Down));

	input.BindKeyboardCommand(SDL_SCANCODE_LEFT, dae::InputState::Down,
		std::make_unique<dae::MenuMoveCommand>(
			menu,
			dae::MenuMoveDirection::Left));

	input.BindKeyboardCommand(SDL_SCANCODE_RIGHT, dae::InputState::Down,
		std::make_unique<dae::MenuMoveCommand>(
			menu,
			dae::MenuMoveDirection::Right));

	input.BindKeyboardCommand(SDL_SCANCODE_RETURN, dae::InputState::Down,
		std::make_unique<dae::ConfirmCommand>(menu));

	input.BindControllerCommand(
		dae::ControllerButton::DPadUp,
		dae::InputState::Down,
		std::make_unique<dae::MenuMoveCommand>(
			menu,
			dae::MenuMoveDirection::Up),
		0);

	input.BindControllerCommand(
		dae::ControllerButton::DPadDown,
		dae::InputState::Down,
		std::make_unique<dae::MenuMoveCommand>(
			menu,
			dae::MenuMoveDirection::Down),
		0);

	input.BindControllerCommand(
		dae::ControllerButton::DPadLeft,
		dae::InputState::Down,
		std::make_unique<dae::MenuMoveCommand>(
			menu,
			dae::MenuMoveDirection::Left),
		0);

	input.BindControllerCommand(
		dae::ControllerButton::DPadRight,
		dae::InputState::Down,
		std::make_unique<dae::MenuMoveCommand>(
			menu,
			dae::MenuMoveDirection::Right),
		0);

	input.BindControllerCommand(
		dae::ControllerButton::A,
		dae::InputState::Down,
		std::make_unique<dae::ConfirmCommand>(menu),
		0);

	menuScene.Add(std::move(menuController));
}

static void LoadScoreScene(dae::Scene& scoreScene)
{
	constexpr float screenWidth = dae::GameSettings::ScreenWidth;
	constexpr float screenHeight = dae::GameSettings::ScreenHeight;

	auto fontMenu = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto scoreText = std::make_unique<dae::GameObject>();
	scoreText->SetLocalPosition(
		screenWidth * 0.5f - 45.f,
		screenHeight * 0.5f,
		0.f);

	scoreText->AddComponent<dae::TextComponent>(
		fontMenu,
		"Score",
		SDL_Color{ 255,255,0,255 });

	scoreScene.Add(std::move(scoreText));
}

static void load()
{
	auto& menuScene = dae::SceneManager::GetInstance().CreateScene();
	auto& mainScene = dae::SceneManager::GetInstance().CreateScene();
	auto& scoreScene = dae::SceneManager::GetInstance().CreateScene();

	dae::SceneManager::GetInstance().SetActiveScene(0);

	LoadMenuScene(menuScene);
	LoadScoreScene(scoreScene);

	auto gameLoader = std::make_unique<dae::GameObject>();
	gameLoader->AddComponent<GameSceneLoaderComponent>(mainScene);
	mainScene.Add(std::move(gameLoader));
}

int main(int, char* [])
{
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if (!fs::exists(data_location))
		data_location = "../Data/";
#endif

	dae::SteamManager::Init();

	srand(static_cast<unsigned>(time(nullptr)));

	dae::Minigin engine(data_location);
	engine.Run(load);

	dae::SteamManager::Shutdown();

	return 0;
}
