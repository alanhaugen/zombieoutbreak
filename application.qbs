import "../solid/solid.qbs" as solid

solid {
    Application {
        name: "App"
        cpp.cxxLanguageVersion: "c++23"

        files: [
            "data/bg.frag",
            "data/bg.vert",
            "data/cubemap.frag",
            "data/cubemap.vert",
            "data/app.rc",
            "data/app.res",
            "data/gui.frag",
            "data/gui.vert",
            "data/icon.png",
            "data/phong.frag",
            "data/phong.vert",
            "data/project.json",
            "data/simple.frag",
            "data/simple.vert",
            "data/solid.ico",
            "data/vertexanimation.frag",
            "data/vertexanimation.vert",
            "source/main.cpp",
            "source/main.h",
        ]

        Depends { name: "core"  }
        Depends { name: "nullrenderer"  }
        Depends { name: "nullphysics"  }
        Depends { name: "nullaudio"  }
        Depends { name: "nullphysics"  }
        Depends { name: "nullfilesystem"  }
        Depends { name: "vulkanrenderer"  }
        Depends { name: "stdfilesystem"  }
        Depends { name: "portaudioaudio"  }

        property stringList includePaths: "../solid/source"

        Properties {
            condition: qbs.targetOS.contains("macos")

            cpp.frameworks: macosFrameworks

            cpp.dynamicLibraries: macosSharedLibs
            cpp.staticLibraries: staticLibs.concat("SDL2", "MoltenVK")

            cpp.libraryPaths: [project.buildDirectory, "../solid/lib/debug/darwin/arm64"]
            cpp.includePaths: includePaths.concat("../solid/include/darwin")
            cpp.defines: project.defines.concat(project.sdlDefines)
        }

        Properties {
            condition: qbs.targetOS.contains("linux")

            //cpp.dynamicLibraries: linuxSharedLibs
            cpp.staticLibraries: staticLibs.concat("SDL2")

            cpp.libraryPaths: [project.buildDirectory, "../solid/lib/debug/linux/x86_64"]
            cpp.includePaths: includePaths.concat("../solid/include/linux")
            cpp.defines: project.defines.concat(project.sdlDefines)
        }

        Properties {
            condition: qbs.targetOS.contains("windows")

            cpp.dynamicLibraries: windowsSharedLibs
            cpp.staticLibraries: staticLibs

            cpp.libraryPaths: [project.buildDirectory, "../solid/lib/debug/mingw32/x86_64"]
            cpp.includePaths: includePaths.concat("../solid/include/mingw32")
            cpp.defines: project.defines.concat(project.windowsDefines)
        }

        Depends { name: "cpp" }
        Depends { name: "core" }
        Depends { name: "nullphysics" }
        Depends { name: "bulletphysics" }
    }
}
