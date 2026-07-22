#include "MainGame.h"
#include "Objects/Player.h"
#include <DX3D/Component/MeshGenerator.h>
#include <DX3D/Component/CubeComponent.h>
#include <DX3D/Component/SphereComponent.h>
#include <DX3D/Component/CameraComponent.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Math/MathUtils.h>
#include <DX3D/Input/GameCommands.h>
#include <DX3D/Component/BouncingSphere.h>
#include <imgui.h>
#include <cmath>
#include <vector>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool LoadTextureFromFile(const char* filename, ID3D11Device* d3dDevice, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	int image_width = 0;
	int image_height = 0;
	// Decompress file into raw RGBA data
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == nullptr)
		return false;

	// Set up D3D11 Texture description
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = nullptr;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;

	HRESULT hr = d3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);
	if (FAILED(hr))
	{
		stbi_image_free(image_data);
		return false;
	}

	// Create Texture View so shader pipelines can sample it
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	hr = d3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();
	stbi_image_free(image_data);

	if (FAILED(hr))
		return false;

	*out_width = image_width;
	*out_height = image_height;
	return true;
}

MainGame::MainGame(const dx3d::GameDesc& desc) : dx3d::Game(desc)
{
}

void MainGame::onCreate()
{
	Game::onCreate();

	auto& world = getWorld();

	auto plane = world.createGameObject<dx3d::GameObject>();
	plane->createOrGetComponent<dx3d::CubeComponent>();
	plane->getTransform().setScale({ 1024.0f, 0.1f, 1024.8f }); //6.8f, 0.1f, 6.8f
	plane->getTransform().setPosition({ 0, -350.0f, 0 });

	auto cube = world.createGameObject<dx3d::GameObject>();
	cube->createOrGetComponent<dx3d::CubeComponent>();
	cube->getTransform().setScale({ 100.0f, 100.0f, 100.0f });
	cube->getTransform().setPosition({ 0, -250.0f, 0 });

	auto customModelObj = world.createGameObject<dx3d::GameObject>();
	customModelObj->createOrGetComponent<dx3d::MeshComponent>();
	customModelObj->getTransform().setScale({ 10.0f, 10.0f, 10.0f });
	customModelObj->getTransform().setPosition({ 0.0f, 0.0f, 0.0f });
	
	auto player = world.createGameObject<Player>();
	player->getTransform().setPosition({ 0.0f, 0.0f, -6.0f });
}

void MainGame::onUpdate(dx3d::f32 deltaTime)
{
	Game::onUpdate(deltaTime);
	auto& input = getInputSystem();
	auto& world = getWorld();
	
	m_commandManager.processQueue();

	if (!m_isInitialized)
	{
		auto& device = getGraphicsDevice();
		m_cubeMesh = dx3d::MeshGenerator::createCube(device); //Call to initialize a cube from Mesh Generator
		m_sphereMesh = dx3d::MeshGenerator::createSphere(device, 32, 16, 100.0f); //Call to initialize a sphere from Mesh Generator
		//m_customModelMesh = dx3d::MeshGenerator::createFromOBJ(device, "DX3D/Assets/Models/teapot.obj", { 0.2f, 0.8f, 0.4f, 1.0f });
		m_customModelMesh = dx3d::MeshGenerator::createFromOBJ(device, "DX3D/Assets/Models/mosasaurus.obj", { 0.2f, 0.8f, 0.4f, 1.0f });

		dx3d::ui32 outCount = 0;
		auto cubes = world.getComponents<dx3d::CubeComponent>(outCount);
		for (dx3d::ui32 i = 0; i < outCount; ++i) 
		{
			cubes[i]->setMesh(&m_cubeMesh);
		}

		dx3d::ui32 meshCount = 0;
		auto meshComps = world.getComponents<dx3d::MeshComponent>(meshCount);
		for (dx3d::ui32 i = 0; i < meshCount; ++i)
		{
			meshComps[i]->setMesh(&m_customModelMesh);
		}

		bool loaded = LoadTextureFromFile("DX3D/Assets/Sprites/logo.png", device.getRawDevice(), &m_logoSRV, &m_logoWidth, &m_logoHeight);
		if (!loaded)
		{
			OutputDebugStringA("WARNING: Failed to load logo.png! Check path directory.\n");
		}

		m_isInitialized = true;
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit", "Esc"))
			{
				PostQuitMessage(0);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::BeginMenu("About"))
			{
				if (ImGui::MenuItem("Credits"))
				{
					m_showCreditsPanel = !m_showCreditsPanel;
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Color Picker", nullptr, &m_showColorPickerPanel))
			{
				// Toggles color picker panel visibility
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (m_showCreditsPanel)
	{
		ImGui::Begin("Credits", &m_showCreditsPanel);

		if (m_logoSRV != nullptr)
		{
			float displayWidth = 150.0f; 
			float aspectRatio = (float)m_logoWidth / (float)m_logoHeight;
			float displayHeight = displayWidth / aspectRatio;

			ImGui::Image((void*)m_logoSRV, ImVec2(displayWidth, displayHeight)); //Passing the shader resource view to ImGui for rendering
			ImGui::Spacing();
		}

		ImGui::TextWrapped("GDENG03 DirectX11 Engine "
			"Developed By Rafael Ira R. Villanueva");
		ImGui::Separator();
		ImGui::TextWrapped("\"Things you have now, things you've lost. People who're near by, "
			"people who've gone far away. No matter what you choose, truth is, both regret "
			"and reluctance are going to follow you around. You just have to make "
			"sure you don't make excuses to yourself down the road.\"");
		ImGui::Spacing();
		ImGui::Text("- Kentaro Miura, Berserk, Vol. 38");
		ImGui::Separator();
		ImGui::TextWrapped("Acknowledgments:");
		ImGui::TextWrapped("PardCode Game Engine Tutorial");
		ImGui::End();
	}

	if (m_showColorPickerPanel)
	{
		ImGui::Begin("Color Picker Settings", &m_showColorPickerPanel);

		ImGui::Text("Choose a Color:");
		ImGui::Spacing();

		// A: Standard Color Picker widget
		ImGui::ColorPicker4("##picker", m_pickedColor);

		ImGui::Spacing();
		//ImGui::Separator();
		//ImGui::Spacing();

		// B: Display Box Showing Current Color Selection
		//ImGui::Text("Selected Color Box Preview:");
		//ImGui::Spacing();

		// ColorButton draws a solid colored rectangle representing the exact color
		/*ImGui::ColorButton("##current_color_box", *(ImVec4*)&m_pickedColor, ImGuiColorEditFlags_None, ImVec2(120, 60));*/

		ImGui::End();
	}

	std::vector<dx3d::GameObject*> uniqueObjects;
	dx3d::ui32 count = 0;

	//Extract Cubes
	auto cubes = world.getComponents<dx3d::CubeComponent>(count);
	for (dx3d::ui32 i = 0; i < count; ++i) {
		auto* obj = &cubes[i]->getGameObject();
		if (std::find(uniqueObjects.begin(), uniqueObjects.end(), obj) == uniqueObjects.end())
			uniqueObjects.push_back(obj);
	}
	//Extract Spheres
	auto spheres = world.getComponents<dx3d::SphereComponent>(count);
	for (dx3d::ui32 i = 0; i < count; ++i) {
		auto* obj = &spheres[i]->getGameObject();
		if (std::find(uniqueObjects.begin(), uniqueObjects.end(), obj) == uniqueObjects.end())
			uniqueObjects.push_back(obj);
	}
	//Extract Players/Cameras
	auto cameras = world.getComponents<dx3d::CameraComponent>(count);
	for (dx3d::ui32 i = 0; i < count; ++i) {
		auto* obj = &cameras[i]->getGameObject();
		if (std::find(uniqueObjects.begin(), uniqueObjects.end(), obj) == uniqueObjects.end())
			uniqueObjects.push_back(obj);
	}

	auto customModels = world.getComponents<dx3d::MeshComponent>(count);
	for (dx3d::ui32 i = 0; i < count; ++i) {
		auto* obj = &customModels[i]->getGameObject();
		if (std::find(uniqueObjects.begin(), uniqueObjects.end(), obj) == uniqueObjects.end())
			uniqueObjects.push_back(obj);
	}

	//Safety Check: Reset selection pointer if object was deleted via hotkeys/commands
	if (m_selectedObject) {
		if (std::find(uniqueObjects.begin(), uniqueObjects.end(), m_selectedObject) == uniqueObjects.end()) {
			m_selectedObject = nullptr;
		}
	}

	//Draw the Scene Hierarchy UI
	ImGui::Begin("Scene Hierarchy");
	for (size_t i = 0; i < uniqueObjects.size(); ++i)
	{
		dx3d::GameObject* obj = uniqueObjects[i];
		std::string label = "GameObject_" + std::to_string(i);

		//Dynamic naming conventions based on attached archetypes
		if (obj->getComponent<dx3d::CubeComponent>()) {
			label = (i == 0) ? "Plane (Static Environment)" : "Cube Primitive_" + std::to_string(i);
		}
		else if (obj->getComponent<dx3d::SphereComponent>()) {
			label = "Spawned Sphere_" + std::to_string(i);
		}
		else if (obj->getComponent<dx3d::CameraComponent>()) {
			label = "Main Player (Camera)";
		}

		bool isSelected = (m_selectedObject == obj);
		if (ImGui::Selectable(label.c_str(), isSelected))
		{
			m_selectedObject = obj;
		}
	}
	ImGui::End();

	//Inspector Panel for Transform Manipulation
	ImGui::Begin("Inspector");
	if (m_selectedObject)
	{
		auto& transform = m_selectedObject->getTransform();

		dx3d::Vec3 pos = transform.getPosition();
		dx3d::Vec3 rot = transform.getRotation();
		dx3d::Vec3 scale = transform.getScale();

		float p[3] = { pos.x, pos.y, pos.z };
		float r[3] = { rot.x, rot.y, rot.z };
		float s[3] = { scale.x, scale.y, scale.z };

		ImGui::Text("Transform Matrix Attributes");
		ImGui::Separator();
		ImGui::Spacing();

		//Render drag inputs that instantly recalculate the entity affine matrices
		if (ImGui::DragFloat3("Position", p, 1.0f))
			transform.setPosition({ p[0], p[1], p[2] });

		if (ImGui::DragFloat3("Rotation", r, 0.01f))
			transform.setRotation({ r[0], r[1], r[2] });

		if (ImGui::DragFloat3("Scale", s, 0.1f, 0.01f, 5000.0f))
			transform.setScale({ s[0], s[1], s[2] });
	}
	else
	{
		ImGui::TextDisabled("Select an active GameObject from the hierarchy \nto inspect and manipulate its transformation vectors.");
	}
	ImGui::End();

	//Keyboard Shorcut Handling
	//Bypass physical keystrokes if the user is typing values inside inspector input windows
	if (ImGui::GetIO().WantCaptureKeyboard)
	{
		return;
	}

	if (input.isKeyPressed(dx3d::KeyCode::Escape))
	{
		PostQuitMessage(0);
		return;
	}
}