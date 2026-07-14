#include "Player.h"

Player::Player(const dx3d::GameObjectDesc& desc) : dx3d::GameObject(desc)
{}

Player::~Player()
{}

void Player::onCreate()
{
	dx3d::GameObject::onCreate();

	auto* camera = createOrGetComponent<dx3d::CameraComponent>();
	if (camera)
	{
		camera->setViewportSize(dx3d::Rect{ 0, 0, 1024, 728 });
		camera->setNearPlane(0.01f);
		camera->setFarPlane(4000.0f);

		camera->setProjectionMode(dx3d::ProjectionMode::Orthographic);
		getTransform().setPosition({ 0.0f, 0.0f, -2000.0f });
		getTransform().setRotation({ 0.0f, 0.0f, 0.0f });
		getInputSystem().setCursorLocked(false);
		getInputSystem().setCursorVisible(true);
	}
}

void Player::onUpdate(dx3d::f32 deltaTime)
{
	auto& input = getInputSystem();
	auto* m_camera = getComponent<dx3d::CameraComponent>();

	if (!m_camera) return;

	//Toggle for Perspective Mode
	if (input.isKeyDown(dx3d::KeyCode::P))
	{
		if (m_camera->getProjectionMode() != dx3d::ProjectionMode::Perspective)
		{
			m_camera->setProjectionMode(dx3d::ProjectionMode::Perspective);

			//Positioned the camera so its similar to the reference view for Test Case 7
			getTransform().setPosition({ 102.297028f, 0.067601f, -138.479294f });
			getTransform().setRotation({ 0.445000f, -0.609000f, 0.000000f });

			input.setCursorLocked(true);
			input.setCursorVisible(false);
		}
	}
	//Toggle for Orthographic Mode
	else if (input.isKeyDown(dx3d::KeyCode::O))
	{
		if (m_camera->getProjectionMode() != dx3d::ProjectionMode::Orthographic)
		{
			m_camera->setProjectionMode(dx3d::ProjectionMode::Orthographic);

			// Safely move back to original distance and unlock cursor for ortho viewing
			getTransform().setPosition({ 0.0f, 0.0f, -4000.0f });
			getTransform().setRotation({ 0.0f, 0.0f, 0.0f });
			input.setCursorLocked(false);
			input.setCursorVisible(true);
		}
	}

	//Controls for Perspective Mode
	if (m_camera->getProjectionMode() == dx3d::ProjectionMode::Perspective)
	{
		auto sensitivity = 0.001f;
		auto rot = getTransform().getRotation();
		rot.x += input.getMouseDelta().y * sensitivity;
		rot.y += input.getMouseDelta().x * sensitivity;

		if (rot.x > 1.57f) rot.x = 1.57f;
		else if (rot.x < -1.57f) rot.x = -1.57f;
		getTransform().setRotation(rot);

		auto pos = getTransform().getPosition();
		auto forward = 0.0f;
		auto right = 0.0f;
		auto speed = 200.0f; //Adjust speed as needed for player movement

		if (input.isKeyDown(dx3d::KeyCode::W)) forward = 1.0f;
		if (input.isKeyDown(dx3d::KeyCode::S)) forward = -1.0f;
		if (input.isKeyDown(dx3d::KeyCode::D)) right = 1.0f;
		if (input.isKeyDown(dx3d::KeyCode::A)) right = -1.0f;

		if (forward != 0.0f || right != 0.0f)
		{
			auto forwardDir = getTransform().forward() * forward;
			auto rightDir = getTransform().right() * right;
			auto direction = dx3d::Vec3::normalize(forwardDir + rightDir);
			pos = pos + direction * speed * deltaTime;
			getTransform().setPosition(pos);
		}
	}
	else if (m_camera->getProjectionMode() == dx3d::ProjectionMode::Orthographic)
	{
		return; //Ignore controls completely when running in Ortho setup
	}
}