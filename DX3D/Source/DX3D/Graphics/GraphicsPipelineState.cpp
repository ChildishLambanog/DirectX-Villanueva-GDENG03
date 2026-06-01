#include <DX3D/Graphics/GraphicsPipelineState.h>
#include <DX3D/Graphics/ShaderBinary.h>
#include <DX3D/Graphics/VertexShaderSignature.h>

dx3d::GraphicsPipelineState::GraphicsPipelineState(const GraphicsPipelineStateDesc& desc, const GraphicsResourceDesc& gDesc): GraphicsResource(gDesc)
{
	if (desc.ps.getType() != ShaderType::PixelShader)
		DX3DLogThrowInvalidArgument("The 'ps' member is not a valid pixel shader binary!");

	auto vs = desc.vs.getShaderBinaryData();
	auto ps = desc.ps.getData();
	auto vsInputElements = desc.vs.getInputElementData();

	//float3 position : POSITION0;
	//float4 color : COLOR0;

	constexpr D3D11_INPUT_ELEMENT_DESC elements[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	DX3DGraphicsLogThrowOnFail(m_device.CreateInputLayout(
		static_cast<const D3D11_INPUT_ELEMENT_DESC*>(vsInputElements.data), 
		static_cast<ui32>(vsInputElements.dataSize), 
		vs.data, 
		vs.dataSize, 
		&m_layout), 
		"CreateInputLayout has failed!");

	DX3DGraphicsLogThrowOnFail(m_device.CreateVertexShader(vs.data, vs.dataSize, nullptr, &m_vs), "CreateVertexShader has failed!");

	DX3DGraphicsLogThrowOnFail(m_device.CreatePixelShader(ps.data, ps.dataSize, nullptr, &m_ps), "CreatePixelShader has failed!");
}
