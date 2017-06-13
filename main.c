#include <stdio.h>
#include <stdlib.h>
#define MAXN 1000

typedef struct 
{
    unsigned int physicalAddr;
    char R;
    char M;
    int lastAcessed;
} PageTable;

unsigned int getPageIndex(unsigned int addr, int pageSize)
{
    if(pageSize == 8)
        return addr >> 13;
    else if(pageSize == 16)
        return addr >> 14;
    else if(pageSize == 32)
        return addr >> 15;
		return 0;
}

int getPageTableSize(int pageSize)
{
	if(pageSize == 8)
		return 1<<19;
	else if(pageSize == 16)
		return 1<<18;
	else if(pageSize == 32)
		return 1<<17;
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc < 5)
    {
    	printf("Error! 5 parameters are necessary!\n");
    	exit(1);
    }

    char* replacementAlg = argv[1];
    char* addressFile = argv[2];
    int pageSize = atoi(argv[3]);
    int physicalMemorySize = atoi(argv[4]);
	unsigned int addr;
	char rw;
	int i;
	int numberOfAddr;
  PageTable* pageTableEntries;
	int time = 0;

	printf("Executando o simulador...\n");
	printf("Arquivo de entrada: %s\n", addressFile);
	printf("Tamanho da memoria fisica: %d MB\n", physicalMemorySize);
	printf("Tamanho das paginas: %d KB\n", pageSize);
	printf("Alg de substituicao: %s\n", replacementAlg);
	
	int pageTableSize = getPageTableSize(pageSize);
	printf("Page table size: %d\n", pageTableSize);
	pageTableEntries = (PageTable*)malloc(sizeof(PageTable) * pageTableSize);

	FILE* fp = fopen(addressFile, "r");
	if(fp == NULL)
	{
		printf("File not found!");
		exit(2);
	}

	int qntAddr = 0;
	while(fscanf(fp, "%x %c", &addr, &rw) == 2)
	{
		qntAddr++;
	}

	printf("Number of adresses: %d\n", qntAddr);
}
