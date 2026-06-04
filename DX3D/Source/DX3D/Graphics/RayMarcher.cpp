#include <DX3D/Graphics/RayMarcher.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/ShaderBinary.h>
#include <DX3D/Graphics/GraphicsPipelineState.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <fstream>
#include <string>

dx3d::RayMarcher::RayMarcher(GraphicsDevice& device, const VertexShaderSignature& vsSignature) : Base(BaseDesc{ device.getLogger() })
{
	constexpr char rayMarcherFileFPath[] = "DX3D/Assets/Shaders/RayMarcher.hlsl";
	std::ifstream rmShaderFileStream(rayMarcherFileFPath);
	if (!rmShaderFileStream) DX3DLogThrowError("Failed to open ray marcher shader file!");

	std::string rmShaderFileData
	{	
		std::istreambuf_iterator<char>(rmShaderFileStream), 
		std::istreambuf_iterator<char>() 
	};

	auto rmSourceCode = rmShaderFileData.c_str();
	auto rmSourceCodeSize = rmShaderFileData.length();

	//Compile the Ray Marcher Pixel Shader
	m_psBinary = device.compileShader({ rayMarcherFileFPath, rmSourceCode, rmSourceCodeSize, "PSMain", ShaderType::PixelShader });

	m_rayMarcherPipeline = device.createGraphicsPipelineState({ vsSignature, *m_psBinary });

	const Vertex vertexList[] =
	{
		//This is the Quad that covers the whole window and ray march would be applied to it
		{ { -1.0f, -1.0f, 0.0f }, {-1.0f, -1.0f, 0.0f, 1.0f} },
		{ {-1.0f, 1.0f, 0.0f},    {-1.0f,  1.0f, 0.0f, 1.0f} },
		{ {1.0f, 1.0f, 0.0f},     { 1.0f,  1.0f, 0.0f, 1.0f} },

		{ {1.0f, 1.0f, 0.0f},     {1.0f,  1.0f, 0.0f, 1.0f} },
		{ {1.0f, -1.0f, 0.0f},    {1.0f,  1.0f, 0.0f, 1.0f} },
		{ {-1.0f, -1.0f, 0.0f},   {-1.0f, -1.0f, 0.0f, 1.0f} }
	};

	m_vb = device.createVertexBuffer({ vertexList, std::size(vertexList), sizeof(Vertex) });
}

dx3d::RayMarcher::~RayMarcher()
{
}

void dx3d::RayMarcher::render(DeviceContext& deviceContext)
{
	auto& vb = *m_vb;

	//Set the ray marcher graphics pipeline state which has the ray marcher pixel shader
	deviceContext.setGraphicsPipelineState(*m_rayMarcherPipeline); 

	deviceContext.setVertexBuffer(vb);

	//We call the function to draw the quad and the ray marcher pixel shader will be applied to it
	deviceContext.drawTriangleList(vb.getVertexListSize(), 0); 
}