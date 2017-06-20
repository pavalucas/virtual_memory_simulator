#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 1000

typedef struct 
{
	char R;
	char M;
	int lastAcessed;
} PageFrame;

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

int getPageFrameSize(int pageSize, int physMemSize)
{
	return ((physMemSize * 1024) / pageSize);
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
	int* pageTable;
	PageFrame* pageFrame;
	unsigned long long time = 0;

	printf("Executando o simulador...\n");
	printf("Arquivo de entrada: %s\n", addressFile);
	printf("Tamanho da memoria fisica: %d MB\n", physicalMemorySize);
	printf("Tamanho das paginas: %d KB\n", pageSize);
	printf("Alg de substituicao: %s\n", replacementAlg);
	
	int pageTableSize = getPageTableSize(pageSize);
	int pageFrameSize = getPageFrameSize(pageSize, physicalMemorySize);
	printf("Page table size: %d\n", pageTableSize);
	printf("Page frame size: %d\n", pageFrameSize);
	pageTable = (int*)malloc(sizeof(int) * pageTableSize);
	pageFrame = (PageFrame*)malloc(sizeof(PageFrame) * pageFrameSize);

	int pagesEmpty = pageFrameSize;


	FILE* fp = fopen(addressFile, "r");
	if(fp == NULL)
	{
		printf("File not found!");
		exit(2);
	}

	int qntAddr = 0;
	

	while(fscanf(fp, "%x %c", &addr, &rw) == 2)
	{
		int newPageIndex = getPageIndex(addr, pageSize);
		int indexNewPageFrame = pageTable[newPageIndex];
		int oldPageIndex;
		

		PageFrame newPage;

		if(pagesEmpty != 0)
		{
			if(indexNewPageFrame == -1)
			{
				newPage.lastAcessed = time;
		
				if(rw == 'W')
				{
					newPage.R = 0;
					newPage.M = 1;
				}
				else
				{
					newPage.R = 1;
					newPage.M = 0;
				}

				pagesEmpty--;
				oldPageIndex = pageFrameSize - pagesEmpty;
				pageFrame[oldPageIndex] = newPage;
				pageTable[newPageIndex] = oldPageIndex;
			}
			else
			{
				if(rw == 'W')
				{
					pageFrame[indexNewPageFrame].M = 1;
				}
				else
				{
					pageFrame[indexNewPageFrame].R = 1;
				}
				pageFrame[indexNewPageFrame].lastAcessed = time;

			}

		}

		else
		{
			if(strcmp(replacementAlg, "NRU")==0)
			{}
			else if(strcmp(replacementAlg, "NRU")==0)
			{}
			else if(strcmp(replacementAlg, "NOVO") == 0)
			{}
		}

		time++;
		qntAddr++;		
	}

	printf("Number of adresses: %d\n", qntAddr);

}