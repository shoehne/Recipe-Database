#pragma once
#ifndef RECIPEDATABASEPCH_H
#define RECIPEDATABASEPCH_H

//------Recipe Database------
#include "Core/Core.h"
#include "Core/Window.h"

//------STL------
#include <chrono>
#include <exception>
#include <functional>
#include <mutex>
#include <stdio.h>
#include <string>
#include <sstream>
#include <thread>
#include <vector>

//------Windows specific------
#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#include <windef.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <tchar.h>
//------Linux specific------
#elif PLATFORM_LINUX
#endif // PLATFORM_WINDOWS


#endif // !RECIPE_DATABASE_PCH_H

