#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Core/Base.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Math/Vec4.h>

namespace dx3d
{
	class RayMarcher : Base
	{
		public:
			explicit RayMarcher(GraphicsDevice& device, const VertexShaderSignature& vsSignature);
			virtual ~RayMarcher() override;

			//Called during the render loop to render our ray marched scene
			void render(DeviceContext& deviceContext);
		private:
			struct Vertex
			{
				Vec3 position;
				Vec4 color;
			};

			std::shared_ptr<ShaderBinary> m_psBinary{};
			GraphicsPipelineStatePtr m_rayMarcherPipeline{};
			VertexBufferPtr m_vb{};
	};
}

