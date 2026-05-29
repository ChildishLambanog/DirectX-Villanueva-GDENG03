#include <DX3D/Game/Game.h>
#include <DX3D/Window/Window.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Core/Logger.h>
#include <DX3D/Game/Display.h>

dx3d::Game::Game(const GameDesc& desc) : Base({*std::make_unique<Logger>(desc.logLevel).release()}), m_loggerPtr(&m_logger)
{
	m_graphicsEngine = std::make_unique<GraphicsEngine>(GraphicsEngineDesc{m_logger}); //Create an instance of the GraphicsEngine class in the constructor of the Game class.
	m_display = std::make_unique<Display>(DisplayDesc{ {m_logger, desc.windowSize}, m_graphicsEngine->getGraphicsDevice()}); //Create an instance of the Window class in the constructor of the Game class.

	DX3DLogInfo("Game is initialized successfully.");
}

dx3d::Game::~Game()
{
	DX3DLogInfo("Game memory deallocation started. Will shutdown");
}

void dx3d::Game::onInternalUpdate()
{
	m_graphicsEngine->render(m_display->getSwapChain());
}