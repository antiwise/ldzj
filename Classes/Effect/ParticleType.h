#pragma once
#include "cocos2d.h"

USING_NS_CC;
std::string getLine(std::istringstream &ifs);
std::string readString (std::string &line);
bool readBool (std::string &line);
int readInt (std::string &line);
float readFloat (std::string &line);

class ParticleValue
{
public:
	ParticleValue()
		: m_bActive(false),
		m_bAlwaysActive(false)
	{
	}

	~ParticleValue() {}
	
public:
	
	void setAlwaysActive (bool value) {
		m_bAlwaysActive = value;
	}

	bool isAlwaysActive () {
		return m_bAlwaysActive;
	}

	bool isActive () {
		return m_bAlwaysActive || m_bActive;
	}

	void setActive (bool value) {
		m_bActive = value;
	}

	void load (std::istringstream &ifs)
	{
		if (!m_bAlwaysActive)
		{
			std::string line = getLine(ifs);
			m_bActive = readBool(line); //, "m_bActive"
		}
		else
		{
			m_bActive = true;
		}
	}
		
	void load (const ParticleValue &value) {
		m_bActive = value.m_bActive;
		m_bAlwaysActive = value.m_bAlwaysActive;
	}

public:
	bool m_bActive;
	bool m_bAlwaysActive;

};


class NumericValue : public ParticleValue 
{
public:
	NumericValue()
		: ParticleValue(),
		m_fValue(0.0f)
	{
	}

	~NumericValue() {}

public:
	float getValue () 
	{
		return m_fValue;
	}

	void setValue (float value) 
	{
		m_fValue = value;
	}

	void load (std::istringstream &ifs)
	{
		ParticleValue::load(ifs);

		if (!m_bActive) return;

		std::string line = getLine(ifs);
		m_fValue = readFloat(line); //, "value"
	}
	
	void load (const NumericValue& value) {
		ParticleValue::load(value);
		m_fValue = value.m_fValue;
	}
	
public:
	float m_fValue;
};

class RangedNumericValue : public ParticleValue 
{
public:
	RangedNumericValue()
		: ParticleValue(),
		m_fLowMin(0.0f),
		m_fLowMax(0.0f)
	{
	}

	~RangedNumericValue() {}

public:

	float newLowValue () {
		return m_fLowMin + CCRANDOM_0_1() * (m_fLowMax - m_fLowMin);
	}

	void setLow (float value) {
		m_fLowMin = value;
		m_fLowMax = value;
	}

	void setLow (float min, float max) {
		m_fLowMin = min;
		m_fLowMax = max;
	}

	float getLowMin () {
		return m_fLowMin;
	}

	void setLowMin (float lowMin) {
		m_fLowMin = lowMin;
	}

	float getLowMax () {
		return m_fLowMax;
	}

	void setLowMax (float lowMax) {
		m_fLowMax = lowMax;
	}

	void load (std::istringstream &ifs){
		ParticleValue::load(ifs);
		
		if (!m_bActive) return;
		
		std::string line = getLine(ifs);
		m_fLowMin = readFloat(line); //, "lowMin"
		
		line = getLine(ifs);
		m_fLowMax = readFloat(line); //, "lowMax"
	}
	
	void load (const RangedNumericValue& value) {
		ParticleValue::load(value);
		m_fLowMax = value.m_fLowMax;
		m_fLowMin = value.m_fLowMin;
	}

public:
	float m_fLowMin;
	float m_fLowMax;
};

class ScaledNumericValue : public RangedNumericValue 
{
public:
	ScaledNumericValue() 
		: RangedNumericValue(),
		m_fHighMin(0.0f),
		m_fHighMax(0.0f),
		m_bRelative(false)
	{
		for (int i = 0; i < 100; i++)
		{
			scaling[i] = 1;
			timeline[i] = 0;
		}

		m_nResNum = 1;
	}

public:
	
	float newHighValue () {
		return m_fHighMin + CCRANDOM_0_1() * (m_fHighMax - m_fHighMin);
	}

	void setHigh (float value) {
		m_fHighMin = value;
		m_fHighMax = value;
	}

	void setHigh (float min, float max) {
		m_fHighMin = min;
		m_fHighMax = max;
	}

	float getHighMin () {
		return m_fHighMin;
	}

	void setHighMin (float highMin) {
		m_fHighMin = highMin;
	}

	float getHighMax () {
		return m_fHighMax;
	}

	void setHighMax (float highMax) {
		m_fHighMax = highMax;
	}

	bool isRelative () {
		return m_bRelative;
	}

	void setRelative (bool relative) {
		m_bRelative = relative;
	}

	float getScale (float percent) {
		int endIndex = -1;
		for(int i = 0; i < m_nResNum; i++)
		{
			float t = timeline[i];
			if (t > percent) {
				endIndex = i;
				break;
			}
		} 

		if (endIndex == -1) return scaling[m_nResNum - 1];

		int startIndex = endIndex - 1;
		float startValue = scaling[startIndex];
		float startTime = timeline[startIndex];
		return startValue + (scaling[endIndex] - startValue) * ((percent - startTime) / (timeline[endIndex] - startTime));
	}

	void load (std::istringstream &ifs){
		RangedNumericValue::load(ifs);
		
		if (!m_bActive) return;

		std::string line = getLine(ifs);
		m_fHighMin = readFloat(line); //, "highMin"
		
		line = getLine(ifs);
		m_fHighMax = readFloat(line); //, "highMax"
		
		line = getLine(ifs);
		m_bRelative = readBool(line); //, "relative"
		
		line = getLine(ifs);
		int num = readInt(line); //, "scalingCount"

		for (int i = 0; i < 100; i++)
		{
			scaling[i] = 1;
			timeline[i] = 0;
		}

		m_nResNum = num;
		for (int i = 0; i < num; i++)
		{
			line = getLine(ifs);
			scaling[i] = readFloat(line); //, "scaling" + i
		}
		
		line = getLine(ifs);
		num = readInt(line); //, "timelineCount"
		
		for (int i = 0; i < num; i++)
		{
			line = getLine(ifs);
			timeline[i] = readFloat(line); //, "timeline" + i
		}


	}

	void load (const ScaledNumericValue& value) {
		RangedNumericValue::load(value);
		m_fHighMax = value.m_fHighMax;
		m_fHighMin = value.m_fHighMin;
		
		for (int i = 0; i < 100; i++)
		{
			scaling[i] = 0;
			timeline[i] = 0;
		}

		m_nResNum = value.m_nResNum;
		for(int i = 0; i < value.m_nResNum; i++)
		{
			scaling[i] = value.scaling[i];
		}
		for(int i = 0; i < value.m_nResNum; i++)
		{
			timeline[i] = value.timeline[i];
		}
		m_bRelative = value.m_bRelative;
	}

public:
	float		targetScaling[100];		// Ô¤¼ÆËã
	int			m_nResNum;
	float		scaling[100];
	float		timeline[100];
	float		m_fHighMin;
	float		m_fHighMax;
	bool		m_bRelative;
};

class GradientColorValue : public ParticleValue 
{
public:
	GradientColorValue()
		: ParticleValue()
	{
		m_bAlwaysActive = true;
		for (int i = 0; i < 100; i++)
		{
			colors[i*3] = 1;
			colors[i*3 + 1] = 1;
			colors[i*3 + 2] = 1;

			timeline[i] = 0;
		}

		m_nResNum = 1;
	}

	~GradientColorValue() {}

	Color3B getColor (float percent) {
		int startIndex = 0;
		int endIndex = -1;
		for(int i = 0; i < m_nResNum; i++)
		{
			float t = timeline[i];
			if (t > percent) {
				endIndex = i;
				break;
			}
			startIndex = i;
		} 

		Color3B targetColor;
		float startTime = timeline[startIndex];
		startIndex *= 3;
		float r1 = colors[startIndex];
		float g1 = colors[startIndex + 1];
		float b1 = colors[startIndex + 2];
		if (endIndex == -1) {
			targetColor.r = r1 * 255;
			targetColor.g = g1 * 255;
			targetColor.b = b1 * 255;
			return targetColor;
		}
		float factor = (percent - startTime) / (timeline[endIndex] - startTime);
		endIndex *= 3;
		targetColor.r = (r1 + (colors[endIndex] - r1) * factor) * 255;
		targetColor.g = (g1 + (colors[endIndex + 1] - g1) * factor) * 255;
		targetColor.b = (b1 + (colors[endIndex + 2] - b1) * factor) * 255;
		return targetColor;
	}

	void load (std::istringstream &ifs)  {
		ParticleValue::load(ifs);
		
		if (!m_bActive) return;

		std::string line = getLine(ifs);
		int num = readInt(line); //, "colorsCount"

		for (int i = 0; i < 100; i++)
		{
			colors[i*3] = 1;
			colors[i*3 + 1] = 1;
			colors[i*3 + 2] = 1;

			timeline[i] = 0;
		}

		for (int i = 0; i < num; i++)
		{
			line = getLine(ifs);
			colors[i] = readFloat(line); //, "colors" + i
		}

		line = getLine(ifs);
		num = readInt(line); //, "timelineCount"
		m_nResNum = num;
		for (int i = 0; i < num; i++)
		{
			line = getLine(ifs);
			timeline[i] = readFloat(line); //, "timeline" + i
		}
	}
		
	void load (const GradientColorValue& value) {
		ParticleValue::load(value);
		
		for (int i = 0; i < 100; i++)
		{
			colors[i*3] = 1;
			colors[i*3 + 1] = 1;
			colors[i*3 + 2] = 1;

			timeline[i] = 0;
		}

		for(int i = 0; i < m_nResNum * 3; i++)
		{
			colors[i] = value.colors[i];
		}
		for(int i = 0; i < m_nResNum; i++)
		{
			timeline[i] = value.timeline[i];
		}
	}

public:
	int					m_nResNum;	

	float				colors[300];
	float				timeline[100];
};

class SpawnShapeValue : public ParticleValue 
{
public:
	SpawnShapeValue()
		: ParticleValue(),
		m_nShape("point"),
		m_bEdges(false),
		m_nSide("both")
	{
	}

	~SpawnShapeValue() {}

public:
	std::string getShape () {
		return m_nShape;
	}

	void setShape (std::string& shape) {
		m_nShape = shape;
	}

	bool isEdges () {
		return m_bEdges;
	}

	void setEdges (bool edges) {
		m_bEdges = edges;
	}

	std::string getSide () {
		return m_nSide;
	}

	void setSide (std::string& side) {
		m_nSide = side;
	}

	void load (std::istringstream &ifs) {
		ParticleValue::load(ifs);
		if (!m_bActive) return;
		
		std::string line = getLine(ifs);
		m_nShape = readString(line); //, "shape"
		if (m_nShape == "ellipse") {
			line = getLine(ifs);
			m_bEdges = readBool(line); //, "edges"

			line = getLine(ifs);
			m_nSide = readString(line); //, "side"
		}
	}
	
	void load (const SpawnShapeValue& value) {
		ParticleValue::load(value);

		m_nShape = value.m_nShape;
		m_bEdges = value.m_bEdges;
		m_nSide = value.m_nSide;
	}

public:
	std::string m_nShape;
	bool m_bEdges;
	std::string m_nSide;
};
