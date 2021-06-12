#pragma once

#include "Arduino.h"

namespace libRGBW {

struct RGB { //units that are going to be projected on the LEDS directly if there were only RGB LEDS. That's why the maximal intensity can be bigger than physically possible on a LED
    constexpr RGB(uint32_t c) : R(static_cast<uint8_t>(c >> 16)),
                                G(static_cast<uint8_t>(c >> 8)),
                                B(static_cast<uint8_t>(c)) {}

    constexpr RGB(float r, float g, float b) : R(r), G(g), B(b) {}
    constexpr RGB() : R(0), G(0), B(0) {}

    float R;
    float G;
    float B;

    operator uint32_t() {
        return ((static_cast<uint32_t>(R) << 16) |
                (static_cast<uint32_t>(G) << 8) | static_cast<uint8_t>(B));
    }
};

struct RGBW { //units that are going to be projected on the LEDS directly
    constexpr RGBW(uint32_t c) : R(static_cast<uint8_t>(c >> 16)),
                                 G(static_cast<uint8_t>(c >> 8)),
                                 B(static_cast<uint8_t>(c)),
                                 W(static_cast<uint8_t>(c >> 24)) {}

    constexpr RGBW(float r, float g, float b, float w) : R(r), G(g), B(b), W(w) {}
    constexpr RGBW() : R(0), G(0), B(0), W(0) {}

    float R;
    float G;
    float B;
    float W;

    operator uint32_t() {
        return (static_cast<uint32_t>(W) << 24) | (static_cast<uint32_t>(R) << 16) |
               (static_cast<uint32_t>(G) << 8) | static_cast<uint8_t>(B);

    }
};

struct perceptualRGB { //Units that are scaled 0..1.0. 100% of each mean a neutral white, as defined in the Neutral member.
    constexpr perceptualRGB() : pR(0), pG(0), pB(0) {}

    perceptualRGB(float r, float g, float b) : pR(r), pG(g), pB(b) {}

    float pR;
    float pG;
    float pB;
};

perceptualRGB operator*(const perceptualRGB &rgb1, const float f);


perceptualRGB operator+(const perceptualRGB &rgb1, const perceptualRGB &rgb2);

perceptualRGB operator+=(perceptualRGB rgb1, perceptualRGB rgb2);
}
