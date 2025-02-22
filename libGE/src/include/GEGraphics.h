#ifndef GAME_ENGINE_GRAPHICS_H
# define GAME_ENGINE_GRAPHICS_H

# include "GEVars.h"
# include "GESparseSet.h"

typedef struct {
	GLuint	VAO;
	GLuint	VBO;
}	VertexObject;

typedef struct {
	GESparseSet	*VOs;
	GESparseSet	*shaders;
}	GEGraphics;

bool	GEInitGraphics(GEGraphics *graphics);
bool	GERegisterShaderProgram(GEGraphics *graphics, const char *vertexFilePath, const char *fragFilePath, u32 shaderID);
bool	GERegisterVO(GEGraphics *graphics, VertexObject *vo, u32 voID);
void	GEUseShader(GEGraphics *graphics, u32 shaderID);
void	GEUseVAO(GEGraphics *graphics, u32 voID);
void	GEDestroyGraphics(GEGraphics *graphics);

#endif