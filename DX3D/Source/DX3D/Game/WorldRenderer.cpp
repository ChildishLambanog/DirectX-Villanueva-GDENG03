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

#include <DX3D/Component/MeshComponent.h>

#include <imgui.h>
#include <imgui_impl_dx11.h>

#include <ranges>

dx3d::WorldRenderer::WorldRenderer(const WorldRendererDesc& desc) : Base(desc.base), m_graphicsDevice(desc.engine)
{
	auto& device = m_graphicsDevice;
	m_deviceContext = device.createDeviceContext();

	m_shaderLibrary.init(device);
	//m_shaderLibrary.loadShader(device, "Basic", "DX3D/Assets/Shaders/Basic.hlsl");
	m_shaderLibrary.loadShader(device, "Textured", "DX3D/Assets/Shaders/Textured.hlsl");
	
	m_defaultWhiteSRV = Create1x1WhiteTexture(device.getRawDevice());

	m_cb = device.createConstantBuffer({ {}, sizeof(ConstantData) });
}

dx3d::WorldRenderer::~WorldRenderer()
{

}

void dx3d::WorldRenderer::render(const World& world, SwapChain& swapChain, f32 deltaTime)
{
	auto size = swapChain.getSize();
	auto& context = *m_deviceContext;

	context.clearAndSetBackBuffer(swapChain, { 0.0f, 0.0f, 0.12f, 1.0f }); //change the second parameter to change the color of the back buffer. 0.22f, 0.73f, 0.73f, 1.0f
	context.setViewportSize(size);

	// Bind default linear sampler to slot 0
	context.setSampler(0, m_shaderLibrary.getLinearSampler());

	// Set textured pipeline as default
	auto texturedPipeline = m_shaderLibrary.getShader("Textured");
	if (texturedPipeline)
	{
		context.setGraphicsPipelineState(*texturedPipeline);
	}

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

	// Helper Lambda to Render Component Meshes
	auto renderMeshComponents = [&](auto components, ui32 count)
	{
		for (auto i : std::views::iota(0u, count))
		{
			auto* comp = components[i];
			const Mesh* mesh = comp->getMesh();
			if (!mesh) continue;

			data.world = comp->getGameObject().getTransform().getAffineWorldMatrix();
			context.updateConstantBuffer(*m_cb, &data);

			// Check if mesh has a custom texture; if not, fallback to 1x1 White Texture!
			ID3D11ShaderResourceView* srvToBind = mesh->textureSRV ? mesh->textureSRV : m_defaultWhiteSRV.Get();
			context.setTexture(0, srvToBind);

			context.setVertexBuffer(*(mesh->vb));
			context.setConstantBuffer(*m_cb);
			context.setIndexBuffer(*(mesh->ib));
			context.drawIndexedTriangleList(mesh->indexCount, 0u, 0u);
		}
	};

	//Cubes
	auto numCubes = 0u;
	auto cubes = world.getComponents<CubeComponent>(numCubes);
	renderMeshComponents(cubes, numCubes);

	//Spheres
	auto numSpheres = 0u;
	auto spheres = world.getComponents<SphereComponent>(numSpheres);
	renderMeshComponents(spheres, numSpheres);

	//OBJ Models
	auto numModels = 0u;
	auto models = world.getComponents<MeshComponent>(numModels);
	renderMeshComponents(models, numModels);

	m_graphicsDevice.executeCommandList(context);

	//UI
	ImGui::Render();

	//Bind swap chain's render target view and depth stencil view to the output merger stage of the pipeline before rendering ImGui draw data
	ID3D11RenderTargetView* rtv = swapChain.getRTV();
	ID3D11DepthStencilView* dsv = swapChain.getDSV();

	ID3D11DeviceContext* immediateContext = m_graphicsDevice.getImmediateDeviceContext();

	immediateContext->OMSetRenderTargets(1, &rtv, dsv);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); //Record ImGui draw commands into the command list of the device context

	swapChain.present(false); //Set it to false so that it won't clash with Game.cpp onInternalUpdate().
}
