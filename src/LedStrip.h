#pragma once

#include "Color.h"
#include "Adafruit_NeoPixel.h"

enum PixelMixMode {
    PMM_NORMAL = 0,
    PMM_ADD = 1,
    PMM_SUBTRACT = 2,
    PMM_MULTIPLY = 3,
    PMM_MAX = 4,
    PMM_MIN = 5
};

class LedStrip
{
private:
    Adafruit_NeoPixel strip;
    unsigned long *array;
    int len;

public:
    LedStrip(int lenght, int pin);
    void Fill(Color clr);
    void SetPixel(int pos, Color clr, PixelMixMode mode = PMM_NORMAL);
    void SetLine(int pos1, int pos2, Color clr, PixelMixMode mode = PMM_NORMAL);
    void SetGradient(int pos1, int pos2, Color clr1, Color clr2, PixelMixMode mode = PMM_NORMAL);
    void Write();
    void Print();
};

LedStrip::LedStrip(int lenght, int pin)
{
    strip = Adafruit_NeoPixel(lenght, pin, NEO_GRB + NEO_KHZ800);
    strip.begin();
    array = (unsigned long *)malloc(lenght);
    len = lenght;
}

void LedStrip::Fill(Color clr)
{
    for (size_t i = 0; i < len; i++)
    {
        array[i] = clr;
    }
}

void LedStrip::SetPixel(int pos, Color clr, PixelMixMode mode)
{
    if (pos < 0)
        pos = len - abs(pos % len);
    else
        pos = pos % len;

    Color prev;

    switch (mode)
    {
    case PMM_NORMAL:
        array[pos] = clr;
        break;
    case PMM_ADD:
        prev = Color(array[pos]);
        array[pos] = Color(MIN(prev.R + clr.R, 0xFF), MIN(prev.G + clr.G, 0xFF), MIN(prev.B + clr.B, 0xFF));
        break;
    case PMM_SUBTRACT:
        prev = Color(array[pos]);
        array[pos] = Color(MAX(prev.R - clr.R, 0x00), MAX(prev.G - clr.G, 0x00), MAX(prev.B - clr.B, 0x00));
        break;
    case PMM_MULTIPLY:
        prev = Color(array[pos]);
        array[pos] = Color(prev.R * clr.R / 255, prev.G * clr.G / 255, prev.B * clr.B / 255);
        break;
    case PMM_MAX:
        prev = Color(array[pos]);
        array[pos] = Color(MAX(prev.R, clr.R), MAX(prev.G, clr.G), MAX(prev.B, clr.B));
        break;
    case PMM_MIN:
        prev = Color(array[pos]);
        array[pos] = Color(MIN(prev.R, clr.R), MIN(prev.G, clr.G), MIN(prev.B, clr.B));
        break;
    default:
        array[pos] = 0;
        break;
    }

    
}

void LedStrip::Write()
{
    for (size_t i = 0; i < len; i++)
    {
        strip.setPixelColor(i, array[i]);
    }
    strip.show();
}

void LedStrip::SetLine(int pos1, int pos2, Color clr, PixelMixMode mode)
{
    for (int i = pos1; i <= pos2; i++)
    {
        SetPixel(i, clr, mode);
    }
}

void LedStrip::SetGradient(int pos1, int pos2, Color clr1, Color clr2, PixelMixMode mode)
{
    for (int i = pos1; i <= pos2; i++)
    {
        float lerp = i - pos1;
        lerp /= pos2 - pos1;
        Color clr = Color::MergeColors(clr1, clr2, lerp);
        SetPixel(i, clr, mode);
    }
}

void LedStrip::Print()
{
    for (size_t i = 0; i < len; i++)
    {
        printf("%06x\n", (unsigned long)array[i]);
    }
}