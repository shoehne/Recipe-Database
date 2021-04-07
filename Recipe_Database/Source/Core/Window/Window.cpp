//------Precompiled header------
#include "RecipeDatabasePch.h"

#include "Core/Window/Window.h"

#ifdef PLATFORM_WINDOWS
#include "Core/Window/WindowWindows.h"
#elif PLATFORM_LINUX
#include "Core/Window/WindowLinux.h"
#endif // PLATFORM_WINDOWS

Recipe_Database::Ref<Recipe_Database::Window> Recipe_Database::Window::Create(const Recipe_Database::WindowProps& props) {

	#ifdef PLATFORM_WINDOWS
	return CreateRef<Recipe_Database::WindowWindows>(props);
	#elif PLATFORM_LINUX
	#else
	RECIPE_DATABASE_ERROR("Impossible to create window on this platform");
	#endif // PLATFORM_WINDOWS
}