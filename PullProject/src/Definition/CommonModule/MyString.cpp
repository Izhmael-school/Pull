#include "MyString.h"
#include <iostream>
#include <iomanip>
#include <sstream>

void MyString::StringCenterPos(const TCHAR* _str, int _fontHandle, int* posX, int* posY, int exRateX, int exRateY) {
	int w, h, line;

	GetDrawStringSizeToHandle(&w, &h, &line, _str, (int)_tcslen(_str), _fontHandle);
	*posX -= (int)(w * exRateX) / 2;
	*posY -= (int)(h * exRateY) / 2;
}

int MyString::StringRightPos(const TCHAR* _str, int _fontHandle, int posX, int exRateX) {
	int w, h, line;

	GetDrawStringSizeToHandle(&w, &h, &line, _str, (int)_tcslen(_str), _fontHandle);

	return posX -= (int)(w * exRateX);
}

std::string MyString::ToZeroPadded(int _str, int _zeroIndex) {
	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(_zeroIndex) << _str;
	return oss.str();
}
