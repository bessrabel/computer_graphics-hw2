# computer_graphics-hw1

## CGIP_hw1
---

### Prerequisites
#### 1. Visual Studio Code & Extensions
Visual Studio Code 설치 및 아래의 2개 확장 설치 필요

+ CMake

![CMake](https://github.com/bessrabel/computer_graphics-hw2/blob/main/readmePng/cmake.PNG)


+ CMake Tools

![CMake Tools](https://github.com/bessrabel/computer_graphics-hw2/blob/main/readmePng/cmakeTools.PNG)

#### 2. g++
Using GCC with MinGW

설치 참조: <https://code.visualstudio.com/docs/cpp/config-mingw#_create-hello-world> **PATH 추가 필수**

#### 3. CMake 
Using CMake

설치 참조: <https://cmake.org/download/> **version 3.29.2**

---

### Directory Layout
> + .vscode
> + builds
> + include
> + lib
> + src
> + readmePng
> + screenshot

#### .vscode
visual studio code 프로젝트 설정 파일들 폴더

#### builds
실제 프로젝트가 빌드되는 폴더

#### include & lib
라이브러리 폴더
+ GLFW (Version 3.3.bin.win32)
+ GLAD (gl: 4.6, profile: Core, extensions: none)
+ GLM (Version 0.9.9.6)

#### readmePng
README.md 파일 이미지 첨부를 위한 폴더

#### screenshot
코드 실행 결과 스크린샷

#### src
소스 코드 폴더 (.cpp파일)
+ unshaded.cpp 
---

### compilation instructions

> CMake 디버그 도구로 g++ 선택 후 CMake:디버그 시 CMakeList.txt 파일을 참고하여 자동으로 빌드됨.

#### Unshaded

+ result
![Unshaded](https://github.com/bessrabel/computer_graphics-hw2/blob/main/readmePng/unshaded.png)

