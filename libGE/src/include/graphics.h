#ifndef GAMEENGINE_SHADER_H
# define GAMEENGINE_SHADER_H

# include "vars.h"

# define DEFAULT_CIRCLE_ROUNDNESS 32
# define VERTEXOBJECT_CHUNK_SIZE 24

typedef enum {
	CIRCLE_VO,
	DEFAULT_VERTEX_OBJECT_COUNT,
}	VertexObjectDefault;

typedef struct {
	GLuint	VAO;
	GLuint	VBO;
}	VertexObject;

Bool	InitDefaultVOs();
void	DestroyVertexObject(void *vvo);

Mat4x4	CreateOrthoProjMatrice();
Mat4x4	CreateProjectionMatrice(ProjType pt);

void	CreateCirleVAO(GLuint *circleVAO, GLuint *circleVBO, i32 segments);

GLuint	CompileShader(GLenum type, const char *source);
GLuint	CreateShaderProgram();

#endif