#include "stdafx.h"
#include "PlanetGenerator.h"
#include "GPU.h"
#include "Input.h"


#include "stdafx.h"
#include "PlanetGenerator.h"
#include "GPU.h"
#include "Input.h"
#include "ShaderLoader.h"
#include "DirectXMathHelper.h"

bool PlanetGenerator::setVertexBuffers()
{

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = sizeof(Vertex) * this->vertices.size();
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = vertices.data();
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    HRESULT hr = GPU::device->CreateBuffer(&bufferDesc, &data, &triangleBuffer);

    bufferDesc.ByteWidth = sizeof(Vertex) * this->lines.size();
    data.pSysMem = lines.data();

    hr = GPU::device->CreateBuffer(&bufferDesc, &data, &lineBuffer);

    stride = sizeof(Vertex);
    offset = 0;

    return !FAILED(hr);
}

bool PlanetGenerator::setWorldMatrix()
{
    DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4X4);
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = &worldMatrix;

    HRESULT hr = GPU::device->CreateBuffer(&bufferDesc, &data, &worldMatrixBuffer);
    return !FAILED(hr);
}

bool PlanetGenerator::setIndexBuffer()
{
    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
    indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    indexBufferDesc.ByteWidth = sizeof(DWORD) * indices.size();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = sizeof(DWORD);

    D3D11_SUBRESOURCE_DATA indexBufferData;
    indexBufferData.pSysMem = indices.data();
    indexBufferData.SysMemPitch = 0;
    indexBufferData.SysMemSlicePitch = 0;
    HRESULT hr = GPU::device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);
    return !FAILED(hr);
}

void PlanetGenerator::recreateVertexBuffers()
{
    this->vertices.clear();
    this->lines.clear();
    int currentSubdivisions = planetImGuiStruct.currentSubdivisions;

    triangleBuffer->Release();
    lineBuffer->Release();

    bool newVertice = true;
    for (int i = 0; i < sphereMeshes[currentSubdivisions%(sphereMeshes.size())].size(); i++)
    {
        /*for (int j = 0; j < 3; j++)
        {
            sphereMeshes[currentSubdivisions][i].vertices[j].position.x += 0.1f * 0.01f * (rand() % 101);
            sphereMeshes[currentSubdivisions][i].vertices[j].position.y += 0.1f * 0.01f * (rand() % 101);
            sphereMeshes[currentSubdivisions][i].vertices[j].position.z += 0.1f * 0.01f * (rand() % 101);
        }*/

        for (int j = 0; j < 3; j++)
        {
            newVertice = true;
            Vertex vertex = sphereMeshes[currentSubdivisions][i].vertices[j];
            vertex.position.Normalize();
            vertex.normal = DirectX::SimpleMath::Vector3(36.f / 255.f, 36.f / 255.f, 36.f / 255.f);
            vertex.uv = DirectX::SimpleMath::Vector2(1.f);
            int loops = vertices.size();
            for (int k = 0; k < loops; k++)
            {
                if (vertices[k] == vertex)
                {
                    std::cout << "X: " << vertices[k].position.x << " VS " << vertex.position.x << "\n";
                    std::cout << "Y: " << vertices[k].position.y << " VS " << vertex.position.y << "\n";
                    std::cout << "Z: " << vertices[k].position.z << " VS " << vertex.position.z << "\n\n";

                    vertices.push_back(vertices[k]);
                    newVertice = false;
                    break;
                }
            }
            if (newVertice) vertices.push_back(vertex);
           

        }

        /*vertices.push_back(sphereMeshes[currentSubdivisions][i].vertices[0]);
        vertices.back().normal = DirectX::SimpleMath::Vector3(36.f / 255.f, 36.f / 255.f, 36.f / 255.f);
        vertices.push_back(sphereMeshes[currentSubdivisions][i].vertices[1]);
        vertices.back().normal = DirectX::SimpleMath::Vector3(36.f / 255.f, 36.f / 255.f, 36.f / 255.f);
        vertices.push_back(sphereMeshes[currentSubdivisions][i].vertices[2]);
        vertices.back().normal = DirectX::SimpleMath::Vector3(36.f / 255.f, 36.f / 255.f, 36.f / 255.f);*/

        lines.push_back(sphereMeshes[currentSubdivisions][i].vertices[0]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
        lines.push_back(sphereMeshes[currentSubdivisions][i].vertices[1]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
        lines.push_back(sphereMeshes[currentSubdivisions][i].vertices[1]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
        lines.push_back(sphereMeshes[currentSubdivisions][i].vertices[2]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
        lines.push_back(sphereMeshes[currentSubdivisions][i].vertices[2]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
        lines.push_back(sphereMeshes[currentSubdivisions][i].vertices[0]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
    }


    //vertices[0]->position.z -= 1.0f;
    for (int i = 0; i < vertices.size(); i++)
    {
        (vertices[i]).position.x += 0.1f * 0.01f * (rand() % 101);
        (vertices[i]).position.y += 0.1f * 0.01f * (rand() % 101);
        (vertices[i]).position.z += 0.1f * 0.01f * (rand() % 101);
        (vertices[i]).position.Normalize();
    }

    //lines.clear();
    for (int i = 0; i < lines.size(); i++)
    {
        lines[i].position.Normalize();
    }

    int nr = 60;
    std::vector<Vertex> plsWork;
    if (vertices.size() >= nr)
    {
        for (int i = nr-12; i < nr; i++)//vertices.size(); i++)
        {
            plsWork.emplace_back(vertices[i]);
        }
    }
    else
    {
        for (int i = 0; i < 3; i++)//vertices.size(); i++)
        {
            plsWork.emplace_back(vertices[i]);
        }
    }

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = UINT(sizeof(Vertex) * plsWork.size());
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data = {};
    
    data.pSysMem = plsWork.data();
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    GPU::device->CreateBuffer(&bufferDesc, &data, &triangleBuffer);

    bufferDesc.ByteWidth = UINT(sizeof(Vertex) * this->lines.size());
    data.pSysMem = this->lines.data();

    GPU::device->CreateBuffer(&bufferDesc, &data, &lineBuffer);

    lastSubdivisions = planetImGuiStruct.currentSubdivisions;
}

void PlanetGenerator::createInitialSphere()
{
    switch (typeOfSphere)
    {
    case UV:
        this->createUVSphere();
        break;
    case ICO:
        this->createIcoSphere();
        break;
    case QUAD:
        this->createQuadSphere();
        break;
    default:
        this->createIcoSphere();
        break;
    }
}

void PlanetGenerator::createQuadSphere()
{
   /* for (int h = 0; h < horizontal; h++)
    {
        Vertex newVertex;
        newVertex.position.x = h * (1.f / horizontal);
        newVertex.position.x = h * (1.f / horizontal);
        newVertex.position.x = h * (1.f / horizontal);

        Vertex 
        for (int v = 0; v < vertical; v++)
        {

        }
    }*/

    for (int h = 0; h < horizontal; h++)
    {
        for (int v = 0; v < vertical; v++)
        {

        }
    }
}

void PlanetGenerator::createUVSphere()
{
    for (int h = 0; h < horizontal; h++)
    {
        for (int v = 0; v < vertical; v++)
        {

        }
    }
}

void PlanetGenerator::createIcoSphere()
{
    DirectX::XMFLOAT3 icoSphere[12] =
    {
        {0.f,  GoldenRatio, -1.f},  //0
        { GoldenRatio,  1.f, 0.f},  //6
        {-GoldenRatio,  1.f, 0.f},  //4
        {0.f,  GoldenRatio,  1.f},  //2
        {0.f, -GoldenRatio,  1.f},  //3
        {-1.f, 0.f,  GoldenRatio},  //9
        {0.f, -GoldenRatio, -1.f},  //1
        { 1.f, 0.f, -GoldenRatio},   //11
        { 1.f, 0.f,  GoldenRatio},  //10
        {-1.f, 0.f, -GoldenRatio},  //8
        { GoldenRatio, -1.f, 0.f},  //5
        {-GoldenRatio, -1.f, 0.f},  //7
    };

    std::vector<Triangle> triangles;

    triangles.emplace_back(icoSphere[2], icoSphere[1], icoSphere[0]);
    triangles.emplace_back(icoSphere[1], icoSphere[2], icoSphere[3]);
    triangles.emplace_back(icoSphere[5], icoSphere[4], icoSphere[3]);
    triangles.emplace_back(icoSphere[4], icoSphere[8], icoSphere[3]);
    triangles.emplace_back(icoSphere[7], icoSphere[6], icoSphere[0]);

    triangles.emplace_back(icoSphere[6], icoSphere[9], icoSphere[0]);
    triangles.emplace_back(icoSphere[11], icoSphere[10], icoSphere[4]);
    triangles.emplace_back(icoSphere[10], icoSphere[11], icoSphere[6]);
    triangles.emplace_back(icoSphere[9], icoSphere[5], icoSphere[2]);
    triangles.emplace_back(icoSphere[5], icoSphere[9], icoSphere[11]);

    triangles.emplace_back(icoSphere[8], icoSphere[7], icoSphere[1]);
    triangles.emplace_back(icoSphere[7], icoSphere[8], icoSphere[10]);
    triangles.emplace_back(icoSphere[2], icoSphere[5], icoSphere[3]);
    triangles.emplace_back(icoSphere[8], icoSphere[1], icoSphere[3]);
    triangles.emplace_back(icoSphere[9], icoSphere[2], icoSphere[0]);

    triangles.emplace_back(icoSphere[1], icoSphere[7], icoSphere[0]);
    triangles.emplace_back(icoSphere[11], icoSphere[9], icoSphere[6]);
    triangles.emplace_back(icoSphere[7], icoSphere[10], icoSphere[6]);
    triangles.emplace_back(icoSphere[5], icoSphere[11], icoSphere[4]);
    triangles.emplace_back(icoSphere[10], icoSphere[8], icoSphere[4]);




    //DirectX::XMFLOAT3 icoSphere[12] =
    //{
    //    {0.f,  GoldenRatio / 2, -1.f},  //0
    //    {0.f, -GoldenRatio / 2, -1.f},  //1
    //    {0.f,  GoldenRatio / 2,  1.f},  //2
    //    {0.f, -GoldenRatio / 2,  1.f},  //3

    //    {-GoldenRatio / 2,  1.f, 0.f},  //4
    //    { GoldenRatio / 2, -1.f, 0.f},  //5
    //    { GoldenRatio / 2,  1.f, 0.f},  //6
    //    {-GoldenRatio / 2, -1.f, 0.f},  //7

    //    {-1.f, 0.f, -GoldenRatio / 2},  //8
    //    {-1.f, 0.f,  GoldenRatio / 2},  //9
    //    { 1.f, 0.f,  GoldenRatio / 2},  //10
    //    { 1.f, 0.f, -GoldenRatio / 2}   //11
    //};

    ////Creating icoSphere
    ////https://www.danielsieger.com/blog/2021/01/03/generating-platonic-solids.html

    //std::vector<Triangle> triangles;

    //triangles.emplace_back(icoSphere[4], icoSphere[5], icoSphere[0]);   //1
    //triangles.emplace_back(icoSphere[6], icoSphere[4], icoSphere[2]);   //2
    //triangles.emplace_back(icoSphere[9], icoSphere[3], icoSphere[2]);   //3
    //triangles.emplace_back(icoSphere[3], icoSphere[10], icoSphere[2]);  //4
    //triangles.emplace_back(icoSphere[11], icoSphere[1], icoSphere[0]);  //5

    //triangles.emplace_back(icoSphere[1], icoSphere[8], icoSphere[2]);   //6
    //triangles.emplace_back(icoSphere[7], icoSphere[5], icoSphere[3]);   //7
    //triangles.emplace_back(icoSphere[5], icoSphere[7], icoSphere[1]);   //8
    //triangles.emplace_back(icoSphere[8], icoSphere[9], icoSphere[4]);   //9
    //triangles.emplace_back(icoSphere[9], icoSphere[8], icoSphere[7]);   //10

    //triangles.emplace_back(icoSphere[10], icoSphere[11], icoSphere[5]); //11
    //triangles.emplace_back(icoSphere[11], icoSphere[10], icoSphere[7]); //12
    //triangles.emplace_back(icoSphere[4], icoSphere[9], icoSphere[2]);   //13
    //triangles.emplace_back(icoSphere[10], icoSphere[5], icoSphere[2]);  //14
    //triangles.emplace_back(icoSphere[8], icoSphere[4], icoSphere[0]);   //15

    //triangles.emplace_back(icoSphere[5], icoSphere[11], icoSphere[0]);  //16
    //triangles.emplace_back(icoSphere[7], icoSphere[8], icoSphere[1]);   //17
    //triangles.emplace_back(icoSphere[11], icoSphere[5], icoSphere[1]);  //18
    //triangles.emplace_back(icoSphere[9], icoSphere[7], icoSphere[3]);   //19
    //triangles.emplace_back(icoSphere[5], icoSphere[10], icoSphere[3]);  //20

    int maxSubdivisions = 4;
    DirectX::XMFLOAT3 posOne;
    DirectX::XMFLOAT3 posTwo;
    DirectX::XMFLOAT3 posThree;
    Vertex newTriangle[4];
    std::vector<Triangle> newTriangleVec;

    sphereMeshes.push_back(triangles);

    for (int i = 0; i < maxSubdivisions; i++)
    {
        int size = triangles.size();
        for (int i = 0; i < size; i++)
        {
            //posOne = getScalarMultiplicationXMFLOAT3(0.3333333f, getAdditionXMFLOAT3(triangles[i].vertices[2].position, (getAdditionXMFLOAT3(triangles[i].vertices[0].position, triangles[i].vertices[1].position))));
            //for (int k = 0; k < 3; k++)
            //{
            //    newTriangle[0].position = posOne;                                   //Middle
            //    newTriangle[1].position = triangles[i].vertices[k].position;
            //    newTriangle[2].position = triangles[i].vertices[(k+1)%3].position;
            //    newTriangleVec.emplace_back(Triangle(newTriangle[0].position, newTriangle[1].position, newTriangle[2].position));
            //}
            posOne = getScalarMultiplicationXMFLOAT3(0.5f, (getAdditionXMFLOAT3(triangles[i].vertices[0].position, triangles[i].vertices[1].position)));
            posTwo = getScalarMultiplicationXMFLOAT3(0.5f, (getAdditionXMFLOAT3(triangles[i].vertices[1].position, triangles[i].vertices[2].position)));
            posThree = getScalarMultiplicationXMFLOAT3(0.5f, (getAdditionXMFLOAT3(triangles[i].vertices[2].position, triangles[i].vertices[0].position)));

            newTriangle[0].position = triangles[i].vertices[0].position;
            newTriangle[1].position = posOne;
            newTriangle[2].position = posThree;
            newTriangleVec.emplace_back(Triangle(newTriangle[0].position, newTriangle[1].position, newTriangle[2].position));

            newTriangle[0].position = posOne;
            newTriangle[1].position = triangles[i].vertices[1].position;
            newTriangle[2].position = posTwo;
            newTriangleVec.emplace_back(Triangle(newTriangle[0].position, newTriangle[1].position, newTriangle[2].position));

            newTriangle[0].position = posThree;
            newTriangle[1].position = posTwo;
            newTriangle[2].position = triangles[i].vertices[2].position;
            newTriangleVec.emplace_back(Triangle(newTriangle[0].position, newTriangle[1].position, newTriangle[2].position));

            newTriangle[0].position = posOne;
            newTriangle[1].position = posTwo;
            newTriangle[2].position = posThree;
            newTriangleVec.emplace_back(Triangle(newTriangle[0].position, newTriangle[1].position, newTriangle[2].position));

        }
        sphereMeshes.push_back(newTriangleVec);
        triangles.clear();
        triangles = newTriangleVec;
        newTriangleVec.clear();
    }

    /*for (int i = 0; i < sphereMeshes.size(); i++)
    {
        std::vector<Triangle*> addresses;
        for (int j = 0; j < sphereMeshes[i].size(); j++) 
        {
            Triangle* newStuff = &sphereMeshes[i][j];
            addresses.emplace_back();
            for (int k = 0; k < addresses.size(); k++)
            {
                if (addresses[k]->vertices[0] == newStuff)
                {

                }
            }
        }
    }*/
    bool newVertice = true;
    int indexSphere = 0;
    Vertex vertex;
    int loops;
    int count = 0;

    for (int i = 0; i < sphereMeshes[indexSphere].size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            newVertice = true;
            vertex = sphereMeshes[indexSphere][i].vertices[j];
            vertex.position.Normalize();
            vertex.normal = DirectX::SimpleMath::Vector3(36.f / 255.f, 36.f / 255.f, 36.f / 255.f);
            vertex.uv = DirectX::SimpleMath::Vector2(1.f);
            loops = vertices.size();
            for (int k = 0; k < loops; k++)
            {
                if ((vertices[k]) == (vertex))/*(*vertices[k]).position.x == (*vertex).position.x &&
                    (*vertices[k]).position.y == (*vertex).position.y &&
                    (*vertices[k]).position.z == (*vertex).position.z)*/
                {
                    count++;
                    /*if (k == 0)
                    {
                        std::cout << "X: " << vertices[k]->position.x << " \tVS " << vertex->position.x << "\n";
                        std::cout << "Y: " << vertices[k]->position.y << " \tVS " << vertex->position.y << "\n";
                        std::cout << "Z: " << vertices[k]->position.z << " \tVS " << vertex->position.z << "\n\n";
                    }*/
                    

                    indices.push_back(k);
                    /*vertices.push_back(nullptr);
                    vertices.back() = vertices[k];*/
                    newVertice = false;
                    break;
                }
            }
            if (newVertice) 
            { 
                std::cout << i << " " << j << "\n";
                vertices.push_back(vertex); 
                indices.push_back(vertices.size()-1); 
            }
           /* vertex->position.x += 0.1f * 0.01f * (rand() % 101);
            vertex->position.y += 0.1f * 0.01f * (rand() % 101);
            vertex->position.z += 0.1f * 0.01f * (rand() % 101);*/
        }
        /*vertices.push_back(sphereMeshes[0][i].vertices[0]);
        vertices.back().normal = DirectX::SimpleMath::Vector3(36.f / 255.f, 36.f / 255.f, 36.f / 255.f);
        vertices.push_back(sphereMeshes[0][i].vertices[1]);
        vertices.back().normal = DirectX::SimpleMath::Vector3(36.f / 255.f, 36.f / 255.f, 36.f / 255.f);
        vertices.push_back(sphereMeshes[0][i].vertices[2]);
        vertices.back().normal = DirectX::SimpleMath::Vector3(36.f / 255.f, 36.f / 255.f, 36.f / 255.f);*/

        lines.push_back(sphereMeshes[indexSphere][i].vertices[0]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
        lines.push_back(sphereMeshes[indexSphere][i].vertices[1]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
        lines.push_back(sphereMeshes[indexSphere][i].vertices[1]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
        lines.push_back(sphereMeshes[indexSphere][i].vertices[2]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
        lines.push_back(sphereMeshes[indexSphere][i].vertices[2]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
        lines.push_back(sphereMeshes[indexSphere][i].vertices[0]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
    }

    std::cout << "Count: " << count << "\n";
    //bool newVertice = true;
    //for (int i = 0; i < vertices.size(); i++)
    //{
    //    newVertice = true;
    //    int loops = newVertices.size();
    //    for (int j = 0; j < loops; j++)
    //    {
    //        if (vertices[i] == *newVertices[j])
    //        {
    //            newVertice = false;
    //            newVertices.emplace_back(newVertices[j]);
    //        }
    //    }
    //    if (newVertice) newVertices.emplace_back(&vertices[i]);
    //    //if (getLength(vertices[i].position) != 1.f) std::cout << "LENGTH: " << getLength(vertices[i].position) << "\n";
    //    //vertices[i].position.Normalize();
    //    //vertices[i].normal = vertices[i].position;
    //    newVertices.back()->position.Normalize();
    //}

    for (int i = 0; i < vertices.size(); i++)
    {
        //vertices[i]->position.Normalize();
        //vertices[i]->normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
    }

    for (int i = 0; i < indices.size(); i++)
    {
        if (indices[i] == 0) std::cout << "WHAT\n";
    }

    for (int i = 0; i < lines.size(); i++)
    {
        //if (getLength(vertices[i].position) != 1.f) std::cout << "LENGTH: " << getLength(vertices[i].position) << "\n";
        lines[i].position.Normalize();
    }
 /*   vertices[indices[599]]->position.x -= 1.0f;
    vertices[indices[600]]->position.x -= 1.0f;
    vertices[indices[601]]->position.x -= 1.0f;

    vertices[indices[599]]->position.y -= 5.0f;
    vertices[indices[600]]->position.y -= 5.0f;
    vertices[indices[601]]->position.y -= 5.0f;

    vertices[indices[599]]->position.z -= 1.0f;
    vertices[indices[600]]->position.z -= 1.0f;
    vertices[indices[601]]->position.z -= 1.0f;*/

   /* for (int i = 0; i < indices.size(); i++)
    {
        vertices[indices[599]]->position.x -= 1.0f;
        vertices[indices[600]]->position.x -= 1.0f;
        vertices[indices[601]]->position.x -= 1.0f;
    }*/

    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i].position.x += 0.1f * 0.1f * (rand() % 101);
        vertices[i].position.y += 0.1f * 0.1f * (rand() % 101);
        vertices[i].position.z += 0.1f * 0.1f * (rand() % 101);
        vertices[i].position.Normalize();
    }

}

PlanetGenerator::PlanetGenerator()
    :camera(Camera()), cameraPosition(DirectX::XMFLOAT3(0.f, 0.f, -10.f)), lastSubdivisions(0)
{
    /*meshes.emplace_back(new Mesh());
    planet = new Planet(meshes[0]);*/
    renderer.initiateRenderer(GPU::immediateContext, GPU::device, GPU::swapChain, GPU::windowWidth, GPU::windowHeight);
    typeOfSphere = PlanetType::ICO;
    this->createInitialSphere();
    this->setWorldMatrix();
    this->setVertexBuffers();
    this->setIndexBuffer();
    camera.setPosition(DirectX::XMFLOAT3(0.f, 0.f, -10.f));
    LoadVertexShader(GPU::device, vShader, "playerVectorVertex");
    LoadPixelShader(GPU::device, pShader, "plaverVectorPixel");
}

PlanetGenerator::~PlanetGenerator()
{
    triangleBuffer->Release();
    lineBuffer->Release();

    vShader->Release();
    pShader->Release();
    /*delete planet;
    for (int i = 0; i < meshes.size(); i++)
    {
        delete meshes[i];
    }*/
}

GAMESTATE PlanetGenerator::Update()
{
    if (GetAsyncKeyState('I')) this->cameraPosition.y += 0.01f;
    if (GetAsyncKeyState('J')) this->cameraPosition.y -= 0.01f;
    if (GetAsyncKeyState('A')) this->cameraPosition.x -= 0.01f;
    if (GetAsyncKeyState('D')) this->cameraPosition.x += 0.01f;
    if (GetAsyncKeyState('W')) this->cameraPosition.z += 0.01f;
    if (GetAsyncKeyState('S')) this->cameraPosition.z -= 0.01f;

    if (planetImGuiStruct.currentSubdivisions != lastSubdivisions) this->recreateVertexBuffers();
   /* for (int i = 0; i < vertices.size(); i++)
    {
        if (getLength(vertices[i].position) != 1.f) std::cout << "LENGTH: " << getLength(vertices[i].position) << "\n";
        vertices[i].position.Normalize();
    }*/
    this->camera.setPosition(cameraPosition);
    return NOCHANGE;
}

void PlanetGenerator::Render()
{
    renderer.setUpScene();
    GPU::immediateContext->VSSetShader(vShader, nullptr, 0);
    GPU::immediateContext->PSSetShader(pShader, nullptr, 0);
    GPU::immediateContext->VSSetConstantBuffers(0, 1, &worldMatrixBuffer);
    if (planetImGuiStruct.renderTriangles)
    {
        //GPU::immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
        GPU::immediateContext->IASetVertexBuffers(0, 1, &triangleBuffer, &stride, &offset);
        GPU::immediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
        GPU::immediateContext->DrawIndexed(this->indices.size(), 0, 0);
    }
    if (planetImGuiStruct.renderLines)
    {
        GPU::immediateContext->IASetVertexBuffers(0, 1, &lineBuffer, &stride, &offset);
        GPU::immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        GPU::immediateContext->Draw(this->lines.size(), 0);
    }
    

    imGuiHelper.planetEditor(planetImGuiStruct);
}
