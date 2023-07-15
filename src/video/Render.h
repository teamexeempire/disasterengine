#pragma once
#include "Color.h"
#include "Shader.h"
#include "Sprite.h"
#include "GLExt.h"
#include "Rectangle.h"

namespace video
{
	// OpenGL-based renderer
	class Render
	{
	public:
		static bool Init();

		static void Clear(Color color = Colors::Black);
		static void Draw(const Sprite& sprite, double x, double y, double angle = 0, Color color = Colors::White);
		static void Draw(const Sprite& sprite, double x, double y, const Rectangle& src, double angle = 0, Color color = Colors::White);
		static void Draw(const Sprite& sprite, const Rectangle& dst, double originX = 0, double originY = 0, double angle = 0, Color color = Colors::White);
		static void Draw(const Sprite& sprite, const Rectangle& src, const Rectangle& dst, double originX = 0, double originY = 0, double angle = 0, Color color = Colors::White);
		static void DrawString(std::string string, double x, double y, double scale = 1, Color color = Colors::White);
		
		static bool ShaderUse(Shader& shader);
		static void ShaderReset();
		static void Resize(int width, int height);

	private:
		inline static Shader sprShader;
		inline static Uniform sprImage, sprColor, sprX, sprY, sprSrcWidth, sprSrcHeight, sprWidth, sprHeight;
		inline static GLuint vao;
	};
}