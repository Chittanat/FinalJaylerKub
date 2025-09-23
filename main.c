#include <stdio.h>
#include <string.h>

int choice;
char name[50];
char id[50];
char date[50];
char details[50];




int addData() {  //เพิ่มข้อมูล
    FILE *file = fopen("khomul_lukka.csv", "a"); 
    
    if (file == NULL) {
        printf("Unable to open file\n");
        return 1;
    }

    printf("Please enter asset name: ");
    scanf("%s", name);
    printf("Please enter asset ID: ");
    scanf("%s", id);
    printf("Please enter maintenance date (YYYY-MM-DD):  ");
    scanf("%s", date);
    printf("Please enter maintenance delails: ");
    scanf("%s", details);
    
    fprintf(file, "%s, %s, %s, %s\n", name, id, date, details); // เขียนลงไฟล์ CSV
    fclose(file);


    printf("Data added successfully!\n");

    return 0;
}

int createCSV() //สร้างไฟล์
{
    FILE *file = fopen("khomul_lukka.csv", "w");  
    if (file == NULL) {
        printf("Unable to open file\n");
        return 1;
    }

    fprintf(file, "AssetName, AssetID, MaintenanceDate, MaintenanceDetails\n");
    printf("File opened successfully!\n");
    fclose(file);
    return 0;
}

void searchData() {
    FILE *file = fopen("khomul_lukka.csv", "r");
    char line[200];
    char keyword[50];
    int found = 0;

    if (file == NULL) {
        printf("Cannot open file.\n");
        return;
    }

    printf("Enter asset name or ID to search: ");
    scanf("%s", keyword);

     while (fgets(line, sizeof(line), file)) {
        char *name = strtok(line, ",");       // คอลัมน์แรก
        char *id = strtok(NULL, ",");     // คอลัมน์สอง

        if (id && name) {
            // ตรวจสอบ keyword ว่าตรงกับ id หรือ name
            if (strstr(name, keyword) || strstr(id, keyword)) {
                printf("Found: %s, %s, %s, %s\n", name,id,date,details);
                found = 1;
        }
    }
    }

    if (!found) {
        printf("No result found.\n");
    }

    fclose(file);
}







int main()
{
    createCSV(); 
 do {
        printf("\n=== Asset Maintenance Management System ===\n");
        printf("1. Add New Data\n");
        printf("2. Search Assets\n");
        printf("3. Edit Data\n");
        printf("4. Delete Data\n");
        printf("5. Display All Data\n");
        printf("0. Exit the Program\n");
        printf("Select Menu: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addData(); 
                    break;
            case 2: searchData(); 
                    break;
            case 3: printf("Edit Data"); 
                    break;
            case 4: printf("Delete Data"); 
                    break;
            case 5: printf("Display All Data"); 
                    break;
            case 0: printf("Operation Completed\n"); 
                    break;
            default: printf("Invalid Menu\n");
        }
    } while (choice != 0);
   
    return 0;
}

