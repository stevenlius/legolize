#ifndef _Triangles_h_
#define _Triangles_h_

struct PntVector;
struct Vertice;
struct Triangle;

#include <vector>
#include <istream>
#include "CommonClassMacros.h"
#include "OpenGL.h"

struct PntVector
{
	DEFAULT_COPYMOVE(PntVector);
	DEFAULT_DESTROY(PntVector);

	GLfloat const X;
	GLfloat const Y;
	GLfloat const Z;

	// [X],[Y],[Z]
	PntVector(std::istream & stream);

	void Print(std::vector<GLfloat> & target) const;
};

struct Vertice
{
	DEFAULT_COPYMOVE(Vertice);
	DEFAULT_DESTROY(Vertice);

	PntVector const Position;
	PntVector const Color;

	// Pos.X, Pos.Y, Pos.Z, Col.R, Col.G, Col.B
	Vertice(std::istream & stream);
};

struct Triangle
{
	DEFAULT_COPYMOVE(Triangle);
	DEFAULT_DESTROY(Triangle);

	Vertice const A;
	Vertice const B;
	Vertice const C;
	PntVector const Normal;

	// A.X, A.Y, A.Z, A.R, A.G, A.B
	// B.X, B.Y, B.Z, B.R, B.G, B.B
	// C.X, C.Y, C.Z, C.R, C.G, C.B
	// Norm.X, Norm.Y, Norm.Z
	Triangle(std::istream & stream);

	void PrintVertexPosition(std::vector<GLfloat> & target);
	void PrintVertexColor(std::vector<GLfloat> & target);
};

#endif