#ifndef GAMEENGINE_SHADER_H
# define GAMEENGINE_SHADER_H

# include "vars.h"

# define DEFAULT_CIRCLE_ROUNDNESS 32

# define VERTEXOBJECT_CHUNK_SIZE 24
# define SHADERPROGRAM_CHUNK_SIZE 24

typedef enum {
	CIRCLE_VO,
	RECT_VO,
	CROSS_VO,
	DEFAULT_VERTEX_OBJECT_COUNT,
}	VertexObjectDefault;

typedef enum {
	SHADERPROG_CIRCLE_DEFAULT,
	SHADERPROG_RECT_DEFAULT,
	SHADERPROG_CROSS_DEFAULT,
	DEFAULT_SHADER_PROGRAM_COUNT,
}	ShaderProgramDefault;

typedef struct {
	GLuint	VAO;
	GLuint	VBO;
}	VertexObject;

typedef struct {
	f32		radius;
	Color	color;
	f32		outlineSize;
	Color	outlineColor;
}	CircleSprite;

typedef struct {
	f32		width;
	f32		height;
	Color	color;
	f32		outlineSize;
	Color	outlineColor;
}	RectSprite;

typedef struct {
	f32		width;
	f32		height;
	f32		angle;
	Color	color;
	f32		outlineSize;
	Color	outlineColor;
}	CrossSprite;

Bool	InitDefaultVOs();
void	DestroyVertexObject(void *vvo);

Mat4x4	CreateOrthoProjMatrice();
Mat4x4	CreateProjectionMatrice(ProjType pt);

void	CreateRectVAO(GLuint *rectVAO, GLuint *rectVBO);
void	CreateCirleVAO(GLuint *circleVAO, GLuint *circleVBO, i32 segments);
void	CreateCrossVAO(GLuint *crossVAO, GLuint *crossVBO);

Bool	InitDefaultShaderProgram();

GLuint	CompileShader(GLenum type, const char *source);
void	DestroyShaderProgram(void *shaderProg);

#endif