#include "Player.h"

Player::Player(const dx3d::GameObjectDesc& desc) : dx3d::GameObject(desc)
{}

Player::~Player()
{}

void Player::onCreate()
{
	dx3d::GameObject::onCreate();

	// 1. Fetch or create your engine's camera component
	auto* camera = createOrGetComponent<dx3d::CameraComponent>();

	if (camera)
	{
		// 2. Map the viewport to aspect bounds (Width = 4.0, Height = 3.0 matching 1024x768 ratio)
		// and switch the projection engine mode over to Orthographic flat space.
		camera->setViewportSize(dx3d::Rect{ 0, 0, 4, 3 });
		camera->setNearPlane(0.01f);
		camera->setFarPlane(1000.0f);
		camera->setProjectionMode(dx3d::ProjectionMode::Perspective);
	}

	// 3. Lock camera position perfectly flat looking at the 2D plane center {0, 0}
	// W-clipping boundaries are safe here; Z at -5.0f works wonderfully.
	getTransform().setPosition({ 0.0f, 0.0f, -5.0f });
	getTransform().setRotation({ 0.0f, 0.0f, 0.0f });
}

void Player::onUpdate(dx3d::f32 deltaTime)
{
	auto& input = getInputSystem();

	auto sensitivity = 0.001f;
	auto rot = getTransform().getRotation();
	rot.x += getInputSystem().getMouseDelta().y * sensitivity;
	rot.y += getInputSystem().getMouseDelta().x * sensitivity;
	if (rot.x > 1.57f) rot.x = 1.57f;
	else if (rot.x < -1.57f) rot.x = -1.57f;
	getTransform().setRotation(rot);


	auto pos = getTransform().getPosition();
	auto forward = 0.0f;
	auto right = 0.0f;
	auto speed = 3.0f;
	if (getInputSystem().isKeyDown(dx3d::KeyCode::W)) forward = 1.0f;
	if (getInputSystem().isKeyDown(dx3d::KeyCode::S)) forward = -1.0f;
	if (getInputSystem().isKeyDown(dx3d::KeyCode::D)) right = 1.0f;
	if (getInputSystem().isKeyDown(dx3d::KeyCode::A)) right = -1.0f;
	auto forwardDir = getTransform().forward() * forward;
	auto rightDir = getTransform().right() * right;
	auto direction = dx3d::Vec3::normalize(forwardDir + rightDir);
	pos = pos + direction * speed * deltaTime;
	getTransform().setPosition(pos);
}