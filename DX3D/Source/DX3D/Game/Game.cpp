#include <DX3D/Game/Game.h>
#include <DX3D/Window/Window.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Core/Logger.h>
#include <DX3D/Input/InputSystem.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Game/World.h>
#include <DX3D/Game/GameObject.h>
#include <DX3D/Game/WorldRenderer.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <thread>

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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; //Enable keyboard controls

	ImGui::StyleColorsDark();

	HWND hwnd = static_cast<HWND>(m_display->getHandle());
	ID3D11Device* device = m_graphicsDevice->getD3DDevice();
	ID3D11DeviceContext* immediateCtx = m_graphicsDevice->getImmediateDeviceContext();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, immediateCtx);

	DX3DLogInfo("Game is initialized successfully.");
}

dx3d::Game::~Game()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

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

dx3d::GraphicsDevice& dx3d::Game::getGraphicsDevice() noexcept
{
	return *m_graphicsDevice;
}

void dx3d::Game::onInternalUpdate()
{
	constexpr f32 targetFrameTime = 1.0f / 60.0f; //Target is 60 FPS

	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<f32> delta = currentTime - m_previousTime;

	if (delta.count() < targetFrameTime)
	{
		f32 sleepTimeNeeded = targetFrameTime - delta.count(); //Calculate how much time we need to sleep to maintain a consistent frame rate

		std::this_thread::sleep_for(std::chrono::duration<f32>(sleepTimeNeeded)); //Sleep for the remaining time in order to maintain a consistent frame rate

		//Recalculate both currentTime and delta after waking up from sleep to get accurate delta time for the frame
		currentTime = std::chrono::steady_clock::now();
		delta = currentTime - m_previousTime;
	}

	m_previousTime = currentTime;
	auto deltaTime = delta.count();

	m_inputSystemPtr->update();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	onUpdate(deltaTime);
	m_world->update(deltaTime);

	m_worldRenderer->render(*m_world, m_display->getSwapChain(), deltaTime);
}