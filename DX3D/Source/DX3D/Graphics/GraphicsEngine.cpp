#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/QuadMesh.h>
#include <fstream>

dx3d::GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc) : Base(desc.base)
{
	m_graphicsDevice = std::make_shared<GraphicsDevice>(GraphicsDeviceDesc{m_logger});

	auto& device = *m_graphicsDevice;
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

	m_quadMesh.push_back(std::make_unique<QuadMesh>(device, -0.6f, 0.0f, Vec4{ 0.22f, 0.73f, 0.73f, 1.0f })); //Left Quad Hatsune Miku
	m_quadMesh.push_back(std::make_unique<QuadMesh>(device, 0.0f, 0.0f, Vec4{ 0.89f, 0.44f, 0.47f, 1.0f })); //Middle Quad Kasane Teto
	m_quadMesh.push_back(std::make_unique<QuadMesh>(device, 0.6f, 0.0f, Vec4{ 1.0f, 0.71f, 0.0f, 1.0f })); //Right Quad Akita Neru
}

dx3d::GraphicsEngine::~GraphicsEngine()
{

}

dx3d::GraphicsDevice& dx3d::GraphicsEngine::getGraphicsDevice() noexcept
{
	return *m_graphicsDevice;
}

void dx3d::GraphicsEngine::render(SwapChain& swapChain)
{
	auto& context = *m_deviceContext;
	context.clearAndSetBackBuffer(swapChain, { 0.91f, 0.67f, 0.73f, 1.0f }); //Megurine Luka Background Color
	context.setGraphicsPipelineState(*m_pipeline);

	context.setViewportSize(swapChain.getSize());

	for (auto& quad : m_quadMesh)
	{
		quad->drawQuad(context);
	}

	auto& device = *m_graphicsDevice;
	device.executeCommandList(context);
	swapChain.present();
}
