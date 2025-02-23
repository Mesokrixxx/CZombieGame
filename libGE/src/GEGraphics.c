#include "GEPrivate.h"

GLuint	lastUseShaderProgID;
GLuint	lastUseVAOID;

static void	_clearSS(void *ss) { GEDestroySparseSet(ss); _free(ss); }
static bool _createShaderProgram(GEGraphics *, GLuint *, const char *, const char *);
static bool	_compileShader(GLuint *, GLenum, const char *);
static char	*_getFileContent(const char *);

GEVertexObject	GECreateVertexObject(GLuint VAO, GLuint VBO)
{
	return ((GEVertexObject){ VAO, VBO });
}

void	GEDestroyVO(void *vVO)
{
	GEVertexObject	*VO;

	ASSERT(VO,
		"Trying to delete a NULL vertex object\n");
	
	VO = vVO;
	glDeleteVertexArrays(1, &VO->VAO);
	glDeleteBuffers(1, &VO->VBO);
	_free(vVO);
}

void	GEDestroyShader(void *sshader)
{
	GLuint	shader = *(GLuint *)sshader;

	glDeleteProgram(shader);
	_free(sshader);
}

bool	GEInitGraphics(GEGraphics *graphics, iVec2 size, GEProjection proj)
{
	ASSERT(graphics,
		"Trying to init graphics for instance but given one is NULL\n");

	GEMat4x4ToFloat(graphics->projectionMatrice, GECreateProjectionMatrice(size, proj));

	graphics->VOs = _malloc(sizeof(GESparseSet));
	if (!graphics->VOs)
		return (false);
	if (!GECreateSparseSet(graphics->VOs, sizeof(GEVertexObject), GE_VOS_CHUNK_SIZE, NULL, GEDestroyVO))
		return (_free(graphics->VOs), false);
		
	graphics->shaders = _malloc(sizeof(GESparseSet));
	if (!graphics->shaders)
		return (false);
	if (!GECreateSparseSet(graphics->shaders, sizeof(GLuint), GE_SHADERPROGS_CHUNK_SIZE, NULL, GEDestroyShader))
		return (_clearSS(graphics->VOs), _free(graphics->shaders), false);
	
	return (true);
}

bool	GERegisterShaderProgram(GEGraphics *graphics, const char *vertexFilePath, const char *fragFilePath, u32 shaderID)
{
	GLuint	shaderProg;

	ASSERT(graphics,
		"Trying to register a shader prog to a NULL graphics\n");

	ASSERT(vertexFilePath || fragFilePath,
		"Trying to create a shader prog with NULL path\n");

	ASSERT(!GEGetFromSparseSet(graphics->shaders, shaderID),
		"Trying to create a new shader but given ID already exist\n")
	
	if (!_createShaderProgram(graphics, &shaderProg, vertexFilePath, fragFilePath))
		return (false);
	
	return (GEAddToSparseSet(graphics->shaders, &shaderProg, shaderID));
}

bool	GERegisterVO(GEGraphics *graphics, GEVertexObject vo, u32 voID)
{
	ASSERT(graphics,
		"Failed to register VO, given graphics is NULL\n");

	ASSERT(!GEGetFromSparseSet(graphics->VOs, voID),
		"Failed to register VO, given one is NULL or already set\n");

	return (GEAddToSparseSet(graphics->VOs, &vo, voID));
}

void	GEUseShader(GEGraphics *graphics, u32 shaderID)
{
	static bool	firstSet = true;

	ASSERT(graphics,
		"Trying to use a shader but given graphics is NULL\n");

	if (firstSet || shaderID != lastUseShaderProgID)
	{
		glUseProgram(*(GLuint *)GEGetFromSparseSet(graphics->shaders, shaderID));
		lastUseShaderProgID = shaderID;
		firstSet = false;
	}
}

void	GEUseVAO(GEGraphics *graphics, u32 voID)
{
	static bool	firstSet = true;

	ASSERT(graphics,
		"Trying to use a shader but given graphics is NULL\n");

	if (firstSet || voID != lastUseVAOID)
	{
		glBindVertexArray(((GEVertexObject *)GEGetFromSparseSet(graphics->VOs, voID))->VAO);
		lastUseVAOID = voID;
		firstSet = false;
	}
}

void	GEDestroyGraphics(GEGraphics *graphics)
{
	ASSERT(graphics,
		"Trying to destroy NULL graphics\n");

	_clearSS(graphics->shaders);
	_clearSS(graphics->VOs);
}

static bool	_createShaderProgram(GEGraphics *graphics, GLuint *shaderProg, const char *vertPath, const char *fragPath)
{
	GLuint	vertProg;
	GLuint	fragProg;
	char	*vertSource;
	char	*fragSource;

	vertSource = _getFileContent(vertPath);
	if (!vertSource)
		return (false);
	fragSource = _getFileContent(fragPath);
	if (!fragSource)
		return (_free(vertSource), false);
	
	if (!_compileShader(&vertProg, GL_VERTEX_SHADER, vertSource)
		|| !_compileShader(&fragProg, GL_FRAGMENT_SHADER, fragSource))
	{
		_free(fragSource);
		_free(vertSource);
		return (false);
	}

	*shaderProg = glCreateProgram();
	glAttachShader(*shaderProg, vertProg);
	glAttachShader(*shaderProg, fragProg);
	glLinkProgram(*shaderProg);

	// Check errors
	int success;
	char infoLog[512];

	glGetProgramiv(*shaderProg, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(*shaderProg, 512, NULL, infoLog);
		
		_free(vertSource);
		_free(fragSource);
		return (false);
	}

	glDeleteShader(vertProg);
	_free(vertSource);
	glDeleteShader(fragProg);
	_free(fragSource);

	glUseProgram(*shaderProg);
	GLint	projLoc = glGetUniformLocation(*shaderProg, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, graphics->projectionMatrice);
	
	return (true);
}

static bool	_compileShader(GLuint *shader, GLenum type, const char *source)
{
	*shader = glCreateShader(type);
	glShaderSource(*shader, 1, &source, NULL);
	glCompileShader(*shader);

	// Check errors
	i32		success;
	char	infoLog[512];

	glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(*shader, 512, NULL, infoLog);
		return (false);
	}
	return (true);
}

static char	*_getFileContent(const char *path)
{
	FILE	*f;
	size_t	fsize;
	char	*fcontent;

	f = fopen(path, "rb");
	ASSERT(f,
		"Failed to open: %s\n", path);

	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	
	fcontent = _malloc(sizeof(char) * (fsize + 1));
	if (!fcontent)
		return ((void)fclose(f), NULL);
	
	if (fread(fcontent, fsize, sizeof(char), f) < 1)	
		return (fclose(f), _free(fcontent), NULL);
	fclose(f);
	fcontent[fsize] = '\0';
	return (fcontent);
}
