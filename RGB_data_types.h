#pragma once

struct RGB { //units that are going to be projected on the LEDS directly if there were only RGB LEDS. That's why the maximal intensity can be bigger than physically possible on a LED
	float R;
	float G;
	float B;
};

struct RGBW { //units that are going to be projected on the LEDS directly
	float R;
	float G;
	float B;
	float W;
};

struct perceptualRGB { //Units that are scaled 0..1.0. 100% of each mean a neutral white, as defined in the Neutral member.
	float pR;
	float pG;
	float pB;
};

