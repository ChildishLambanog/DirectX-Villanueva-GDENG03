#include <DX3D/Component/MeshGenerator.h>
#include <DX3D/Math/MathUtils.h>
#include <DX3D/Core/Logger.h>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>

dx3d::Mesh dx3d::MeshGenerator::createCube(GraphicsDevice& device)
{
    Mesh mesh{};

    //Cube with rainbow pixel shader applied to it.
    const LocalVertex vertexList[] =
    {
        {{-0.5f,-0.5f,-0.5f}, {1,0,0,1}},
        {{-0.5f,0.5f,-0.5f},  {0,1,0,1}},
        {{0.5f,0.5f,-0.5f},   {0,0,1,1}},
        {{0.5f,-0.5f,-0.5f},  {1,0,1,1}},

        {{0.5f,-0.5f,0.5f},   {1,0,1,1}},
        {{0.5f,0.5f,0.5f},    {0,0,1,1}},
        {{-0.5f,0.5f,0.5f},   {0,1,0,1}},
        {{-0.5f,-0.5f,0.5f},  {1,0,0,1}}
    };

	//Cube with white shader applied to it.
    //const LocalVertex vertexList[] =
    //{
    //    {{-0.5f,-0.5f,-0.5f}, {1,1,1,1}},
    //    {{-0.5f,0.5f,-0.5f},  {1,1,1,1}},
    //    {{0.5f,0.5f,-0.5f},   {1,1,1,1}},
    //    {{0.5f,-0.5f,-0.5f},  {1,1,1,1}},

    //    {{0.5f,-0.5f,0.5f},   {1,1,1,1}},
    //    {{0.5f,0.5f,0.5f},    {1,1,1,1}},
    //    {{-0.5f,0.5f,0.5f},   {1,1,1,1}},
    //    {{-0.5f,-0.5f,0.5f},  {1,1,1,1}}
    //};

    const dx3d::ui32 indexList[] =
    {
        0,1,2,  2,3,0, //front
        4,5,6,  6,7,4, //back
        1,6,5,  5,2,1, //top
        7,0,3,  3,4,7, //bottom
        3,2,5,  5,4,3, //right
        7,6,1,  1,0,7  //left
    };

    mesh.indexCount = static_cast<dx3d::ui32>(std::size(indexList));
    mesh.vb = device.createVertexBuffer({ vertexList, static_cast<dx3d::ui32>(std::size(vertexList)), sizeof(LocalVertex) });
    mesh.ib = device.createIndexBuffer({ indexList, mesh.indexCount });

    return mesh;
}

dx3d::Mesh dx3d::MeshGenerator::createSphere(GraphicsDevice& device, uint32_t sliceCount, uint32_t stackCount, float radius)
{
    Mesh mesh{};
    std::vector<LocalVertex> sphereVertices;
    std::vector<dx3d::ui32> sphereIndices;

    sphereVertices.push_back({ dx3d::Vec3(0.0f, radius, 0.0f), dx3d::Vec4(1.0f, 1.0f, 1.0f, 1.0f) });

    float phiStep = dx3d::MathUtils::PI / stackCount;
    float thetaStep = 2.0f * dx3d::MathUtils::PI / sliceCount;

    for (uint32_t i = 1; i < stackCount; ++i)
    {
        float phi = i * phiStep;
        for (uint32_t j = 0; j <= sliceCount; ++j)
        {
            float theta = j * thetaStep;
            dx3d::Vec3 pos(radius * sinf(phi) * cosf(theta), radius * cosf(phi), radius * sinf(phi) * sinf(theta));
            dx3d::Vec4 color(0.91f, 0.67f, 0.73f, 1.0f);
            sphereVertices.push_back({ pos, color });
        }
    }
    sphereVertices.push_back({ dx3d::Vec3(0.0f, -radius, 0.0f), dx3d::Vec4(0.2f, 0.2f, 0.2f, 1.0f) });

    for (uint32_t j = 0; j < sliceCount; ++j) 
    {
        sphereIndices.push_back(0); sphereIndices.push_back(j + 2); sphereIndices.push_back(j + 1);
    }

    uint32_t baseIndex = 1;
    uint32_t ringVertexCount = sliceCount + 1;
    for (uint32_t i = 0; i < stackCount - 2; ++i) 
    {
        for (uint32_t j = 0; j < sliceCount; ++j) 
        {
            sphereIndices.push_back(baseIndex + i * ringVertexCount + j);
            sphereIndices.push_back(baseIndex + i * ringVertexCount + j + 1);
            sphereIndices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
            sphereIndices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
            sphereIndices.push_back(baseIndex + i * ringVertexCount + j + 1);
            sphereIndices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
        }
    }

    uint32_t southPoleIndex = static_cast<uint32_t>(sphereVertices.size()) - 1;
    baseIndex = southPoleIndex - ringVertexCount;

    for (uint32_t j = 0; j < sliceCount; ++j) 
    {
        sphereIndices.push_back(southPoleIndex);
        sphereIndices.push_back(baseIndex + j);
        sphereIndices.push_back(baseIndex + j + 1);
    }

    mesh.indexCount = static_cast<dx3d::ui32>(sphereIndices.size());
    mesh.vb = device.createVertexBuffer({ sphereVertices.data(), static_cast<dx3d::ui32>(sphereVertices.size()), sizeof(LocalVertex) });
    mesh.ib = device.createIndexBuffer({ sphereIndices.data(), static_cast<dx3d::ui32>(sphereIndices.size()) });

    return mesh;
}

dx3d::Mesh dx3d::MeshGenerator::createFromOBJ(GraphicsDevice& device, const std::string& filePath, Vec4 color)
{
    Mesh mesh{};
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::string errMsg = "[MeshGenerator Error] Failed to open OBJ model file: " + filePath + "\n";
        OutputDebugStringA(errMsg.c_str());
        return mesh;
    }

    std::vector<Vec3> temp_positions;
    std::vector<LocalVertex> vertices;
    std::vector<dx3d::ui32> indices;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        // Parse 3D Vertex Position
        if (prefix == "v")
        {
            Vec3 pos{};
            ss >> pos.x >> pos.y >> pos.z;
            temp_positions.push_back(pos);
        }
        // Parse Face (Indices)
        else if (prefix == "f")
        {
            std::vector<dx3d::ui32> facePositionIndices;
            std::string vertexStr;

            while (ss >> vertexStr)
            {
                // Handles format variants: "v", "v/vt", "v/vt/vn", or "v//vn"
                std::istringstream vss(vertexStr);
                std::string posIndexStr;
                std::getline(vss, posIndexStr, '/');

                if (!posIndexStr.empty())
                {
                    dx3d::ui32 idx = static_cast<dx3d::ui32>(std::stoul(posIndexStr));
                    // OBJ indices are 1-based; convert to C++ 0-based
                    facePositionIndices.push_back(idx - 1);
                }
            }

            // Triangulate polygons / quad faces (e.g. quad v0 v1 v2 v3 -> tri v0 v1 v2 and tri v0 v2 v3)
            for (size_t i = 1; i + 1 < facePositionIndices.size(); ++i)
            {
                indices.push_back(facePositionIndices[0]);
                indices.push_back(facePositionIndices[i]);
                indices.push_back(facePositionIndices[i + 1]);
            }
        }
    }

    // Populate local vertex buffer array
    for (const auto& pos : temp_positions)
    {
        vertices.push_back({ pos, color });
    }

    mesh.indexCount = static_cast<dx3d::ui32>(indices.size());
    mesh.vb = device.createVertexBuffer({ vertices.data(), static_cast<dx3d::ui32>(vertices.size()), sizeof(LocalVertex) });
    mesh.ib = device.createIndexBuffer({ indices.data(), static_cast<dx3d::ui32>(indices.size()) });

    return mesh;
}
