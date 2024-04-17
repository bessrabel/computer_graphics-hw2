cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름과 버전 설정
project(MyProject VERSION 1.0)


# 빌드 디렉토리 설정
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# 헤더 파일 디렉토리 설정
include_directories(
    ${CMAKE_SOURCE_DIR}/include/GLFW
    ${CMAKE_SOURCE_DIR}/include/GLAD
    ${CMAKE_SOURCE_DIR}/include/GLM
)

# 예제 실행 파일 설정
add_executable(MyExecutable ${CMAKE_SOURCE_DIR}/src/unshaded.cpp ${CMAKE_SOURCE_DIR}/include/GLAD/glad.c)

# OpenGL 라이브러리 링크
find_package(OpenGL REQUIRED)
target_link_libraries(MyExecutable PRIVATE OpenGL::GL)

# GLFW 라이브러리 링크
target_link_libraries(MyExecutable PRIVATE ${CMAKE_SOURCE_DIR}/lib/libglfw3dll.a)

# 빌드 결과물 설정
set_target_properties(MyExecutable PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# C++17 표준 설정
set_property(TARGET MyExecutable PROPERTY CXX_STANDARD 17)