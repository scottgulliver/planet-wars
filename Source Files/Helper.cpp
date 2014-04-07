////////////////////////////////////////////////////////////////////////
// File: Helper.cpp			Class: Helper
// 
// Description:
// Provides helper methods for use by other classes.
//
////////////////////////////////////////////////////////////////////////

#include "Helper.h"

float Helper::ToRadians (float degrees)
{
	float ratio = 180.0 / 3.141592653589793238;
	return degrees / ratio;
};

float Helper::ToDegrees (float radians)
{
	float ratio = 180.0 / 3.141592653589793238;
	return radians * ratio;
};