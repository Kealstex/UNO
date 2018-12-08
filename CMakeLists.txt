cmake_minimum_required(VERSION 3.12)
project(untitled1)

set(CMAKE_CXX_STANDARD 14)
set(LIBS -static-libstdc++ -static-libgcc)
add_executable(${PROJECT_NAME} main.cpp Card.cpp Game.cpp Game.h)

target_link_libraries(${PROJECT_NAME} ${LIBS} -lSOIL -lopengl32 -lwinmm -lgdi32 -lfreeglut)
