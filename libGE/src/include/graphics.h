#ifndef GAMEENGINE_SHADER_H
# define GAMEENGINE_SHADER_H

# include "vars.h"

# define DEFAULT_CIRCLE_ROUNDNESS 32

# define VERTEXOBJECT_CHUNK_SIZE 24
# define SHADERPROGRAM_CHUNK_SIZE 24

typedef enum {
	CIRCLE_VO,
	DEFAULT_VERTEX_OBJECT_COUNT,
}	VertexObjectDefault;

typedef enum {
	SHADERPROG_CIRCLE_DEFAULT,
	DEFAULT_SHADER_PROGRAM_COUNT,
}	ShaderProgramDefault;

typedef struct {
	GLuint	VAO;
	GLuint	VBO;
}	VertexObject;

Bool	InitDefaultVOs();
void	DestroyVertexObject(void *vvo);

Mat4x4	CreateOrthoProjMatrice();
Mat4x4	CreateProjectionMatrice(ProjType pt);

void	CreateCirleVAO(GLuint *circleVAO, GLuint *circleVBO, i32 segments);

Bool	InitDefaultShaderProgram();

GLuint	CompileShader(GLenum type, const char *source);
void	DestroyShaderProgram(void *shaderProg);

#endif