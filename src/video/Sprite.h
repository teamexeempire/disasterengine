#pragma once
#include "GLExt.h"
#include <cstdint>
#include <string>

class Resource;
namespace video
{
	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		bool Load(const uint8_t* data, uint64_t size);

		GLuint Width() const;
		GLuint Height() const;

	private:
		friend class Render;

		GLuint id = 0;
		GLuint width = 0, height = 0;
	};
}