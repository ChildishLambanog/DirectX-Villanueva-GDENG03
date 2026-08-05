#include <DX3D/Component/MeshGenerator.h>
#include <DX3D/Math/MathUtils.h>
#include <DX3D/Core/Logger.h>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <utility>

dx3d::Mesh dx3d::MeshGenerator::createCube(GraphicsDevice& device)
{
    Mesh mesh{};

    //Cube with rainbow pixel shader applied to it.
    const LocalVertex vertexList[] =
    {
        // Front Face (UVs 0,0 to 1,1)
        {{-0.5f,-0.5f,-0.5f}, {0.0f, 1.0f}, {1,1,1,1}},
        {{-0.5f, 0.5f,-0.5f}, {0.0f, 0.0f}, {1,1,1,1}},
        {{ 0.5f, 0.5f,-0.5f}, {1.0f, 0.0f}, {1,1,1,1}},
        {{ 0.5f,-0.5f,-0.5f}, {1.0f, 1.0f}, {1,1,1,1}},

        // Back Face
        {{ 0.5f,-0.5f, 0.5f}, {0.0f, 1.0f}, {1,1,1,1}},
        {{ 0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}, {1,1,1,1}},
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}, {1,1,1,1}},
        {{-0.5f,-0.5f, 0.5f}, {1.0f, 1.0f}, {1,1,1,1}},

        // Top Face
        {{-0.5f, 0.5f,-0.5f}, {0.0f, 1.0f}, {1,1,1,1}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}, {1,1,1,1}},
        {{ 0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}, {1,1,1,1}},
        {{ 0.5f, 0.5f,-0.5f}, {1.0f, 1.0f}, {1,1,1,1}},

        // Bottom Face
        {{-0.5f,-0.5f, 0.5f}, {0.0f, 1.0f}, {1,1,1,1}},
        {{-0.5f,-0.5f,-0.5f}, {0.0f, 0.0f}, {1,1,1,1}},
        {{ 0.5f,-0.5f,-0.5f}, {1.0f, 0.0f}, {1,1,1,1}},
        {{ 0.5f,-0.5f, 0.5f}, {1.0f, 1.0f}, {1,1,1,1}},

        // Right Face
        {{ 0.5f,-0.5f,-0.5f}, {0.0f, 1.0f}, {1,1,1,1}},
        {{ 0.5f, 0.5f,-0.5f}, {0.0f, 0.0f}, {1,1,1,1}},
        {{ 0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}, {1,1,1,1}},
        {{ 0.5f,-0.5f, 0.5f}, {1.0f, 1.0f}, {1,1,1,1}},

        // Left Face
        {{-0.5f,-0.5f, 0.5f}, {0.0f, 1.0f}, {1,1,1,1}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}, {1,1,1,1}},
        {{-0.5f, 0.5f,-0.5f}, {1.0f, 0.0f}, {1,1,1,1}},
        {{-0.5f,-0.5f,-0.5f}, {1.0f, 1.0f}, {1,1,1,1}}
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
        0,1,2,    2,3,0,    // Front
        4,5,6,    6,7,4,    // Back
        8,9,10,   10,11,8,  // Top
        12,13,14, 14,15,12, // Bottom
        16,17,18, 18,19,16, // Right
        20,21,22, 22,23,20  // Left
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

    sphereVertices.push_back({ dx3d::Vec3(0.0f, radius, 0.0f), dx3d::Vec2(0.5f, 0.0f), dx3d::Vec4(1,1,1,1) });

    float phiStep = dx3d::MathUtils::PI / stackCount;
    float thetaStep = 2.0f * dx3d::MathUtils::PI / sliceCount;

    for (uint32_t i = 1; i < stackCount; ++i)
    {
        float phi = i * phiStep;
        float v = phi / dx3d::MathUtils::PI; //Vertical UV

        for (uint32_t j = 0; j <= sliceCount; ++j)
        {
            float theta = j * thetaStep;
            float u = theta / (2.0f * dx3d::MathUtils::PI); //Horizontal UV

            dx3d::Vec3 pos(radius * sinf(phi) * cosf(theta), radius * cosf(phi), radius * sinf(phi) * sinf(theta));
            sphereVertices.push_back({ pos, dx3d::Vec2(u, v), dx3d::Vec4(1,1,1,1) });
        }
    }

    sphereVertices.push_back({ dx3d::Vec3(0.0f, -radius, 0.0f), dx3d::Vec2(0.5f, 1.0f), dx3d::Vec4(1,1,1,1) });

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
    std::vector<Vec2> temp_texcoords;
    std::vector<LocalVertex> vertices;
    std::vector<dx3d::ui32> indices;

    // Map to prevent duplicate vertices and share indices: key = (posIndex, uvIndex)
    std::map<std::pair<dx3d::ui32, dx3d::ui32>, dx3d::ui32> uniqueVertexMap;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") // Position
        {
            Vec3 pos{};
            ss >> pos.x >> pos.y >> pos.z;
            temp_positions.push_back(pos);
        }
        else if (prefix == "vt") // Texture Coordinate
        {
            Vec2 uv{};
            ss >> uv.x >> uv.y;
            uv.y = 1.0f - uv.y; // DirectX flips V coordinate upside down compared to OpenGL
            temp_texcoords.push_back(uv);
        }
        else if (prefix == "f") // Parse Face Indices (e.g., f v/vt/vn or f v/vt)
        {
            std::vector<dx3d::ui32> faceIndices;
            std::string vertexStr;

            while (ss >> vertexStr)
            {
                std::istringstream vss(vertexStr);
                std::string posStr, uvStr;

                std::getline(vss, posStr, '/'); // Get 'v' index
                std::getline(vss, uvStr, '/');  // Get 'vt' index

                dx3d::ui32 posIdx = !posStr.empty() ? static_cast<dx3d::ui32>(std::stoul(posStr)) : 0;
                dx3d::ui32 uvIdx = !uvStr.empty() ? static_cast<dx3d::ui32>(std::stoul(uvStr)) : 0;

                auto key = std::make_pair(posIdx, uvIdx);

                // Create a unique vertex if this (position, UV) pair hasn't been added yet
                if (uniqueVertexMap.find(key) == uniqueVertexMap.end())
                {
                    LocalVertex vert{};

                    if (posIdx > 0 && posIdx <= temp_positions.size())
                        vert.position = temp_positions[posIdx - 1];

                    if (uvIdx > 0 && uvIdx <= temp_texcoords.size())
                        vert.texcoord = temp_texcoords[uvIdx - 1];
                    else
                        vert.texcoord = Vec2(0.0f, 0.0f);

                    vert.color = color;

                    dx3d::ui32 newIndex = static_cast<dx3d::ui32>(vertices.size());
                    vertices.push_back(vert);
                    uniqueVertexMap[key] = newIndex;
                    faceIndices.push_back(newIndex);
                }
                else
                {
                    faceIndices.push_back(uniqueVertexMap[key]);
                }
            }

            // Triangulate polygons / quad faces
            for (size_t i = 1; i + 1 < faceIndices.size(); ++i)
            {
                indices.push_back(faceIndices[0]);
                indices.push_back(faceIndices[i]);
                indices.push_back(faceIndices[i + 1]);
            }
        }
    }

    mesh.indexCount = static_cast<dx3d::ui32>(indices.size());
    mesh.vb = device.createVertexBuffer({ vertices.data(), static_cast<dx3d::ui32>(vertices.size()), sizeof(LocalVertex) });
    mesh.ib = device.createIndexBuffer({ indices.data(), static_cast<dx3d::ui32>(indices.size()) });

    return mesh;
}
