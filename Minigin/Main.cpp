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

#include <filesystem>
#include "ControllerButton.h"
#include "GameActor.h"
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
	auto parent = std::make_unique<dae::GameObject>();

	parent->SetLocalPosition(screenCenter);
	parent->AddComponent<dae::RenderComponent>("Player.png");
	parent->GetComponent<dae::RenderComponent>()->SetSize(20, 20);

	auto actor = parent->AddComponent<dae::GameActor>();
	parent->AddComponent<dae::HealthComponent>(3);

	auto& input = dae::InputManager::GetInstance();

	input.BindKeyboardCommand(SDL_SCANCODE_SPACE, dae::InputState::Down,
		std::make_unique<dae::ShotCommand>(parent.get()));

	input.BindKeyboardCommand(SDL_SCANCODE_W, dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ 0,-1,0 }, parent.get()));

	input.BindKeyboardCommand(SDL_SCANCODE_S, dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ 0,1,0 }, parent.get()));

	input.BindKeyboardCommand(SDL_SCANCODE_A, dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ -1,0,0 }, parent.get()));

	input.BindKeyboardCommand(SDL_SCANCODE_D, dae::InputState::Pressed,
		std::make_unique<dae::MoveCommand>(glm::vec3{ 1,0,0 }, parent.get()));

	scene.Add(std::move(parent));

	return actor;
}

void CreateControllerPlayer(dae::Scene& scene, const glm::vec3& screenCenter)
{
	auto parent = std::make_unique<dae::GameObject>();

	parent->SetLocalPosition(screenCenter);
	parent->AddComponent<dae::RenderComponent>("Player.png");
	parent->GetComponent<dae::RenderComponent>()->SetSize(30, 30);

	parent->AddComponent<dae::GameActor>();
	parent->AddComponent<dae::HealthComponent>(3);

	auto& input = dae::InputManager::GetInstance();

	input.BindControllerCommand(
		dae::ControllerButton::B,
		dae::InputState::Pressed,
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

	scene.Add(std::move(parent));
}

void CreateLivesUI(dae::Scene& scene, dae::GameActor* actor, std::shared_ptr<dae::Font> font)
{
	auto uiLives = std::make_unique<dae::GameObject>();

	uiLives->SetLocalPosition(20, 20);

	uiLives->AddComponent<dae::TextComponent>(
		font,
		"Lives: 3",
		SDL_Color{ 255,255,255,255 });

	uiLives->AddComponent<dae::HealthDisplayComponent>(actor);

	scene.Add(std::move(uiLives));
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

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	const glm::vec3 screenCenter{ 512.f,288.f,0.f };

	CreateBackground(scene);
	CreateLogo(scene, screenCenter);

	auto playerActor = CreateKeyboardPlayer(scene, screenCenter);
	CreateControllerPlayer(scene, screenCenter);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	CreateTexts(scene, font);
	CreateLivesUI(scene, playerActor, font);
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
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}

