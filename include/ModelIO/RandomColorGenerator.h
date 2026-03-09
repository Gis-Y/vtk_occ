/*******************************************************************************

	基于HSV色彩模式定义的一个颜色生成器，过滤掉一些不适合显示模型的颜色，比如纯色
	另外，生成的相邻颜色差别也较大，避免颜色过于相似

*******************************************************************************/


#pragma once

#include <Quantity_Color.hxx>
#include <vector>

#define HUESTEP			360
#define SATURATIONSTEP	17
#define LIGHTNESSSTEP	19


class RandomColorGenerator
{
public:
	RandomColorGenerator() {
		Ref = 0;
		for (int i = 1; i <= HUESTEP; i++)
		{
			HueSamplers.push_back((83 * i) % HUESTEP);
		}

		// 饱和度 0.0 - 1.0
		for (int i = 1; i <= SATURATIONSTEP; i++)
		{
			double randomNum = (7 * i) % SATURATIONSTEP;
			SaturationSamplers.push_back(0.5 * randomNum / SATURATIONSTEP + 0.5);
		}

		// 亮度 0.2 - 0.8
		for (int i = 1; i <= LIGHTNESSSTEP; i++)
		{
			int randomNum = (11 * i) % LIGHTNESSSTEP;
			LightSamplers.push_back(0.5 * randomNum / LIGHTNESSSTEP + 0.5);
		}

	}
	~RandomColorGenerator() {}

	Quantity_Color GetColor()
	{
		int indexHue = Ref % HUESTEP;
		int indexSaturation = Ref % SATURATIONSTEP;
		int indexLight = Ref % LIGHTNESSSTEP;

		color.SetValues(HueSamplers[indexHue], LightSamplers[indexLight], SaturationSamplers[indexSaturation], Quantity_TOC_HLS);
		Ref++;

		return color;
	}

	void ResetRef()
	{
		Ref = 0;
	}

	void SetRefBegin(const int _Ref)
	{
		Ref = _Ref;
	}


private:
	int Ref;
	std::vector<int> HueSamplers;
	std::vector<double> SaturationSamplers;
	std::vector<double> LightSamplers;
	Quantity_Color color;

};