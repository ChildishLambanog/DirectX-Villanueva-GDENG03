#pragma once
#include <stdexcept>
#include <memory>

#define dx3d_disable_copy_and_move(Class) \
protected:\
	Class(const Class&) = delete; \
	Class& operator=(const Class&) = delete; \
	Class(Class&&) = delete; \
	Class& operator=(Class&&) = delete;

namespace dx3d
{
	class Base;			//We use forward declaration to declare the existence of a class without providing its full definition to avoid circular dependencies.
	class Window;
	class Game; 

	class GraphicsEngine;
	class GraphicsDevice;

	class Logger;
	class SwapChain;
	class Display;
	class DeviceContext;
	class ShaderBinary;
	class GraphicsPipelineState;
	class VertexBuffer;
	class VertexShaderSignature;
	class ConstantBuffer;

	using i32 = int;
	using ui32 = unsigned int;
	using f32 = float;
	using d64 = double;

	template <typename T> using RefPtr = std::shared_ptr<T>;
	template <typename T> using UniquePtr = std::unique_ptr<T>;

	using SwapChainPtr = std::shared_ptr<SwapChain>;
	using DeviceContextPtr = std::shared_ptr<DeviceContext>;
	using ShaderBinaryPtr = std::shared_ptr<ShaderBinary>;
	using GraphicsPipelineStatePtr = std::shared_ptr<GraphicsPipelineState>;
	using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
	using VertexShaderSignaturePtr = std::shared_ptr<VertexShaderSignature>;
}
