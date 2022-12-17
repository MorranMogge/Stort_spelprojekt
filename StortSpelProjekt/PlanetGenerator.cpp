#include "stdafx.h"
#include "PlanetGenerator.h"
#include "GPU.h"
#include "Input.h"
#include "ObjSaver.h"

#include "stdafx.h"
#include "PlanetGenerator.h"
#include "GPU.h"
#include "Input.h"
#include "ShaderLoader.h"
#include "DirectXMathHelper.h"
#include "stb_image.h"

void PlanetGenerator::calculateUVValues()
{   
    DirectX::SimpleMath::Vector3 newVec;

    for (int i = 0; i < vertices[planetImGuiStruct.currentSubdivisions-1].size(); i++)
    {
        newVec = vertices[planetImGuiStruct.currentSubdivisions - 1][i].position;
        newVec.Normalize();
        vertices[planetImGuiStruct.currentSubdivisions - 1][i].uv.x = std::atan2(newVec.x, newVec.z) / (2 * DirectX::XM_PI) + 0.5;
        vertices[planetImGuiStruct.currentSubdivisions - 1][i].uv.y = newVec.y * 0.5 + 0.5;
    }
}

bool PlanetGenerator::addTextures()
{
    int width, height, channel;
    unsigned char* picture = stbi_load("../Textures/Red.png", &width, &height, &channel, STBI_rgb_alpha);

    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

    textureDesc.Width = (UINT)width;
    textureDesc.Height = (UINT)height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.MiscFlags = 0;
    textureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA subResource;
    ZeroMemory(&subResource, sizeof(D3D11_SUBRESOURCE_DATA));

    subResource.pSysMem = picture;
    subResource.SysMemPitch = (UINT)width * 4;

    ID3D11Texture2D* texture = {};
    HRESULT hr = GPU::device->CreateTexture2D(&textureDesc, &subResource, &texture);
    if (FAILED(hr)) { std::cout << "Could not create Texture2D!\n"; return false; }

    if (texture != nullptr)
    {
        ID3D11ShaderResourceView* newSrv = nullptr;
        hr = GPU::device->CreateShaderResourceView(texture, nullptr, &newSrv);
        if (FAILED(hr)) { std::cout << "Could not create srv!\n"; return false; }
        planetTexutes.push_back(newSrv);
    }

    stbi_image_free(picture);
    texture->Release();
    return !FAILED(hr);
}

void PlanetGenerator::updateColours()
{
    int index = planetImGuiStruct.currentSubdivisions - 1;
    float length = 0;
    int loops = vertices[index].size();

    planetImGuiStruct.minLength = 10;
    planetImGuiStruct.maxLength = 0;

    for (int i = 0; i < loops; i++)
    {
        length = vertices[index][i].position.Length();
        if (length < planetImGuiStruct.minLength) planetImGuiStruct.minLength = length;
        if (length > planetImGuiStruct.maxLength) planetImGuiStruct.maxLength = length;
    }

    bool ground = true;
    for (int i = 0; i < loops; i++)
    {
        ground = true;
        length = vertices[index][i].position.Length();
        if (planetImGuiStruct.useColours > 1 && length < planetImGuiStruct.colourFactor[1]) { vertices[index][i].normal = planetImGuiStruct.colourSelection[1]; ground = false; }
        if (planetImGuiStruct.useColours > 2 && length > planetImGuiStruct.colourFactor[2]) { vertices[index][i].normal = planetImGuiStruct.colourSelection[2]; ground = false; }
        if (planetImGuiStruct.useColours > 3 && length < planetImGuiStruct.colourFactor[3]) { vertices[index][i].normal = planetImGuiStruct.colourSelection[3]; ground = false; }
        if (planetImGuiStruct.useColours > 4 && length > planetImGuiStruct.colourFactor[4]) { vertices[index][i].normal = planetImGuiStruct.colourSelection[4]; ground = false; }
        if (ground) vertices[index][i].normal = planetImGuiStruct.colourSelection[0];
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

    planetImGuiStruct.recreateOriginalSphere = false;
    planetImGuiStruct.recreateMesh = false;
    this->updateVertexBuffer();
    this->calculateUVValues();
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
    //Not decided on whether or not it will be implemented
}

void PlanetGenerator::createUVSphere()
{
    //Not decided on whether or not it will be implemented
}

void PlanetGenerator::createIcoSphere()
{

    DirectX::XMFLOAT3 icoSphere[12] =
    {
        {0.f,  GoldenRatio, -1.f},  
        { GoldenRatio,  1.f, 0.f},  
        {-GoldenRatio,  1.f, 0.f},  
        {0.f,  GoldenRatio,  1.f},  
        {0.f, -GoldenRatio,  1.f},  
        {-1.f, 0.f,  GoldenRatio},  
        {0.f, -GoldenRatio, -1.f},  
        { 1.f, 0.f, -GoldenRatio},  
        { 1.f, 0.f,  GoldenRatio},  
        {-1.f, 0.f, -GoldenRatio},  
        { GoldenRatio, -1.f, 0.f},  
        {-GoldenRatio, -1.f, 0.f}  
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


    //Creating icoSphere
    //https://www.danielsieger.com/blog/2021/01/03/generating-platonic-solids.html

    
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
    }
   
    if (! this->addTextures()) std::cout << "Could not load texture\n";
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
    planetImGuiStruct.colourFactor[0] = 1.0f;
    planetImGuiStruct.colourFactor[1] = 0.95f;
    planetImGuiStruct.colourFactor[2] = 1.1f;
    planetImGuiStruct.colourFactor[3] = 0.85f;
    planetImGuiStruct.colourFactor[4] = 1.2f;

    for (int i = 0; i < 5; i++)
    {
        planetImGuiStruct.colourSelection[i].x = (rand() % 256) / 256.f;
        planetImGuiStruct.colourSelection[i].y = (rand() % 256) / 256.f;
        planetImGuiStruct.colourSelection[i].z = (rand() % 256) / 256.f;
    }

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
    if (planetImGuiStruct.saveObj) { saveObj(planetImGuiStruct.fileName, vertices[planetImGuiStruct.currentSubdivisions-1], indices[planetImGuiStruct.currentSubdivisions-1]); planetImGuiStruct.saveObj = false; }
   
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
