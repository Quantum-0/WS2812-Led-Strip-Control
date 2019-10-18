#pragma once

#define HUE_SECTOR_SIZE 256
#define HUES (HUE_SECTOR_SIZE * 6 - 1)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#include <inttypes.h>

class Color
{
public:
    uint8_t R, G, B;
    Color(int red, int green, int blue);
    Color(ulong hex);
    Color(){};
    ~Color(){};
    operator int() const
    {
        return (long)R << 16 | (long)G << 8 | (long)B;
    }
    static Color FromHSV(int hue, int saturation, int value);
    static Color MergeColors(Color clr1, Color clr2, float val);
};

Color::Color(int red, int green, int blue)
{
    R = red;
    G = green;
    B = blue;
}

Color::Color(ulong hex)
{
    R = (hex & 0xFF0000) >> 16;
    G = (hex & 0x00FF00) >> 8;
    B = (hex & 0x0000FF) >> 0;
}

Color Color::FromHSV(int hue, int saturation, int value)
{
    int i, f;
    if (hue < 0)
        hue = HUES - abs(hue % HUES);
    else
        hue = hue % HUES;
    printf("hue = %d   ", hue * 80);
    i = hue / HUE_SECTOR_SIZE;
    f = hue - (i * HUE_SECTOR_SIZE);
    switch (i)
    {
    case 0:
        return Color(255, f, 0);
    case 1:
        return Color(255 - f, 255, 0);
    case 2:
        return Color(0, 255, f);
    case 3:
        return Color(0, 255 - f, 255);
    case 4:
        return Color(f, 0, 255);
    case 5:
        return Color(255, 0, 255 - f);
    }
}

Color Color::MergeColors(Color clr1, Color clr2, float val)
{
    int r = clr2.R * val + clr1.R * (1 - val);
    int g = clr2.G * val + clr1.G * (1 - val);
    int b = clr2.B * val + clr1.B * (1 - val);
    return Color(r, g, b);
}