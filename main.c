#include <stdio.h>
#include <string.h>

int choice;
char name[50];
char id[50];
int day;
int month;
int year = 2025;
char date[30];
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
   do {
        printf("Please enter maintenance day (1-31): ");
        scanf("%d", &day);
        if(day < 1 || day > 31){
            printf("============================================================================\n");
            printf("Invalid day. Please enter maintenance day again.\n");
             printf("============================================================================\n");
        }
    } while (day < 1 || day > 31);
       
    do {
        printf("Please enter maintenance month (1-12): ");
        scanf("%d", &month);
        if(month < 1 || month > 12){
            printf("============================================================================\n");
            printf("Invalid month. Please enter maintenance month again.\n");
            printf("============================================================================\n");
        }
    } while (month < 1 || month > 12);
        
    sprintf(date, "%d-%d-%d", day , month , year); // แปลง int เป็น string
    
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

    fprintf(file, "AssetName, AssetID, MaintenanceDate(DD-MM-YYYY), MaintenanceDetails\n");
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
        char *name = strtok(line, ",");       
        char *id = strtok(NULL, ",");     

        if (id && name) {
            // ตรวจสอบ keyword ว่าตรงกับ id หรือ name
            if (strstr(name, keyword) || strstr(id, keyword)) {
                printf("Found: %s, %s, %s-%s-%s, %s\n", name, id, day, month, year, details);
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
        printf("3. Update Data\n");
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
            case 3: printf("Update Data"); 
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

