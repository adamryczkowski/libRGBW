#include "libRGBW.h"
#include "Arduino.h"

namespace libRGBW {


RGBW WhiteConverter::getRGBW(RGB rgb, float amplify) {
	RGB ile_jest_bialego_w;
	ile_jest_bialego_w.R = amplify * rgb.R / m_WhiteLED.R; //ile jednostek białego jest w aktualnym czerwonym
	ile_jest_bialego_w.G = amplify * rgb.G / m_WhiteLED.G;
	ile_jest_bialego_w.B = amplify * rgb.B / m_WhiteLED.B;

	RGBW result;

	result.W = min(ile_jest_bialego_w.R, min(ile_jest_bialego_w.G, ile_jest_bialego_w.B));
	if(result.W > m_max.W) {
		//musimy część białego zaświecić RGB
		result.W = m_max.W;
	}
	result.R = amplify * rgb.R - result.W * m_WhiteLED.R;
	result.G = amplify * rgb.G - result.W * m_WhiteLED.G;
	result.B = amplify * rgb.B - result.W * m_WhiteLED.B;

	return result;
}

RGB WhiteConverter::getRGB(RGBW kolor) {
//	RGB RGB_from_RGBW(RGBW kolor, float& intensywnosc) {
	RGB result;
	result.R = kolor.R + kolor.W * m_WhiteLED.R;
	result.G = kolor.G + kolor.W * m_WhiteLED.G;
	result.B = kolor.B + kolor.W * m_WhiteLED.B;

	return result;
}

float WhiteConverter::calculateMaxAmplification(RGB rgb){
	// Algorytm:
	// 1. Sprawdzamy, co najbardziej ogranicza nasze światło w rozjaśnianiu: R, G, B czy W

	RGBW rgbw = getRGBW(rgb);
	float maxC = max(max(rgbw.R, rgbw.G), max(rgbw.B, rgbw.W));
	RGBW absMax = AbsoluteMaxIntensity();

	if(rgbw.W == maxC) {
		//Kolor jest zdominowany przez biały; można jeszcze uzyskać trochę więcej światła dodając RGB
		//
		//Ile razy można zwiększyć intenstywność, aby uzyskać 100% białego.
		//Nie uwzględniająmy tego, że możemy dosztukować jeszcze trochę zadanej barwy używając samych diód RGB.
		float max_factor_white = absMax.W / rgbw.W;
		RGB res; //ile jeszcze zostało nam mocy diód RGB (już bez W, bo W=100%) do doświecenia, aby jeszcze dodać ciut-ciut więcej światła
		res.R = absMax.R - rgbw.R * max_factor_white;
		res.G = absMax.G - rgbw.G * max_factor_white;
		res.B = absMax.B - rgbw.B * max_factor_white;

		//Teraz musimy policzyć, ile z tego RGB jest w kierunku naszego rgb
		RGB tmp; //res=residuum, czyli reszta. Określa, ile światła mamy jeszcze nie wykorzystanego w diodach R, G i B.
		if(rgb.R>0)
			tmp.R = res.R / rgb.R; //ile razy możemy dodatkowo zwiększyć kanał R, zanim wykorzystamy 100% mocy tego kanału?
		else
			tmp.R = 100 * absMax.R; //Arbitrarly large number, i.e. infinity

		if(rgb.G>0)
			tmp.G = res.G / rgb.G;
		else
			tmp.G = 100 * absMax.G; //infinity

		if(rgb.B>0)
			tmp.B = res.B / rgb.B;
		else
			tmp.B = 100 * absMax.B; //infinity

		float max_brightness = min(tmp.R, min(tmp.G, tmp.B));//jaką część światła o kolorze rgb można dodać używając rgb. Jak pomnożymy przez intensywność naszego obecnego światła, to dostaniemy wartość w jednostce natężenia.
		return max_brightness + max_factor_white;
	} else {
		if(rgbw.R == maxC) {
			return absMax.R / rgbw.R;
		} else if (rgbw.G == maxC) {
			return absMax.G / rgbw.G;
		} else if (rgbw.B == maxC) {
			return absMax.B / rgbw.B;
		} else {
			return -1;//error
		}
	}
}


float PerceptualConverter::getIntensity(RGB rgb) {
	return(sqrt((rgb.R*rgb.R)/(m_Neutral.R*m_Neutral.R)+
			    (rgb.G*rgb.G)/(m_Neutral.G*m_Neutral.G)+
				(rgb.B*rgb.B)/(m_Neutral.B*m_Neutral.B))*0.577350269); // 0.577350269=1/sqrt(3)
}


float PerceptualConverter::getIntensity(perceptualRGB pcolor) {
	return(sqrt((pcolor.pR*pcolor.pR)+(pcolor.pG*pcolor.pG)+(pcolor.pB*pcolor.pB))*0.577350269); // 0.577350269=1/sqrt(3)
}


perceptualRGB PerceptualConverter::convertFromRGB(RGB rgb) {
	perceptualRGB rgbp;
	rgbp.pR = rgb.R/m_Neutral.R;
	rgbp.pG = rgb.G/m_Neutral.G;
	rgbp.pB = rgb.B/m_Neutral.B;
	return rgbp;
}

RGB PerceptualConverter::convertToRGB(perceptualRGB rgbp, float amplitude) {
	RGB rgb;
	rgb.R = rgbp.pR*m_Neutral.R*amplitude;
	rgb.G = rgbp.pG*m_Neutral.G*amplitude;
	rgb.B = rgbp.pB*m_Neutral.B*amplitude;
	return rgb;
}


perceptualRGB PerceptualConverter::Temperature2RGB(float temperature, float intensity){
    temperature = temperature / 100.;

    perceptualRGB result;

	if (temperature <= 66) {
		result.pR = 255;
	} else {
		result.pR = temperature - 60;
		result.pR = 329.698727446 * pow(result.pR, -0.1332047592);
		if (result.pR < 0) {
			result.pR = 0;
		}
		if (result.pR > 255) {
			result.pR = 255;
		}
	}

    if (temperature <= 66) {
        result.pG = temperature;
        result.pG = 99.4708025861 * log(result.pG) - 161.1195681661;
        if (result.pG < 0) {
        	result.pG = 0;
        }
        if (result.pG > 255) {
        	result.pG = 255;
        }
    }else {
    	result.pG = temperature - 60;
        result.pG = 288.1221695283 * pow(result.pG, -0.0755148492);
        if (result.pG < 0) {
        	result.pG = 0;
        }
        if (result.pG > 255) {
        	result.pG = 255;
        }
    }

    if (temperature >= 66) {
    	result.pB = 255;
    }else if (temperature <= 19) {
        	result.pB = 0;
    }else {
       	result.pB = temperature - 10;
       	result.pB = 138.5177312231 * log(result.pB) - 305.0447927307;
        if (result.pB < 0) {
           	result.pB = 0;
       	}
        if (result.pB > 255) {
           	result.pB = 255;
       }
    }
    result.pR/=255.;
	result.pB/=255.;
	result.pG/=255.;

	float cur_intensity = getIntensity(result);
	result.pR = result.pR / cur_intensity * intensity;
	result.pG = result.pG / cur_intensity * intensity;
	result.pB = result.pB / cur_intensity * intensity;
    return result;
}

}
/*
float smartRGBW::ile_razy_rozjasnic(RGB rgb, float target_natezenie) {
	float current_natezenie = getIntensity(rgb);
	return target_natezenie/current_natezenie;
}

RGBW smartRGBW::getRGBW(perceptualRGB prgb) {
	RGB rawRGB = getRGB(prgb);
	return getRGBW(rawRGB);
}

RGBW smartRGBW::getRGBW(perceptualRGB prgb, float intensity) {
	RGB rawRGB = getRGB(prgb);
	return getRGBW(rawRGB, intensity);
}

RGBW smartRGBW::getRGBW(RGB rgb, float intensity) {
	float ile_rozjasnic = ile_razy_rozjasnic(rgb, intensity);
	rgb.R *= ile_rozjasnic;
	rgb.G *= ile_rozjasnic;
	rgb.B *= ile_rozjasnic;
	return getRGBW(rgb);
}

RGB smartRGBW::getRGB(perceptualRGB rgbp) {
	RGB rgb;
	rgb.R = rgbp.pR*Neutral.R;
	rgb.G = rgbp.pG*Neutral.G;
	rgb.B = rgbp.pB*Neutral.B;
	return rgb;
}

RGB smartRGBW::getRGB(RGBW rgbw) {
	RGB rgb;
	rgb.R = rgbw.R + WhiteLED.R * rgbw.W;
	rgb.G = rgbw.G + WhiteLED.G * rgbw.W;
	rgb.B = rgbw.B + WhiteLED.B * rgbw.W;
	return rgb;
}

float smartRGBW::calculateMaxIntensity(perceptualRGB pcolor) {
	return calculateMaxIntensity(getRGB(pcolor));
}



float smartRGBW::getIntensity(perceptualRGB pcolor) {
	return(sqrt((pcolor.pR*pcolor.pR)+(pcolor.pG*pcolor.pG)+(pcolor.pB*pcolor.pB))*0.577350269);
}




float smartRGBW::getIntensity(RGBW rgbw) {
	RGB rgb;
		rgb.R = rgbw.R + WhiteLED.R * rgbw.W;
		rgb.G = rgbw.G + WhiteLED.G * rgbw.W;
		rgb.B = rgbw.B + WhiteLED.B * rgbw.W;
	return getIntensity(rgb);
}

RGBW smartRGBW::ColorFromTime(float time, float intensity) {
	float tmp = temp_from_time(time);
	float nat_raw = intensity_from_time(time);
	RGB kolor = getRGB(Temperature2RGB(tmp));
	float nat_max = calculateMaxIntensity(kolor);
	float nat_scaled = nat_raw/100 * nat_max * intensity;
	return(getRGBW(kolor, nat_scaled));
}
*/
