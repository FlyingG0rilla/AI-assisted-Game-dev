cmake_minimum_required(VERSION 3.31)
project(ProjektarbeitSpiel_V1)

set(CMAKE_CXX_STANDARD 20)

#SDL2 Pfad
set(SDL2_PATH "C:/DevLib/SDL2-2.32.2/x86_64-w64-mingw32")

#SDL2_ttf Pfad
set(SDL2_TTF_PATH "C:/DevLib/SDL2_ttf-2.22.0/x86_64-w64-mingw32")

#SDL2 Image
set(SDL2_IMAGE_PATH "C:/DevLib/SDL2_image-2.8.6/x86_64-w64-mingw32")


#include_directories(${SDL2_PATH}/include)
#link_directories(${SDL2_PATH}/lib)

#Alle .cpp-Dateien aus src/
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(ProjektarbeitSpiel_V1 ${SOURCES}
        src/render/Renderer.cpp
        src/render/Renderer.h
        src/core/Vector2.h
        src/core/Input.cpp
        src/core/Input.h
        src/entities/Player.cpp
        src/entities/Player.h
        src/entities/Enemy.h
        src/entities/Runner.cpp
        src/entities/Runner.h
        src/entities/Bullet.cpp
        src/entities/Bullet.h
        src/gameplay/wavemanager.cpp
        src/gameplay/wavemanager.h
        src/entities/SpikyWalker.cpp
        src/entities/SpikyWalker.h
        src/entities/EnemyBullet.cpp
        src/entities/EnemyBullet.h
        src/entities/Shooter.cpp
        src/entities/Shooter.h
        src/entities/Item.h
        src/entities/AmmoItem.h
        src/entities/HealthItem.h
        src/render/AnimationController.cpp
        src/render/AnimationController.h)


#SDL2 main weglassen, SDL_MAIN_HANDLED definiert
target_include_directories(ProjektarbeitSpiel_V1 PRIVATE
        "${SDL2_PATH}/include"
        "${SDL2_TTF_PATH}/include"
        "${SDL2_IMAGE_PATH}/include"
        "src"
)

target_link_directories(ProjektarbeitSpiel_V1 PRIVATE
        "${SDL2_PATH}/lib"
        "${SDL2_TTF_PATH}/lib"
        "${SDL2_IMAGE_PATH}/lib"
)

target_include_directories(ProjektarbeitSpiel_V1 PRIVATE "src")
target_link_libraries(ProjektarbeitSpiel_V1
        SDL2main
        SDL2
        SDL2_ttf
        SDL2_image
)


file(COPY "${CMAKE_SOURCE_DIR}/assets" DESTINATION "${CMAKE_BINARY_DIR}")



#DLL kopieren (Windows)
if(WIN32 AND EXISTS "${SDL2_PATH}/bin/SDL2.dll")
file(COPY "${SDL2_PATH}/bin/SDL2.dll" DESTINATION ${CMAKE_BINARY_DIR})
endif()

# SDL2_ttf DLL
if (WIN32 AND EXISTS "${SDL2_TTF_PATH}/bin/SDL2_ttf.dll")
    file(COPY "${SDL2_TTF_PATH}/bin/SDL2_ttf.dll" DESTINATION ${CMAKE_BINARY_DIR})
endif()

if (WIN32 AND EXISTS "${SDL2_IMAGE_PATH}/bin/SDL2_image.dll")
    file(COPY "${SDL2_IMAGE_PATH}/bin/SDL2_image.dll"
            DESTINATION ${CMAKE_BINARY_DIR})
endif()

