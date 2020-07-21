# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library(
        # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp

        engine_3d/GameObjectComponent.cpp
        engine_3d/GameObject.cpp
        engine_3d/TransformationComponent.cpp
        engine_3d/CameraComponent.cpp
        engine_3d/OrthoCameraComponent.cpp
        engine_3d/MeshComponent.cpp
        engine_3d/AmbientLightComponent.cpp
        engine_3d/MaterialComponent.cpp
        engine_3d/TextComponent.cpp
        engine_3d/TextAppearance.cpp

        game/touch_screen/TouchScreen.cpp
        game/DisplayInfo.cpp
        game/TimeProvider.cpp
        game/Scene.cpp
        game/SceneDataLoader.cpp
        game/dev_scenes/ScreenBlinkingScene.cpp
        game/dev_scenes/RenderingEngineDevScene.cpp
        game/MessageQueue.cpp
        game/UnitsConverter.cpp
        game/MeshStorage.cpp

        main/rendering_engine/OpenGlMeshRendererComponent.cpp
        main/rendering_engine/OpenGlMeshRendererFactory.cpp
        main/rendering_engine/RenderingEngine.cpp
        main/rendering_engine/OpenGLState.cpp
        main/rendering_engine/OpenGLGeometryBuffersStorage.cpp
        main/rendering_engine/OpenGlShadersRepository.cpp
        main/rendering_engine/OpenGlShaderProgramContainer.cpp
        main/rendering_engine/ShaderSourcePreprocessor.cpp
        main/rendering_engine/OpenGLFreeTypeTextRendererComponent.cpp
        main/rendering_engine/OpenGLFreeTypeCharactersRepository.cpp
        main/rendering_engine/OpenGLVerticalQuadBuffersRepository.cpp

        main/GameWrapper.cpp
        main/OpenGLErrorDetector.cpp
        main/L.cpp
        main/AndroidDisplayInfo.cpp
        main/AndroidSceneDataLoader.cpp
        main/AndroidUnitsConverter.cpp
        main/AndroidMeshLoadingRepository.cpp
        main/AndroidShaderSourceRepository.cpp
        main/AndroidFontDataLoader.cpp
)

target_include_directories(native-lib PUBLIC .)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

find_library(opengl-lib GLESv2)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib})
target_link_libraries(native-lib ${opengl-lib})
target_link_libraries(native-lib ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/x86/libassimp.so)
target_link_libraries(native-lib ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/x86/libfreetype.so)