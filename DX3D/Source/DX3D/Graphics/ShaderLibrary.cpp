#include <DX3D/Graphics/ShaderLibrary.h>
#include <DX3D/Core/Logger.h>
#include <fstream>
#include <sstream>

namespace dx3d
{
	void ShaderLibrary::init(GraphicsDevice& device)
	{
		// Create default Linear Texture Sampler State
		D3D11_SAMPLER_DESC sampDesc{};
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT hr = device.getRawDevice()->CreateSamplerState(&sampDesc, m_linearSampler.GetAddressOf());
		if (FAILED(hr))
		{
			std::string errMsg = "Failed to create Sampler State in ShaderLibrary!";
			OutputDebugStringA(errMsg.c_str());
		}
	}

	bool ShaderLibrary::loadShader(GraphicsDevice& device, const std::string& name, const std::string& filePath,
		const std::string& vsEntry, const std::string& psEntry)
	{
		std::ifstream shaderFileStream(filePath);
		if (!shaderFileStream.is_open())
		{
			OutputDebugStringA(("[ShaderLibrary Error] Could not open shader: " + filePath + "\n").c_str());
			return false;
		}

		std::string shaderFileData(
			(std::istreambuf_iterator<char>(shaderFileStream)),
			std::istreambuf_iterator<char>()
		);

		auto vs = device.compileShader({ filePath.c_str(), shaderFileData.c_str(), shaderFileData.length(), vsEntry.c_str(), ShaderType::VertexShader });
		auto ps = device.compileShader({ filePath.c_str(), shaderFileData.c_str(), shaderFileData.length(), psEntry.c_str(), ShaderType::PixelShader });
		auto vsSignature = device.createVertexShaderSignature({ vs });

		auto pipeline = device.createGraphicsPipelineState({ *vsSignature, *ps });
		m_pipelines[name] = pipeline;

		return true;
	}

	std::shared_ptr<GraphicsPipelineState> ShaderLibrary::getShader(const std::string& name) const
	{
		auto it = m_pipelines.find(name);
		if (it != m_pipelines.end())
		{
			return it->second;
		}
		return nullptr;
	}
}
