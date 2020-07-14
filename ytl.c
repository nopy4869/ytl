#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char args[] = "./youtube-dl --write-description --write-info-json --write-thumbnail --all-subs --embed-subs --embed-thumbnail --add-metadata ";
unsigned long int arglen = sizeof(args);

unsigned long int argmax;
char* argbuf;

FILE* ytl;
unsigned long int ytllen;
char* ytlbuf;

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		printf("Invalid argument.\nPlease pass path providing properly parsable previous preservations.\nUsage: ytl *.ytl\n\n>.ytl\n>literally a youtube URL in a text document named with .ytl\n");
		return 1;
	}
	if(strncmp(argv[1], "-u", 2) == 0)
	{
		system("wget -c https://youtube-dl.org/downloads/latest/youtube-dl");
		return system("chmod +x youtube-dl");
	}
	if(system("stat youtube-dl") != 0)
	{
		printf("youtube-dl not installed to current directory. Please re-run with -u to download\n");
		return 1;
	}
	printf("\n\nFound youtube-dl!\n\n");
	
	argmax = sysconf(_SC_ARG_MAX);
	printf("system ARG_MAX = %ld\n", argmax);
	argbuf = malloc(argmax);
	if(argbuf == NULL)
	{
		printf("Argbuf allocation failed!\n");
		return 1;
	}
	
	ytl = fopen(argv[1], "r");
	if(ytl == NULL)
	{
		printf("File open failed!");
		free(argbuf);
		return 1;
	}
	
	fseek(ytl, 0, SEEK_END);											//novel idea to seek to the end of the line instead of file
	unsigned long int upper = ftell(ytl);
	fseek(ytl, 0, SEEK_SET);
	char a;
	for(unsigned long int x = 0; x < upper; x++)
	{
		fread(&a, 1, 1, ytl);
		if((a == 10) || (a == 13))
			break;
	}
	ytllen = ftell(ytl);
	fseek(ytl, 0, SEEK_SET);
	
	ytlbuf = malloc(ytllen);
	if(ytlbuf == NULL)
	{
		printf("Buffer allocation failed!\nUnable to open file!\n");
		free(argbuf);
		fclose(ytl);
		return 1;
	}
	if(fread(ytlbuf, 1, ytllen, ytl) != ytllen)
	{
		printf("failed to read full file! not a fatal error tho ;)\n");
	}
	fclose(ytl);
	if(argmax < arglen + ytllen)
	{
		printf("ARG_MAX is too small to fit our argument!\n");
		free(argbuf);
		free(ytlbuf);
		return 1;
	}
	
	printf("arg: %s\nytl: %s\narglen: %ld\nytllen: %ld\n", args, ytlbuf, arglen, ytllen);
	strncpy(argbuf, args, arglen);
	strncpy(argbuf + arglen - 1, ytlbuf, ytllen);
	printf("%s\n", argbuf);
	
	return system(argbuf);
}
