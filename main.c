#include <stdio.h>
#include <stdlib.h>
#define MAXN 1000

typedef struct 
{
    unsigned int physicalAddr;
    char R;
    char M;
    time_t lastAcessed;
} PageTable;

int getPageIndex(unsigned int addr, int pageSize)
{
    if(pageSize == 8)
        return addr >> 13;
    else if(pageSize == 16)
        return addr >> 14;
    else if(pageSize == 32)
        return addr >> 15;
}

int readLogFile(char* fileName, unsigned int addr[], char rw[])
{
	FILE* fp = fopen(fileName, "r");
	int i = 0;
	while(fscanf(fp, "%x %c", &addr[i], &rw[i]) == 2)
		i++;
	return i;
}

int main(int argc, char *argv[])
{
    printf("Entrou\n");
	if (argc < 5)
    {
    	printf("Error! 5 parameters are necessary!\n");
    	exit(1);
    }

    char* replacementAlg = argv[1];
    char* adressFile = argv[2];
    int pageSize = atoi(argv[3]);
    int physicalMemorySize = atoi(argv[4]);
	unsigned int addr[MAXN];
	char rw[MAXN];
	int i;
	int numberOfAddr;
    PageTable pageTableEntries[520000];

	for(i = 0; i < argc; i++)
		printf("%s ", argv[i]);
	printf("\n");

	numberOfAddr = readLogFile(adressFile, addr, rw);

	printf("Number of adresses: %d\n", numberOfAddr);
	for(i = 0; i < numberOfAddr; i++)
	{
		printf("%x %c\n", addr[i], rw[i]);
	}
}
