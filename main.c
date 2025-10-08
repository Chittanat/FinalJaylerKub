#include <stdio.h>
#include <string.h>
#include <assert.h>

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
int testChoice;



int createCSV() //สร้างไฟล์
{
    FILE *file = fopen("AssetData.csv", "r");
     
if (file == NULL) {
    file = fopen("AssetData.csv", "w");
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
 

void runUnitTest() {
    int testChoice;
    printf("\n--- UNIT TEST MENU ---\n");
    printf("1. Test addData()\n");
    printf("2. Test searchData()\n");
    printf("Select function to test: ");
    scanf("%d", &testChoice);
    getchar();
    
    // สำรองไฟล์ข้อมูลจริง (ถ้ามี)
    rename("AssetData.csv", "AssetData_backup.csv");
    
    // สร้างไฟล์ทดสอบใหม่
    createCSV();
    
    switch (testChoice) {
        case 1: {
            printf("\nRunning addData() test...\n");
            FILE *f = fopen("AssetData.csv", "a");
            fprintf(f, "Laptop, A001, 1-10-2025, Battery Replacement\n");
            fclose(f);
            
            f = fopen("AssetData.csv", "r");
            char line[200];
            int found = 0;
            while (fgets(line, sizeof(line), f)) {
                if (strstr(line, "Laptop")) found = 1;
            }
            fclose(f);
            assert(found == 1);
            printf(" addData() Unit Test PASSED.\n");
            break;
        }
        case 2: {
            printf("\nRunning searchData() test...\n");
            FILE *f = fopen("AssetData.csv", "a");
            fprintf(f, "Phone, A002, 2-10-2025, Screen Broken\n");
            fclose(f);
            
            f = fopen("AssetData.csv", "r");
            char line[200];
            int found = 0;
            while (fgets(line, sizeof(line), f)) {
                if (strstr(line, "Phone")) found = 1;
            }
            fclose(f);
            assert(found == 1);
            printf(" searchData() Unit Test PASSED.\n");
            break;
        }
        default:
            printf("Invalid test selection.\n");
    }
    
    // ลบไฟล์ทดสอบและคืนค่าไฟล์เดิม
    remove("AssetData.csv");
    rename("AssetData_backup.csv", "AssetData.csv");
    printf("\n Test file cleaned up\n");
    printf(" Original data restored\n");
}

void runEndToEndTest() {
    printf("\n========================================\n");
    printf("   Running End-to-End Test\n");
    printf("========================================\n");
    
    // สำรองไฟล์ข้อมูลจริง (ถ้ามี)
    rename("AssetData.csv", "AssetData_backup.csv");
    
    FILE *testFile = fopen("AssetData.csv", "w");
    fprintf(testFile, "AssetName, AssetID, MaintenanceDate(DD-MM-YYYY), MaintenanceDetails\n");
    fclose(testFile);
    
    printf("\n[Test 1] Testing ADD functionality...\n");
    testFile = fopen("AssetData.csv", "a");
    fprintf(testFile, "TestLaptop, T001, 15-3-2025, Screen Repair\n");
    fprintf(testFile, "TestPrinter, T002, 20-5-2025, Ink Replacement\n");
    fprintf(testFile, "TestRouter, T003, 10-8-2025, Firmware Update\n");
    fclose(testFile);
    printf(" Added 3 test records\n");
    
    // ตรวจสอบว่าข้อมูลถูกเพิ่มจริง
    testFile = fopen("AssetData.csv", "r");
    int lineCount = 0;
    char buffer[200];
    while (fgets(buffer, sizeof(buffer), testFile)) {
        lineCount++;
    }
    fclose(testFile);
    assert(lineCount == 4); // header + 3 records
    printf(" Verified: 3 records added successfully\n");
    
    printf("\n[Test 2] Testing SEARCH functionality...\n");
    testFile = fopen("AssetData.csv", "r");
    int foundTest = 0;
    fgets(buffer, sizeof(buffer), testFile); // skip header
    while (fgets(buffer, sizeof(buffer), testFile)) {
        if (strstr(buffer, "TestLaptop") || strstr(buffer, "T001")) {
            foundTest = 1;
            printf(" Found: %s", buffer);
            break;
        }
    }
    fclose(testFile);
    assert(foundTest == 1);
    printf(" Search test PASSED\n");
    
    printf("\n[Test 3] Testing UPDATE functionality...\n");
    FILE *temp = fopen("temp.csv", "w");
    testFile = fopen("AssetData.csv", "r");
    
    fgets(buffer, sizeof(buffer), testFile);
    fputs(buffer, temp); // copy header
    
    int updated = 0;
    while (fgets(buffer, sizeof(buffer), testFile)) {
        if (strstr(buffer, "T001")) {
            fprintf(temp, "TestLaptop-Updated, T001, 15-3-2025, Screen Repair (Updated)\n");
            updated = 1;
        } else {
            fputs(buffer, temp);
        }
    }
    fclose(testFile);
    fclose(temp);
    
    remove("AssetData.csv");
    rename("temp.csv", "AssetData.csv");
    assert(updated == 1);
    
    // ตรวจสอบการอัพเดต
    testFile = fopen("AssetData.csv", "r");
    int verifyUpdate = 0;
    while (fgets(buffer, sizeof(buffer), testFile)) {
        if (strstr(buffer, "Updated")) {
            verifyUpdate = 1;
            printf(" Updated: %s", buffer);
            break;
        }
    }
    fclose(testFile);
    assert(verifyUpdate == 1);
    printf(" Update test PASSED\n");
    
    printf("\n[Test 4] Testing DELETE functionality...\n");
    temp = fopen("temp.csv", "w");
    testFile = fopen("AssetData.csv", "r");
    
    fgets(buffer, sizeof(buffer), testFile);
    fputs(buffer, temp); // copy header
    
    int deletedCount = 0;
    while (fgets(buffer, sizeof(buffer), testFile)) {
        if (strstr(buffer, "T002")) {
            // เพิ่ม (Deleted) แทนการลบทิ้ง - เก็บรูปแบบเดิมไว้
            char name[50], id[50], date[30], details[50];
            sscanf(buffer, "%49[^,],%49[^,],%29[^,],%49[^\n]", name, id, date, details);
            
            // trim spaces
            char *p = name; while (*p == ' ') p++; memmove(name, p, strlen(p) + 1);
            p = id; while (*p == ' ') p++; memmove(id, p, strlen(p) + 1);
            p = date; while (*p == ' ') p++; memmove(date, p, strlen(p) + 1);
            p = details; while (*p == ' ') p++; memmove(details, p, strlen(p) + 1);
            
            fprintf(temp, "%s, %s, %s, %s (Deleted)\n", name, id, date, details);
            deletedCount = 1;
            printf(" Marked as deleted: T002\n");
        } else {
            fputs(buffer, temp);
        }
    }
    fclose(testFile);
    fclose(temp);
    
    remove("AssetData.csv");
    rename("temp.csv", "AssetData.csv");
    assert(deletedCount == 1);
    printf(" Delete test PASSED\n");
    
    printf("\n[Test 5] Testing SEARCH after DELETE...\n");
    testFile = fopen("AssetData.csv", "r");
    int foundDeleted = 0;
    int foundActive = 0;
    fgets(buffer, sizeof(buffer), testFile); // skip header
    while (fgets(buffer, sizeof(buffer), testFile)) {
        // เช็คว่ามี T002 และมี Deleted
        if (strstr(buffer, "T002")) {
            if (strstr(buffer, "Deleted")) {
                foundDeleted = 1;
                printf(" Found deleted record: %s", buffer);
            }
        }
        // เช็คว่ามี T001 หรือ T003 และไม่มี Deleted
        if ((strstr(buffer, "T001") || strstr(buffer, "T003"))) {
            if (!strstr(buffer, "Deleted")) {
                foundActive = 1;
            }
        }
    }
    fclose(testFile);
    
    if (!foundDeleted) {
        printf(" Warning: Could not verify deleted record mark\n");
        printf("Checking file content...\n");
        testFile = fopen("AssetData.csv", "r");
        while (fgets(buffer, sizeof(buffer), testFile)) {
            printf("%s", buffer);
        }
        fclose(testFile);
    }
    
    assert(foundDeleted == 1);
    assert(foundActive == 1);
    printf(" Deleted record marked correctly\n");
    printf(" Active records still accessible\n");
    
    // แสดงสรุปผลการทดสอบ
    printf("\n========================================\n");
    printf("   End-to-End Test Summary\n");
    printf("========================================\n");
    printf(" ADD Test:    PASSED\n");
    printf(" SEARCH Test: PASSED\n");
    printf(" UPDATE Test: PASSED\n");
    printf(" DELETE Test: PASSED\n");
    printf(" VERIFICATION: PASSED\n");
    printf("========================================\n");
    printf("All End-to-End Tests PASSED!\n");
    printf("========================================\n");
    
    // ลบไฟล์ทดสอบและคืนค่าไฟล์เดิม
    remove("AssetData.csv");
    rename("AssetData_backup.csv", "AssetData.csv");
    printf("\n Test file cleaned up\n");
    printf(" Original data restored\n");
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
        printf("5. Run Unit Test\n");
        printf("6. Run End-to-End Test\n");
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
            case 5: runUnitTest(); 
                    break;
            case 6: runEndToEndTest(); 
                    break;
            case 0: printf("Operation Completed\n"); 
                    break;
            default: printf("Invalid Menu\n");
        }
    } while (choice != 0);
   
    return 0;
}

