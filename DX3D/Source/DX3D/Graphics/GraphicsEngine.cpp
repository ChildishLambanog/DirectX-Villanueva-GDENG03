#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <fstream>

dx3d::GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc) : Base(desc.base)
{
	m_graphicsDevice = std::make_shared<GraphicsDevice>(GraphicsDeviceDesc{m_logger});

	auto& device = *m_graphicsDevice;
	m_deviceContext = device.createDeviceContext();

	constexpr char shaderFileFPath[] = "DX3D/Assets/Shaders/Basic.hlsl";
	std::ifstream shaderFileStream(shaderFileFPath);
	if (!shaderFileStream) DX3DLogThrowError("Failed to open shader file");

	//Load the RayMarcher Pixel Shader
	constexpr char rayMarcherFileFPath[] = "DX3D/Assets/Shaders/RayMarcher.hlsl";
	std::ifstream rmShaderFileStream(rayMarcherFileFPath);
	if (!rmShaderFileStream) DX3DLogThrowError("Failed to open ray marcher shader file!");
	
	std::string rmShaderFileData
	{ 
		std::istreambuf_iterator<char>(rmShaderFileStream), 
		std::istreambuf_iterator<char>() 
	};

	std::string shaderFileData
	{
		std::istreambuf_iterator<char>(shaderFileStream), 
		std::istreambuf_iterator<char>()
	};
 
	auto shaderSourceCode = shaderFileData.c_str();
	auto shaderSourceCodeSize = shaderFileData.length();\

	auto rmSourceCode = rmShaderFileData.c_str();\
	auto rmSourceCodeSize = rmShaderFileData.length();\

	auto vs = device.compileShader({ rayMarcherFileFPath, shaderSourceCode, shaderSourceCodeSize, "VSMain", ShaderType::VertexShader });
	auto ps = device.compileShader({ shaderFileFPath, shaderSourceCode, shaderSourceCodeSize, "PSMain", ShaderType::PixelShader });
	auto vsSignature = device.createVertexShaderSignature({ vs });
	m_pipeline = device.createGraphicsPipelineState({ *vsSignature, *ps });


	//Load standard Basic Shader for VS 
	auto m_rayMarcherPsBinary = device.compileShader({ rayMarcherFileFPath, rmSourceCode, rmSourceCodeSize, "PSMain", dx3d::ShaderType::PixelShader });
	m_rayMarcherPipeline = device.createGraphicsPipelineState({ *vsSignature, *m_rayMarcherPsBinary });


	const Vertex vertexList[] =
	{
		//Quad
		{ { -1.0f, -1.0f, 0.0f }, {-1.0f, -1.0f, 0.0f, 1.0f} },
		{ {-1.0f, 1.0f, 0.0f}, {-1.0f,  1.0f, 0.0f, 1.0f} },
		{ {1.0f, 1.0f, 0.0f}, { 1.0f,  1.0f, 0.0f, 1.0f} },

		{ {1.0f, 1.0f, 0.0f}, {1.0f,  1.0f, 0.0f, 1.0f} },
		{ {1.0f, -1.0f, 0.0f}, {1.0f,  1.0f, 0.0f, 1.0f} },
		{ {-1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f, 1.0f} }
	};

	m_vb = device.createVertexBuffer({vertexList, std::size(vertexList), sizeof(Vertex)});
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
	auto& vb = *m_vb;

	context.clearAndSetBackBuffer(swapChain, { 0.22f, 0.73f, 0.73f, 1.0f }); //change the second parameter to change the color of the back buffer.
	context.setGraphicsPipelineState(*m_pipeline);

	context.setViewportSize(swapChain.getSize());

	//Bind the Pipeline state that contains the RayMarcher pixel shader
	context.setGraphicsPipelineState(*m_rayMarcherPipeline);

	context.setVertexBuffer(vb);
	context.drawTriangleList(vb.getVertexListSize(), 0);

	device.executeCommandList(context);
	swapChain.present();
}
