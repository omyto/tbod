#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
struct LightPropertiesLocation
{
	GLint color;
	GLint position;
	GLint isDirection;
};