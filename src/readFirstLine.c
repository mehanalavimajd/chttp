/*
 * readFirstLine.c:
 * Gets the buffer from client and returns the struct of first line
 * consisting of Method, Path and Http version.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define MAXPROPSIZE 2000000 // ~2MB
struct firstLine
{
	char *method;
	char *path;
	char *httpVersion;
};
typedef struct firstLine firstLine;
void createFirstLine(char *buff, firstLine *firstLine)
{
	/*
	 * NOTE: Caller must call freeFirstLine after the job is done. 
	 * not calling it would result in memory leaks!
	*/
	char *firstLineProps[3];
	for (int i = 0; i < 3; i++)
	{
		firstLineProps[i] = malloc(MAXPROPSIZE);
	}
	int currProp = 0;
	int currPosOfProp = 0;
	for (int i = 0; i < strlen(buff); i++)
	{
		if (buff[i] == '\r' || buff[i] == '\n')
		{
			break;
		}
		if (buff[i] == ' ')
		{
			firstLineProps[currProp][currPosOfProp] = '\0';
			currProp++;
			currPosOfProp = 0;
			continue;
		}
		if(currPosOfProp + 1 >= MAXPROPSIZE){
			write(2, "MAXPROPSIZE exceed\n", sizeof("MAXPROPSIZE exceed\n"));
			return;
		}
		firstLineProps[currProp][currPosOfProp] = buff[i];
		currPosOfProp++;
	}
	firstLine->method = firstLineProps[0];
	firstLine->path = firstLineProps[1];
	firstLine->httpVersion = firstLineProps[2];
}
void freeFirstLine(firstLine *firstLine){
	free(firstLine->httpVersion);
	free(firstLine->method);
	free(firstLine->path);
}