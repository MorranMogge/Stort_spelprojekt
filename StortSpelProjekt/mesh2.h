#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <directxmath.h>
#include "ErrorLog.h"


struct vertex
{
    DirectX::XMFLOAT3 pos; // Position
    DirectX::XMFLOAT2 uv; // UV coordination
    DirectX::XMFLOAT3 nor; // Normal

    vertex() {
        pos = DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f };
        uv = DirectX::XMFLOAT2{ 0.0f,0.0f };
        nor = DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f };
    };
    vertex(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT2& uv, DirectX::XMFLOAT3& nor) : pos(pos), uv(uv), nor(nor) {};
};

class Mesh2
{
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    ID3D11Device* device;
    //Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
    DWORD size = 0;

protected:
    HRESULT CreateIndexBuffer(std::vector<DWORD>& indexTriangle);
    HRESULT CreateVertexBuffer(std::vector<vertex>& vertexTriangle);
public:
    Mesh2(ID3D11Device* device, std::vector<vertex>& vertexTriangle, std::vector<DWORD>& indexTriangle);
    void Draw(ID3D11DeviceContext* immediateContext);
    Mesh2(const Mesh2& mesh);
    //ID3D11Buffer* getIndexBuffer() const;
    //ID3D11Buffer* getVertexBuffer() const;
    ~Mesh2();
};