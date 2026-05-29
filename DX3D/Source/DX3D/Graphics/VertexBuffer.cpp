#include <DX3D/Graphics/VertexBuffer.h>

dx3d::VertexBuffer::VertexBuffer(const VertexBufferDesc& desc, const GraphicsResourceDesc& gDesc) : GraphicsResource(gDesc), m_vertexSize(desc.vertexSize), 
	m_vertexListSize(desc.vertexListSize)
{
	if (!desc.vertexList) DX3DLogThrowInvalidArgument("No vertex list provided!");
	if (!desc.vertexListSize) DX3DLogThrowInvalidArgument("Vertex List Size Must Be Non-Zero!");
	if (!desc.vertexSize) DX3DLogThrowInvalidArgument("Vertex Size Must Be Non-Zero!");

	D3D11_BUFFER_DESC buffDesc{};
	buffDesc.ByteWidth = desc.vertexListSize * desc.vertexSize;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = desc.vertexList;

	DX3DGraphicsLogThrowOnFail(m_device.CreateBuffer(&buffDesc, &initData, &m_buffer), "CreateBuffer has failed!");

}

dx3d::ui32 dx3d::VertexBuffer::getVertexListSize() const noexcept
{
	return m_vertexListSize;
}
