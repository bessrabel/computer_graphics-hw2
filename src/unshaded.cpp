#include <iostream>
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>

using namespace std;
using namespace glm;

#define Nx 512
#define Ny 512

unsigned char Image[Nx*Ny*3];
float Depth[Nx*Ny];

class Modeling{
    float scale;
    vec3 transform;

    public:
        Modeling(int scale, vec3 transform) : scale(scale), transform(transform){}

        mat4 doModelingTransformation(){
            mat4 Mm = mat4(scale, 0.f, 0.f, transform.x,
                           0.f, scale, 0.f, transform.y,
                           0.f, 0.f, scale, transform.z,
                           0.f, 0.f, 0.f, 1.f);
            return Mm; 
        }
};

class Camera{
    vec3 e;
    vec3 u;
    vec3 v;
    vec3 w;

    public:
        Camera(vec3 e, vec3 u, vec3 v, vec3 w) : e(e), u(u), v(v), w(w) {}

        mat4 doCameraTransformation(){
            mat4 Mcam = inverse(mat4(u.x, v.x, w.x, e.x,
                                     u.y, v.y, w.y, e.y,
                                     u.z, v.z, w.z, e.z,
                                     0, 0, 0, 1));
            return Mcam; 
        }
};

class PerspectiveProjection{
    float l;
    float r;
    float b;
    float t;
    float n;
    float f;
    
    public:
        PerspectiveProjection(float l, float r, float b, float t, float n, float f) : l(l), r(r), b(b), t(t), n(n), f(f) {}

        mat4 doPerspectiveProjectionTransformation(){
            mat4 Mper = mat4(2*n/(r-l), 0.f, (l+r)/(l-r), 0.f,
                              0.f, 2*n/(t-b), (b+t)/(b-t), 0.f,
                              0.f, 0.f, (f+n)/(n-f), 2*f*n/(f-n),
                              0.f, 0.f, 1.f, 0.f);
            return Mper; 
        }
};

class Viewport{
    float nx;
    float ny;

    public:
    Viewport(int nx, int ny) : nx(nx), ny(ny) {}

    mat4 doViewportPositionTranformation(){
        mat4 Mvp = mat4(nx/2, 0.f, 0.f, (nx-1)/2,
                        0.f, ny/2, 0.f, (ny-1)/2,
                        0.f, 0.f, 1.f, 0.f,
                        0.f, 0.f, 0.f, 1.f);
        return Mvp; 
    }
};

mat4 mulmat4(mat4 a, mat4 b){ // 4x4 행렬 끼리의 곱 계산 함수
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

vec4 mulmatvec4(mat4 a, vec4 v){ // 4x4 행렬 x 1x4 행렬 곱 계산 함수
    vec4 result;
    for(int i = 0; i<4;i++){
        float sum = 0;
        for(int j =0; j < 4; j ++){
            sum += a[i][j] * v[j];
        }
        result[i] = sum;
    } return result;
}

vec3 w_normalize(vec4 v){ // vec4의 w 값이 1이 되도록 normalize
    v /= v.w;
    return v;
}

//
//  sphere_scene.c
//  Rasterizer
//
//

int     gNumVertices    = 0;    // Number of 3D vertices.
int     gNumTriangles   = 0;    // Number of triangles.
vec3*   vertices        = NULL;
int*    gIndexBuffer    = NULL; // Vertex indices for the triangles.

void create_scene(Modeling model, Camera cam, PerspectiveProjection projection, Viewport view)
{
    int width   = 32;
    int height  = 16;
    
    float theta, phi;
    int t;
    
    gNumVertices    = (height - 2) * width + 2;
    gNumTriangles   = (height - 2) * (width - 1) * 2;
    
    // TODO: Allocate an array for gNumVertices vertices.
    vertices        = new vec3[gNumVertices];
    gIndexBuffer    = new int[3*gNumTriangles];
    
    mat4 Mm = model.doModelingTransformation();
    mat4 Mcam = cam.doCameraTransformation();
    mat4 Mper = projection.doPerspectiveProjectionTransformation();
    mat4 Mvp = view.doViewportPositionTranformation();

    mat4 Mtrans = mulmat4(mulmat4(mulmat4(Mvp, Mper), Mcam), Mm); //순서대로 행렬 곱 계산

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

            // TODO: Set vertex t in the vertex array to {x, y, z}.
            vec4 p0 = vec4(x,y,z,1);
            vec3 ps = w_normalize(mulmatvec4(Mtrans, p0));
            vertices[t] = ps;
            t++;
        }
    }
    
    // TODO: Set vertex t in the vertex array to {0, 1, 0}.
    vec4 p0 = vec4(0,1,0,1);
    vec3 ps = w_normalize(mulmatvec4(Mtrans, p0));
    vertices[t] = ps;
    t++;
    
    // TODO: Set vertex t in the vertex array to {0, -1, 0}.
    p0 = vec4(0,-1,0,1);
    ps = w_normalize(mulmatvec4(Mtrans, p0));
    vertices[t] = ps;
    t++;

    t = 0;
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

int main(){
 
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(Nx, Ny, "Transformations and Rasterization Unshaded HW 2 (a) 202372001_김소영", NULL, NULL);

    if(window == NULL) 
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }   

    Modeling model = {2, vec3(0,0,-7)};
    Camera cam = {vec3(0,0,0), vec3(1,0,0), vec3(0,1,0), vec3(0,0,1)};
    PerspectiveProjection projection = {-0.1, 0.1, -0.1, 0.1, -0.1, -1000};
    Viewport view= {Nx, Ny};
    create_scene(model, cam, projection, view);

    glViewport(0, 0, 512, 512);
    
    for(int y = 0; y < Ny; y++){
        for(int x = 0; x < Nx; x++){
            Depth[y * Nx+ x] = -1*INFINITY; //음의 무한대로 깊이 초기화
        }
    }

    for(int i = 0; i < gNumTriangles; i++){
        int k0 = gIndexBuffer[3*i + 0]; 
        int k1 = gIndexBuffer[3*i + 1];
        int k2 = gIndexBuffer[3*i + 2];

        vec3 a = vertices[k0]; // 삼각형 꼭짓점 좌표 a
        vec3 b = vertices[k1]; // 삼각형 꼭짓점 좌표 b
        vec3 c = vertices[k2]; // 삼각형 꼭짓점 좌표 c

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

        for(int y = ymin; y <= ymax; y ++){
            for(int x = xmin; x <= xmax; x ++){
                if (beta > 0 && gamma > 0 && beta + gamma < 1){
                    if((a.z + b.z + c.z) / 3 > Depth[y * Nx + x]) { //삼각형이 앞에 있으면 출력
                        Depth[y * Nx + x] = (a.z + b.z + c.z) / 3;
                        for (int i = 0; i < 3; i ++){
                            int idx = (y * Nx + x) * 3;
                            Image[idx+i] = 255;
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

    while(!glfwWindowShouldClose(window))
    {   
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glad_glDrawPixels(Nx, Ny, GL_RGB, GL_UNSIGNED_BYTE, Image);
        glfwSwapBuffers(window);
        glfwPollEvents();     
    }

    glfwTerminate();

    return 0;
}
