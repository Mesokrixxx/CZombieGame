#include "GEPrivate.h"

bool	GEInitGraphics(GEGraphics *graphics)
{
	ASSERT(graphics,
		"Trying to init graphics for instance but given one is NULL\n");

	graphics->VOs = _malloc(sizeof(GESparseSet));
	if (!graphics->VOs && !GECreateSparseSet(graphics->VOs, sizeof(VertexObject), ));
}

bool	GERegisterShaderProgram(GEGraphics *graphics, const char *vertexFilePath, const char *fragFilePath, u32 shaderID)
{

}

bool	GERegisterVO(GEGraphics *graphics, VertexObject *vo, u32 voID)
{

}

void	GEUseShader(GEGraphics *graphics, u32 shaderID)
{

}

void	GEUseVAO(GEGraphics *graphics, u32 voID)
{

}

void	GEDestroyGraphics(GEGraphics *graphics)
{

}
