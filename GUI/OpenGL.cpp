#include "OpenGL.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <WinBase.h>
#include <assert.h>
#include "Locker.h"

HDC OpenGL::hDC = NULL;         //< Private GDI Device Context
HGLRC OpenGL::hRC = NULL;       //< Permanent Rendering Context
HWND OpenGL::hWnd = NULL;       //< Holds Our Window Handle
HINSTANCE OpenGL::hInstance;  //< Holds The Instance Of The Application

bool OpenGL::keys[256];			      //< Array Used For The Keyboard Routine
bool OpenGL::active(true);		      //< Window Active Flag Set To TRUE By Default
bool OpenGL::fullscreen(true);	   //< Fullscreen Flag Set To Fullscreen Mode By Default

GLfloat OpenGL::xRot(0); //< X rotation
GLfloat OpenGL::yRot(0); //< Y rotation
GLfloat OpenGL::zRot(0); //< Z rotation

GLfloat OpenGL::xPos(0);   //< X position
GLfloat OpenGL::yPos(0);   //< Y position
GLfloat OpenGL::zPos(0);   //< Z position (first set on windows resize)

HANDLE OpenGL::lock(CreateMutex(NULL, FALSE, L"LegoGuiMutex"));
std::vector<GLfloat> OpenGL::vertex;
std::vector<GLfloat> OpenGL::colors;

long OpenGL::FPS_PERIOD(15);        //< Shortest delay between periods

GLvoid OpenGL::resizeGLScene(GLsizei width, GLsizei height)
{
	if (height == 0) // Prevent A Divide By Zero By
		height = 1;   // Making Height Equal One

	zPos = -3.0; // Z position offset (so we can see the terrain)

	glViewport(0, 0, width, height); // Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix
	glLoadIdentity();             // Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);   // Select The Modelview Matrix
	glLoadIdentity();             // Reset The Modelview Matrix
}

int OpenGL::initGL()
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);			// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glTranslatef(xPos, yPos, zPos);
	return TRUE;										// Initialization Went OK
}

void OpenGL::drawGLScene()
{
	Locker Lock(lock);

	// Batched approach
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Rotations and movements
	glTranslatef(xPos, yPos, zPos);
	xPos = yPos = zPos = 0;

	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	xRot = yRot = zRot = 0;

	// Enable batch approach at graphics
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Provide with pointers
	glColorPointer(3, GL_FLOAT, 0, colors.data());
	glVertexPointer(3, GL_FLOAT, 0, vertex.data());

	// Load data
	glDrawArrays(GL_TRIANGLES, 0, vertex.size() / 3);

	// Disable batch approach for graphics
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	// Additional graphics
	glBegin(GL_LINES);
	glEnd();
}

void OpenGL::saveImage()
{
	///////////////////////
	// obtain image part //
	///////////////////////

	unsigned const bpp(3);

	// obtain sizing
	double ViewPortParams[4]; // [x, y, width, height]
	glGetDoublev(GL_VIEWPORT, ViewPortParams);
	unsigned int nWidth((unsigned int)ViewPortParams[2]);
	unsigned int nHeight((unsigned int)ViewPortParams[3]);

	// Compute alignement
	unsigned lineSize(nWidth*bpp);
	unsigned alignement((4 - (lineSize % 4)) % 4);

	// sizes
	unsigned pictSize((lineSize + alignement) * nHeight);

	// prepare buffer
	unsigned char *pixels = new unsigned char[pictSize];

	// obtain data
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glFlush();
	glFinish();
	glReadPixels(0, 0, nWidth, nHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

	/////////////////////
	// save image part //
	/////////////////////

	// Name generation - get time
	std::time_t time(std::time(0));
	tm nowTime;
	localtime_s(&nowTime, &time);

	// name generation - construct BMP
	std::stringstream stream;
	stream << nowTime.tm_year + 1900 << '-' << nowTime.tm_mon + 1 << '-' << nowTime.tm_mday << ' ';
	stream << nowTime.tm_hour << '-' << nowTime.tm_min << '-' << nowTime.tm_sec << " navMeshes.bmp";

	// BMP headers
	BITMAPFILEHEADER fileHeader;
	memset(&fileHeader, 0, sizeof(fileHeader));
	BITMAPINFOHEADER infoHeader;
	memset(&infoHeader, 0, sizeof(infoHeader));

	fileHeader.bfType = 0x4D42; // BM
	fileHeader.bfOffBits = sizeof(fileHeader) + sizeof(infoHeader);
	fileHeader.bfSize = sizeof(fileHeader) + sizeof(infoHeader) + pictSize;

	infoHeader.biSize = sizeof(infoHeader);
	infoHeader.biWidth = nWidth;
	infoHeader.biHeight = nHeight;
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = bpp << 3;

	// Output BMP
	std::ofstream outFile(stream.str().c_str(), std::ios::binary);
	outFile.write((char*)&fileHeader, sizeof(fileHeader));
	outFile.write((char*)&infoHeader, sizeof(infoHeader));
	outFile.write((char*)pixels, pictSize);
	outFile.close();

	// Cleaning
	delete[] pixels;
}

GLvoid OpenGL::killGLWindow()
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse PntVectorer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, L"Release Of DC And RC Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, L"Release Rendering Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, L"Release Device Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, L"Could Not Release hWnd.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass(L"OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, L"Could Not Unregister Class.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}

BOOL OpenGL::createGLWindow(wchar_t* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	fullscreen = fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)wndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow PntVectorer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = L"OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, L"Failed To Register The Window Class.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL, L"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", L"NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, L"Program Will Now Close.", L"ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle = WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse PntVectorer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		L"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL, L"Window Creation Error.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
	   sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
	   1,											// Version Number
	   PFD_DRAW_TO_WINDOW |						// Format Must Support Window
	   PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
	   PFD_DOUBLEBUFFER,							// Must Support Double Buffering
	   PFD_TYPE_RGBA,								// Request An RGBA Format
	   bits,										// Select Our Color Depth
	   0, 0, 0, 0, 0, 0,							// Color Bits Ignored
	   0,											// No Alpha Buffer
	   0,											// Shift Bit Ignored
	   0,											// No Accumulation Buffer
	   0, 0, 0, 0,									// Accumulation Bits Ignored
	   16,											// 16Bit Z-Buffer (Depth Buffer)  
	   0,											// No Stencil Buffer
	   0,											// No Auxiliary Buffer
	   PFD_MAIN_PLANE,								// Main Drawing Layer
	   0,											// Reserved
	   0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL, L"Can't Create A GL Device Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL, L"Can't Find A Suitable PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL, L"Can't Set The PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL, L"Can't Create A GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL, L"Can't Activate The GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	resizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!initGL())									// Initialize Our Newly Created GL Window
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL, L"Initialization Failed.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK OpenGL::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/// Process window message (return if message processing complete)
	switch (uMsg)									// Check For Windows Messages
	{
	case WM_ACTIVATE: // Handle window active state (minimzed)
		active = !HIWORD(wParam);
		return 0;

	case WM_SYSCOMMAND:           // Intercept System Commands
		switch (wParam)			   // Check System Calls
		{
		case SC_SCREENSAVE:	   // Screensaver Trying To Start?
		case SC_MONITORPOWER:   // Monitor Trying To Enter Powersave?
			return 0;			   // Prevent From Happening
		}
		break;							// Exit (i.e. continue processing other system commands)

	case WM_CLOSE:                // Close message received => Send quit message
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:              // Mark pressed keys
		keys[wParam] = true;
		return 0;

	case WM_KEYUP:                // Unmark released keys
		keys[wParam] = false;
		return 0;

	case WM_SIZE:						// Resize The OpenGL Window
		resizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void OpenGL::controllBox()
{
	// Print Screen
	if (keys[VK_DELETE])
	{
		keys[VK_DELETE] = false; // Prevent multiple operations
		saveImage();
	}

	// Movement handling
	if (!keys[VK_SHIFT])
	{
		if (keys[VK_RIGHT])
			xPos += .1f;
		else if (keys[VK_LEFT])
			xPos -= .1f;

		if (keys[VK_UP])
			yPos += .1f;
		else if (keys[VK_DOWN])
			yPos -= .1f;

		if (keys[VK_HOME])
			zPos += .1f;
		else if (keys[VK_END])
			zPos -= .1f;
	}

	// Rotation handling
	if (keys[VK_SHIFT])
	{
		if (keys[VK_DOWN])
			xRot += 1.0f;
		else if (keys[VK_UP])
			xRot -= 1.0f;

		if (keys[VK_RIGHT])
			yRot += 1.0f;
		else if (keys[VK_LEFT])
			yRot -= 1.0f;

		if (keys[VK_HOME])
			zRot += 1.0f;
		else if (keys[VK_END])
			zRot -= 1.0f;
	}

	// Config reload
	if (keys[VK_RETURN])
	{
		keys[VK_RETURN] = false; // Prevent multiple operations
		Loader::ProcessConfig("NavMeshes.conf"); // on change also change constant on first load (main.cpp)
		std::cout << "Config Reloaded" << std::endl;
	}
}

void OpenGL::SwapInputBuffers(std::vector<GLfloat> & vertex, std::vector<GLfloat> & colors)
{
	Locker Lock(lock);
	OpenGL::vertex.swap(vertex);
	OpenGL::colors.swap(colors);
}

void OpenGL::OpenGLMain()
{
	// Fullscreen or not
	fullscreen = (MessageBox(NULL, L"Would You Like To Run In Fullscreen Mode?", L"Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDYES);

	// Create Our OpenGL Window
	if (!createGLWindow(L"LegoDisassemble", 800, 600, 32, fullscreen))
		return;

	// Windows message structure
	MSG msg;

	// Major loop
	while (1)
	{
		// Process waiting message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// "Quit message" received, exit major loop (i.e. user closed window, taskmgr trying to kill us)
			if (msg.message == WM_QUIT)
				break;
			// "Quit message" not received
			else
			{
				TranslateMessage(&msg); // Translate The Message
				DispatchMessage(&msg);  // Dispatch The Message
			}
		}
		// No message to be processed
		else
		{
			// Watch keys only when program window active
			if (active)
			{
				// Controlls
				controllBox();

				// Esc pressed, exit major loop
				if (keys[VK_ESCAPE])
					break;
				// Esc not pressed, draw screen and swap buffers
				else
				{
					drawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
			}

			// Fullscreen mode change requested
			if (keys[VK_F1])
			{
				keys[VK_F1] = false;      // If So Make Key FALSE
				killGLWindow();         // Kill Our Current Window
				fullscreen = !fullscreen; // Toggle Fullscreen / Windowed Mode

				// Recreate OpenGL Window
				if (!createGLWindow(L"LegoDisassemble", 800, 600, 32, fullscreen))
					return;
			}
		}

		Sleep(FPS_PERIOD);
	}

	// Shutdown (program exit follows)
	killGLWindow();         // Kill The Window
}