#include "Model.h"

std::vector<Model> Model::Models;

std::vector<Triangle> loadModel(std::istream & stream)
{
	stream.seekg(0, std::ios::end);
	std::streamoff size(stream.tellg());
	stream.seekg(0, std::ios::beg);
	unsigned int records((unsigned int)(size / sizeof(Triangle)));

	std::vector<Triangle> triangles;
	triangles.reserve(records);

	for (auto i(0U); i < records && stream.good(); ++i)
		triangles.push_back(Triangle(stream));

	return triangles;
}

Model::Model(std::istream & stream)
	: Triangles(loadModel(stream))
{}

void Model::PrintVertexPositions(std::vector<GLfloat> & target) const
{
	for (auto triangle : Triangles)
		triangle.PrintVertexPosition(target);
}

void Model::PrintVertexColors(std::vector<GLfloat> & target) const
{
	for (auto triangle : Triangles)
		triangle.PrintVertexColor(target);
}

void Model::PrintNormalsVertex(std::vector<GLfloat> & target) const
{
	for (auto triangle : Triangles)
		triangle.Normal.Print(target);
}