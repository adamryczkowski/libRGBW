#pragma once
#include "RGB_data_types.h"

namespace libRGBW {


class PerceptualConverter;

//Class that converts 4-dimensional RGBW into 3-dimensional normalized RGB (i.e. only hue) + intensity
//Internaly it does not store any color, only the calibration data
//Additionally it contains a nice driver for color and its intensity based on the time of day.
class WhiteConverter {
public:
//	WhiteConverter(RGB whiteLEDColor, RGB neutralWhiteColor) : WhiteLED(whiteLEDColor), Neutral(neutralWhiteColor) { }
	WhiteConverter(RGB whiteLEDColor, RGBW maxValue) : m_WhiteLED(whiteLEDColor), m_max(maxValue) { }
	WhiteConverter() { }
//	RGBW getRGBW(perceptualRGB color); //Returns RGBW that is equivalent to the color, but uses white LED as much as possible to achieve more brightness
//	RGBW getRGBW(perceptualRGB color, float intensity); //Returns RGBW that is equivalent to the color, but uses white LED as much as possible to achieve more brightness
	RGBW getRGBW(RGB color, float amplify=1.0); //Returns RGBW that is equivalent to the color, but uses white LED as much as possible to achieve more brightness
//	RGBW getRGBW(RGB color, float intensity); //Returns RGBW that is equivalent to the color, but uses white LED as much as possible to achieve more brightness
	RGB  getRGB(RGBW color); //Inverse function that returns RGB from RGBW
//	RGB  getRGB(perceptualRGB color); //Gets raw RGB from perceptualRGB by using the definition of the Neutral white
	float calculateMaxAmplification(RGB color); //calculates maximum achievable intensity with the color color
//	float calculateMaxIntensity(perceptualRGB color); //calculates maximum achievable intensity with the color color
//	float getIntensity(RGB color); //calculates current intensity of the color
//	float getIntensity(perceptualRGB pcolor); //calculates current intensity of the color
//	float getIntensity(RGBW color);

	RGBW AbsoluteMaxIntensity() {return m_max;}

private:
	RGB m_WhiteLED = {255./26, 88./26, 8./26};
//	RGB m_WhiteLED = {255./26, 88./26, 8./26};
	RGBW m_max = {255., 255., 255., 255.}; //maximum intensity of light supported by LEDS
	float ile_razy_rozjasnic(RGB rgb, float target_natezenie);
};


//class that allows perceptual RGB channels, that are normalized so that 1.0 equal the intensity of the unit neutral light
class PerceptualConverter{
public:
	PerceptualConverter(RGB neutralWhiteColor) : m_Neutral(neutralWhiteColor) { }
	PerceptualConverter() { }
	perceptualRGB convertFromRGB(RGB input);
	RGB convertToRGB(perceptualRGB input, float amplitude=1.0);
	float getIntensity(RGB color); //calculates current intensity of the color, based on the intensity of the neutral light as 100%
	float getIntensity(perceptualRGB pcolor); //calculates current intensity of the color
	void setNeutralColor(RGB neutralWhiteColor) {m_Neutral=neutralWhiteColor;}
	perceptualRGB Temperature2RGB(float temperature, float intensity=1.0);
//	RGBW ColorFromTime(float time, float intensity=100.); //Both hue and intensity depends on time, but additionally the RGBW is scaled by the intensity variable
private:
	RGB m_Neutral = {255., 152., 52.}; //only color, intensity is irrelevant
};

}
