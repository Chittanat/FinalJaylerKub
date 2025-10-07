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
char line[200];
char keyword[50];
int found = 0;
char searchID[50];
char header[200];
char delID[50];
int deleted = 0;
char searchName[50];
int matchCount = 0;
char updateName[50];
char matchName[50][50];
char matchID[50][50];
char matchDate[50][30];
char matchDetails[50][50];
int count = 0;
int foundMatch = 0;



int createCSV() //สร้างไฟล์
{
    FILE *file = fopen("AssetData.csv", "r");
if (file == NULL) {
    file = fopen("khomul_lukka.csv", "w");
    fprintf(file, "AssetName, AssetID, MaintenanceDate(DD-MM-YYYY), MaintenanceDetails\n");
}
fclose(file);
   
    return 0;
}

int addData() {  //เพิ่มข้อมูล
    FILE *file = fopen("AssetData.csv", "a"); 
    
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
    

    printf("Please enter maintenance details: ");
    getchar(); // clear newline
    fgets(details, sizeof(details), stdin);
    details[strcspn(details, "\n")] = 0; // remove newline
    
    fprintf(file, "%s, %s, %s, %s\n", name, id, date, details); // เขียนลงไฟล์ CSV
    fclose(file);
 
    printf("Data added successfully!\n");

    return 0;
}


void searchData() {
    FILE *file = fopen("AssetData.csv", "r");

    if (file == NULL) {
        printf("Cannot open file.\n");
        return;
    }

    printf("Enter asset name or ID to search: ");
    scanf("%s", keyword);

    fgets(line, sizeof(line), file); // ข้าม header

     while (fgets(line, sizeof(line), file)) {

        char *name = strtok(line, ",");       
        char *id = strtok(NULL, ",");
        char *date = strtok(NULL, ",");
        char *details = strtok(NULL, "\n");     

        if (id && name && date && details) {

            while (*name == ' ') name++;
            while (*id == ' ') id++;

            //  เพิ่มการตรวจสอบว่ามี (Deleted) หรือไม่
            if (!strstr(details, "(Deleted)")) {
            // ตรวจสอบ keyword ว่าตรงกับ id หรือ name
            if (strstr(name, keyword) || strstr(id, keyword)) {
                printf("Found: %s, %s, %s, %s\n", name, id, date, details);
              found = 1;
            }
        }
     }
 }

    if (!found) {
        printf("No result found.\n");
    }

    fclose(file);
}

void trim(char *str) {
    char *p = str;
    while (*p == ' ') p++;
    memmove(str, p, strlen(p) + 1);
    for (int i = strlen(str) - 1; i >= 0 && str[i] == ' '; i--)
        str[i] = '\0';
}

int updateData() {
    FILE *file, *temp;
   
    file = fopen("AssetData.csv", "r");
    temp = fopen("temp.csv", "w");

    if (file == NULL || temp == NULL) {
        printf("Unable to open file\n");
        return 1;
    }

    if (fgets(header, sizeof(header), file)) // Copy header to temp file
      fputs(header, temp);
    

    printf("Enter asset name to search: ");
    getchar(); // clear leftover newline
fgets(searchName, sizeof(searchName), stdin);
searchName[strcspn(searchName, "\r\n")] = 0; // remove newline
    trim(searchName);

    count = 0;

    while (fscanf(file, "%49[^,],%49[^,],%29[^,],%49[^\n]\n", name, id, date, details) != EOF) {
        
        trim(name);
        trim(id);
        trim(date);
        trim(details);

        if (strcmp(name, searchName) == 0 && !strstr(details, "(Deleted)")) {
            strcpy(matchName[count], name);
            strcpy(matchID[count], id);
            strcpy(matchDate[count], date);
            strcpy(matchDetails[count], details);
            count++;
        }
    }

    if (count == 0) {
        printf("No matching asset found.\n");
        fclose(file);
        fclose(temp);
        remove("temp.csv");
        return 0;
    }

    if (count > 1) {
        printf("Multiple assets found for '%s':\n", searchName);
        for (int i = 0; i < count; i++) {
            printf("%d) %s, %s, %s, %s\n", i+1, matchName[i], matchID[i], matchDate[i], matchDetails[i]);
        }
        printf("Enter Asset ID to update: ");
        scanf(" %[^\n]", searchID);
        getchar();
        trim(searchID);
        
    } else {
        strcpy(searchID, matchID[0]);
    }

    rewind(file);
    fgets(header, sizeof(header), file); // skip header

    found = 0;
    while (fscanf(file, "%49[^,],%49[^,],%29[^,],%49[^\n]\n",
                  name, id, date, details) != EOF) {

         trim(name);
        trim(id);
        trim(date);
        trim(details);
                 
        if (strcmp(id, searchID) == 0 && !strstr(details, "(Deleted)")) {
            found = 1;
            printf("\nData found:\nName: %s\nID: %s\nDate: %s\nDetails: %s\n", name, id, date, details);
            printf("\nWhich field do you want to update?\n");
            printf("1. Name\n2. ID\n3. Date\n4. Details\nChoice: ");
            scanf("%d", &choice);
            getchar();

            switch (choice) {
                case 1:
                    printf("Enter new name: ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\r\n")] = 0;
                    trim(name);
                    break;
                case 2:
                    printf("Enter new ID: ");
                    fgets(id, sizeof(id), stdin);
                    id[strcspn(id, "\r\n")] = 0;
                    trim(id);
                    break;
                case 3:
                    do {
                        printf("Enter new day (1-31): ");
                        scanf("%d", &day);
                    } while (day < 1 || day > 31);
                    do {
                        printf("Enter new month (1-12): ");
                        scanf("%d", &month);
                    } while (month < 1 || month > 12);
                    sprintf(date, "%d-%d-%d", day, month, year);
                    break;
                case 4:
                    printf("Enter new details: ");
                    fgets(details, sizeof(details), stdin);
                    details[strcspn(details, "\r\n")] = 0;
                    trim(details);
                    break;
                default:
                    printf("Invalid choice\n");
            }
        }
        fprintf(temp, "%s, %s, %s, %s\n", name, id, date, details);
    }


    fclose(file);
    fclose(temp);

    remove("AssetData.csv");
    rename("temp.csv", "AssetData.csv");

    if (found)
        printf("Update complete\n");
    else
        printf("ID not found\n");

    return 0;
}


int deleteData() {
    FILE *file, *temp;

    printf("Enter asset name to delete: ");
    scanf("%s", searchName);

    file = fopen("AssetData.csv", "r");
    temp = fopen("temp.csv", "w");

    if (file == NULL || temp == NULL) {
        printf("Unable to open file\n");
        return 1;
    }

    // คัดลอก header ไปไฟล์ชั่วคราว
    if (fgets(header, sizeof(header), file)) {
        fputs(header, temp);
    }

    // อ่านแต่ละบรรทัดและตรวจสอบ
    while (fscanf(file, "%49[^,],%49[^,],%29[^,],%49[^\n]\n",
                  name, id, date, details) != EOF) {
        
        trim(name);
        trim(id);
        trim(date);
        trim(details);


        if (strstr(name, searchName) && !strstr(details, "(Deleted)")) {
            printf("%d) %s, %s, %s, %s\n", matchCount + 1, name, id, date, details);
            strcpy(matchName[matchCount], name);
            strcpy(matchID[matchCount], id);
            strcpy(matchDate[matchCount], date);
            strcpy(matchDetails[matchCount], details);
            matchCount++;
            foundMatch = 1;
        }
    }
    if (!foundMatch) {
        printf("No matching asset found.\n");
        fclose(file);
        fclose(temp);
        remove("temp.csv");
        return 0;
    }

    fclose(file);

    if (matchCount > 1) {
        printf("Multiple assets found with same name.\nEnter Asset ID to delete: ");
        scanf("%s", delID);
    } else {
        strcpy(delID, matchID[0]);
    }

    file = fopen("AssetData.csv", "r");
    fgets(header, sizeof(header), file); // ข้าม header

    while (fscanf(file, "%49[^,],%49[^,],%29[^,],%49[^\n]\n",
                  name, id, date, details) != EOF) {

        trim(name);
        trim(id);
        trim(date);
        trim(details);

        if (strcmp(id, delID) == 0 && !strstr(details, "(Deleted)")) {
            deleted = 1;
            printf("Deleting: %s, %s, %s, %s\n", name, id, date, details);
            fprintf(temp, "%s , %s, %s, %s (Deleted)\n", name, id, date, details);
        } else {
            fprintf(temp, "%s, %s, %s, %s\n", name, id, date, details);
        }
    }


    fclose(file);
    fclose(temp);

    remove("AssetData.csv");
    rename("temp.csv", "AssetData.csv");

    if (deleted)
        printf("Delete complete\n");
    else
        printf("ID not found\n");

    return 0;
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
        printf("0. Exit the Program\n");
        printf("Select Menu: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addData(); 
                    break;
            case 2: searchData(); 
                    break;
            case 3: updateData();
                    break;
            case 4: deleteData(); 
                    break;
            case 0: printf("Operation Completed\n"); 
                    break;
            default: printf("Invalid Menu\n");
        }
    } while (choice != 0);
   
    return 0;
}

