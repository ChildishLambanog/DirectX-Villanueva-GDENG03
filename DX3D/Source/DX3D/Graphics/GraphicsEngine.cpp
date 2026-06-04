#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/RayMarcher.h>
#include <fstream>

dx3d::GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc) : Base(desc.base)
{
	m_graphicsDevice = std::make_shared<GraphicsDevice>(GraphicsDeviceDesc{ m_logger });

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

	auto shaderSourceCode = shaderFileData.c_str(); \
	auto shaderSourceCodeSize = shaderFileData.length(); \

	auto vs = device.compileShader({ shaderFileFPath, shaderSourceCode, shaderSourceCodeSize, "VSMain", ShaderType::VertexShader });
	auto vsSignature = device.createVertexShaderSignature({ vs });

	//Create the ray marcher object then supply the graphics device and the vertex shader signature to the ray marcher constructor
	m_rayMarcher = std::make_unique<RayMarcher>(device, *vsSignature);
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
	auto& device = *m_graphicsDevice;

	context.clearAndSetBackBuffer(swapChain, { 0, 0, 0, 0 });

	context.setViewportSize(swapChain.getSize());

	//If the ray marcher object is created successfully the render function is called to render the ray marched scene to the back buffer
	if (m_rayMarcher) 
	{
		m_rayMarcher->render(context);
	}

	device.executeCommandList(context);
	swapChain.present();
}
