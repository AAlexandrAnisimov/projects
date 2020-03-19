#ifndef _IRenderer_h_
#define _IRenderer_h_

#include <vector>

#include <GL/glew.h>

#include "../util.h"
using namespace Util;
class IRenderer {
private:
	GLuint vbo;
	GLuint ebo;
	int elements_size;
	int vertices_size;
protected:
	const int vertex_size = 5;

	void gen_vertices_buffer(std::vector<GLfloat> *vertices);
	void gen_elements_buffer(std::vector<GLuint> *elements);
public:
	void draw(GLuint program, GLuint texture);

};

#endif
