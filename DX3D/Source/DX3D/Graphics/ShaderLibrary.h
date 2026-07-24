#pragma once
#include <DX3D/Graphics/GraphicsPipelineState.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <unordered_map>
#include <string>
#include <memory>
#include <d3d11.h>
#include <wrl/client.h>

namespace dx3d
{
    inline ID3D11ShaderResourceView* Create1x1WhiteTexture(ID3D11Device* device)
    {
        uint32_t pixel = 0xFFFFFFFF; // Pure White (R:255, G:255, B:255, A:255)

        D3D11_TEXTURE2D_DESC desc{};
        desc.Width = 1;
        desc.Height = 1;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA initData{};
        initData.pSysMem = &pixel;
        initData.SysMemPitch = sizeof(uint32_t);

        ID3D11Texture2D* tex = nullptr;
        device->CreateTexture2D(&desc, &initData, &tex);

        ID3D11ShaderResourceView* srv = nullptr;
        device->CreateShaderResourceView(tex, nullptr, &srv);

        if (tex) tex->Release();
        return srv;
    }

	class ShaderLibrary
	{
		public:
			ShaderLibrary() = default;
			~ShaderLibrary() = default;

			void init(GraphicsDevice& device);

			// Compiles and stores a shader pipeline under a lookup key (e.g., "Textured", "Basic")
			bool loadShader(GraphicsDevice& device, const std::string& name, const std::string& filePath,
				const std::string& vsEntry = "VSMain", const std::string& psEntry = "PSMain");

			// Retrieves a stored pipeline by name
			std::shared_ptr<GraphicsPipelineState> getShader(const std::string& name) const;

			// Default linear sampler state getter
			ID3D11SamplerState* getLinearSampler() const { return m_linearSampler.Get(); }

		private:
			std::unordered_map<std::string, std::shared_ptr<GraphicsPipelineState>> m_pipelines;
			Microsoft::WRL::ComPtr<ID3D11SamplerState> m_linearSampler;
	};
}

