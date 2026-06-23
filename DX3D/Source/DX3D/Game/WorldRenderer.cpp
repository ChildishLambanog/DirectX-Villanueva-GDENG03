#include <DX3D/Game/WorldRenderer.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>

#include <DX3D/Game/World.h>
#include <DX3D/Game/GameObject.h>

#include <DX3D/Component/TransformComponent.h>
#include <DX3D/Component/CubeComponent.h>
#include <DX3D/Component/SphereComponent.h>
#include <DX3D/Component/CameraComponent.h>

#include <fstream>
#include <ranges>

dx3d::WorldRenderer::WorldRenderer(const WorldRendererDesc& desc) : Base(desc.base), m_graphicsDevice(desc.engine)
{
	auto& device = m_graphicsDevice;
	m_deviceContext = device.createDeviceContext();

	constexpr char shaderFileFPath[] = "DX3D/Assets/Shaders/Basic.hlsl";
	std::ifstream shaderFileStream(shaderFileFPath);
	if (!shaderFileStream) DX3DLogThrowError("Failed to open shader file");

	std::string shaderFileData
	{
		std::istreambuf_iterator<char>(shaderFileStream),
		std::istreambuf_iterator<char>()
	};
 
	auto shaderSourceCode = shaderFileData.c_str();
	auto shaderSourceCodeSize = shaderFileData.length();

	auto vs = device.compileShader({ shaderFileFPath, shaderSourceCode, shaderSourceCodeSize, "VSMain", ShaderType::VertexShader });
	auto ps = device.compileShader({ shaderFileFPath, shaderSourceCode, shaderSourceCodeSize, "PSMain", ShaderType::PixelShader });
	auto vsSignature = device.createVertexShaderSignature ({ vs });

	m_pipeline = device.createGraphicsPipelineState({ *vsSignature, *ps });
	m_cb = device.createConstantBuffer({ {}, sizeof(ConstantData) });
}

dx3d::WorldRenderer::~WorldRenderer()
{

}

void dx3d::WorldRenderer::render(const World& world, SwapChain& swapChain, f32 deltaTime)
{
	auto size = swapChain.getSize();
	auto& context = *m_deviceContext;

	context.clearAndSetBackBuffer(swapChain, { 0.22f, 0.73f, 0.73f, 1.0f }); //change the second parameter to change the color of the back buffer.
	context.setGraphicsPipelineState(*m_pipeline);
	context.setViewportSize(size);

	auto numComponents = 0u;
	ConstantData data{};
	
	{
		auto components = world.getComponents<CameraComponent>(numComponents);
		for (auto i : std::views::iota(0u, numComponents))
		{
			auto component = components[i];
			data.view = component->getViewMatrix();
			component->setViewportSize(size);
			data.proj = component->getProjectionMatrix();
			break;
		}
	}

	//Cubes
	auto numCubes = 0u;
	auto cubes = world.getComponents<CubeComponent>(numCubes);
	for (auto i : std::views::iota(0u, numCubes))
	{
		auto* comp = cubes[i];
		const Mesh* mesh = comp->getMesh();
		if (!mesh) continue;

		data.world = comp->getGameObject().getTransform().getAffineWorldMatrix();
		context.updateConstantBuffer(*m_cb, &data);

		context.setVertexBuffer(*(mesh->vb));
		context.setConstantBuffer(*m_cb);
		context.setIndexBuffer(*(mesh->ib));
		context.drawIndexedTriangleList(mesh->indexCount, 0u, 0u);
	}

	//Spheres
	auto numSpheres = 0u;
	auto spheres = world.getComponents<SphereComponent>(numSpheres);
	for (auto i : std::views::iota(0u, numSpheres))
	{
		auto* comp = spheres[i];
		const Mesh* mesh = comp->getMesh();
		if (!mesh) continue;

		data.world = comp->getGameObject().getTransform().getAffineWorldMatrix();
		context.updateConstantBuffer(*m_cb, &data);

		context.setVertexBuffer(*(mesh->vb));
		context.setConstantBuffer(*m_cb);
		context.setIndexBuffer(*(mesh->ib));
		context.drawIndexedTriangleList(mesh->indexCount, 0u, 0u);
	}

	m_graphicsDevice.executeCommandList(context);
	swapChain.present();
}
