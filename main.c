#include <stdio.h>

int choice;
void askExit()
{
    int ansexit; //ออกโปรแกรม
    do {
        printf("Do you want to exit the program? (1 = yes , 0 = no): ");
        scanf("%d", &ansexit);
    } while (ansexit != 1);
    
    printf("You have exited the program.\n");
    
}

int createCSV() //สร้างไฟล์
{
    FILE *file = fopen("khomul_lukka.csv", "a");  
    if (file == NULL) {
        printf("Unable to open file.\n");
        return 1;
    }
    printf("File opened successfully!\n");
    fclose(file);
    return 0;
}
int main()
{
    createCSV(); 
 do {
        printf("\n=== Asset Maintenance Management System ===\n");
        printf("1. Display All Data\n");
        printf("2. Add New Data\n");
        printf("3. Search Assets\n");
        printf("4. Edit Data\n");
        printf("5. Delete Data\n");
        printf("0. Exit the Program\n");
        printf("Select Menu: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: printf("case1"); break;
            case 2: printf("case2"); break;
            case 3: printf("case3"); break;
            case 4: printf("case4"); break;
            case 5: printf("case5"); break;
            case 0: printf("Operation Completed\n"); break;
            default: printf("Invalid Menu\n");
        }
    } while (choice != 0);
    return 0;
}

