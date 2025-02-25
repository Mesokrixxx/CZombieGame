#ifndef GAME_ENGINE_GRAPHICS_H
# define GAME_ENGINE_GRAPHICS_H

# include "GEVars.h"
# include "GESparseSet.h"

# define GE_COLOR_BLACK      (Color){ 0.0f, 0.0f, 0.0f, 1.0f }
# define GE_COLOR_WHITE      (Color){ 1.0f, 1.0f, 1.0f, 1.0f }
# define GE_COLOR_RED        (Color){ 1.0f, 0.0f, 0.0f, 1.0f }
# define GE_COLOR_GREEN      (Color){ 0.0f, 1.0f, 0.0f, 1.0f }
# define GE_COLOR_BLUE       (Color){ 0.0f, 0.0f, 1.0f, 1.0f }
# define GE_COLOR_YELLOW     (Color){ 1.0f, 1.0f, 0.0f, 1.0f }
# define GE_COLOR_CYAN       (Color){ 0.0f, 1.0f, 1.0f, 1.0f }
# define GE_COLOR_MAGENTA    (Color){ 1.0f, 0.0f, 1.0f, 1.0f }
# define GE_COLOR_ORANGE     (Color){ 1.0f, 0.647f, 0.0f, 1.0f }
# define GE_COLOR_PURPLE     (Color){ 0.5f, 0.0f, 0.5f, 1.0f }
# define GE_COLOR_BROWN      (Color){ 0.647f, 0.165f, 0.165f, 1.0f }
# define GE_COLOR_PINK       (Color){ 1.0f, 0.753f, 0.796f, 1.0f }
# define GE_COLOR_GRAY       (Color){ 0.5f, 0.5f, 0.5f, 1.0f }
# define GE_COLOR_LIGHT_GRAY (Color){ 0.827f, 0.827f, 0.827f, 1.0f }
# define GE_COLOR_DARK_GRAY  (Color){ 0.663f, 0.663f, 0.663f, 1.0f }
# define GE_COLOR_GOLD       (Color){ 1.0f, 0.843f, 0.0f, 1.0f }
# define GE_COLOR_SILVER     (Color){ 0.753f, 0.753f, 0.753f, 1.0f }
# define GE_COLOR_TEAL       (Color){ 0.0f, 0.5f, 0.5f, 1.0f }
# define GE_COLOR_NAVY       (Color){ 0.0f, 0.0f, 0.5f, 1.0f }
# define GE_COLOR_MAROON     (Color){ 0.5f, 0.0f, 0.0f, 1.0f }
# define GE_COLOR_OLIVE      (Color){ 0.5f, 0.5f, 0.0f, 1.0f }
# define GE_COLOR_CHOCOLATE  (Color){ 0.824f, 0.412f, 0.118f, 1.0f }
# define GE_COLOR_CORAL      (Color){ 1.0f, 0.498f, 0.314f, 1.0f }
# define GE_COLOR_VIOLET     (Color){ 0.933f, 0.51f, 0.933f, 1.0f }
# define GE_COLOR_INDIGO     (Color){ 0.294f, 0.0f, 0.509f, 1.0f }
# define GE_COLOR_LAVENDER   (Color){ 0.902f, 0.902f, 0.98f, 1.0f }
# define GE_COLOR_SALMON     (Color){ 0.98f, 0.502f, 0.447f, 1.0f }
# define GE_COLOR_TURQUOISE  (Color){ 0.251f, 0.878f, 0.816f, 1.0f }
# define GE_COLOR_BEIGE      (Color){ 0.961f, 0.961f, 0.863f, 1.0f }
# define GE_COLOR_CHARTREUSE (Color){ 0.498f, 1.0f, 0.0f, 1.0f }
# define GE_COLOR_CRIMSON    (Color){ 0.863f, 0.078f, 0.235f, 1.0f }
# define GE_COLOR_PERU       (Color){ 0.804f, 0.522f, 0.247f, 1.0f }

typedef struct {
	GLuint	VAO;
	GLuint	VBO;
}	GEVertexObject;

GEVertexObject	GECreateVertexObject(GLuint VAO, GLuint VBO);
void			GEDestroyVO(void *vVO);
void			GEDestroyShader(void *sshader);

typedef struct {
	GESparseSet	*VOs;
	GESparseSet	*shaders;
	f32			*projectionMatrice;
}	GEGraphics;

bool			GEInitGraphics(GEGraphics *graphics, iVec2 size, GEProjection proj);
bool			GERegisterShaderProgram(GEGraphics *graphics, const char *vertexFilePath, const char *fragFilePath, u32 shaderID);
bool			GERegisterVO(GEGraphics *graphics, GEVertexObject vo, u32 voID);
GLuint			GEUseShader(GEGraphics *graphics, u32 shaderID);
void			GEUseVAO(GEGraphics *graphics, u32 voID);
void			GEDestroyGraphics(GEGraphics *graphics);

#endif