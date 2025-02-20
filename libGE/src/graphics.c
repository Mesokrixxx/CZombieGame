#include "GE.h"

extern Instance	*instance;

GLuint	lastShaderProg;
GLuint	lastVAO;

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

void		BindVAO(u32 voID)
{
	static Bool	firstUse = true;
	
	if (firstUse || voID != lastVAO)
	{
		glBindVertexArray(GetVAO(voID));
		lastVAO = voID;
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
		glDeleteShader(shader);
		return 0;
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
		return ((void)LOG("Failed to get vertex shader source\n"), 0);
	
	fragmentShaderSource = GetFileContent(fragmentShaderPath);
	if (!fragmentShaderSource)
	{
		_free(vertexShaderSource);
		return ((void)LOG("Failed to get fragment shader source\n"), 0);
	}

	vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	if (!vertexShader)
	{
		_free(vertexShaderSource);
		_free(fragmentShaderSource);
		return 0;
	}

	fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	if (!fragmentShader)
	{
		_free(vertexShaderSource);
		_free(fragmentShaderSource);
		glDeleteShader(vertexShader);
		return 0;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check errors
	int success;
	char infoLog[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		LOG("Shader error: linking fail: %s\n", infoLog);
		glDeleteProgram(shaderProgram);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		_free(vertexShaderSource);
		_free(fragmentShaderSource);
		return 0;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	_free(vertexShaderSource);
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

void	CreateRectVAO(GLuint *rectVAO, GLuint *rectVBO)
{
	f32 vertices[] = {
		0.0f, 0.0f, // bas-gauche
		0.0f, 1.0f, // haut-gauche
		1.0f, 0.0f, // bas-droit
		1.0f, 1.0f  // haut-droit
	};

	glGenVertexArrays(1, rectVAO);
	glGenBuffers(1, rectVBO);

	glBindVertexArray(*rectVAO);

	glBindBuffer(GL_ARRAY_BUFFER, *rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void		DrawCircle(Vec2 pos, CircleSprite circle)
{
	GLuint	shaderProg = GetShaderProgram(SHADERPROG_CIRCLE_DEFAULT);
	
	UseShader(shaderProg);

	BindVAO(CIRCLE_VO);
	
	Mat4x4	model = Mat4x4Identity();
	TranslateMat4x4(&model, Vec3FromVec2(pos, 0));
	ScaleMat4x4(&model, (Vec3){ circle.radius, circle.radius, 1 });

	GLint	modelLoc = glGetUniformLocation(shaderProg, "model");
	GLint	colorLoc = glGetUniformLocation(shaderProg, "color");
	GLint	outlineSizeLoc = glGetUniformLocation(shaderProg, "outlineSize");
	GLint	outlineColorLoc = glGetUniformLocation(shaderProg, "outlineColor");
	float	matrix[16];
	
	Mat4x4ToFloat(model, matrix);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, matrix);
	glUniform1f(outlineSizeLoc, circle.outlineSize);
	glUniform4f(colorLoc, circle.color.r, circle.color.g,
		circle.color.b, circle.color.a);
	glUniform4f(outlineColorLoc, circle.outlineColor.r, circle.outlineColor.g,
		circle.outlineColor.b, circle.outlineColor.a);
	glDrawArrays(GL_TRIANGLE_FAN, 0, DEFAULT_CIRCLE_ROUNDNESS);
}

void	DrawRect(Vec2 pos, RectSprite rect)
{
	GLuint	shaderProg = GetShaderProgram(SHADERPROG_RECT_DEFAULT);

	UseShader(shaderProg);

	BindVAO(RECT_VO);

	Mat4x4	model = Mat4x4Identity();
	TranslateMat4x4(&model, Vec3FromVec2(pos, 0));
	ScaleMat4x4(&model, (Vec3){ rect.width, rect.height, 1 });

	GLint	modelLoc = glGetUniformLocation(shaderProg, "model");
	GLint	colorLoc = glGetUniformLocation(shaderProg, "color");
	GLint	outlineSizeLoc = glGetUniformLocation(shaderProg, "outlineSize");
	GLint	outlineColorLoc = glGetUniformLocation(shaderProg, "outlineColor");
	GLint	rectSizeLoc = glGetUniformLocation(shaderProg, "rectSize");
	float	matrix[16];
	
	Mat4x4ToFloat(model, matrix);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, matrix);
	glUniform4f(colorLoc, rect.color.r, rect.color.g, rect.color.b, rect.color.a);
	glUniform1f(outlineSizeLoc, rect.outlineSize);
	glUniform4f(outlineColorLoc, rect.outlineColor.r, rect.outlineColor.g, rect.outlineColor.b, rect.outlineColor.a);
	glUniform2f(rectSizeLoc, rect.width, rect.height);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void CreateCrossVAO(GLuint *crossVAO, GLuint *crossVBO)
{
    float size = 1.0f;
    float thickness = 0.4f;
    float halfThickness = thickness / 2.0f;

    float vertices1[8] = {
        -size, -halfThickness,
         size, -halfThickness,
         size,  halfThickness,
        -size,  halfThickness
    };

    float vertices2[8] = {
        -halfThickness, -size,
         halfThickness, -size,
         halfThickness,  size,
        -halfThickness,  size
    };

    float vertices[16];
    memcpy(vertices, vertices1, sizeof(vertices1));
    memcpy(vertices + 8, vertices2, sizeof(vertices2));

    glGenVertexArrays(1, crossVAO);
    glGenBuffers(1, crossVBO);

    glBindVertexArray(*crossVAO);
    glBindBuffer(GL_ARRAY_BUFFER, *crossVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void	DrawCross(Vec2 pos, CrossSprite cross)
{
	GLuint	shaderProg = GetShaderProgram(SHADERPROG_CROSS_DEFAULT);

	UseShader(shaderProg);
    
    BindVAO(CROSS_VO);

	Mat4x4	model = Mat4x4Identity();
	TranslateMat4x4(&model, Vec3FromVec2(pos, 0));
	ScaleMat4x4(&model, (Vec3){ cross.width, cross.height, 1 });
	if (cross.angle != 0)
		RotateMat4x4(&model, cross.angle);

	GLint	modelLoc = glGetUniformLocation(shaderProg, "model");
	GLint	colorLoc = glGetUniformLocation(shaderProg, "color");
	GLint	outlineSizeLoc = glGetUniformLocation(shaderProg, "outlineSize");
	GLint	outlineColorLoc = glGetUniformLocation(shaderProg, "outlineColor");
	GLint	crossSizeLoc = glGetUniformLocation(shaderProg, "crossSize");
	float	matrix[16];
	
	Mat4x4ToFloat(model, matrix);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, matrix);
	glUniform4f(colorLoc, cross.color.r, cross.color.g, cross.color.b, cross.color.a);
	glUniform1f(outlineSizeLoc, cross.outlineSize);
	glUniform4f(outlineColorLoc, cross.outlineColor.r, cross.outlineColor.g, cross.outlineColor.b, cross.outlineColor.a);
	glUniform2f(crossSizeLoc, cross.width, cross.height);
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    glBindVertexArray(0);
}

void	DestroyShaderProgram(void *shaderProg)
{
	GLuint	*shprog = shaderProg;

	glDeleteProgram(*shprog);
}
