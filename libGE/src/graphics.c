#include "GE.h"

extern Instance	*instance;

GLuint	lastShaderProg;

Bool	RegisterVertexObject(VertexObject vo, u32 voID)
{
	if (!AddToSparseSet(instance->VOs, &vo, voID))
		return ((void)LOG("Failed to add VO to instance\n"), false);
	return (true);
}

GLuint	GetVAO(u32 voID)
{
	SparseSet	*ss;

	ss = instance->VOs;
	return (((VertexObject *)ss->comp[ss->sparse[voID]])->VAO);
}

GLuint	GetVBO(u32 voID)
{
	SparseSet	*ss;

	ss = instance->VOs;
	return (((VertexObject *)ss->comp[ss->sparse[voID]])->VBO);
}

GLuint	GetShaderProgram(u32 shaderProgID)
{
	SparseSet	*ss;

	ss = instance->shaderPrograms;
	return (*(GLuint *)ss->comp[ss->sparse[shaderProgID]]);
}

void		UseShader(GLuint shaderProg)
{
	static Bool	firstUse = true;
	
	if (firstUse || shaderProg != lastShaderProg)
	{
		glUseProgram(shaderProg);
		lastShaderProg = shaderProg;
		firstUse = false;
	}
}

void	DestroyVertexObject(void *vvo)
{
	VertexObject	*vo = vvo;
	
	glDeleteBuffers(1, &vo->VBO);
	glDeleteVertexArrays(1, &vo->VAO);
}

GLuint	CompileShader(GLenum type, const char *source)
{
	GLuint	shader;

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	// Check errors
	i32		success;
	char	infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		LOG("Shader error: compilation fail: %s\n", infoLog);
	}
	return (shader);
}

GLuint	CreateShaderProgram(const char *vertexShaderPath, const char *fragmentShaderPath)
{
	GLuint	shaderProgram;

	GLuint	vertexShader;
	char	*vertexShaderSource;
	GLuint	fragmentShader;
	char	*fragmentShaderSource;

	vertexShaderSource = GetFileContent(vertexShaderPath);
	if (!vertexShaderSource)
		return ((void)LOG("Failed to get base vertex shader source\n"), 0);
	fragmentShaderSource = GetFileContent(fragmentShaderPath);
	if (!fragmentShaderSource)
		return ((void)LOG("Failed to get base vertex shader source\n"), _free(vertexShaderSource), 0);

	vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check errors
	int success;
	char infoLog[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		LOG("Shader error: compilation fail: %s\n", infoLog);
		return (0);
	}

	glDeleteShader(vertexShader);
	_free(vertexShaderSource);
	glDeleteShader(fragmentShader);
	_free(fragmentShaderSource);

	glUseProgram(shaderProgram);
	GLint	projLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, instance->projectionMatrice);

	return (shaderProgram);
}

void		CreateCirleVAO(GLuint *circleVAO, GLuint *circleVBO, i32 segments)
{
	f32		*vertices;
	
	vertices = _malloc(sizeof(f32) * segments * 2);
	if (!vertices)
		return ((void)LOG("Failed to allocate for vertices\n"));

	f32	angleStep = (PI * 2.0f) / segments;
	for (i32 i = 0; i < segments; i++)
	{
		float	theta = angleStep * i;

		vertices[i * 2] = SDL_cosf(theta);
		vertices[i * 2 + 1] = SDL_sinf(theta);
	}

	glGenVertexArrays(1, circleVAO);
	glGenBuffers(1, circleVBO);

	glBindVertexArray(*circleVAO);

	glBindBuffer(GL_ARRAY_BUFFER, *circleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * segments * 2, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	_free(vertices);
}

void		DrawCircle(Vec2 *pos, CircleSprite *circle)
{
	GLuint	shaderProg = GetShaderProgram(SHADERPROG_CIRCLE_DEFAULT);
	
	UseShader(shaderProg);

	glBindVertexArray(GetVAO(CIRCLE_VO));
	
	Mat4x4	model = Mat4x4Identity();
	TranslateMat4x4(&model, Vec3FromVec2(*pos, 0));
	ScaleMat4x4(&model, (Vec3){ circle->radius, circle->radius, 1 });

	GLint	modelLoc = glGetUniformLocation(shaderProg, "model");
	GLint	colorLoc = glGetUniformLocation(shaderProg, "color");
	GLint	outlineSizeLoc = glGetUniformLocation(shaderProg, "outlineSize");
	GLint	outlineColorLoc = glGetUniformLocation(shaderProg, "outlineColor");
	float	matrix[16];
	
	Mat4x4ToFloat(model, matrix);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, matrix);
	glUniform1f(outlineSizeLoc, circle->outlineSize);
	glUniform4f(colorLoc, circle->color.r, circle->color.g,
		circle->color.b, circle->color.a);
	glUniform4f(outlineColorLoc, circle->outlineColor.r, circle->outlineColor.g,
		circle->outlineColor.b, circle->outlineColor.a);
	glDrawArrays(GL_TRIANGLE_FAN, 0, DEFAULT_CIRCLE_ROUNDNESS);

	glBindVertexArray(0);
}

void	DestroyShaderProgram(void *shaderProg)
{
	GLuint	*shprog = shaderProg;

	glDeleteProgram(*shprog);
}
