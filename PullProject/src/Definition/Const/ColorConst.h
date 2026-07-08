#pragma once
#include "DxLib.h"

/*
 * @file ColorConst.h
 * @author Sekino
 */

constexpr unsigned int white = 0xFFFFFF;
constexpr unsigned int gray = 0x646464;
constexpr unsigned int black = 0x000000;
constexpr unsigned int red = 0xFF0000;
constexpr unsigned int green = 0x00FF00;
constexpr unsigned int blue = 0x0000FF;
constexpr unsigned int yellow = 0xFFFF00;

const COLOR_F red_f = GetColorF(255, 0, 0, 255);
const COLOR_F yellow_f = GetColorF(255, 255, 0, 255);
const COLOR_F white_f = GetColorF(255, 255, 255, 255);