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

<<<<<<< Updated upstream
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
=======
	auto plane = world.createGameObject<dx3d::GameObject>();
	plane->createOrGetComponent<dx3d::CubeComponent>();
	plane->getTransform().setScale({ 1024.0f, 0.1f, 1024.8f }); //6.8f, 0.1f, 6.8f
	plane->getTransform().setPosition({ 0, -350.0f, 0 });

	auto cube = world.createGameObject<dx3d::GameObject>();
	cube->createOrGetComponent<dx3d::CubeComponent>();
	cube->getTransform().setScale({ 100.0f, 100.0f, 100.0f });
	cube->getTransform().setPosition({ 0, -250.0f, 0 });

	auto player = world.createGameObject<Player>();
	player->getTransform().setPosition({ 0.0f, 0.0f, -6.0f });
>>>>>>> Stashed changes
}

void MainGame::onUpdate(dx3d::f32 deltaTime)
{
	Game::onUpdate(deltaTime);
<<<<<<< Updated upstream
=======
	auto& input = getInputSystem();
	
	m_commandManager.processQueue();
>>>>>>> Stashed changes

	if (!m_isInitialized)
	{
		auto& device = getGraphicsDevice();

<<<<<<< Updated upstream
		m_cubeMesh = dx3d::MeshGenerator::createCube(device);
		m_sphereMesh = dx3d::MeshGenerator::createSphere(device, 32, 16, 0.5f);

=======
>>>>>>> Stashed changes
		auto& world = getWorld();
		dx3d::ui32 outCount = 0;

		auto cubes = world.getComponents<dx3d::CubeComponent>(outCount);
		for (dx3d::ui32 i = 0; i < outCount; ++i) {
			cubes[i]->setMesh(&m_cubeMesh);
		}

<<<<<<< Updated upstream
		auto spheres = world.getComponents<dx3d::SphereComponent>(outCount);
		for (dx3d::ui32 i = 0; i < outCount; ++i) {
			spheres[i]->setMesh(&m_sphereMesh);
		}

		m_isInitialized = true;
	}
=======
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

		pos.x += vel.x * deltaTime;
		pos.y += vel.y * deltaTime;

		//Orthographic Screen Bouncing Logic
		float padding = 100.0f; //Match the radius of the sphere from createSphere so that it bounces accurately at the edges of the screen

		if (pos.x - padding <= -512.0f) { pos.x = -512.0f + padding; vel.x *= -1.0f; }
		if (pos.x + padding >= 512.0f) { pos.x = 512.0f - padding;  vel.x *= -1.0f; }
		if (pos.y - padding <= -384.0f) { pos.y = -384.0f + padding; vel.y *= -1.0f; }
		if (pos.y + padding >= 384.0f) { pos.y = 384.0f - padding;  vel.y *= -1.0f; }

		bouncer->setVelocity(vel);
		transform.setPosition(pos);
	}

	if (input.isKeyPressed(dx3d::KeyCode::Escape))
	{
		PostQuitMessage(0);
		return;
	}

	if (input.isKeyPressed(dx3d::KeyCode::Space)) //Spawn a sphere whenever space is pressed and add it to the registry for tracking
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

	//When delete is pressed clear all spheres from the world and the registry but can be undone with the undo command
	if (input.isKeyPressed(dx3d::KeyCode::Delete))
	{
		if (!m_spawnedSpheres.empty())
		{
			m_commandManager.enqueueCommand(std::make_shared<dx3d::ClearAllSpheresCommand>(m_spawnedSpheres, m_commandManager.getUndoStack()));
		}
	}
>>>>>>> Stashed changes
}