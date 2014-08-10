#pragma once

#include <gl/glew.h>
#include <sstream>
#include <windows.h>

enum FilterType
{
	FILTER_MIN_NEAREST,
	FILTER_MIN_LINEAR,
	FILTER_MIN_NEAREST_MIPMAP,
	FILTER_MIN_LINEAR_MIPMAP_NEAREST,
	FILTER_MIN_LINEAR_MIPMAP_LINEAR,
	FILTER_MAG_NEAREST,
	FILTER_MAG_LINEAR,
};

class Texture
{
public:
	Texture();
	void Bind(int i = 0);
	void Release();

	bool LoadTexture(std::string path, bool generateMipMaps = false);
	void CreateTexture(BYTE* data, int width, int height, int bpp, GLenum format, bool generateMipMaps);

	void SetFilterType(int minif, int magnif);

private:
	unsigned int textureID;
	unsigned int samplerID;
	int width;
	int height;
	int bpp;
	bool generateMipMaps = false;
	std::string path;
	int minification;
	int	magnification;
};