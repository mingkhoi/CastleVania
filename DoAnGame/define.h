#pragma once
#define NOMINMAX

// some basic library

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
#include <windowsx.h>
#include <random>
#include <mmsystem.h>
#include <dsound.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// xml reader library

#include "./rapidxml.hpp"
#include "./rapidxml_iterators.hpp"
#include "./rapidxml_print.hpp"
#include "./rapidxml_utils.hpp"

#include "./Audio.h"

#include <winsdkver.h>
#include <sdkddkver.h>


// ==================================================

#define Right 1
#define Left -1

#define PI 3.14159265

#define epsilon 0.5f

#define wndWidth 256
#define wndHeight 240
#define wndFps 80
#define scoreBoard 48

#define _point_10 10
#define _point_20 20
#define _point_40 40
#define _point_70 70
#define _point_100 100
#define _point_200 200
#define _point_400 400
#define _point_700 700
#define _point_1000 1000
#define _point_2000 2000
#define _point_4000 4000
#define _point_7000 7000

// some most use function
// define macro for shorter code

#define xml_atr rapidxml::xml_attribute<>
#define xml_n rapidxml::xml_node<>
#define xml_doc rapidxml::xml_document<>

#define get_atr(a, b) a->first_attribute(b)->value()

#define scan(a, b, c) sscanf_s(a, b, c)
#define print(f, ...) game::debugOutString(f, __VA_ARGS__)
#define printW(f, ...) game::debugOut(f, __VA_ARGS__)
#define toRadian(a) (((float)a * PI) / 180.0f) 

#define cast(a, type) ((type)a)

#define item_state 0
#define item_die_time 8000
#define item_damage 4

struct FRect // Rect with float point
{
	float left, top, right, bottom;

	FRect() {
		left = top = right = bottom = 0;
	}

	FRect(float left, float top,
		float right, float bottom) {
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}
};
typedef FRect* LPFRECT;

struct Counter // assing different id for each object created
{
	static long long index;
};

struct Numeric // for converting from database
{
	static int toInt(const char* value) {
		int re;
		scan(value, "%i", &re);

		return re;
	}

	static float toFloat(const char* value) {
		float re;
		scan(value, "%f", &re);

		return re;
	}

	static DWORD toDWORD(const char* value) {
		unsigned long re;
		scan(value, "%lu", &re);

		return re;
	}

	static bool equalFloat(float a, float b) {
		if (std::abs(a - b) <= epsilon)
			return true;
		return false;
	}
};
