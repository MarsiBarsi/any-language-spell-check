#include <stdio.h>

int menu() {
    int x;
    printf("● Select an action:\n1.Try again\n0.Exit\n");
    for(;;) {
        scanf("%d",&x);
        if (x == 1) { return 1; }
        else if (x == 0) { return 0; }
        else {
            printf("invalid input, try again:\n");
        }
    }
}

int main()
{
    int status = 1;

    char filename[256];
    FILE *f;

    while (status != 0) {
        printf("Enter file name: ");

        scanf("%s",filename);

        if (f = fopen(filename,"r")){
            // in this block you can write your program that works with files

            char inFile[256];

            fscanf(f, "%s", inFile);
            printf("the file %s  has been successfully opened and contains: %s",filename,inFile);
            return 1;


        }
        else { printf("File could not be found.\nCheck the spelling of the file name and verify that the file location is correct.\n");
            status = menu();
        }
    }

    return 0;
}