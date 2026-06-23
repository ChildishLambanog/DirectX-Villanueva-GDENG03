#include "MainGame.h"
#include "Objects/Player.h"
#include <DX3D/Component/MeshGenerator.h>
#include <DX3D/Component/CubeComponent.h>
#include <DX3D/Component/SphereComponent.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Math/MathUtils.h>
#include <DX3D/Input/GameCommands.h>
#include <DX3D/Component/BouncingSphere.h>
#include <DX3D/Input/InputListener.h>
#include <cmath>
#include <vector>

MainGame::MainGame(const dx3d::GameDesc& desc) : dx3d::Game(desc)
{
}

void MainGame::onCreate()
{
	Game::onCreate();

	auto& world = getWorld();

	//auto floor = world.createGameObject<dx3d::GameObject>();
	//floor->createOrGetComponent<dx3d::CubeComponent>();
	//floor->getTransform().setScale({ 6.8f, 0.1f, 6.8f });
	//floor->getTransform().setPosition({ 0, -1, 0 });


	//srand((unsigned int)time(NULL));

	//for (auto y = -2; y < 3; y++)
	//{
	//	for (auto x = -2; x < 3; x++)
	//	{
	//		auto obj = world.createGameObject<dx3d::GameObject>();
	//		auto height = ((rand() % 120) + 80.0f) / 100.0f;
	//		auto width = ((rand() % 600) + 200.0f) / 1000.0f;

	//		obj->getTransform().setScale({ width, height, width });
	//		obj->getTransform().setPosition({ x * 1.4f, (height / 2.0f) - 1.0f, y * 1.4f });

	//		//Assign the layout structural tags
	//		if (rand() % 2 == 0)
	//		{
	//			obj->createOrGetComponent<dx3d::CubeComponent>();
	//		}
	//		else
	//		{
	//			obj->createOrGetComponent<dx3d::SphereComponent>();
	//		}
	//	}
	//}

	auto player = world.createGameObject<Player>();
	player->getTransform().setPosition({ 0.0f, 0.0f, -6.0f });

	getInputSystem().setCursorLocked(true);
	getInputSystem().setCursorVisible(false);
}

void MainGame::onUpdate(dx3d::f32 deltaTime)
{
	Game::onUpdate(deltaTime);
	auto& input = getInputSystem();
	
	m_commandManager.processQueue();

	if (!m_isInitialized)
	{
		auto& device = getGraphicsDevice();
		m_cubeMesh = dx3d::MeshGenerator::createCube(device);
		m_sphereMesh = dx3d::MeshGenerator::createSphere(device, 32, 16, 0.5f);

		m_isInitialized = true;
	}

	for (size_t i = 0; i < m_spawnedSpheres.size(); ++i)
	{
		auto* sphereObject = m_spawnedSpheres[i];
		if (!sphereObject) continue;

		auto* bouncer = sphereObject->getComponent<dx3d::BouncingSphere>();
		if (!bouncer)
		{
			m_spawnedSpheres.erase(m_spawnedSpheres.begin() + i);
			--i;
			continue;
		}

		auto& transform = sphereObject->getTransform();
		dx3d::Vec3 pos = transform.getPosition();
		dx3d::Vec3 vel = bouncer->getVelocity();

		// Accumulate delta movement over time
		pos.x += vel.x * deltaTime;
		pos.y += vel.y * deltaTime;

		// Orthographic Screen Bouncing (4.0 x 3.0 window box)
		float padding = 0.15f;

		if (pos.x - padding <= -2.0f) { pos.x = -2.0f + padding; vel.x *= -1.0f; }
		if (pos.x + padding >= 2.0f) { pos.x = 2.0f - padding;  vel.x *= -1.0f; }
		if (pos.y - padding <= -1.5f) { pos.y = -1.5f + padding; vel.y *= -1.0f; }
		if (pos.y + padding >= 1.5f) { pos.y = 1.5f - padding;  vel.y *= -1.0f; }

		bouncer->setVelocity(vel);
		transform.setPosition(pos);
	}

	if (input.isKeyPressed(dx3d::KeyCode::Escape))
	{
		PostQuitMessage(0);
		return;
	}

	// REQUIREMENT 1: Spacebar spawns an tracking sphere
	if (input.isKeyPressed(dx3d::KeyCode::Space))
	{
		m_commandManager.enqueueCommand(std::make_shared<dx3d::SpawnSphereCommand>(getWorld(), &m_sphereMesh, m_spawnedSpheres));
	}

	if (input.isKeyPressed(dx3d::KeyCode::Backspace)) //Undo the last command whenever backspace is pressed
	{
		m_commandManager.undo();
	}

	if (input.isKeyPressed(dx3d::KeyCode::R)) //Redo the last undone command whenever R is pressed
	{
		m_commandManager.redo();
	}

	// REQUIREMENT 3: Delete drops everything
	if (input.isKeyPressed(dx3d::KeyCode::Delete))
	{
		if (!m_spawnedSpheres.empty())
		{
			m_commandManager.enqueueCommand(std::make_shared<dx3d::ClearAllSpheresCommand>(m_spawnedSpheres, m_commandManager.getUndoStack()));
		}
	}
}