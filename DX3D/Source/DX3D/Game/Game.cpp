#include <DX3D/Game/Game.h>
#include <DX3D/Window/Window.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Core/Logger.h>
#include <DX3D/Input/InputSystem.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Game/World.h>
#include <DX3D/Game/GameObject.h>
#include <DX3D/Game/WorldRenderer.h>

dx3d::Game::Game(const GameDesc& desc)
{
	m_loggerPtr = std::make_unique<Logger>(desc.logLevel);

	DX3DLogInfo("Rafael Ira R. Villanueva DirectX 3D Engine GDENG03");
	DX3DLogInfo("--------------------------------------------------");

	m_inputSystemPtr = std::make_unique<InputSystem>(InputSystemDesc{ *m_loggerPtr });
	m_graphicsDevice = std::make_shared<GraphicsDevice>(GraphicsDeviceDesc{ *m_loggerPtr });
	m_display = std::make_unique<Display>(DisplayDesc{ {*m_loggerPtr,desc.windowSize},*m_graphicsDevice });
	m_world = std::make_unique<World>(WorldDesc{ BaseDesc{*m_loggerPtr}, GameContext{*m_inputSystemPtr} });
	m_worldRenderer = std::make_unique<WorldRenderer>(WorldRendererDesc{ {*m_loggerPtr},*m_graphicsDevice });

	m_inputSystemPtr->setCursorLockArea(m_display->getClientAreaInScreenSpace());

	DX3DLogInfo("Game is initialized successfully.");
}

dx3d::Game::~Game()
{
	DX3DLogInfo("Game memory deallocation started. Will shutdown");
}

dx3d::World& dx3d::Game::getWorld() noexcept
{
	return *m_world;
}

dx3d::Logger& dx3d::Game::getLogger() noexcept
{
	return *m_loggerPtr;
}

dx3d::InputSystem& dx3d::Game::getInputSystem() noexcept
{
	return *m_inputSystemPtr;
}

void dx3d::Game::onInternalUpdate()
{
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<f32> delta = currentTime - m_previousTime;
	m_previousTime = currentTime;
	auto deltaTime = delta.count();

	m_inputSystemPtr->update();

	onUpdate(deltaTime);
	m_world->update(deltaTime);

	m_worldRenderer->render(*m_world, m_display->getSwapChain(), deltaTime);
}