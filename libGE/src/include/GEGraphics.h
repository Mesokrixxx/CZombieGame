#ifndef GAME_ENGINE_GRAPHICS_H
# define GAME_ENGINE_GRAPHICS_H

# include "GEVars.h"
# include "GESparseSet.h"

typedef struct {
	GLuint	VAO;
	GLuint	VBO;
}	GEVertexObject;

GEVertexObject	GECreateVertexObject(GLuint VAO, GLuint VBO);
void			GEDestroyVO(void *vVO);

typedef struct {
	GESparseSet	*VOs;
	GESparseSet	*shaders;
	float		projectionMatrice[16];
}	GEGraphics;

bool			GEInitGraphics(GEGraphics *graphics, GEProjection proj);
bool			GERegisterShaderProgram(GEGraphics *graphics, const char *vertexFilePath, const char *fragFilePath, u32 shaderID);
bool			GERegisterVO(GEGraphics *graphics, GEVertexObject *vo, u32 voID);
void			GEUseShader(GEGraphics *graphics, u32 shaderID);
void			GEUseVAO(GEGraphics *graphics, u32 voID);
void			GEDestroyGraphics(GEGraphics *graphics);

#endif