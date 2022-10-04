workspace "Recipe-Database"
    location "../"
    architecture "x64"

    configurations {

        "Debug",
        "Release"
    }

    output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    IncludeDir = {}
    IncludeDir["SQLiteCpp"] = "../Libraries/SQLiteCpp/include"
    IncludeDir["ImGui"] = "../Libraries/imgui"

    project "Recipe-Database"
        location "../%{prj.name}"
        language "C++"
        kind "WindowedApp"

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
            "%{IncludeDir.SQLiteCpp}",
            "%{IncludeDir.ImGui}"
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

                "Rpcrt4",
                "d3dcompiler",
                "d3d11"
            }

			filter {"system:windows", "configurations:Debug"}
				buildoptions {

					"/MTd"
				}
                links {

                    "../bin/" .. output_dir .. "/sqlite3_win/sqlite3",
                    "../bin/" .. output_dir .. "/SQLiteCpp_win/SQLiteCpp",
                    "../bin/" .. output_dir .. "/imgui_win/imgui"
                }

			filter {"system:windows", "configurations:Release"}
				buildoptions {

					"/MT"
				}
                links {

                    "../bin/" .. output_dir .. "/sqlite3_win/sqlite3",
                    "../bin/" .. output_dir .. "/SQLiteCpp_win/SQLiteCpp",
                    "../bin/" .. output_dir .. "/imgui_win/imgui"
                }
        
        filter "system:linux"
            defines {

                "PLATFORM_LINUX"
            }

            filter {"system:linux", "configurations:Debug"}
                buildoptions {


                }
                links {

                    "../bin/" .. output_dir .. "/sqlite3_win/sqlite3",
                    "../bin/" .. output_dir .. "/SQLiteCpp_win/SQLiteCpp",
                    "../bin/" .. output_dir .. "/imgui_win/imgui"
                }
            
            filter {"system:linux", "configurations:Release"}
                buildoptions {


                }
                links {

                    "../bin/" .. output_dir .. "/sqlite3_win/sqlite3",
                    "../bin/" .. output_dir .. "/SQLiteCpp_win/SQLiteCpp",
                    "../bin/" .. output_dir .. "/imgui_win/imgui"
                }

        filter "configurations:Debug"
            defines "DEBUG"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines "RELEASE"
            runtime "Release"
            optimize "on"

    group "Libraries"

        include "../Libraries/SQLiteCpp"
        include "../Libraries/imgui"