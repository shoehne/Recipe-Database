workspace "Recipe-Database"
    location "../"
    architecture "x64"

    configurations {

        "Debug",
        "Release"
    }

    output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    project "Recipe-Database"
        location "../%{prj.name}"
        language "C++"

        targetdir ("../bin/" .. output_dir .. "/%{prj.name}/")
        objdir ("../bin-obj/" .. output_dir .. "/%{prj.name}/")

        pchheader "RecipeDatabasePch.h"
        pchsource "../Recipe-Database/Source/RecipeDatabasePch.cpp"

        files {

            "../Recipe-Database/Include/**.h",
            "../Recipe-Database/Source/**.cpp",
        }

        includedirs {

            "../Recipe-Database/Include",
            "../Libraries/spdlog/include",
            "../Libraries/SQLiteCpp/include"
        }

        links {


        }

        defines  {


        }

        filter "system:windows"
            systemversion "latest"
            defines {

                "PLATFORM_WINDOWS"
            }

            links {

                "Rpcrt4"
            }

			filter {"system:windows", "configurations:Debug"}
				buildoptions {

					"/MTd"
				}
                links {

                    "../Libraries/SQLiteCpp/build/Debug/SQLiteCpp",
                    "../Libraries/SQLiteCpp/build/sqlite3/Debug/sqlite3"
                }

			filter {"system:windows", "configurations:Release"}
				buildoptions {

					"/MT"
				}
                links {

                    "../Libraries/SQLiteCpp/build/Release/SQLiteCpp",
                    "../Libraries/SQLiteCpp/build/sqlite3/Release/sqlite3"
                }
        
        filter "system:linux"
            defines {

                "PLATFORM_LINUX"
            }

        filter "configurations:Debug"
            kind "ConsoleApp"
            defines "DEBUG"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
        kind "WindowedApp"
            defines "RELEASE"
            runtime "Release"
            optimize "on"