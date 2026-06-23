#include "MainGame.h"
#include "Objects/Player.h"
#include <DX3D/Component/MeshGenerator.h>
#include <DX3D/Component/CubeComponent.h>
#include <DX3D/Component/SphereComponent.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Math/MathUtils.h>
#include <DX3D/Input/GameCommands.h>
#include <DX3D/Component/BouncingSphere.h>
#include <cmath>
#include <vector>

MainGame::MainGame(const dx3d::GameDesc& desc) : dx3d::Game(desc)
{
}

void MainGame::onCreate()
{
	Game::onCreate();

	auto& world = getWorld();

	auto player = world.createGameObject<Player>();
	player->getTransform().setPosition({ 0.0f, 0.0f, -6.0f });

	//getInputSystem().setCursorLocked(true);
	//getInputSystem().setCursorVisible(false);
}

void MainGame::onUpdate(dx3d::f32 deltaTime)
{
	Game::onUpdate(deltaTime);
	auto& input = getInputSystem();
	
	m_commandManager.processQueue();

	if (!m_isInitialized)
	{
		auto& device = getGraphicsDevice();
		//m_cubeMesh = dx3d::MeshGenerator::createCube(device); //Call to initialize a cube from Mesh Generator
		m_sphereMesh = dx3d::MeshGenerator::createSphere(device, 32, 16, 100.0f); //Call to initialize a sphere from Mesh Generator

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
}