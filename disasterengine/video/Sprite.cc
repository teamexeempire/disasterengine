#include "Sprite.h"
#include "GLExt.h"
#include <SDL2/SDL_image.h>
#include <spdlog/spdlog.h>

namespace video
{
	Sprite::Sprite()
	{
	}

	Sprite::~Sprite()
	{
		glDeleteTextures(1, &id);
	}

	bool Sprite::Load(const uint8_t* data, uint64_t size)
	{
		if (!data)
		{
			spdlog::error("Failed to load from resource: Resource is nullptr_t");
			return false;
		}

		SDL_RWops* rw = SDL_RWFromConstMem(data, size);
		SDL_Surface* sur = IMG_Load_RW(rw, true);
		if (!sur)
		{
			spdlog::error("Failed to load from resource: {}",IMG_GetError());
			return false;
		}

		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		int mode = GL_RGB;
		int rgb = 3;

		if (sur->format->BytesPerPixel == 4)
		{
			mode = GL_RGBA;
			rgb = 4;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, rgb, sur->w, sur->h, 0, mode, GL_UNSIGNED_BYTE, sur->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		width = sur->w;
		height = sur->h;

		SDL_FreeSurface(sur);
		return true;
	}

	GLuint Sprite::Width() const
	{
		return width;
	}

	GLuint Sprite::Height() const
	{
		return height;
	}
}