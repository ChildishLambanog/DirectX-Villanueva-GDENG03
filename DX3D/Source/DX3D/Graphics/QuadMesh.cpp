#include <DX3D/Graphics/QuadMesh.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <iterator>

dx3d::QuadMesh::QuadMesh(GraphicsDevice& device, float xOffset, float yOffset, Vec4 color)
{
	float size = 0.25f;

    const GraphicsEngine::Vertex vertexList[] =
    {
        { {xOffset - size, yOffset - size, 0.0f}, color },
        { {xOffset - size, yOffset + size, 0.0f}, color },
        { {xOffset + size, yOffset + size, 0.0f}, color },

        { {xOffset + size, yOffset + size, 0.0f}, color },
        { {xOffset + size, yOffset - size, 0.0f}, color },
        { {xOffset - size, yOffset - size, 0.0f}, color }
    };

    m_vb = device.createVertexBuffer({ vertexList, std::size(vertexList), sizeof(GraphicsEngine::Vertex) });
}

void dx3d::QuadMesh::drawQuad(DeviceContext& context)
{
    auto& vb = *m_vb;
	context.setVertexBuffer(vb);
	context.drawTriangleList(vb.getVertexListSize(), 0u);
}

