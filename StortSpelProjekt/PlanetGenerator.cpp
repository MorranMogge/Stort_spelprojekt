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
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;//D3D11_USAGE_IMMUTABLE;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = vertices.data();
    //data.pSysMem = lines.data();
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

    int subdivisions = 1;
    int size = triangles.size();
   /* for (int i = 0; i < subdivisions; i++)
    {
        for (int i = 0; i < size; i++)
        {
            Vertex newTriangle[3];
            newTriangle[0].position = getScalarMultiplicationXMFLOAT3(0.5f, (getAdditionXMFLOAT3(triangles[i].vertices[0].position, triangles[i].vertices[1].position)));
            newTriangle[1].position = getScalarMultiplicationXMFLOAT3(0.5f, (getAdditionXMFLOAT3(triangles[i].vertices[1].position, triangles[i].vertices[2].position)));
            newTriangle[2].position = getScalarMultiplicationXMFLOAT3(0.5f, (getAdditionXMFLOAT3(triangles[i].vertices[2].position, triangles[i].vertices[0].position)));
            triangles.emplace_back(Triangle(newTriangle[0].position, newTriangle[1].position, newTriangle[2].position));
        }
    }*/

    for (int i = 0; i < triangles.size(); i++)
    {
        vertices.push_back(triangles[i].vertices[0]);
        vertices.back().normal = DirectX::SimpleMath::Vector3(36.f / 255.f, 36.f / 255.f, 36.f / 255.f);
        vertices.push_back(triangles[i].vertices[1]);
        vertices.back().normal = DirectX::SimpleMath::Vector3(36.f / 255.f, 36.f / 255.f, 36.f / 255.f);
        vertices.push_back(triangles[i].vertices[2]);
        vertices.back().normal = DirectX::SimpleMath::Vector3(36.f / 255.f, 36.f / 255.f, 36.f / 255.f);

        lines.push_back(triangles[i].vertices[0]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
        lines.push_back(triangles[i].vertices[1]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
        lines.push_back(triangles[i].vertices[1]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
        lines.push_back(triangles[i].vertices[2]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
        lines.push_back(triangles[i].vertices[2]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
        lines.push_back(triangles[i].vertices[0]);
        lines.back().normal = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
    }

    /*for (int i = 0; i < vertices.size(); i++)
    {
        if (getLength(vertices[i].position) != 1.f) std::cout << "LENGTH: " << getLength(vertices[i].position) << "\n";
        vertices[i].position.Normalize();
    }*/
}

PlanetGenerator::PlanetGenerator()
    :camera(Camera()), cameraPosition(DirectX::XMFLOAT3(0.f, 0.f, -10.f))
{
    /*meshes.emplace_back(new Mesh());
    planet = new Planet(meshes[0]);*/
    renderer.initiateRenderer(GPU::immediateContext, GPU::device, GPU::swapChain, GPU::windowWidth, GPU::windowHeight);
    typeOfSphere = PlanetType::ICO;
    this->createInitialSphere();
    this->setVertexBuffers();
    this->setWorldMatrix();
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
    if (GetAsyncKeyState('W')) this->cameraPosition.y += 0.05f;
    if (GetAsyncKeyState('S')) this->cameraPosition.y -= 0.05f;
    if (GetAsyncKeyState('A')) this->cameraPosition.x -= 0.05f;
    if (GetAsyncKeyState('D')) this->cameraPosition.x += 0.05f;

    for (int i = 0; i < vertices.size(); i++)
    {
        if (getLength(vertices[i].position) != 1.f) std::cout << "LENGTH: " << getLength(vertices[i].position) << "\n";
        vertices[i].position.Normalize();
    }
    this->camera.setPosition(cameraPosition);
    return NOCHANGE;
}

void PlanetGenerator::Render()
{
    renderer.setUpScene();
    //GPU::immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
    GPU::immediateContext->VSSetShader(vShader, nullptr, 0);
    GPU::immediateContext->PSSetShader(pShader, nullptr, 0);
    GPU::immediateContext->VSSetConstantBuffers(0, 1, &worldMatrixBuffer);
    
    GPU::immediateContext->IASetVertexBuffers(0, 1, &triangleBuffer, &stride, &offset);
    GPU::immediateContext->Draw(this->vertices.size(), 0);
    
    GPU::immediateContext->IASetVertexBuffers(0, 1, &lineBuffer, &stride, &offset);
    GPU::immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    GPU::immediateContext->Draw(this->lines.size(), 0);

}
