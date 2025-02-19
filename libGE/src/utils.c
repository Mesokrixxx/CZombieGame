#include "GE.h"

char	*GetFileContent(const char *filePath)
{
	FILE	*f;
	char	*fileContent;
	i64		fileSize;
	
	if (!filePath)
		return ((void)LOG("Can't open file, path is set to NULL\n"), NULL);
	f = fopen(filePath, "r");
	if (!f)
		return ((void)LOG("Failed to open '%s'\n", filePath), NULL);
	
	fseek(f, 0, SEEK_END);
	fileSize = ftell(f);
	fseek(f, 0, SEEK_SET);

	fileContent = _malloc(sizeof(char) * fileSize);
	if (!fileContent)
		return ((void)LOG("Failed to allocate for file content\n"), NULL);
	
	if (!fread(fileContent, sizeof(char), fileSize, f))
		return((void)LOG("Failed to read file content\n"), _free(fileContent), NULL);
	return (fileContent);
}
