#ifndef _OpenGL_h_
#define _OpenGL_h_

class OpenGL;

#include <vector>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Loader.h"

class OpenGL
{
   friend class Loader; //< Allows loader to change values

   static HDC hDC;               //< Private GDI Device Context
   static HGLRC hRC;             //< Permanent Rendering Context
   static HWND hWnd;             //< Holds Our Window Handle
   static HINSTANCE hInstance;   //< Holds The Instance Of The Application

   static bool keys[256];	   // Array Used For The Keyboard Routine
   static bool active;		   // Window Active Flag Set To TRUE By Default
   static bool fullscreen;    // Fullscreen Flag Set To Fullscreen Mode By Default

   static GLfloat xRot; //< X rotation
   static GLfloat yRot; //< Y rotation
   static GLfloat zRot; //< Z rotation

   static GLfloat xPos; //< X position
   static GLfloat yPos; //< Y position
   static GLfloat zPos; //< Z position

   static HANDLE lock;
   static std::vector<GLfloat> vertex;
   static std::vector<GLfloat> colors;

   static long FPS_PERIOD;          //< Shortest delay between periods

   /// Resize And Initialize The GL Window
   /// \param [in] width Window width
   /// \param [in] height Window height
   static GLvoid resizeGLScene(GLsizei width, GLsizei height);

   /// Setup of OpenGL
   static int initGL();

   /// Drawing
   static void drawGLScene();

   /// Saves image of current scene
   static void saveImage();
   
   /// Properly kill the window
   static GLvoid killGLWindow();

   /// Creates OpenGL Window
   /// \param [in] title Window title
   /// \param [in] width Window/Fullscreen width
   /// \param [in] height Window/Fullscreen height
   /// \param [in] bits Bits per color (8/16/24/32)
   /// \param [in] fullscreenflag Whether to use fullscreen (otherwise window)
   static BOOL createGLWindow(wchar_t* title, int width, int height, int bits, bool fullscreenflag);

   /// Processing callback
   /// \param [in] hWnd Handle for window
   /// \param [in] uMsg Message
   /// \param [in] wParam Additional message information
   /// \param [in] lParam Additional message information
   static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

   /// Process program controlls
   static void controllBox();
   
	public:
		static void SwapInputBuffers(std::vector<GLfloat> & vertex, std::vector<GLfloat> & colors);

		static void OpenGLMain();
};

#endif