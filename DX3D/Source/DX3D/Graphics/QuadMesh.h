#pragma once
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Math/Vec4.h>

namespace dx3d
{
	class QuadMesh
	{
		public:
			QuadMesh(GraphicsDevice& device, float xOffset, float yOffset, Vec4 color);
			~QuadMesh() = default;

			void drawQuad(DeviceContext& context);
		private:
			VertexBufferPtr m_vb{};
	};
}

