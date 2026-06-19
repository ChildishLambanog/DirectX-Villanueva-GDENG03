#include <DX3D/Game/Game.h>
#include <DX3D/Window/Window.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Core/Logger.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Game/World.h>
#include <DX3D/Game/GameObject.h>

dx3d::Game::Game(const GameDesc& desc)
{
	m_loggerPtr = std::make_unique<Logger>(desc.logLevel);

	DX3DLogInfo("Rafael Ira R. Villanueva DirectX 3D Engine GDENG03");
	DX3DLogInfo("--------------------------------------------------");

	m_graphicsEngine = std::make_unique<GraphicsEngine>(GraphicsEngineDesc{ *m_loggerPtr });
	m_display = std::make_unique<Display>(DisplayDesc{ {*m_loggerPtr, desc.windowSize}, m_graphicsEngine->getGraphicsDevice() });
	m_world = std::make_unique<World>(WorldDesc{ {*m_loggerPtr} });

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

void dx3d::Game::onInternalUpdate()
{
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<f32> delta = currentTime - m_previousTime;
	m_previousTime = currentTime;
	auto deltaTime = delta.count();

	onUpdate(deltaTime);
	m_world->update(deltaTime);

	m_graphicsEngine->render(m_display->getSwapChain(), deltaTime);
}