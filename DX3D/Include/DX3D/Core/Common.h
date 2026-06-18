#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Core/Logger.h>
#include <DX3D/Math/Rect.h>

namespace dx3d
{
	struct BaseDesc
	{
		Logger& logger;
	};

	struct WindowDesc
	{
		BaseDesc base;
		Rect size{};
	};

	struct GraphicsEngineDesc
	{
		BaseDesc base;
	};

	struct GraphicsDeviceDesc
	{
		BaseDesc base;
	};

	struct DisplayDesc
	{
		WindowDesc window;
		GraphicsDevice& graphicsDevice;
	};

	struct SwapChainDesc
	{
		void* windowHandle{};
		Rect windowSize{};
	};

	enum class ShaderType
	{
		VertexShader = 0,
		PixelShader
	};

	struct ShaderCompileDesc
	{
		const char* shaderSourceName{};
		const void* shaderSourceCode{};
		size_t shaderSourceCodeSize{};
		const char* shaderEntryPoint{};
		ShaderType shaderType{};
	};

	struct VertexShaderSignatureDesc
	{
		const RefPtr<ShaderBinary>& vsBinary;
	};

	struct BinaryData
	{
		const void* data{};
		size_t dataSize{};
	};

	struct ConstantBufferDesc
	{
		const void* buffer{};
		ui32 bufferSize{};
	};

	struct GraphicsPipelineStateDesc
	{
		const VertexShaderSignature& vs;
		const ShaderBinary& ps;
	};

	struct VertexBufferDesc
	{
		const void* vertexList{};
		ui32 vertexListSize{};
		ui32 vertexSize{};
	};

	struct IndexBufferDesc
	{
		const ui32* indexList{};
		ui32 indexListSize{};
	};

	struct GameDesc
	{
		Rect windowSize{ 1280, 720 };
		Logger::LogLevel logLevel = Logger::LogLevel::Error;
	};
}
