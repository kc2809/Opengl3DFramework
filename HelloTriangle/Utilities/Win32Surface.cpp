#if WIN32

#include <GLES2\gl2.h>
#include <windows.h>
#include <EGL\egl.h>
#include <cstdio>
#include "ESContext.h"
#include "Timer.h"

EGLDisplay			eglDisplay;
EGLConfig			eglConfig;
EGLSurface			eglSurface;
EGLContext			eglContext;
EGLNativeWindowType	eglWindow;

ESContext context_tt;
// Windows variables
HWND hWnd;
HDC	 hDC;
void CleanWin();
bool TestEGLError(HWND hWnd, char* pszLocation);
bool InitWindow(int width, int height);

//Process Input
LRESULT CALLBACK WndProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT  lRet = 1; 
	int x = LOWORD(lParam); 
	int y = HIWORD(lParam);
	static bool mouseDown = false;

	switch (Msg) 
	{ 
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(WM_QUIT);
		break; 

	case WM_KEYDOWN:
		{
			if (context_tt.OnKey != 0)
			{
				context_tt.OnKey((int) wParam, true);
			}
		}		
		break;
	case WM_KEYUP:
		{
			if (context_tt.OnKey != 0)
			{
				context_tt.OnKey((int) wParam, false);
			}
		}
		break;
	case WM_LBUTTONDOWN:		
	case WM_RBUTTONDOWN:		
		mouseDown = true;		
		{
			if (context_tt.OnMouse != 0)
			{
				context_tt.OnMouse(0, x, y);
			}
		}
		break;

	case WM_LBUTTONUP:	
	case WM_RBUTTONUP:
		{
			mouseDown = false;
			if (context_tt.OnMouse != 0)
			{
				context_tt.OnMouse(1, x, y);
			}
		}
		break;
	case WM_MOUSEMOVE:		
		if (mouseDown)
		{			
			if (context_tt.OnMouse != 0)
			{
				context_tt.OnMouse(2, x, y);
			}
		}		
		break;
	default: 
		lRet = DefWindowProc (hWnd, Msg, wParam, lParam); 
		break; 
	} 

	return lRet; 
}
void CleanWin()
{
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

	eglTerminate(eglDisplay);

	if (hDC) 
		ReleaseDC(hWnd, hDC);

	if (hWnd) 
		DestroyWindow(hWnd);
}


bool InitWindow(int width, int height)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	// Register the windows class
	WNDCLASS sWC;
	sWC.style = CS_HREDRAW | CS_VREDRAW;
	sWC.lpfnWndProc = WndProcedure; //Procedute Callback Event
	sWC.cbClsExtra = 0;
	sWC.cbWndExtra = 0;
	sWC.hInstance = hInstance;
	sWC.hIcon = 0;
	sWC.hCursor = 0;
	sWC.lpszMenuName = 0;
	sWC.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	sWC.lpszClassName = "OpenGLES2.0";


	ATOM registerClass = RegisterClass(&sWC);
	if (!registerClass)
	{
		MessageBox(0, ("Failed to register the window class"), ("Error"), MB_OK | MB_ICONEXCLAMATION);
	}
	// Create the eglWindow
	RECT	sRect;
	SetRect(&sRect, 0, 0, width, height);
	AdjustWindowRectEx(&sRect, WS_CAPTION | WS_SYSMENU, false, 0);
	hWnd = CreateWindow( "OpenGLES2.0", "Sample", WS_VISIBLE | WS_SYSMENU,
		0, 0, sRect.right - sRect.left, sRect.bottom - sRect.top, NULL, NULL, hInstance, NULL);
	eglWindow = hWnd;

	// Get the associated device context
	hDC = GetDC(hWnd);
	if (!hDC)
	{
		MessageBox(0, "Failed to create the device context", "Error", MB_OK|MB_ICONEXCLAMATION);
		CleanWin();
		return false;
	}

	eglDisplay = eglGetDisplay(hDC);

	if(eglDisplay == EGL_NO_DISPLAY)
		eglDisplay = eglGetDisplay((EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);

	EGLint iMajorVersion, iMinorVersion;
	if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion))
	{
		MessageBox(0, ("eglInitialize() failed."), ("Error"), MB_OK|MB_ICONEXCLAMATION);
		CleanWin();
		return false;
	}

	const EGLint pi32ConfigAttribs[] =
	{
		EGL_LEVEL,				0,
		EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
		EGL_NATIVE_RENDERABLE,	EGL_FALSE,		
		EGL_DEPTH_SIZE,			8,
		EGL_STENCIL_SIZE, 8,
		EGL_SAMPLE_BUFFERS, 1,
		EGL_NONE
	};


	int iConfigs;
	if (!eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs) || (iConfigs != 1))
	{
		MessageBox(0, ("eglChooseConfig() failed."), ("Error"), MB_OK|MB_ICONEXCLAMATION);
		CleanWin();
		return false;
	}


	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);

	if(eglSurface == EGL_NO_SURFACE)
	{
		eglGetError(); // Clear error
		eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, NULL, NULL);
	}

	if (!TestEGLError(hWnd, "eglCreateWindowSurface"))
	{
		CleanWin();
		return false;
	}


	// Bind the API (It could be OpenGLES or OpenVG)
	eglBindAPI(EGL_OPENGL_ES_API);
	EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);

	if (!TestEGLError(hWnd, "eglCreateContext"))
	{
		CleanWin();
		return false;
	}


	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

	if (!TestEGLError(hWnd, "eglMakeCurrent"))
	{
		CleanWin();
		return false;
	}

	return true;
}
bool TestEGLError(HWND hWnd, char* pszLocation)
{	
	EGLint iErr = eglGetError();
	if (iErr != EGL_SUCCESS)
	{
		TCHAR pszStr[256];
		sprintf(pszStr, ("%s failed (%d).\n"), pszLocation, iErr);
		MessageBox(hWnd, pszStr, ("Error"), MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

void WinLoop()
{	
	MSG msg = {0};
	bool done = false;
	if (context_tt.OnInit)
	{
		context_tt.OnInit();
	}
	while (!done)
	{
		Timer::Tick();	
		if  (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{
				done = true; 
			}
			else
			{
				TranslateMessage(&msg); 
				DispatchMessage(&msg); 
			}
		}
		else
		{
			
		}

		if (context_tt.OnDraw)
		{
			context_tt.OnDraw();
		}
		if (context_tt.OnUpdate)
		{
			context_tt.OnUpdate();
		}

		eglSwapBuffers(eglDisplay, eglSurface);	
	}

	if (context_tt.OnDestroy)
	{
		context_tt.OnDestroy();
	}
}
void RegisterInitFunc(void (*OnInit)(void))
{
	context_tt.OnInit = OnInit;
}

void RegisterDestroyFunc(void (*OnDestroy)(void))
{
	context_tt.OnDestroy = OnDestroy;
}

void RegisterDrawFunc(void (*OnDraw)(void))
{
	context_tt.OnDraw = OnDraw;
}
void RegisterUpdateFunc(void (*OnUpdate)(void))
{
	context_tt.OnUpdate = OnUpdate;
}
void RegisterKeyFunc(void (*OnKey)(int, bool))
{
	context_tt.OnKey = OnKey;
}

void RegisterMouseFunc(void (*OnMouse)(int type, int x, int y))
{
	context_tt.OnMouse = OnMouse;
}

#endif