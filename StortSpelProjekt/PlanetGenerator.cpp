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

void PlanetGenerator::updateColours()
{
    int index = planetImGuiStruct.currentSubdivisions - 1;
    float length = 0;
    for (int i = 0; i < vertices[index].size(); i++)
    {
        length = vertices[index][i].position.Length();
        if (planetImGuiStruct.useColours > 0 && length >= 0.95 * planetImGuiStruct.colourFactor[0].x 
            && length <= 1.1 * planetImGuiStruct.colourFactor[0].y) vertices[index][i].normal = planetImGuiStruct.colourSelection[0];
        else if (planetImGuiStruct.useColours > 1 && length < 0.95 * planetImGuiStruct.colourFactor[1].y 
            && length >= 0.85 * planetImGuiStruct.colourFactor[0].x)  vertices[index][i].normal = planetImGuiStruct.colourSelection[1];
        else if (planetImGuiStruct.useColours > 2 && length > 1.1 * planetImGuiStruct.colourFactor[2].x
            && length <= 1.2 * planetImGuiStruct.colourFactor[2].y) vertices[index][i].normal = planetImGuiStruct.colourSelection[2];
        else if (planetImGuiStruct.useColours > 3 && length < 0.85 * planetImGuiStruct.colourFactor[3].x) vertices[index][i].normal = planetImGuiStruct.colourSelection[3];
        else if (planetImGuiStruct.useColours > 4 && length > 1.2 * planetImGuiStruct.colourFactor[4].y && length >= 0.85) vertices[index][i].normal = planetImGuiStruct.colourSelection[4];
    }

    this->updateVertexBuffer();
}

void PlanetGenerator::recreateMesh()
{
    int index = planetImGuiStruct.currentSubdivisions-1;

    std::cout << "Recreated Mesh\nCurrent Subdivisions: " << planetImGuiStruct.currentSubdivisions << "\n";
    if (planetImGuiStruct.recreateOriginalSphere) vertices[index] = baseVertices[index];
    else
    {
        for (int i = 0; i < vertices[index].size(); i++)
        {
            vertices[index][i].position.x = baseVertices[index][i].position.x + (1 - rand() % 2) * 0.001f * (rand() % (planetImGuiStruct.randomizedFactor + 1));
            vertices[index][i].position.y = baseVertices[index][i].position.y + (1 - rand() % 2) * 0.001f * (rand() % (planetImGuiStruct.randomizedFactor + 1));
            vertices[index][i].position.z = baseVertices[index][i].position.z + (1 - rand() % 2) * 0.001f * (rand() % (planetImGuiStruct.randomizedFactor + 1));

            if (planetImGuiStruct.normalised) vertices[index][i].position.Normalize();

            vertices[index][i].normal.x = std::abs(vertices[index][i].position.x);
            vertices[index][i].normal.y = std::abs(vertices[index][i].position.y);
            vertices[index][i].normal.z = std::abs(vertices[index][i].position.z);
            vertices[index][i].normal.Normalize();

        }
    }
  
    int loops = indices[index].size();
    int lineIndex = 0;
    for (int i = 0; i < loops; i += 3)
    {
        
        lines[index][lineIndex++].position = vertices[index][indices[index][i + 0]].position;
        lines[index][lineIndex++].position = vertices[index][indices[index][i + 1]].position;
        lines[index][lineIndex++].position = vertices[index][indices[index][i + 1]].position;
        lines[index][lineIndex++].position = vertices[index][indices[index][i + 2]].position;
        lines[index][lineIndex++].position = vertices[index][indices[index][i + 2]].position;
        lines[index][lineIndex++].position = vertices[index][indices[index][i + 0]].position;

    }
    if (planetImGuiStruct.geoColours)
    {
        float length = 0;
        for (int i = 0; i < vertices[index].size(); i++)
        {
            length = vertices[index][i].position.Length();
            if (planetImGuiStruct.useColours > 0 && length < 0.8)  vertices[index][i].normal = planetImGuiStruct.colourSelection[0];
            else if (planetImGuiStruct.useColours > 1 && length < 1.0) vertices[index][i].normal = planetImGuiStruct.colourSelection[1];
            else if (planetImGuiStruct.useColours > 2 && length > 1.2) vertices[index][i].normal = planetImGuiStruct.colourSelection[2];
            else if (planetImGuiStruct.useColours > 3 && length > 1.4) vertices[index][i].normal = planetImGuiStruct.colourSelection[3];
            else if (planetImGuiStruct.useColours > 4) vertices[index][i].normal = planetImGuiStruct.colourSelection[4];
        }
    }
    
    /*for (int i = 0; i < lines[a].size(); i++)
    {
        lines[a][i].position.Normalize();
    }*/

    planetImGuiStruct.recreateOriginalSphere = false;
    planetImGuiStruct.recreateMesh = false;
    this->updateVertexBuffer();
}

bool PlanetGenerator::updateVertexBuffer()
{
    triangleBuffer[planetImGuiStruct.currentSubdivisions-1]->Release();

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = UINT(sizeof(Vertex) * reactphysics3d::uint64(this->vertices[planetImGuiStruct.currentSubdivisions-1].size()));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;


    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = vertices[planetImGuiStruct.currentSubdivisions-1].data();
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    HRESULT hr = GPU::device->CreateBuffer(&bufferDesc, &data, &triangleBuffer[planetImGuiStruct.currentSubdivisions-1]);
    if (FAILED(hr)) return false;

    lineBuffer[planetImGuiStruct.currentSubdivisions - 1]->Release();

    bufferDesc.ByteWidth = UINT(sizeof(Vertex) * this->lines[planetImGuiStruct.currentSubdivisions - 1].size());
    data.pSysMem = lines[planetImGuiStruct.currentSubdivisions - 1].data();

    hr = GPU::device->CreateBuffer(&bufferDesc, &data, &lineBuffer[planetImGuiStruct.currentSubdivisions - 1]);

    return !FAILED(hr);
}

bool PlanetGenerator::setVertexBuffers()
{
    for (int i = 0; i < MAXITERATIONS; i++)
    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.ByteWidth = sizeof(Vertex) * this->vertices[i].size();
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA data = {};
        data.pSysMem = vertices[i].data();
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;

        HRESULT hr = GPU::device->CreateBuffer(&bufferDesc, &data, &triangleBuffer[i]);
        if (FAILED(hr)) return false;

        bufferDesc.ByteWidth = sizeof(Vertex) * this->lines[i].size();
        data.pSysMem = lines[i].data();

        hr = GPU::device->CreateBuffer(&bufferDesc, &data, &lineBuffer[i]);
        if (FAILED(hr)) return false;

        stride = sizeof(Vertex);
        offset = 0;
    }

   

    return true;
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
    for (int i = 0; i < MAXITERATIONS; i++)
    {
        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
        indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        indexBufferDesc.ByteWidth = sizeof(DWORD) * indices[i].size();
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;;
        indexBufferDesc.MiscFlags = 0;
        indexBufferDesc.StructureByteStride = sizeof(DWORD);

        D3D11_SUBRESOURCE_DATA indexBufferData;
        indexBufferData.pSysMem = indices[i].data();
        indexBufferData.SysMemPitch = 0;
        indexBufferData.SysMemSlicePitch = 0;
        HRESULT hr = GPU::device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer[i]);
        if (FAILED(hr)) return false;
    }
    return true;

}


void PlanetGenerator::recreateVertexBuffers()
{
    //this->vertices.clear();
    //this->lines.clear();
    //int currentSubdivisions = planetImGuiStruct.currentSubdivisions;

    //triangleBuffer->Release();
    //lineBuffer->Release();

    //bool newVertice = true;
    //for (int i = 0; i < sphereMeshes[currentSubdivisions%(sphereMeshes.size())].size(); i++)
    //{
    //    /*for (int j = 0; j < 3; j++)
    //    {
    //        sphereMeshes[currentSubdivisions][i].vertices[j].position.x += 0.1f * 0.01f * (rand() % 101);
    //        sphereMeshes[currentSubdivisions][i].vertices[j].position.y += 0.1f * 0.01f * (rand() % 101);
    //        sphereMeshes[currentSubdivisions][i].vertices[j].position.z += 0.1f * 0.01f * (rand() % 101);
    //    }*/

    //    for (int j = 0; j < 3; j++)
    //    {
    //        newVertice = true;
    //        Vertex vertex = sphereMeshes[currentSubdivisions][i].vertices[j];
    //        vertex.position.Normalize();
    //        vertex.normal = DirectX::SimpleMath::Vector3(36.f / 255.f, 36.f / 255.f, 36.f / 255.f);
    //        vertex.uv = DirectX::SimpleMath::Vector2(1.f);
    //        int loops = vertices.size();
    //        for (int k = 0; k < loops; k++)
    //        {
    //            if (vertices[k] == vertex)
    //            {
    //                std::cout << "X: " << vertices[k].position.x << " VS " << vertex.position.x << "\n";
    //                std::cout << "Y: " << vertices[k].position.y << " VS " << vertex.position.y << "\n";
    //                std::cout << "Z: " << vertices[k].position.z << " VS " << vertex.position.z << "\n\n";

    //                vertices.push_back(vertices[k]);
    //                newVertice = false;
    //                break;
    //            }
    //        }
    //        if (newVertice) vertices.push_back(vertex);
    //       

    //    }

    //    /*vertices.push_back(sphereMeshes[currentSubdivisions][i].vertices[0]);
    //    vertices.back().normal = DirectX::SimpleMath::Vector3(36.f / 255.f, 36.f / 255.f, 36.f / 255.f);
    //    vertices.push_back(sphereMeshes[currentSubdivisions][i].vertices[1]);
    //    vertices.back().normal = DirectX::SimpleMath::Vector3(36.f / 255.f, 36.f / 255.f, 36.f / 255.f);
    //    vertices.push_back(sphereMeshes[currentSubdivisions][i].vertices[2]);
    //    vertices.back().normal = DirectX::SimpleMath::Vector3(36.f / 255.f, 36.f / 255.f, 36.f / 255.f);*/

    //    lines.push_back(sphereMeshes[currentSubdivisions][i].vertices[0]);
    //    lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
    //    lines.push_back(sphereMeshes[currentSubdivisions][i].vertices[1]);
    //    lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
    //    lines.push_back(sphereMeshes[currentSubdivisions][i].vertices[1]);
    //    lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
    //    lines.push_back(sphereMeshes[currentSubdivisions][i].vertices[2]);
    //    lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
    //    lines.push_back(sphereMeshes[currentSubdivisions][i].vertices[2]);
    //    lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
    //    lines.push_back(sphereMeshes[currentSubdivisions][i].vertices[0]);
    //    lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
    //}


    ////vertices[0]->position.z -= 1.0f;
    //for (int i = 0; i < vertices.size(); i++)
    //{
    //    (vertices[i]).position.x += 0.1f * 0.01f * (rand() % 101);
    //    (vertices[i]).position.y += 0.1f * 0.01f * (rand() % 101);
    //    (vertices[i]).position.z += 0.1f * 0.01f * (rand() % 101);
    //    (vertices[i]).position.Normalize();
    //}

    ////lines.clear();
    //for (int i = 0; i < lines.size(); i++)
    //{
    //    lines[i].position.Normalize();
    //}

    //int nr = 60;
    //std::vector<Vertex> plsWork;
    //if (vertices.size() >= nr)
    //{
    //    for (int i = nr-12; i < nr; i++)//vertices.size(); i++)
    //    {
    //        plsWork.emplace_back(vertices[i]);
    //    }
    //}
    //else
    //{
    //    for (int i = 0; i < 3; i++)//vertices.size(); i++)
    //    {
    //        plsWork.emplace_back(vertices[i]);
    //    }
    //}

    //D3D11_BUFFER_DESC bufferDesc = {};
    //bufferDesc.ByteWidth = UINT(sizeof(Vertex) * plsWork.size());
    //bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    //bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    //bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    //bufferDesc.MiscFlags = 0;
    //bufferDesc.StructureByteStride = 0;

    //D3D11_SUBRESOURCE_DATA data = {};
    //
    //data.pSysMem = plsWork.data();
    //data.SysMemPitch = 0;
    //data.SysMemSlicePitch = 0;

    //GPU::device->CreateBuffer(&bufferDesc, &data, &triangleBuffer);

    //bufferDesc.ByteWidth = UINT(sizeof(Vertex) * this->lines.size());
    //data.pSysMem = this->lines.data();

    //GPU::device->CreateBuffer(&bufferDesc, &data, &lineBuffer);

    //lastSubdivisions = planetImGuiStruct.currentSubdivisions;
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
    /*
    posOne = getScalarMultiplicationXMFLOAT3(0.3333333f, getAdditionXMFLOAT3(triangles[i].vertices[2].position, (getAdditionXMFLOAT3(triangles[i].vertices[0].position, triangles[i].vertices[1].position))));
    for (int k = 0; k < 3; k++)
    {
        newTriangle[0].position = posOne;                                   //Middle
        newTriangle[1].position = triangles[i].vertices[k].position;
        newTriangle[2].position = triangles[i].vertices[(k+1)%3].position;
        newTriangleVec.emplace_back(Triangle(newTriangle[0].position, newTriangle[1].position, newTriangle[2].position));
    }
    */

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




    ////Creating icoSphere
    ////https://www.danielsieger.com/blog/2021/01/03/generating-platonic-solids.html

    
    int maxSubdivisions = MAXITERATIONS;
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

    bool newVertice = true;
    int indexSphere = 0;
    Vertex vertex;
    int loops;

    for (int a = 0; a < MAXITERATIONS; a++)
    {
        for (int i = 0; i < sphereMeshes[a].size(); i++)
        {
            for (int j = 0; j < 3; j++)
            {
                newVertice = true;
                vertex = sphereMeshes[a][i].vertices[j];
                vertex.position.Normalize();
                //vertex.normal = DirectX::SimpleMath::Vector3(36.f / 255.f, 36.f / 255.f, 36.f / 255.f);
                vertex.normal.x = std::abs(vertex.position.x);
                vertex.normal.y = std::abs(vertex.position.y);
                vertex.normal.z = std::abs(vertex.position.z);
                vertex.normal.Normalize();
                vertex.uv = DirectX::SimpleMath::Vector2(1.f);
                loops = vertices[a].size();
                for (int k = 0; k < loops; k++)
                {
                    if (vertices[a][k] == vertex)
                    {
                        indices[a].push_back(k);
                        newVertice = false;
                        break;
                    }
                }
                if (newVertice)
                {
                    vertices[a].push_back(vertex);
                    indices[a].push_back(vertices[a].size() - 1);
                }
            }
          

            lines[a].push_back(sphereMeshes[a][i].vertices[0]);
            lines[a].back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
            lines[a].push_back(sphereMeshes[a][i].vertices[1]);
            lines[a].back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
            lines[a].push_back(sphereMeshes[a][i].vertices[1]);
            lines[a].back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
            lines[a].push_back(sphereMeshes[a][i].vertices[2]);
            lines[a].back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
            lines[a].push_back(sphereMeshes[a][i].vertices[2]);
            lines[a].back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
            lines[a].push_back(sphereMeshes[a][i].vertices[0]);
            lines[a].back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
        }
    }
   
    for (int a = 0; a < MAXITERATIONS; a++)
    {
        int lineIndex = 0;
        int newLoops = indices[a].size();
        baseVertices[a] = vertices[a];
        for (int i = 0; i < newLoops; i += 3)
        {

            lines[a][lineIndex++].position = vertices[a][indices[a][i + 0]].position;
            lines[a][lineIndex++].position = vertices[a][indices[a][i + 1]].position;
            lines[a][lineIndex++].position = vertices[a][indices[a][i + 1]].position;
            lines[a][lineIndex++].position = vertices[a][indices[a][i + 2]].position;
            lines[a][lineIndex++].position = vertices[a][indices[a][i + 2]].position;
            lines[a][lineIndex++].position = vertices[a][indices[a][i + 0]].position;

        }
        //for (int i = 0; i < vertices[a].size(); i++)
        //{
        //    vertices[a][i].position.x += (1 - rand()%2) * 0.001f * (rand() % 101);
        //    vertices[a][i].position.y += (1 - rand()%2) * 0.001f * (rand() % 101);
        //    vertices[a][i].position.z += (1 - rand()%2) * 0.001f * (rand() % 101);
        //    //vertices[a][i].position.Normalize();
        //    vertices[a][i].normal.x = std::abs(vertices[a][i].position.x);
        //    vertices[a][i].normal.y = std::abs(vertices[a][i].position.y);
        //    vertices[a][i].normal.z = std::abs(vertices[a][i].position.z);
        //    vertices[a][i].normal.Normalize();
        //}
        //for (int i = 0; i < lines[a].size(); i++)
        //{
        //    lines[a][i].position.Normalize();
        //}
    }
   

}

PlanetGenerator::PlanetGenerator()
    :camera(Camera()), cameraPosition(DirectX::XMFLOAT3(0.f, 0.f, -10.f)), lastSubdivisions(0)
{
 
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
    for (int i = 0; i < MAXITERATIONS; i++)
    {
        triangleBuffer[i]->Release();
        lineBuffer[i]->Release();
    }
    
    vShader->Release();
    pShader->Release();
}

GAMESTATE PlanetGenerator::Update()
{
    if (planetImGuiStruct.recreateMesh || planetImGuiStruct.recreateOriginalSphere) this->recreateMesh();
    if (planetImGuiStruct.updateColours) this->updateColours();
    if (GetAsyncKeyState('I')) this->cameraPosition.y += 0.01f;
    if (GetAsyncKeyState('J')) this->cameraPosition.y -= 0.01f;
    if (GetAsyncKeyState('A')) this->cameraPosition.x -= 0.01f;
    if (GetAsyncKeyState('D')) this->cameraPosition.x += 0.01f;
    if (GetAsyncKeyState('W')) this->cameraPosition.z += 0.01f;
    if (GetAsyncKeyState('S')) this->cameraPosition.z -= 0.01f;
    if (planetImGuiStruct.currentSubdivisions != lastSubdivisions) this->recreateVertexBuffers();
   
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
        GPU::immediateContext->IASetVertexBuffers(0, 1, &triangleBuffer[planetImGuiStruct.currentSubdivisions-1], &stride, &offset);
        GPU::immediateContext->IASetIndexBuffer(indexBuffer[planetImGuiStruct.currentSubdivisions-1], DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
        GPU::immediateContext->DrawIndexed(this->indices[planetImGuiStruct.currentSubdivisions-1].size(), 0, 0);
    }
    if (planetImGuiStruct.renderLines)
    {
        GPU::immediateContext->IASetVertexBuffers(0, 1, &lineBuffer[planetImGuiStruct.currentSubdivisions-1], &stride, &offset);
        GPU::immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        GPU::immediateContext->Draw(this->lines[planetImGuiStruct.currentSubdivisions-1].size(), 0);
    }
    

    imGuiHelper.planetEditor(planetImGuiStruct);
}
