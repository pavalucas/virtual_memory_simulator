#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
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

void NRU(int* pageTable, PageFrame* pageFrame, int pageFrameSize, int pageSize, int deltaTime, FILE* fp)
{
	unsigned int addr;
	char rw;
	int i;
	int qtPageFault = 0;
	int qtPhysMemAcess = 0;
	int pagesEmpty = pageFrameSize;
	unsigned long long time = 1;

	while(fscanf(fp, "%x %c", &addr, &rw) == 2)
	{
		int newPageIndex = getPageIndex(addr, pageSize);
		int indexNewPageFrame = pageTable[newPageIndex];
		int oldPageIndex;

		PageFrame newPage;

		// Setting all R bit to zero
		if(time % deltaTime == 0)
		{
			for(i = 0; i < pageFrameSize; i++)
			{
				pageFrame[i].R = 0;
			}
		}

		if(indexNewPageFrame == -1)
		{
			if(pagesEmpty != 0)
			{
				newPage.lastAcessed = time;

				if(rw == 'W')
					newPage.M = 1;
				else
					newPage.M = 0;
				newPage.R = 1;

				oldPageIndex = pageFrameSize - pagesEmpty;
				pagesEmpty--;
				pageFrame[oldPageIndex] = newPage;
				pageTable[newPageIndex] = oldPageIndex;
				#ifdef _DEBUG
					printf("Nova page frame %d preenchida\n", oldPageIndex);
				#endif
			}
			else
			{
				PageFrame choosed = pageFrame[0];
				int choosedIndex = 0;

				for(i = 1; i < pageFrameSize; i++)
				{
					if(choosed.R == 0)
					{
						if(choosed.M == 0)
						{
							break;
						}
						else if(pageFrame[i].M == 0)
						{
							choosed = pageFrame[i];
							choosedIndex = i;
						}
					}
					else
					{
						if(pageFrame[i].R == 0)
						{
							choosed = pageFrame[i];
							choosedIndex = i;
						}
						else if(pageFrame[i].R == 0) 
						{
							choosed = pageFrame[i];
							choosedIndex = i;														
						}
					}
				}

				#ifdef _DEBUG
					printf("Page fault!\n");
					printf("Pagina %d sera retirada.\n", choosedIndex);
				#endif

				if(choosed.M == 1)
				{
					#ifdef _DEBUG
						printf("Acesso a memoria fisica!\n");
					#endif
					qtPhysMemAcess++;
				}

				if(rw == 'W')
					pageFrame[choosedIndex].M = 1;
				else
					pageFrame[choosedIndex].M = 0;
				pageFrame[choosedIndex].R = 1;
				pageFrame[choosedIndex].lastAcessed = time;

				pageTable[newPageIndex] = choosedIndex;

				qtPageFault++;
			}
		}
		else
		{
			#ifdef _DEBUG
				printf("Page %d hit!\n", indexNewPageFrame);
			#endif

			if(rw == 'W')
				pageFrame[indexNewPageFrame].M = 1;
			pageFrame[indexNewPageFrame].R = 1;

			pageFrame[indexNewPageFrame].lastAcessed = time;
		}

		time++;
	}
	printf("Numero de Faltas de Páginas: %d\n", qtPageFault);
	printf("Numero de Paginas escritas: %d\n", qtPhysMemAcess);
	
}

void LRU(int* pageTable, PageFrame* pageFrame, int pageFrameSize, int pageSize, int deltaTime, FILE* fp)
{
	unsigned int addr;
	char rw;
	int i;
	int qtPageFault = 0;
	int qtPhysMemAcess = 0;
	int pagesEmpty = pageFrameSize;
	unsigned long long time = 1;
	unsigned int* ageArr = (unsigned int*)malloc(sizeof(unsigned int) * pageFrameSize);
	memset(ageArr, 0, sizeof(ageArr));

	while(fscanf(fp, "%x %c", &addr, &rw) == 2)
	{
		int newPageIndex = getPageIndex(addr, pageSize);
		int indexNewPageFrame = pageTable[newPageIndex];
		int oldPageIndex;

		PageFrame newPage;

		// Setting all R bit to zero
		if(time % deltaTime == 0)
		{
			#ifdef _DEBUG
				printf("Atualizando os valor do vetor de envelhecimento.\n");
			#endif
			for(i = 0; i < pageFrameSize; i++)
			{
				ageArr[i] >>= 1;
				if(pageFrame[i].R == 1)
				{
					ageArr[i] |= (1 << 31);
					pageFrame[i].R = 0;
				}
				#ifdef _DEBUG
					//printf("ageArr[%d] -> %u\n", i, ageArr[i]);
				#endif
			}
		}

		if(indexNewPageFrame == -1)
		{
			if(pagesEmpty != 0)
			{
				newPage.lastAcessed = time;

				if(rw == 'W')
					newPage.M = 1;
				else
					newPage.M = 0;
				newPage.R = 1;

				oldPageIndex = pageFrameSize - pagesEmpty;
				pagesEmpty--;
				pageFrame[oldPageIndex] = newPage;
				pageTable[newPageIndex] = oldPageIndex;
				#ifdef _DEBUG
					printf("Nova page frame %d preenchida\n", oldPageIndex);
				#endif
			}
			else
			{
				int minValue = INT_MAX;
				int minPos;
				for(i = 0; i < pageFrameSize; i++)
				{
					if(ageArr[i] < minValue)
					{
						minValue = ageArr[i];
						minPos = i;
					}
				}
				#ifdef _DEBUG
					printf("Page fault!\n");
					printf("Pagina %d sera retirada.\n", minPos);
				#endif
				if(pageFrame[minPos].M == 1)
				{
					#ifdef _DEBUG
						printf("Acesso a memoria fisica!\n");
					#endif
					qtPhysMemAcess++;
				}

				if(rw == 'W')
					pageFrame[minPos].M = 1;
				else
					pageFrame[minPos].M = 0;
				pageFrame[minPos].R = 1;
				pageFrame[minPos].lastAcessed = time;

				pageTable[newPageIndex] = minPos;

				qtPageFault++;
			}
		}
		else
		{
			#ifdef _DEBUG
				printf("Page %d hit!\n", indexNewPageFrame);
			#endif

			if(rw == 'W')
				pageFrame[indexNewPageFrame].M = 1;
			pageFrame[indexNewPageFrame].R = 1;

			pageFrame[indexNewPageFrame].lastAcessed = time;
		}

		time++;
	}
	printf("Numero de Faltas de Páginas: %d\n", qtPageFault);
	printf("Numero de Paginas escritas: %d\n", qtPhysMemAcess);
}

int main(int argc, char *argv[])
{
	if (argc < 5)
	{
		printf("Erro! 5 parametros são necessários!\n");
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
	unsigned long long time = 1;
	unsigned long long deltaTime = 5;

	#ifdef _DEBUG
		printf("Rodando em modo debug.\n");
	#endif

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
		printf("Arquivo não encontrado!");
		exit(2);
	}

	int qntAddr = 0;


	for(i=0; i<pageTableSize; i++)
	{
		pageTable[i] = -1;
	}

	if(strcmp(replacementAlg, "NRU")==0)
	{
		NRU(pageTable, pageFrame, pageFrameSize, pageSize, deltaTime, fp);
	}
	else if(strcmp(replacementAlg, "LRU")==0)
	{
		LRU(pageTable, pageFrame, pageFrameSize, pageSize, deltaTime, fp);
	}
	else if(strcmp(replacementAlg, "NOVO") == 0)
	{}

}