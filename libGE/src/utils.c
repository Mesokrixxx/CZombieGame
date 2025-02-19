#include "GE.h"

char	*GetFileContent(const char *filePath)
{
	FILE	*f;
	char	*fileContent;
	size_t	fileSize;

	if (!filePath)
		return ((void)LOG("Can't open file, path is set to NULL\n"), NULL);
	f = fopen(filePath, "rb");
	if (!f)
		return ((void)LOG("Failed to open file: %s\n", filePath), NULL);

	fseek(f, 0, SEEK_END);
	fileSize = ftell(f);
	fseek(f, 0, SEEK_SET);

	fileContent = _malloc(sizeof(char) * (fileSize + 1));
	if (!fileContent)
		return ((void)LOG("Failed to allocate for file content\n"), fclose(f), NULL);
	
	if (!fread(fileContent, sizeof(char), fileSize, f))
		return((void)LOG("Failed to read file content\n"), _free(fileContent), fclose(f), NULL);

	fileContent[fileSize] = '\0';
	
	fclose(f);
	return (fileContent);
}
