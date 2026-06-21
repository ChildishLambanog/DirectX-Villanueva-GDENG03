#include "MainGame.h"
#include "Objects/Player.h"
#include <DX3D/Component/MeshGenerator.h>
#include <DX3D/Component/CubeComponent.h>
#include <DX3D/Component/SphereComponent.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Math/MathUtils.h>
#include <cmath>
#include <vector>

MainGame::MainGame(const dx3d::GameDesc& desc) : dx3d::Game(desc)
{
}

void MainGame::onCreate()
{
	Game::onCreate();

	auto& world = getWorld();

	auto floor = world.createGameObject<dx3d::GameObject>();
	floor->createOrGetComponent<dx3d::CubeComponent>();
	floor->getTransform().setScale({ 6.8f, 0.1f, 6.8f });
	floor->getTransform().setPosition({ 0, -1, 0 });


	srand((unsigned int)time(NULL));

	for (auto y = -2; y < 3; y++)
	{
		for (auto x = -2; x < 3; x++)
		{
			auto obj = world.createGameObject<dx3d::GameObject>();
			auto height = ((rand() % 120) + 80.0f) / 100.0f;
			auto width = ((rand() % 600) + 200.0f) / 1000.0f;

			obj->getTransform().setScale({ width, height, width });
			obj->getTransform().setPosition({ x * 1.4f, (height / 2.0f) - 1.0f, y * 1.4f });

			// Assign the layout structural tags
			if (rand() % 2 == 0)
			{
				obj->createOrGetComponent<dx3d::CubeComponent>();
			}
			else
			{
				obj->createOrGetComponent<dx3d::SphereComponent>();
			}
		}
	}

	auto player = world.createGameObject<Player>();
	player->getTransform().setPosition({ 0, 1, -2 });

	getInputSystem().setCursorLocked(true);
	getInputSystem().setCursorVisible(false);
}

void MainGame::onUpdate(dx3d::f32 deltaTime)
{
	Game::onUpdate(deltaTime);

	if (!m_isInitialized)
	{
		auto& device = getGraphicsDevice();

		m_cubeMesh = dx3d::MeshGenerator::createCube(device);
		m_sphereMesh = dx3d::MeshGenerator::createSphere(device, 32, 16, 0.5f);

		auto& world = getWorld();
		dx3d::ui32 outCount = 0;

		auto cubes = world.getComponents<dx3d::CubeComponent>(outCount);
		for (dx3d::ui32 i = 0; i < outCount; ++i) {
			cubes[i]->setMesh(&m_cubeMesh);
		}

		auto spheres = world.getComponents<dx3d::SphereComponent>(outCount);
		for (dx3d::ui32 i = 0; i < outCount; ++i) {
			spheres[i]->setMesh(&m_sphereMesh);
		}

		m_isInitialized = true;
	}
}