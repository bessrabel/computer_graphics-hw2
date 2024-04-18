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
> + build
> + include
> + lib
> + src
> + readmePng
> + screenshot

#### .vscode
visual studio code 프로젝트 설정 파일들 폴더 

#### build
실제 프로젝트가 빌드되는 폴더 (glfw3.dll 포함)

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
```
vscode를 실행 후, 다운받은 파일 폴더를 프로젝트 폴더로 선택 
명령창(F1 단축키)으로 CMake:configure 명령 선택하여 운영체제에 맞는 컴파일러 도구(gcc 등) 선택
다시 command를 입력할 수 있는 명령창을 열고 CMake:Build로 빌드(이때 CMakeList.txt 파일을 참고하여 자동으로 빌드됨)
마지막으로 디버그(명령창 CMake:dubug or ctrl+ F5)하여 실행 결과를 확인
```

 ---
 
### result

+ unshaded
  
![Unshaded](https://github.com/bessrabel/computer_graphics-hw2/blob/main/readmePng/unshaded.png)

 ---

 ### code

+ 모델링 변환을 위한 클래스
``` C++
class Modeling{
    float scale;
    vec3 transform;

    public:
        Modeling(int scale, vec3 transform) : scale(scale), transform(transform){}

        // 모델링 변환 행렬 생성
        mat4 doModelingTransformation(){
            mat4 Mm = mat4(scale, 0.f, 0.f, transform.x,
                           0.f, scale, 0.f, transform.y,
                           0.f, 0.f, scale, transform.z,
                           0.f, 0.f, 0.f, 1.f);
            return Mm; 
        }
};
```

+ 카메라 변환을 위한 클래스
``` C++
class Camera{
    vec3 e;
    vec3 u;
    vec3 v;
    vec3 w;

    public:
        Camera(vec3 e, vec3 u, vec3 v, vec3 w) : e(e), u(u), v(v), w(w) {}

        // 카메라 변환 행렬 생성
        mat4 doCameraTransformation(){
            mat4 Mcam = inverse(mat4(u.x, v.x, w.x, e.x,
                                     u.y, v.y, w.y, e.y,
                                     u.z, v.z, w.z, e.z,
                                     0, 0, 0, 1));
            return Mcam; 
        }
};
```

+ 원근 투영 변환을 위한 클래스
``` C++
class PerspectiveProjection{
    float l;
    float r;
    float b;
    float t;
    float n;
    float f;
    
    public:
        PerspectiveProjection(float l, float r, float b, float t, float n, float f) : l(l), r(r), b(b), t(t), n(n), f(f) {}

        // 원근 투영 변환 행렬 생성
        mat4 doPerspectiveProjectionTransformation(){
            mat4 Mper = mat4(2*n/(r-l), 0.f, (l+r)/(l-r), 0.f,
                              0.f, 2*n/(t-b), (b+t)/(b-t), 0.f,
                              0.f, 0.f, (f+n)/(n-f), 2*f*n/(f-n),
                              0.f, 0.f, 1.f, 0.f);
            return Mper; 
        }
};
```

+ 뷰포트 변환을 위한 클래스
``` C++
class Viewport{
    float nx;
    float ny;

    public:
    Viewport(int nx, int ny) : nx(nx), ny(ny) {}

    // 뷰포트 변환 행렬 생성
    mat4 doViewportPositionTranformation(){
        mat4 Mvp = mat4(nx/2, 0.f, 0.f, (nx-1)/2,
                        0.f, ny/2, 0.f, (ny-1)/2,
                        0.f, 0.f, 1.f, 0.f,
                        0.f, 0.f, 0.f, 1.f);
        return Mvp; 
    }
};
```

+ 4x4 행렬끼리의 곱을 위한 계산 함수
``` C++
mat4 mulmat4(mat4 a, mat4 b){ 
    mat4 result;
    for(int i = 0; i<4;i++){
        for(int j =0; j < 4; j ++){
            float sum = 0;
            for(int k =0; k < 4; k ++){
                sum += a[i][k]* b[k][j];
            }
            result[i][j] = sum;
        }
    } return result;
}
```

+ 4x4 행렬과 1x4 벡터의 곱 계산을 위한 함수
``` C++
vec4 mulmatvec4(mat4 a, vec4 v){ 
    vec4 result;
    for(int i = 0; i<4;i++){
        float sum = 0;
        for(int j =0; j < 4; j ++){
            sum += a[i][j] * v[j];
        }
        result[i] = sum;
    } return result;
}
```

+ 벡터를 정규화하되, w 값이 1이 되도록 보정하는 함수
``` C++
vec3 w_normalize(vec4 v){ 
    v /= v.w;
    return v;
}
```

+ 씬 생성 함수
``` C++
void create_scene(Modeling model, Camera cam, PerspectiveProjection projection, Viewport view)
{
    int width   = 32;
    int height  = 16;
    
    float theta, phi;
    int t;
    
    gNumVertices    = (height - 2) * width + 2;
    gNumTriangles   = (height - 2) * (width - 1) * 2;
    
    // 정점 배열 할당
    vertices        = new vec3[gNumVertices];
    gIndexBuffer    = new int[3*gNumTriangles];
    
    // 변환 행렬 계산
    mat4 Mm = model.doModelingTransformation();
    mat4 Mcam = cam.doCameraTransformation();
    mat4 Mper = projection.doPerspectiveProjectionTransformation();
    mat4 Mvp = view.doViewportPositionTranformation();

    mat4 Mtrans = mulmat4(mulmat4(mulmat4(Mvp, Mper), Mcam), Mm);

    t = 0;
    for (int j = 1; j < height-1; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            theta = (float) j / (height-1) * M_PI;
            phi   = (float) i / (width-1)  * M_PI * 2;
            
            float   x   = sinf(theta) * cosf(phi);
            float   y   = cosf(theta);
            float   z   = -sinf(theta) * sinf(phi);

            // 정점 좌표 계산 및 저장
            vec4 p0 = vec4(x,y,z,1);
            vec3 ps = w_normalize(mulmatvec4(Mtrans, p0));
            vertices[t] = ps;
            t++;
        }
    }
    
    // 추가 정점 추가
    vec4 p0 = vec4(0,1,0,1);
    vec3 ps = w_normalize(mulmatvec4(Mtrans, p0));
    vertices[t] = ps;
    t++;
    
    p0 = vec4(0,-1,0,1);
    ps = w_normalize(mulmatvec4(Mtrans, p0));
    vertices[t] = ps;
    t++;

    t = 0;
    // 삼각형 인덱스 배열 생성
    for (int j = 0; j < height-3; ++j)
    {
        for (int i = 0; i < width-1; ++i)
        {
            gIndexBuffer[t++] = j*width + i;
            gIndexBuffer[t++] = (j+1)*width + (i+1);
            gIndexBuffer[t++] = j*width + (i+1);
            gIndexBuffer[t++] = j*width + i;
            gIndexBuffer[t++] = (j+1)*width + i;
            gIndexBuffer[t++] = (j+1)*width + (i+1);
        }
    }
    for (int i = 0; i < width-1; ++i)
    {
        gIndexBuffer[t++] = (height-2)*width;
        gIndexBuffer[t++] = i;
        gIndexBuffer[t++] = i + 1;
        gIndexBuffer[t++] = (height-2)*width + 1;
        gIndexBuffer[t++] = (height-3)*width + (i+1);
        gIndexBuffer[t++] = (height-3)*width + i;
    }
}
```


+ main

``` C++

 // 픽셀당 DEPTHBUFFER를 저장할 배열
 std::vector<float> depthBuffer(Nx * Ny, -INFINITY);

// 삼각형 개수만큼 반복문 돌림
 for(int i = 0; i < gNumTriangles; i++){
        // 삼각형 정점 인덱스 찾기
        int k0 = gIndexBuffer[3*i + 0]; 
        int k1 = gIndexBuffer[3*i + 1];
        int k2 = gIndexBuffer[3*i + 2];

        vec3 a = vertices[k0]; // 삼각형 꼭짓점 좌표 a
        vec3 b = vertices[k1]; // 삼각형 꼭짓점 좌표 b
        vec3 c = vertices[k2]; // 삼각형 꼭짓점 좌표 c

        // Bounding Rectangle
        int xmin = floor(std::min(std::min(a.x, b.x), c.x));
        int xmax = ceil(std::max(std::max(a.x, b.x), c.x));
        int ymin = floor(std::min(std::min(a.y, b.y), c.y));
        int ymax = ceil(std::max(std::max(a.y, b.y), c.y));
        
        // Barycentric Coordinates
        float beta = ((a.y - c.y)* xmin + (c.x - a.x) * ymin + a.x * c.y - c.x * a.y)/
                     ((a.y - c.y)* b.x + (c.x - a.x) * b.y + a.x * c.y - c.x * a.y);
        float gamma = ((a.y - b.y)* xmin + (b.x - a.x) * ymin + a.x * b.y - b.x * a.y)/
                      ((a.y - b.y)* c.x + (b.x - a.x) * c.y + a.x * b.y - b.x * a.y);
                      
        // incremental algorithm 
        float betax = (a.y - c.y) / ((a.y - c.y)* b.x + (c.x - a.x) * b.y + a.x * c.y - c.x * a.y);
        float betay = (c.x - a.x) / ((a.y - c.y)* b.x + (c.x - a.x) * b.y + a.x * c.y - c.x * a.y);
        float gammax = (a.y - b.y) / ((a.y - b.y)* c.x + (b.x - a.x) * c.y + a.x * b.y - b.x * a.y);
        float gammay = (b.x - a.x) / ((a.y - b.y)* c.x + (b.x - a.x) * c.y + a.x * b.y - b.x * a.y);
        int n = (xmax-xmin) + 1;

        // 바운딩 된 영역 안 픽셀 확인 
        for(int y = ymin; y <= ymax; y ++){
            for(int x = xmin; x <= xmax; x ++){
                if (beta > 0 && gamma > 0 && beta + gamma < 1){ // 픽셀이 삼각형 내부에 있으면 드로우
                    float depth = beta * a.z + gamma * b.z + (1-(beta+gamma)) * c.z; // 픽셀 깊이 구하기 
                    if(depth > depthBuffer[y * Nx + x]) { //삼각형이 앞에 있으면 드로우
                        depthBuffer[y * Nx + x] = depth;
                        for (int i = 0; i < 3; i ++){
                            int idx = (y * Nx + x) * 3;
                            Image[idx+i] = 255; // 흰색으로 드로우
                        }
                    } 
                }
                beta += betax;
                gamma += gammax;
            }
            beta += betay -n * betax;
            gamma += gammay -n * gammax;
        }     
    }
```
