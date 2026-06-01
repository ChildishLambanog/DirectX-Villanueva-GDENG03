#include <DX3D/Graphics/ShaderBinary.h>
#include <DX3D/Graphics/GraphicsUtils.h>
#include <d3dcompiler.h>

dx3d::ShaderBinary::ShaderBinary(const ShaderCompileDesc& desc, const GraphicsResourceDesc& gDesc) : GraphicsResource(gDesc), m_type(desc.shaderType)
{
	if (!desc.shaderSourceName) DX3DLogThrowInvalidArgument("No shader source name provided!");
	if (!desc.shaderSourceCode) DX3DLogThrowInvalidArgument("No shader source code provided!");
	if (!desc.shaderSourceCodeSize) DX3DLogThrowInvalidArgument("No shader source code size provided!");
	if (!desc.shaderEntryPoint) DX3DLogThrowInvalidArgument("No shader entry point provided!");

	UINT compileFlags{};

	#ifdef _DEBUG
		compileFlags |= D3DCOMPILE_DEBUG;
	#endif

	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob{};

	DX3DGraphicsCheckShaderCompile(
		D3DCompile(
			desc.shaderSourceCode, 
			desc.shaderSourceCodeSize, 
			desc.shaderSourceName, 
			nullptr, 
			nullptr, 
			desc.shaderEntryPoint,
			dx3d::GraphicsUtils::GetShaderModelTarget(desc.shaderType),
			compileFlags, 
			0,
			&m_shaderBlob,
			&errorBlob
			),
			errorBlob.Get()
	);
}

dx3d::BinaryData dx3d::ShaderBinary::getData() const noexcept
{
	return
	{
		m_shaderBlob->GetBufferPointer(),
		m_shaderBlob->GetBufferSize()
	};
}

dx3d::ShaderType dx3d::ShaderBinary::getType() const noexcept
{
	return m_type;
}
