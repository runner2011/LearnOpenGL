#pragma once

#include <random>

#define PI 3.14159265359f

float AngleToRadians(float angle)
{
	return angle * PI / 180.0f;
}


float RandomFloat(float min = 0.f, float max = 1.f) 
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}