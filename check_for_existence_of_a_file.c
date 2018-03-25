#include <stdio.h>


FILE *check_for_existence_of_a_file() {
    FILE *TEST_FILE;
    int status = 1;
    char filename[256];

    while (status != 0) {
        printf("Enter file name: ");

        scanf("%s",filename);

        if (TEST_FILE = fopen(filename,"r")){
            return TEST_FILE;
        }
        else { printf("File could not be found.\nCheck the spelling of the file name and verify that the file location is correct.\n");
            status = menu_for_file_existence();
        }
    }
    return NULL;
}

int menu_for_file_existence() {
    int local_status;
    printf("● Select an action:\n1.Try again\n0.Exit\n");
    while(1) {
        scanf("%d",&local_status);
        if (local_status == 1) { return 1; }
        else if (local_status == 0) { return 0; }
        else {
            printf("invalid input, try again:\n");
        }
    }
}


int main()
{
    FILE *FILE;

    FILE = check_for_existence_of_a_file();

    if (FILE) {

        char inFile[256];

        fgets(inFile,256,FILE);
        printf("the file has been successfully opened and contains: %s",inFile);
        return 1;
    }

    return 0;
}
