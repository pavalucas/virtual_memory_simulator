#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        printf("Error! 5 parameters are necessary!\n");
        exit(1);
    }

    char* replacementAlg = argv[1];
    char* adressFile = argv[2];
    int pageSize = atoi(argv[3]);
    int physicalMemorySize = atoi(argv[4]);

}
