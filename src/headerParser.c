#include <stdio.h>
#include <stdlib.h>
#define DEFAULT_HEADER_COUNT 30
#define MAX_HEADER_COUNT 1000
#define DEFAULT_HEADER_FIELD_SIZE 1000
#define MAX_HEADER_FIELD_SIZE 20e6 // 20MB
void exitLog(char *log)
{
	printf("\n%s\n", log);
	exit(1);
}
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
			if (headerCount > MAX_HEADER_COUNT)
				exitLog("MAX_HEADER_COUNT exceed");
		}
		char **currHeaderName = &headers[currHeaderPos].headerName; // Just for convenience.
		char **currHeaderValue = &headers[currHeaderPos].headerValue;
		long int nameSize = DEFAULT_HEADER_FIELD_SIZE;
		long int valueSize = DEFAULT_HEADER_FIELD_SIZE;
		*currHeaderName = malloc(nameSize);
		*currHeaderValue = malloc(valueSize);
		int pos = 0;
		while (buff[i] != ':')
		{
			if (nameSize >= MAX_HEADER_FIELD_SIZE)
				exitLog("MAX_HEADER_FIELD_SIZE exceed");
			if (pos >= nameSize)
			{
				nameSize *= 2; // double the size of field
				*currHeaderName = realloc(*currHeaderName, nameSize + 1);
			}
			(*currHeaderName)[pos++] = buff[i++];
		}
		(*currHeaderName)[pos] = '\0';
		i += 2; // pass space and :
		pos = 0;
		while (buff[i] != '\r')
		{
			if (valueSize >= MAX_HEADER_FIELD_SIZE)
				exitLog("MAX_HEADER_FIELD_SIZE exceed");
			if (pos >= valueSize)
			{
				valueSize *= 2; // double the size of field
				*currHeaderValue = realloc(*currHeaderValue, valueSize + 1);
			}
			(*currHeaderValue)[pos++] = buff[i++];
		}
		(*currHeaderValue)[pos] = '\0';
		printf("%s,%s\n", *currHeaderName, *currHeaderValue);
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
	    "content-type: multipart/form-data; boundary=--------------------------213444826641001619888010\r\n"
	    "content-type: multipart/form-data; boundary=--------------------------213444826641001619888010\r\n"
	    "content-type: multipart/form-data; boundary=--------------------------213444826641001619888010\r\n"
	    "\r\n");
	freeHeaderParser(h);
}