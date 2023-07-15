#pragma once
#include "GLExt.h"
#include <cstdint>
#include <string>

namespace video
{
	class Sprite
	{
	public:
		Sprite(const std::string& name, const std::string& groupName = "default");
		~Sprite();

		GLuint Width() const;
		GLuint Height() const;

	private:
		friend class Render;

		GLuint id = 0;
		GLuint width = 0, height = 0;
	};
}