#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
    float r,g,b;

    Color() { r = 0; g = 0; b = 0;}
    Color(float rr, float gg, float bb)
    {
        r = rr;
        g = gg;
        b = bb;
    }
    Color operator*(float num)
    {
        return Color(r * num, g * num, b * num);
    }
    Color operator*(Color c)
    {
        return Color(r * c.r, g * c.g, b * c.b);
    }

    Color operator+(Color c)
    {
        return Color(c.r + r, c.g + g, c.b + b);
    }
    Color operator-(Color c)
    {
        return Color(c.r - r, c.g - g, c.b - b);
    }
};

#endif