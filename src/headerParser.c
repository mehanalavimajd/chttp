#include <stdio.h>
#include <stdlib.h>
#define DEFAULT_HEADER_COUNT 30
#define MAX_HEADER_COUNT 1000
struct header
{
	char *headerName;
	char *headerValue;
};
typedef struct header Header;
void headerParser(char *buff)
{
	int headerCount = DEFAULT_HEADER_COUNT;
	Header *headers = malloc(headerCount * sizeof(Header));
	int i = 0;
	while (buff[i++] != '\n')
		; // to pass first line
	int currHeaderPos = 0;
	while (1)
	{
		if (currHeaderPos >= headerCount)
		{
			// printf("mama imma realloc!"); -- still works better that GDB
			headers = realloc(headers, sizeof(Header) * (headerCount + DEFAULT_HEADER_COUNT));
			headerCount += DEFAULT_HEADER_COUNT;
		}

		Header currHeader;
		currHeader.headerName = malloc(1000);
		currHeader.headerValue = malloc(1000);
		int pos = 0;
		while (buff[i] != ':') // ++i passes to first char
		{
			currHeader.headerName[pos++] = buff[i++];
		}
		i += 2; // pass space and :
		pos = 0;
		while (buff[i] != '\r') // ++i passes to first char
		{
			currHeader.headerValue[pos++] = buff[i++];
		}
		printf("%s,%s\n", currHeader.headerName, currHeader.headerValue);
		headers[currHeaderPos++] = currHeader;
		i += 2;
		if (buff[i] == '\r')
		{
			printf("%d", currHeaderPos);
			break;
		}
	}
	for (int i = 0; i < currHeaderPos; i++)
	{
		free(headers[i].headerName);
		free(headers[i].headerValue);
	}

	free(headers);
}
int main()
{
	headerParser(
	    "GET /r HTTP1.1\r\n"
	    "hello: world\r\n"
	    "life: good\r\n"
	    "life: good\r\n"
	    "life: good\r\n"
	    "life: good\r\n"
	    "\r\n");
}
