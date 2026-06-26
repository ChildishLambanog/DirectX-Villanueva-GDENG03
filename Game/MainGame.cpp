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

	//auto plane = world.createGameObject<dx3d::GameObject>();
	//plane->createOrGetComponent<dx3d::CubeComponent>();
	//plane->getTransform().setScale({ 1024.0f, 0.1f, 1024.8f }); //6.8f, 0.1f, 6.8f
	//plane->getTransform().setPosition({ 0, -350.0f, 0 });

	//Test Case 1 Render a Cube with a rainbow pixel shader on it.
	//auto cube = world.createGameObject<dx3d::GameObject>();
	//cube->createOrGetComponent<dx3d::CubeComponent>();
	//cube->getTransform().setScale({ 100.0f, 100.0f, 100.0f });
	//cube->getTransform().setPosition({ 0, -350.0f, 0 });

	//Test Case 2 Render a Cube with a white shader on it.
	//m_animatedCube = world.createGameObject<dx3d::GameObject>();
	//m_animatedCube->createOrGetComponent<dx3d::CubeComponent>();
	//m_animatedCube->getTransform().setScale({ 100.0f, 100.0f, 100.0f });
	//m_animatedCube->getTransform().setPosition({ 0, -350.0f, 0 });

	//Test Case 3 Render a Cube with a rainbow pixel shader on it and animation along X and Y axis then scale from 1.0f to 0.25 using linear interpolation.
	//m_animatedCube = world.createGameObject<dx3d::GameObject>();
	//m_animatedCube->createOrGetComponent<dx3d::CubeComponent>();
	//m_animatedCube->getTransform().setScale({ 1.0f, 1.0f, 1.0f });
	//m_animatedCube->getTransform().setPosition({ 0, -100.0f, 0 });

	//Test Case 4 render 50 cubes placed randomly in the world
	//const int numCubes = 50;
	//m_randomCubes.reserve(numCubes);

	//for (int i = 0; i < numCubes; ++i)
	//{
	//	CubeInstance instance;

	//	instance.gameObject = world.createGameObject<dx3d::GameObject>();
	//	instance.gameObject->createOrGetComponent<dx3d::CubeComponent>();

	//	//Assign random positions 
	//	float randomX = static_cast<float>(rand() % 800 - 400);
	//	float randomY = static_cast<float>(rand() % 600 - 300);
	//	float randomZ = static_cast<float>(rand() % 400 - 200);
	//	instance.gameObject->getTransform().setPosition({ randomX, randomY, randomZ });

	//	instance.gameObject->getTransform().setScale({ 70.0f, 70.0f, 70.0f });

	//	instance.rotationSpeed.x = 0.7f;
	//	instance.rotationSpeed.y = 0.7f;
	//	instance.rotationSpeed.z = 0.7f;

	//	m_randomCubes.push_back(instance); //Store the instance to the vector to be used for the animation in the onUpdate function.
	//}

	//Test Case 5 render a uniform scaled cube that performs a warping animation and transform into a flat horizontal plane
	//m_animatedCube = world.createGameObject<dx3d::GameObject>();
	//m_animatedCube->createOrGetComponent<dx3d::CubeComponent>();
	//m_animatedCube->getTransform().setScale({ 100.0f, 100.0f, 100.0f });
	//m_animatedCube->getTransform().setPosition({ 0.0f, 0.0f, 0.0f });

	//Test Case 6 Render a plane and 3 cubes then recreate the view given for test case 6 
	//auto plane = world.createGameObject<dx3d::GameObject>();
	//plane->createOrGetComponent<dx3d::CubeComponent>();
	//plane->getTransform().setScale({ 10.0f, 0.1f, 10.0f }); //6.8f, 0.1f, 6.8f
	//plane->getTransform().setPosition({ 0, 0, 0 });

	//auto cube = world.createGameObject<dx3d::GameObject>();
	//cube->createOrGetComponent<dx3d::CubeComponent>();
	//cube->getTransform().setScale({ 1.0f, 1.0f, 1.0f });
	//cube->getTransform().setPosition({ 0.0f, 0.9f, 0.0f });

	//auto cube2 = world.createGameObject<dx3d::GameObject>();
	//cube2->createOrGetComponent<dx3d::CubeComponent>();
	//cube2->getTransform().setScale({ 1.0f, 1.0f, 1.0f });
	//cube2->getTransform().setPosition({ -1.5f, 2.0f, 0.0f });

	//auto cube3 = world.createGameObject<dx3d::GameObject>();
	//cube3->createOrGetComponent<dx3d::CubeComponent>();
	//cube3->getTransform().setScale({ 1.0f, 1.0f, 1.0f });
	//cube3->getTransform().setPosition({ -1.5f, 3.0f, -2.0f });


	//Test Case 7 Render a house of cards setup
	const dx3d::Vec3 cardScale = { 50.0f, 70.0f, 1.5f };
	const dx3d::Vec3 floorScale = { 70.0f, 50.0f, 1.5f };

	const dx3d::Vec3 leanRight = { 0.2618f, 1.5708f, 0.0f };
	const dx3d::Vec3 leanLeft = { -0.2618f, 1.5708f, 0.0f };
	const dx3d::Vec3 flat = { 1.5708f, 0.0f, 0.0f };

	auto spawnPokerCard = [&](dx3d::Vec3 position, dx3d::Vec3 rotation, dx3d::Vec3 scale) 
	{
		auto cardObj = world.createGameObject<dx3d::GameObject>();
		cardObj->createOrGetComponent<dx3d::CubeComponent>();
		cardObj->getTransform().setScale(scale);
		cardObj->getTransform().setPosition(position);
		cardObj->getTransform().setRotation(rotation);
	};

	//Layer 1
	spawnPokerCard({ -69.0f, -150.0f, 0.0f }, leanRight, cardScale); //Left Leg
	spawnPokerCard({ -51.0f, -150.0f, 0.0f }, leanLeft, cardScale);  //Right Leg

	spawnPokerCard({ -9.0f, -150.0f, 0.0f }, leanRight, cardScale);  //Left Leg
	spawnPokerCard({ 9.0f, -150.0f, 0.0f }, leanLeft, cardScale);	//Right Leg

	spawnPokerCard({ 51.0f, -150.0f, 0.0f }, leanRight, cardScale);  //Left Leg
	spawnPokerCard({ 69.0f, -150.0f, 0.0f }, leanLeft, cardScale);   //Right Leg

	spawnPokerCard({ -30.0f, -115.4f, 0.0f }, flat, floorScale);    //Left Floor
	spawnPokerCard({ 30.0f, -115.4f, 0.0f }, flat, floorScale);     //Right Floor

	//Layer 2
	spawnPokerCard({ -39.0f, -82.4f, 0.0f }, leanRight, cardScale);  //Left Leg
	spawnPokerCard({ -21.0f, -82.4f, 0.0f }, leanLeft, cardScale);   //Right Leg

	spawnPokerCard({ 21.0f, -82.4f, 0.0f }, leanRight, cardScale);  //Left Leg
	spawnPokerCard({ 39.0f, -82.4f, 0.0f }, leanLeft, cardScale);   //Right Leg

	spawnPokerCard({ 0.0f, -48.4f, 0.0f }, flat, floorScale);	   //Center Floor

	//Layer 3 Apex
	spawnPokerCard({ -9.0f, -17.4f, 0.0f }, leanRight, cardScale); //Left Leg
	spawnPokerCard({ 9.0f, -17.4f, 0.0f }, leanLeft, cardScale);   //Right Leg

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

	//Test Case 2 Animating the cube to rotate along the X, Y, and Z axis continuously.
	//if (m_animatedCube)
	//{
	//	auto& transform = m_animatedCube->getTransform();
	//	dx3d::Vec3 rotation = transform.getRotation();

	//	float rotationSpeedX = 0.7f;
	//	float rotationSpeedY = 0.7f;
	//	float rotationSpeedZ = 0.7f;

	//	rotation.x += rotationSpeedX * deltaTime;
	//	rotation.y += rotationSpeedY * deltaTime;
	//	rotation.z += rotationSpeedZ * deltaTime;

	//	//Apply the calculated rotation to the cube
	//	transform.setRotation(rotation);
	//}

	//Test Case 3 animating the cube to move along the X and Y axis and scale from 1.0f to 0.25f using linear interpolation.
	//if (m_animatedCube)
	//{
	//	auto& transform = m_animatedCube->getTransform();

	//	m_animationTime += deltaTime; //Store the total elapsed time for the animation

	//	float movementSpeed = 0.3f; //speed of the movement along the X and Y axis
	//	float waveX = std::sin(m_animationTime * movementSpeed);
	//	float waveY = std::cos(m_animationTime * movementSpeed); //used cosine for the Y axis so that the cube moves in a circular/oval pattern.

	//	float movementRangeX = 20.0f; //range of movement of the cube along the X axis
	//	float movementRangeY = 20.0f; //range of movement of the cube along the Y axis

	//	dx3d::Vec3 currentPos = transform.getPosition();
	//	currentPos.x = 0.0f + (waveX * movementRangeX);
	//	currentPos.y = -350.0f + (waveY * movementRangeY);

	//	transform.setPosition(currentPos); //Apply the calculated position to the cube

	//	float waveSpeed = 1.5f; //speed of the sine wave oscillation for scaling the cube
	//	float sineWave = std::sin(m_animationTime * waveSpeed); //calculate the sine wave value based on the m_animationTime and waveSpeed

	//	float t = (sineWave + 1.0f) * 0.5f; //Normalize the sine wave value to a range between 0 and 1 for interpolation

	//	float startScale = 1.0f; //Starting scale of the cube
	//	float endScale = 0.25f;  //Ending scale of the cube

	//	//Lerp equation -> Start + t * (End - Start)
	//	//Use the calculated t and the start and end scale values to calculate the current scale value for the cube using the Lerp equation
	//	float currentScaleVal = startScale + t * (endScale - startScale);

	//	transform.setScale({ currentScaleVal, currentScaleVal, currentScaleVal }); //Apply the calculated scale value to the cube
	//}

	if (!m_isInitialized)
	{
		auto& device = getGraphicsDevice();
		m_cubeMesh = dx3d::MeshGenerator::createCube(device); //Call to initialize a cube from Mesh Generator
		m_sphereMesh = dx3d::MeshGenerator::createSphere(device, 32, 16, 100.0f); //Call to initialize a sphere from Mesh Generator

		//if (m_animatedCube) //Test Case 5 assigning the generated cube mesh to the animated cube for the warping animation
		//{
		//	if (auto* component = m_animatedCube->getComponent<dx3d::CubeComponent>())
		//	{
		//		component->setMesh(&m_cubeMesh);
		//	}
		//}

		auto& world = getWorld();
		dx3d::ui32 outCount = 0;

		auto cubes = world.getComponents<dx3d::CubeComponent>(outCount);
		for (dx3d::ui32 i = 0; i < outCount; ++i) 
		{
			cubes[i]->setMesh(&m_cubeMesh);
		}

		//for (auto& cubeInstance : m_randomCubes) //Test Case 4 assigning the generated cube mesh to each of the 50 randomly generated cubes.
		//{
		//	if (auto* component = cubeInstance.gameObject->getComponent<dx3d::CubeComponent>())
		//	{
		//		component->setMesh(&m_cubeMesh);
		//	}
		//}

		m_isInitialized = true;
	}

	//for (auto& cube : m_randomCubes) //Test Case 4 animating the 50 random cubes to rotate along X, Y, and Z axis continuously.
	//{
	//	if (!cube.gameObject) continue;

	//	auto& transform = cube.gameObject->getTransform();

	//	//Update the cube's rotation based on the rotation speed and the elapsed time
	//	cube.rotation.x += cube.rotationSpeed.x * deltaTime;
	//	cube.rotation.y += cube.rotationSpeed.y * deltaTime;
	//	cube.rotation.z += cube.rotationSpeed.z * deltaTime;

	//	transform.setRotation(cube.rotation); //Apply the calculated rotation to the cube
	//}

	//if (m_animatedCube) //Test Case 5 animating the cube to warp into a horizontal plane and back into a cube
	//{
	//	auto& transform = m_animatedCube->getTransform();

	//	float warpSpeed = 0.4f; //speed of the warping animation
	//	if (m_isFlat) //if cube is flat make it warp back into a cube shape
	//	{
	//		m_animationTime += deltaTime * warpSpeed;
	//		if (m_animationTime >= 1.0f) 
	//		{ 
	//			m_animationTime = 1.0f; 
	//			m_isFlat = false; 
	//		} 
	//	}
	//	else //if cube is not flat make it warp into a flat horizonal plane
	//	{
	//		m_animationTime -= deltaTime * warpSpeed;
	//		if (m_animationTime <= 0.0f)
	//		{ 
	//			m_animationTime = 0.0f; 
	//			m_isFlat = true; 
	//		} 
	//	}

	//	//Use the smoothstep equation for t to create a smooth transiton between the flat and cube state.
	//	float t = m_animationTime * m_animationTime * (3.0f - 2.0f * m_animationTime);

	//	dx3d::Vec3 startScale = { 100.0f, 100.0f, 100.0f }; //let the cube start at a uniform scale of 100.0f for all axes
	//	dx3d::Vec3 startPos = { 0.0f, 0.0f, 0.0f }; //let the cube start at the center of the scene
	//	dx3d::Vec3 startRot = { 0.0f, 0.0f, 0.0f };

	//	dx3d::Vec3 endScale = { 600.0f, 600.0f, 2.0f }; //flatten the Z scale to make the cube look like a horizontal plane
	//	dx3d::Vec3 endPos = { 0.0f, 0.0f, 49.0f }; //offset the Z position downwards to simulate a floor plane when flattened
	//	dx3d::Vec3 endRot = { 1.5708f, 0.0f, 0.0f }; //1.5708 radians is 90 degrees

	//	//Calculate the scale and position usin Lerp equation
	//	dx3d::Vec3 currentScale;
	//	currentScale.x = startScale.x + t * (endScale.x - startScale.x);
	//	currentScale.y = startScale.y + t * (endScale.y - startScale.y);
	//	currentScale.z = startScale.z + t * (endScale.z - startScale.z);

	//	dx3d::Vec3 currentPos;
	//	currentPos.x = startPos.x + t * (endPos.x - startPos.x);
	//	currentPos.y = startPos.y + t * (endPos.y - startPos.y);
	//	currentPos.z = startPos.z + t * (endPos.z - startPos.z);

	//	dx3d::Vec3 currentRot;
	//	currentRot.x = startRot.x + t * (endRot.x - startRot.x); //Tilts to make the cube look like a horizontal plane when flattened
	//	currentRot.y = startRot.y + t * (endRot.y - startRot.y);
	//	currentRot.z = startRot.z + t * (endRot.z - startRot.z);

	//	transform.setScale(currentScale); //Apply calculated scale to the cube
	//	transform.setPosition(currentPos); //Apply calculated position to the cube
	//	transform.setRotation(currentRot); //Apply calculated rotation to the cube
	//}

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