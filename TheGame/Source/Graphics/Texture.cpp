//
// COMP 371 Term Project
//
// Created by
// Boutas, Vasiliki   (6220304)
// Di Girolamo, John  (6202918)
// Ozgaon, Dror Asher (6296742)
// Siekut, David      (6329810)
// Tran, Quang        (6339816)
// Wan, Kwok - Chak   (6291643)
//
// Contributions to this file:
// David Siekut
//

#include "Texture.h"
#include <FreeImage.h>
//#pragma comment(lib, "FreeImage.lib")

Texture::Texture()
{

}

void Texture::Bind(int i)
{
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBindSampler(i, samplerID);
}

void Texture::Release()
{
	glDeleteSamplers(1, &samplerID);
	glDeleteTextures(1, &textureID);
}

bool Texture::LoadTexture(std::string path, bool generateMipMaps)
{
	FREE_IMAGE_FORMAT imageFormat = FIF_UNKNOWN;
	FIBITMAP* bitmap(0);

	// get image format
	imageFormat = FreeImage_GetFileType(path.c_str(), 0);

	// load image if it can be read
	if (FreeImage_FIFSupportsReading(imageFormat))
		bitmap = FreeImage_Load(imageFormat, path.c_str());

	// load failed
	if (!bitmap)
		return false;

	// get image bytes
	BYTE* imageBytes = FreeImage_GetBits(bitmap);

	// check some stuff
	GLenum format = FreeImage_GetBPP(bitmap) == 24 ? GL_BGR : FreeImage_GetBPP(bitmap) == 8 ? GL_LUMINANCE : 0;

	// get image properties and create the texture
	CreateTexture(imageBytes, FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap), FreeImage_GetBPP(bitmap), format, generateMipMaps);

	// texture is loaded, we can unload the image bitmap
	FreeImage_Unload(bitmap);

	this->path = path;

	return true;
}

void Texture::CreateTexture(BYTE* data, int width, int height, int bpp, GLenum format, bool generateMipMaps)
{
	// generate textureid for this texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	if (format == GL_RGB || format == GL_BGR) // We must handle this because of internal format parameter
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		if (generateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		if (generateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
	}

	glGenSamplers(1, &samplerID);

	this->path = "";
	this->generateMipMaps = generateMipMaps;
	this->width = width;
	this->height = height;
	this->bpp = bpp;
}

void Texture::SetFilterType(int minif, int magnif)
{
	//http://www.opengl.org/sdk/docs/man/html/glSamplerParameter.xhtml

	// set texture minification
	if (minif == FILTER_MIN_NEAREST)
		glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if (minif == FILTER_MIN_LINEAR)
		glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if (minif == FILTER_MIN_NEAREST_MIPMAP)
		glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if (minif == FILTER_MIN_LINEAR_MIPMAP_NEAREST)
		glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if (minif == FILTER_MIN_LINEAR_MIPMAP_LINEAR)
		glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// set texture magnification
	if (magnif == FILTER_MAG_NEAREST)
		glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else if (magnif == FILTER_MAG_LINEAR)
		glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	this->minification = minif;
	this->magnification = magnif;
}