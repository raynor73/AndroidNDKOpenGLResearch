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
        engine_3d/LayoutComponent.cpp
        engine_3d/ViewBoundsComponent.cpp
        engine_3d/TextButtonComponent.cpp
        engine_3d/ImageButtonComponent.cpp
        engine_3d/Transformation2DComponent.cpp
        engine_3d/touch_screen/GestureConsumerComponent.cpp
        engine_3d/PerspectiveCameraComponent.cpp
        engine_3d/DirectionalLightComponent.cpp
        engine_3d/OdePhysicsEngine.cpp
        engine_3d/RigidBodyComponent.cpp
        engine_3d/BulletPhysicsEngine.cpp
        engine_3d/CollisionsInfoComponent.cpp
        engine_3d/Mesh.cpp
        engine_3d/skeletal_animation/SkeletalAnimationComponent.cpp
        engine_3d/skeletal_animation/SkeletalAnimationPlayerComponent.cpp
        engine_3d/SoundStorage.cpp
        engine_3d/SoundPlayerComponent.cpp
        engine_3d/SoundListenerComponent.cpp
        engine_3d/Time.cpp
        engine_3d/DisplayInfo.cpp
        engine_3d/TimeProvider.cpp
        engine_3d/Scene.cpp
        engine_3d/SceneDataLoader.cpp
        engine_3d/MessageQueue.cpp
        engine_3d/UnitsConverter.cpp
        engine_3d/MeshStorage.cpp
        engine_3d/touch_screen/GesturesDispatcher.cpp
        engine_3d/touch_screen/ClickDetectorComponent.cpp
        engine_3d/touch_screen/ScrollDetectorComponent.cpp
        engine_3d/DisplayInfoUpdateDetector.cpp
        engine_3d/SkeletalAnimationStorage.cpp

        game/dev_scenes/ScreenBlinkingScene.cpp
        game/dev_scenes/RenderingEngineDevScene.cpp
        game/FpsCalculator.cpp
        game/dev_scenes/ScenesSelectionScene.cpp
        game/dev_scenes/SceneCloser.cpp
        game/dev_scenes/MultitouchTestScene.cpp
        game/SimpleJoystick.cpp
        game/FreeFlyCameraController.cpp
        game/dev_scenes/LoadingScene.cpp
        game/PlayerController.cpp

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
        main/rendering_engine/OpenGLTextRendererFactory.cpp
        main/rendering_engine/OpenGLTexturesRepository.cpp

        main/GameWrapper.cpp
        main/OpenGLErrorDetector.cpp
        main/L.cpp
        main/AndroidDisplayInfo.cpp
        main/AndroidSceneDataLoader.cpp
        main/AndroidUnitsConverter.cpp
        main/AndroidMeshLoadingRepository.cpp
        main/AndroidShaderSourceRepository.cpp
        main/AndroidFontDataLoader.cpp
        main/AndroidTouchScreen.cpp
        main/AndroidBitmapDataLoader.cpp
        main/AndroidSkeletalAnimationLoadingRepository.cpp
        main/AndroidFsAbstraction.cpp
        main/AndroidReadOnlyFsAbstraction.cpp
        main/AndroidSoundLoadingRepository.cpp
        main/AndroidOpenALSoundScene.cpp
        main/AndroidMediaPlayerMusicPlayer.cpp

        iffdigest/iffdigest.cc
)

target_include_directories(native-lib PUBLIC .)
target_include_directories(native-lib PUBLIC ./bullet/)

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
target_link_libraries(native-lib ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libassimp.so)
target_link_libraries(native-lib ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libfreetype.so)
target_link_libraries(native-lib ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libode.so)

target_link_libraries(native-lib ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libBullet2FileLoader.a)
target_link_libraries(native-lib ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libBullet3Common.a)
target_link_libraries(native-lib ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libBullet3Dynamics.a)
target_link_libraries(native-lib ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libBullet3Collision.a)
target_link_libraries(native-lib ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libBullet3Geometry.a)
target_link_libraries(native-lib ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libBullet3OpenCL_clew.a)
target_link_libraries(native-lib ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libBulletDynamics.a)
target_link_libraries(native-lib ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libBulletCollision.a)
target_link_libraries(native-lib ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libBulletInverseDynamics.a)
target_link_libraries(native-lib ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libBulletSoftBody.a)
target_link_libraries(native-lib ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libLinearMath.a)

target_link_libraries(native-lib ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libopenal.so)
