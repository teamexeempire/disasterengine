#include "Render.h"
#include <spdlog/spdlog.h>

namespace video
{
    bool Render::Init()
    {
        sprShader.LoadFragment("sprite_fragment.shd", "default");
        sprShader.LoadVertex("sprite_vertex.shd", "default");

        if (!sprShader.Compile())
            return false;

        sprImage = sprShader.GetUniform("uImage");
        sprColor = sprShader.GetUniform("uColor");

        sprX = sprShader.GetUniform("uSX");
        sprY = sprShader.GetUniform("uSY");
        sprSrcWidth = sprShader.GetUniform("uSWidth");
        sprSrcHeight = sprShader.GetUniform("uSHeight");
        sprWidth = sprShader.GetUniform("uWidth");
        sprHeight = sprShader.GetUniform("uHeight");

        unsigned int VBO;
        float vertices[] =
        {
            // pos      // tex
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };

        GLExt::GenVertexArrays(1, &vao);
        GLExt::GenBuffers(1, &VBO);

        GLExt::BindBuffer(GL_ARRAY_BUFFER, VBO);
        GLExt::BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        GLExt::BindVertexArray(vao);
        GLExt::EnableVertexAttribArray(0);
        GLExt::VertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        GLExt::BindBuffer(GL_ARRAY_BUFFER, 0);
        GLExt::BindVertexArray(0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return true;
    }

    void Render::Clear(Color color)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void Render::Draw(const Sprite& sprite, double x, double y, double angle, Color color)
    {
        Draw(sprite, { 0, 0, (double)sprite.width, (double)sprite.height }, { x, y, (double)sprite.width, (double)sprite.height }, 0, 0, angle, color);
    }

    void Render::Draw(const Sprite& sprite, double x, double y, const Rectangle& src, double angle, Color color)
    {
        Draw(sprite, src, { x, y, src.w, src.h }, 0, 0, angle, color);
    }

    void Render::Draw(const Sprite& sprite, const Rectangle& dst, double originX, double originY, double angle, Color color)
    {
        Draw(sprite, { 0, 0, (double)sprite.width, (double)sprite.height }, dst, originX, originY, angle, color);
    }

    void Render::Draw(const Sprite& sprite, const Rectangle& src, const Rectangle& dst, double originX, double originY, double angle, Color color)
    {
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        ShaderUse(sprShader);

        float matrix[16] =
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        GLExt::CreateTransform(matrix, dst.x, dst.y, dst.w, dst.h, angle, originX, originY);

        GLExt::ActiveTexture(GL_TEXTURE0);
        GLExt::BindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, sprite.id);

        sprShader.PassProjection(matrix);
        sprShader.PassUniformi(sprImage, 0);
        sprShader.PassUniformRGBA(sprColor, color);
        sprShader.PassUniformf(sprX, src.x);
        sprShader.PassUniformf(sprY, src.y);
        sprShader.PassUniformf(sprWidth, sprite.width);
        sprShader.PassUniformf(sprHeight, sprite.height);
        sprShader.PassUniformf(sprSrcWidth, src.w);
        sprShader.PassUniformf(sprSrcHeight, src.h);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        GLExt::ActiveTexture(0);
        GLExt::BindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        ShaderReset();
    }

    void Render::DrawString(std::string string, double x, double y, double scale, Color color)
    {
        static Sprite font = { "default_font.png" };

        Rectangle dstRect = { x, y, 8 * scale, 6 * scale };
        Rectangle srcRect = { 0, 0, 8, 6 };
        for (size_t i = 0; i < string.length(); i++)
        {
            char c = tolower(string[i]);
            switch (c)
            {
            case '\n':
                dstRect.x = x;
                dstRect.y += 9 * scale;
                continue;

            case ' ':
                dstRect.x += 8 * scale;
                continue;

            case '-':
                srcRect.x = 26 * 8;
                break;

            case ',':
                srcRect.x = 27 * 8;
                break;

            case '1':
                srcRect.x = 28 * 8;
                break;

            case '2':
                srcRect.x = 29 * 8;
                break;

            case '3':
                srcRect.x = 30 * 8;
                break;

            case '4':
                srcRect.x = 31 * 8;
                break;

            case '5':
                srcRect.x = 32 * 8;
                break;

            case '6':
                srcRect.x = 33 * 8;
                break;

            case '7':
                srcRect.x = 34 * 8;
                break;

            case '8':
                srcRect.x = 35 * 8;
                break;

            case '9':
                srcRect.x = 36 * 8;
                break;

            case '0':
                srcRect.x = 37 * 8;
                break;

            case '.':
                srcRect.x = 38 * 8;
                break;
            default:
                srcRect.x = ((int)c - 97) * 8;
                break;
            }

            Draw(font, srcRect, dstRect, 0, 0, 0, color);
            dstRect.x += 6 * scale;

            switch (c)
            {
            case 'w':
            case 'm':
            case 'x':
            case 'n':
                dstRect.x += 2 * scale;
                break;
            }
        }
    }

    bool Render::ShaderUse(Shader& shader)
    {
        if (!shader.compiled)
        {
            spdlog::warn("Use of not compiled shader.");
            return false;
        }

        GLExt::UseProgram(shader.source);
        return true;
    }

    void Render::ShaderReset()
    {
        GLExt::UseProgram(0);
    }

    void Render::Resize(int width, int height)
    {
        glViewport(0, 0, width, height);
    }
}