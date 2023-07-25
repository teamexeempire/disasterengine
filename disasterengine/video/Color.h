#pragma once

namespace video
{
    class Color
    {
    public:
        inline Color() { r = 0; g = 0; b = 0; a = 0; }
        inline Color(int r, int g, int b, int a = 255) { this->r = r / 255.0f; this->g = g / 255.0f; this->b = b / 255.0f; this->a = a / 255.0f; }
        inline Color(float r, float g, float b, float a = 1.0f) { this->r = r; this->g = g; this->b = b; this->a = a; }

        float r, g, b, a;
    };

    namespace Colors
    {
        static const Color Transparent = { 0.f, 0.f, 0.f, 0.f };
        static const Color Black = { 0.f, 0.f, 0.f };
        static const Color White = { 1.f, 1.f, 1.f };
        static const Color Red = { 1.f, 0.f, 0.f };
        static const Color Green = { 0.f, 1.f, 0.f };
        static const Color Blue = { 0.f, 0.f, 1.f };
    }
}