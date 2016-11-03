#include <iostream>
#include "Loader.h"
#include "OpenGL.h"
#include "Scene.h"

int main(int argc, char const * argv[])
{
   Loader::ProcessConfig("LegoDisassemble.conf");
   Scene::StartHandler();
   OpenGL::OpenGLMain();
   Scene::StopHandler();
}