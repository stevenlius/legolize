#ifndef _Model_h_
#define _Model_h_

struct Triangle;

#include <vector>
#include <istream>
#include "CommonClassMacros.h"
#include "OpenGL.h"
#include "Triangles.h"

struct Model
{
	DEFAULT_COPYMOVE(Model);
	DEFAULT_DESTROY(Model);

	static std::vector<Model> Models;

	std::vector<Triangle> const Triangles;

	Model(std::istream & stream);

	void PrintVertexPositions(std::vector<GLfloat> & target) const;
	void PrintVertexColors(std::vector<GLfloat> & target) const;
	void PrintNormalsVertex(std::vector<GLfloat> & target) const;
};

#endif