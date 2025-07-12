struct header
{
	char *headerName;
	char *headerValue;
};
typedef struct header Header;
void headerParser(char *buff)
{
	int i = 0;
	while (buff[i++] != '\n')
		; // to pass first line
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
	while (buff[i] != '\n') // ++i passes to first char
	{
		currHeader.headerValue[pos++] = buff[i++];
	}
	printf("%s,%s\n", currHeader.headerName, currHeader.headerValue);
}
int main(){
	headerParser("GET /r HTTP1.1\r\nhello: world\r\n");
}
