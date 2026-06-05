#include "PadBase.h"

PadBase::PadBase(int _connectIndex)
	:connectIndex(_connectIndex)
	, isConnect(false)
	, lx(0), ly(0), rx(0), ry(0)
	, lt(0), rt(0) 
{}

float PadBase::StickNorm(float _v) {
	float f = _v / SHRT_MAX;

	// ê‚ëŒíl
	if (fabs(f) < DEADZONE)
		return 0.0f;

	return f;
}
