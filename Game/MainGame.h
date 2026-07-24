#pragma once
#include <DX3D/All.h>
#include <DX3D/Component/Mesh.h>
#include <DX3D/Component/MeshComponent.h>
#include <d3d11.h>
#include <DX3D/Input/CommandManager.h>

class MainGame : public dx3d::Game
{
	public:
		explicit MainGame(const dx3d::GameDesc& desc);

	protected:
		virtual void onCreate();
		virtual void onUpdate(dx3d::f32 deltaTime);

	private:
		dx3d::Mesh m_untexturedCubeMesh; // Left without texture -> Renders rainbow vertex colors!
		dx3d::Mesh m_texturedCubeMesh;
		dx3d::Mesh m_untexturedSphereMesh;
		dx3d::Mesh m_texturedSphereMesh;
		dx3d::Mesh m_customModelMesh;

		bool m_isInitialized = false;

		dx3d::CommandManager m_commandManager{};
		std::vector<dx3d::GameObject*> m_spawnedSpheres{};
		
		bool m_showCreditsPanel = false;
		dx3d::GameObject* m_selectedObject = nullptr;

		bool m_showColorPickerPanel = false;
		float m_pickedColor[4] = { 0.0f, 0.0f, 0.12f, 1.0f };

		//Logo Variables
		ID3D11ShaderResourceView* m_logoSRV = nullptr;
		int m_logoWidth = 0;
		int m_logoHeight = 0;

		//Texture Variables
		ID3D11ShaderResourceView* m_cubeTextureSRV = nullptr;
		ID3D11ShaderResourceView* m_sphereTextureSRV = nullptr;
};