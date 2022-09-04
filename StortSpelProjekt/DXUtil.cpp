#include "DXUtil.h"

void Utility::GetTextureDimentions(ID3D11Resource* res, UINT* width, UINT* height)
{
	//What can be stored in a Resource can be anything from 1D to 4D texture so  we need to get what type it is.
	D3D11_RESOURCE_DIMENSION dim;
	res->GetType(&dim);
	switch (dim)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		{
			auto texture = reinterpret_cast<ID3D11Texture2D*>(res);
			D3D11_TEXTURE2D_DESC desc;
			texture->GetDesc(&desc);
			if (width) *width = desc.Width;		//Width of texture in pixels
			if (height) *height = desc.Height;  //Height of texture in pixels

		}
		break;

	default: 
		{
			if (width) *width = 0;		//Width of texture in pixels
			if (height) *height = 0;
		}
		break;

	}

}
