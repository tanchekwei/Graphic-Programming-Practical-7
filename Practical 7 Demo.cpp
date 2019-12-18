#include <Windows.h>
#include <gl/GL.h>
#include <gl/glext.h>

#include <math.h>
#pragma comment(lib, "OpenGL32.lib")
  
#define WINDOW_TITLE "OpenGL Window"

void drawCube(float size);

bool rotate = true;
GLuint texture = 0;
BITMAP BMP;
HBITMAP hBMP = NULL;

// use dedicated GPU to run
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		else if (wParam == VK_SPACE)
		{
			if (rotate)
				rotate = false;
			else
				rotate = true;
		}
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------
void initTexture() {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL),
		"/Box.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION |
		LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);
}

void display()
{
	// handle resolution problem
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double w = 1920;
	double h = 1080;
	double ar = w / h;
	glOrtho(-2 * ar, 2 * ar, -2, 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	if (rotate) {
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

	drawCube(0.5);

	glDisable(GL_TEXTURE_2D);
	DeleteObject(hBMP);
	glDeleteTextures(1, &texture);
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

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc))
		return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc))
		return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	initTexture();

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		display();

		SwapBuffers(hdc);
	}
	UnregisterClass(WINDOW_TITLE, wc.hInstance);
	return true;
}