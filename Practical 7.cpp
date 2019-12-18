#include <Windows.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gl/glext.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#include <string>

#include <cstdio>

using namespace std;

GLuint texture = 0;
BITMAP BMP;
HBITMAP hBMP = NULL;

string textures[3] = {"Brick.bmp", "Wood.bmp", "Metal.bmp"};
int textureNo = 0;

string projectRoot = "D:\\TARUC\\Sem 5\\BACS2173 Graphics Programming\\Practical7\\";
char temp[100];

int rotate = 0;
bool pyramid = true;

double w = 1920;
double h = 1080;
double ar = w / h; // aspect ratio

// use dedicated GPU to run
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
extern "C"
{
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

void initTexture(int textureNo)
{
    strcpy(temp, projectRoot.c_str());
    strcat(temp, textures[textureNo].c_str());

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), temp, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
    GetObject(hBMP, sizeof(BMP), &BMP);
}

void controls(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GL_TRUE);
        else if (key == GLFW_KEY_SPACE)
        {
            textureNo += 1;
            if (textureNo % 3 == 0)
            {
                textureNo = 0;
            }
            strcpy(temp, projectRoot.c_str());
            strcat(temp, textures[textureNo].c_str());
            printf("%d\n", textureNo);
            printf("%s\n", temp);

            initTexture(textureNo);
        }
        else if (key == GLFW_KEY_UP)
        {
            if (rotate == 0)
                rotate = 1;
            else
                rotate = 0;
        }
        else if (key == GLFW_KEY_DOWN)
        {
            if (rotate == 0)
                rotate = 2;
            else
                rotate = 0;
        }
        else if (key == GLFW_KEY_P)
        {
            if (pyramid)
                pyramid = false;
            else
                pyramid = true;
        }
}

GLFWwindow *initWindow(const int resX, const int resY)
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    // Open a window and create its OpenGL context
    // GLFWwindow *window = glfwCreateWindow(resX, resY, "Jaeger", glfwGetPrimaryMonitor(), NULL);
    GLFWwindow *window = glfwCreateWindow(resX, resY, "Practical 7", NULL, NULL);

    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, controls);

    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST); // Depth Testing
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    return window;
}

void drawPyramid()
{
    glPushMatrix();
    glColor3ub(255, 255, 255);
    glTranslatef(0, -0.5f, 0);
    //glRotatef(rotateDeg, 1, 1, 1);
    glBegin(GL_POLYGON);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, -0.5);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(-0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(0.5, 0, 0);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, -0.5);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(-0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(-0.5, 0, -1.0);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, -0.5);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(0.5, 0, -1.0);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, -0.5);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(0.5, 0, -1.0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(-0.5, 0, -1.0);
    }
    glEnd();

    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(-0.5, 0, -1);

        glTexCoord2f(1, 1);
        glVertex3f(-0.5, 0, 0);

        glTexCoord2f(1, 0.0f);
        glVertex3f(0.5, 0, 0);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.5, 0, -1);
    }
    glEnd();

    glPopMatrix();
}

void drawCube(float size)
{
    glBegin(GL_QUADS);
    // front
    // glColor3ub(30, 136, 229);
    glTexCoord2f(0.0f, 1);
    glVertex3f(0, 0, size);

    glTexCoord2f(1, 1);
    glVertex3f(size, 0, size);

    glTexCoord2f(1, 0.0f);
    glVertex3f(size, 0, 0);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, 0, 0);

    // left
    // glColor3ub(223, 120, 239);
    glTexCoord2f(0.0f, 1);
    glVertex3f(0, size, size);
    glTexCoord2f(1, 1);
    glVertex3f(0, 0, size);
    glTexCoord2f(1, 0.0f);
    glVertex3f(0, 0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, size, 0);

    // bottom
    // glColor3ub(128, 226, 126);
    glTexCoord2f(0.0f, 1);
    glVertex3f(0, size, 0);
    glTexCoord2f(1, 1);
    glVertex3f(size, size, 0);
    glTexCoord2f(1, 0.0f);
    glVertex3f(size, 0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, 0, 0);

    // right
    // glColor3ub(255, 255, 114);
    glTexCoord2f(0.0f, 1);
    glVertex3f(size, 0, size);
    glTexCoord2f(1, 1);
    glVertex3f(size, size, size);
    glTexCoord2f(1, 0.0f);
    glVertex3f(size, size, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(size, 0, 0);

    // behind
    // glColor3ub(255, 201, 71);
    glTexCoord2f(0.0f, 1);
    glVertex3f(size, size, size);
    glTexCoord2f(1, 1);
    glVertex3f(0, size, size);
    glTexCoord2f(1, 0.0f);
    glVertex3f(0, size, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(size, size, 0);

    // top
    // glColor3ub(115, 232, 255);
    glTexCoord2f(0.0f, 1);
    glVertex3f(0, size, size);
    glTexCoord2f(1, 1);
    glVertex3f(size, size, size);
    glTexCoord2f(1, 0.0f);
    glVertex3f(size, 0, size);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, 0, size);

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 0);

    if (rotate == 1)
    {
        glRotatef(-1, 1, 1, 1);
    }
    else if (rotate == 2)
    {
        glRotatef(1, 1, 1, 1);
    }

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth,
                 BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

    if (!pyramid)
    {
        drawCube(0.5);
    }
    else
    {
        drawPyramid();
    }

    glDisable(GL_TEXTURE_2D);
    DeleteObject(hBMP);
    glDeleteTextures(1, &texture);
}

int main(int argc, char **argv)
{
    GLFWwindow *window = initWindow(1920, 1080);

    if (NULL != window)
    {
        // handle resolution problem
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        double w = 1920;
        double h = 1080;
        double ar = w / h;
        glOrtho(-2 * ar, 2 * ar, -2, 2, -1, 10);

        glMatrixMode(GL_MODELVIEW);

        initTexture(textureNo);

        while (!glfwWindowShouldClose(window))
        {
            // Scale to window size
            GLint windowWidth, windowHeight;
            glfwGetWindowSize(window, &windowWidth, &windowHeight);
            glViewport(0, 0, windowWidth, windowHeight);

            display();

            // Update Screen
            glfwSwapBuffers(window);

            // Check for any input, or window movement
            glfwPollEvents();
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
