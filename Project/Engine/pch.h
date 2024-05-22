#pragma once

#define SL_ENGINE_EDITOR

// Windows directories
#include <Windows.h>

// std namespace
#include <cmath>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <stdarg.h>
#include <fstream>
#include <ctime>
#include <utility>
#include <algorithm>
#include <thread>
#include <codecvt>
#include <sstream>

// Lib
// DirectXTK
// LUA
//#include "luacpp/LuaCpp.hpp"

// DirectX directories
#include <d3d11.h>
#include <d3d9.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>

#define ENG_PTR std::shared_ptr
#define TO_ENG_PTR std::make_shared

typedef unsigned short USHORT;
typedef unsigned char UCHAR;