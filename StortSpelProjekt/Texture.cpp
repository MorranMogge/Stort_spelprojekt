#include "stdafx.h"
#include "Texture.h"
#include <filesystem>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture::Texture() :width(0), height(0), channels(0), color(nullptr) {}

Texture::Texture(const char* loadPath)
{
	Load(loadPath);
}

Texture::Texture(const std::string loadPath)
{
	Load(loadPath.c_str());
}

Texture::Texture(const unsigned short width, const unsigned short height, const unsigned char channels, unsigned char* color)
	:width(width),height(height),channels(channels), color(color) {}

Texture::~Texture()
{
	stbi_image_free(color);
}

bool Texture::Load(const char* loadPath)
{
	if (std::filesystem::exists(loadPath))
	{
		color = stbi_load(loadPath, &width, &height, &channels, STBI_rgb_alpha);
		return true;
	}
	else
	{
		loadDefault();
		return false;
	}
}

bool Texture::Load(const std::string loadPath)
{
	return Load(loadPath.c_str());;
}

void Texture::loadDefault()
{
	color = new unsigned char[4]{ 127, 127, 127 };
	width = 1;
	height = 1;
	channels = 4;
}

void Texture::save(const char* filename, const Format imageFormat)
{
	std::string ggop(filename);

	switch (imageFormat)
	{
	case Texture::Format::png:
		ggop += ".png";
		stbi_write_png(ggop.c_str(), width, height, channels, color, width * channels);
		break;
	case Texture::Format::jpg:
		ggop += ".jpg";
		stbi_write_jpg(ggop.c_str(), width, height, channels, color, width * channels);
		break;
	case Texture::Format::tga:
		ggop += ".tga";
		stbi_write_tga(ggop.c_str(), width, height, channels, color);
		break;
	case Texture::Format::bmp:
		ggop += ".bmp";
		stbi_write_bmp(ggop.c_str(), width, height, channels, color);
		break;
	}
}
