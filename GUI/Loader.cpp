#include "Loader.h"
#include <fstream>
#include <math.h>
#include <sstream>
#include <iostream>
#include "OpenGL.h"
#include "Model.h"
#include "Scene.h"

bool Loader::readFromConfig_OpenGL(std::istream & stream, std::string const & label)
{
	if (label == "FPS_Period")
	{
		stream >> OpenGL::FPS_PERIOD;
		return true;
	}

	return false;
}

bool Loader::readFromConfig_Scene(std::istream & stream, std::string const & label)
{
	if (label == "Scene_Period")
	{
		stream >> Scene::SCENE_PERIOD;
		return true;
	}

	if (label == "Scene_File")
	{
		stream >> Scene::SceneFile;
		return true;
	}

	return false;
}

bool Loader::readFromConfig_Models(std::istream & stream, std::string const & label)
{
	if (label == "Model")
	{
		std::string fileName;
		stream >> fileName;
		std::fstream file(fileName, std::ios::in | std::ios::binary);
		Model::Models.push_back(Model(file));

		if (!file.good())
			std::cout << "Model Load Failure: " << fileName << " Pos: " << file.tellg();

		file.close();
		return true;
	}

	return false;
}

void Loader::readFromConfig(std::istream & stream)
{
	// walk config line by line
	std::string oneLine;
	while (std::getline(stream, oneLine))
	{
		// extract name
		std::stringstream lineCont(oneLine);
		lineCont >> oneLine;

		// assign to correct variable //

		// HALT
		if (oneLine == "HALT")
			exit(0);

		// Configuration readers
		bool hit(readFromConfig_OpenGL(lineCont, oneLine) 
			|| readFromConfig_Models(lineCont, oneLine)
			|| readFromConfig_Scene(lineCont, oneLine));

		// unknown (filter comment/blank)
		if (!hit && !oneLine.empty() && oneLine[0] != ';')
			std::cout << "unparseable config line: " << oneLine << std::endl;
	}
}

void Loader::createDefaultConfig_OpenGL(std::ostream & stream)
{
	stream
		<< std::endl << ";;;OpenGL settings;;;" << std::endl << std::endl
		<< "FPS_Period " << OpenGL::FPS_PERIOD << "\t\t; Minimal delay between frames" << std::endl
		;
}

void Loader::createDefaultConfig_Scene(std::ostream & stream)
{
	stream
		<< std::endl << ";;;Scene settings;;;" << std::endl << std::endl
		<< "Scene_Period " << Scene::SCENE_PERIOD << "\t\t; Minimal delay between scene reload" << std::endl
		<< ";Scene_File " << "C:\\Temp\\Scene.dat" << "\t\t; Scene file" << std::endl
		;
}

void Loader::createDefaultConfig_Models(std::ostream & stream)
{
	stream
		<< std::endl << ";;;Model settings;;;" << std::endl << std::endl
		<< ";Model " << "C:\\Temp\\sample.model" << "\t\t; Models - to be used in order" << std::endl
		;
}

void Loader::createDefaultConfig(std::ostream & stream)
{
	createDefaultConfig_OpenGL(stream);
	createDefaultConfig_Scene(stream);
	createDefaultConfig_Models(stream);
}

void Loader::ProcessConfig(char const * fileName)
{
	// try to open config
	std::fstream config(fileName, std::ios::in);

	// Process config if present
	if (config)
	{
		readFromConfig(config);
		config.close();
	}
	// Create config
	else
	{
		config.open(fileName, std::ios::out);
		createDefaultConfig(config);
		config.close();

		std::cout << "CONFIG MISSING, attempted to create a default one" << std::endl;
	}
}