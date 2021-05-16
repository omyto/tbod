#pragma once
#include "Math.h"
class Globals
{
public:
	static const int screenWidth = 960;
	static const int screenHeight = 720;
	static const int SCREEN_W_DEFAULT = 960;
	static const int SCREEN_H_DEFAULT = 720;
	static int keyPressed;
	static unsigned char keyPress;
	static bool isKeyPressed;
	static int effect;
	static unsigned int mouseType;
	static int mouseMoveDeltaX;
	static int mouseMoveDeltaY;
	static int mouseTotalRangeX;
	static int mouseTotalRangeY;
	static int mousePointX;
	static int mousePointY;
	static bool isEnableCursor;
	static int currentState;
	static bool isEnableMouseControl;
	static bool isShowLogMousePointer;
};