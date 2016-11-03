#ifndef _Loader_h_
#define _Loader_h_

#include <iostream>

class Loader
{
	static bool readFromConfig_OpenGL(std::istream & stream, std::string const & label);

	static bool readFromConfig_Models(std::istream & stream, std::string const & label);

	static bool readFromConfig_Scene(std::istream & stream, std::string const & label);

	static void readFromConfig(std::istream & stream);

	static void createDefaultConfig_OpenGL(std::ostream & stream);

	static void createDefaultConfig_Models(std::ostream & stream);

	static void createDefaultConfig_Scene(std::ostream & stream);

	static void createDefaultConfig(std::ostream & stream);

	public:
		static void ProcessConfig(char const * fileName);
};

#endif