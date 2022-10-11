#pragma once
#include "stdafx.h"

struct Texture final
{
	int width;
	int height;
	int channels;
	unsigned char* color;

	enum class Format{ png, jpg, tga, bmp };

	Texture();
	Texture(const char* loadPath);
	Texture(const std::string loadPath);
	Texture(const unsigned short width, const unsigned short height, const unsigned char channels, unsigned char* color);
	~Texture();

	bool Load(const char* loadPath);
	bool Load(const std::string loadPath);
	void loadDefault();
	void save(const char* filename, const Format imageType);
};

