#pragma once
#ifndef RECIPEDATABASEPCH_H
#define RECIPEDATABASEPCH_H

//------Recipe Database------
#include "Core/Core.h"
#include "Core/Log.h"
#include "Core/Window/Window.h"

//------STL------
#include <chrono>
#include <functional>
#include <mutex>
#include <string>
#include <sstream>
#include <thread>

//------Windows specific------
#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#include <windef.h>
#elif PLATFORM_LINUX
#endif // PLATFORM_WINDOWS


#endif // !RECIPE_DATABASE_PCH_H

