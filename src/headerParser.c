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
Header *createHeaderParser(char *buff)
{
	/*
	* NOTE: Caller must call freeHeaderParser after they
	* are done with it. 
	*
	* Gets the buffer from client, passes the first line
	* and parse all the `name: value\r\n` format into t-
	* he type Header and returns the pointer to first o-
	* ne.
	*/
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
			headers = realloc(headers, sizeof(Header) * (headerCount + DEFAULT_HEADER_COUNT));
			headerCount += DEFAULT_HEADER_COUNT;
			if(headerCount > MAX_HEADER_COUNT){
				printf("\nMAX_HEADER_COUNT EXCEEDED.\n");
				exit(1);
			}
		}
		headers[currHeaderPos].headerName = malloc(1000);
		headers[currHeaderPos].headerValue = malloc(1000);
		int pos = 0;
		while (buff[i] != ':')
		{
			headers[currHeaderPos].headerName[pos++] = buff[i++];
		}
		headers[currHeaderPos].headerName[pos] = '\0';
		i += 2; // pass space and :
		pos = 0;
		while (buff[i] != '\r')
		{
			headers[currHeaderPos].headerValue[pos++] = buff[i++];
		}
		headers[currHeaderPos].headerValue[pos] = '\0';
		printf("%s,%s\n", headers[currHeaderPos].headerName, headers[currHeaderPos].headerValue);
		currHeaderPos++;
		i += 2; // pass \r\n
		if (buff[i] == '\r')
		{
			headers[currHeaderPos].headerName = NULL; // end
			break;
		}
	}
	return headers;
}
void freeHeaderParser(Header *headers)
{
	for (int i = 0; headers[i].headerName != NULL; i++)
	{
		free(headers[i].headerName);
		free(headers[i].headerValue);
	}
	free(headers);
}
int main()
{
	Header *h;
	h = createHeaderParser(
	    "GET /r HTTP1.1\r\n"
	    "hello: world\r\n"
	    "life: good\r\n"
	    "life: good\r\n"
	    "life: good\r\n"
	    "life: good\r\n"
	    "\r\n");
	freeHeaderParser(h);
}