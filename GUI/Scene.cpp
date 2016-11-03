#include "Scene.h"
#include <assert.h>
#include <process.h>
#include "Model.h"

HANDLE Scene::reloadThread(0);
volatile bool Scene::reloadThreadRunning(false);

unsigned _stdcall Scene::realoadSceneHandler(void * instance)
{
	std::vector<GLfloat> vertex;
	std::vector<GLfloat> colors;

	while (reloadThreadRunning)
	{
		reloadScene(vertex, colors);
		Sleep(SCENE_PERIOD);
	}
	return 0;
}

long Scene::SCENE_PERIOD(15);
std::string Scene::SceneFile;

void Scene::reloadScene(std::vector<GLfloat> & vertex, std::vector<GLfloat> & colors)
{
	vertex.clear();
	colors.clear();

	unsigned trianglesEstimate(0);
	for (auto model : Model::Models)
		trianglesEstimate += model.Triangles.size();

	vertex.reserve(trianglesEstimate * 9); // X,Y,Z × A,B,C
	colors.reserve(trianglesEstimate * 9); // R,G,B × A,B,C

	for (auto model : Model::Models)
	{
		model.PrintVertexPositions(vertex);
		model.PrintVertexColors(colors);
	}

	OpenGL::SwapInputBuffers(vertex, colors);
}

void Scene::StartHandler()
{
	if (reloadThreadRunning)
		return;

	reloadThreadRunning = true;

	assert(!reloadThread);
	reloadThread = (HANDLE)_beginthreadex(NULL, 0, realoadSceneHandler, 0, 0, NULL);
}

void Scene::StopHandler()
{
	if (!reloadThreadRunning)
		return;
	reloadThreadRunning = false;

	WaitForSingleObject(reloadThread, 20*1000);

	CloseHandle(reloadThread);
	reloadThread = 0;
}