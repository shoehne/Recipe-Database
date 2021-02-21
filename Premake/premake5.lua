workspace "Recipe Database"
    location "../"
    architecture "x64"

    configurations {

        "Debug",
        "Release"
    }

    output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    project "Recipe_Database"
        location "../%{prj.name}"
        kind "WindowedApp"
        language "C++"
        systemversion "latest"

        targetdir ("../bin/" .. output_dir .. "/%{prj.name}/")
        objdir ("../bin-obj/" .. output_dir .. "/%{prj.name}/")

        pchheader "Recipe_Database_Pch.h"
        pchsource "../Recipe_Database/Source/Recipe_Database_Pch.cpp"

        files {

            "../Recipe_Database/Include/**.h",
            "../Recipe_Database/Source/**.cpp",
        }

        includedirs {

            "../Recipe_Database/Include",
            "../Libraries/spdlog/include"
        }

        links {


        }

        defines  {


        }

        filter "system:windows"
				defines {

					"PLATFORM_WINDOWS"
				}

			filter {"system:windows", "configurations:Debug"}
				buildoptions {

					"/MTd"
				}

			filter {"system:windows", "configurations:Release"}
				buildoptions {

					"/MT"
				}
        
        filter "system:linux"
            defines {

                "PLATFORM_LINUX"
            }

        filter "configurations:Debug"
            defines "DEBUG"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines "RELEASE"
            runtime "Release"
            optimize "on"