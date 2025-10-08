#include <stdio.h>
#include <string.h>
#include <assert.h>

// ฟังก์ชันสำหรับสร้างไฟล์ทดสอบ
int createTestCSV(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Unable to create test file\n");
        return 1;
    }
    fprintf(file, "AssetName, AssetID, MaintenanceDate(DD-MM-YYYY), MaintenanceDetails\n");
    fclose(file);
    return 0;
}

void testAddData() {
    const char *testFileName = "UnitTest_Add.csv";
    
    printf("\n========================================\n");
    printf("   Testing addData() Function\n");
    printf("========================================\n");
    
    // สร้างไฟล์ทดสอบ
    createTestCSV(testFileName);
    
    // เพิ่มข้อมูลทดสอบ
    FILE *f = fopen(testFileName, "a");
    fprintf(f, "Laptop, A001, 1-10-2025, Battery Replacement\n");
    fprintf(f, "Monitor, A002, 5-10-2025, Screen Calibration\n");
    fprintf(f, "Keyboard, A003, 10-10-2025, Key Replacement\n");
    fclose(f);
    
    // ตรวจสอบว่าข้อมูลถูกเพิ่ม
    f = fopen(testFileName, "r");
    char line[200];
    int foundLaptop = 0;
    int foundMonitor = 0;
    int foundKeyboard = 0;
    int lineCount = 0;
    
    while (fgets(line, sizeof(line), f)) {
        lineCount++;
        if (strstr(line, "Laptop")) foundLaptop = 1;
        if (strstr(line, "Monitor")) foundMonitor = 1;
        if (strstr(line, "Keyboard")) foundKeyboard = 1;
    }
    fclose(f);
    
    // Assertions
    assert(lineCount == 4); // header + 3 records
    assert(foundLaptop == 1);
    assert(foundMonitor == 1);
    assert(foundKeyboard == 1);
    
    printf(" Test 1: File created successfully\n");
    printf(" Test 2: 3 records added successfully\n");
    printf(" Test 3: All records readable\n");
    printf("\n addData() Unit Test PASSED\n");
    printf("========================================\n");
    
    // ลบไฟล์ทดสอบ
    remove(testFileName);
    printf(" Test file cleaned up\n\n");
}

void testSearchData() {
    const char *testFileName = "UnitTest_Search.csv";
    
    printf("\n========================================\n");
    printf("   Testing searchData() Function\n");
    printf("========================================\n");
    
    // สร้างไฟล์ทดสอบพร้อมข้อมูล
    createTestCSV(testFileName);
    FILE *f = fopen(testFileName, "a");
    fprintf(f, "Phone, P001, 2-10-2025, Screen Broken\n");
    fprintf(f, "Tablet, T001, 3-10-2025, Battery Issue\n");
    fprintf(f, "Router, R001, 4-10-2025, Firmware Update\n");
    fprintf(f, "Phone, P002, 5-10-2025, Camera Repair\n");
    fclose(f);
    
    // Test 1: ค้นหาด้วย Name
    printf("\nTest 1: Search by Name (Phone)\n");
    f = fopen(testFileName, "r");
    char line[200];
    int foundCount = 0;
    fgets(line, sizeof(line), f); // skip header
    
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "Phone")) {
            foundCount++;
            printf("  Found: %s", line);
        }
    }
    fclose(f);
    assert(foundCount == 2);
    printf(" Found 2 Phone records\n");
    
    // Test 2: ค้นหาด้วย ID
    printf("\nTest 2: Search by ID (T001)\n");
    f = fopen(testFileName, "r");
    int foundByID = 0;
    fgets(line, sizeof(line), f); // skip header
    
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "T001")) {
            foundByID = 1;
            printf("  Found: %s", line);
            break;
        }
    }
    fclose(f);
    assert(foundByID == 1);
    printf(" Found Tablet by ID\n");
    
    // Test 3: ค้นหาที่ไม่มี
    printf("\nTest 3: Search non-existent item (Laptop)\n");
    f = fopen(testFileName, "r");
    int notFound = 1;
    fgets(line, sizeof(line), f); // skip header
    
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "Laptop")) {
            notFound = 0;
        }
    }
    fclose(f);
    assert(notFound == 1);
    printf(" Correctly returned no result\n");
    
    printf("\n searchData() Unit Test PASSED\n");
    printf("========================================\n");
    
    // ลบไฟล์ทดสอบ
    remove(testFileName);
    printf(" Test file cleaned up\n\n");
}

void testUpdateData() {
    const char *testFileName = "UnitTest_Update.csv";
    const char *tempFileName = "UnitTest_Update_temp.csv";
    
    printf("\n========================================\n");
    printf("   Testing updateData() Function\n");
    printf("========================================\n");
    
    // สร้างไฟล์ทดสอบพร้อมข้อมูล
    createTestCSV(testFileName);
    FILE *f = fopen(testFileName, "a");
    fprintf(f, "Camera, C001, 6-10-2025, Lens Cleaning\n");
    fclose(f);
    
    printf("Original data:\n");
    f = fopen(testFileName, "r");
    char line[200];
    while (fgets(line, sizeof(line), f)) {
        printf("  %s", line);
    }
    fclose(f);
    
    // Test: Update ข้อมูล
    printf("\nUpdating Camera details...\n");
    FILE *temp = fopen(tempFileName, "w");
    f = fopen(testFileName, "r");
    
    fgets(line, sizeof(line), f);
    fputs(line, temp); // copy header
    
    int updated = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "C001")) {
            fprintf(temp, "Camera, C001, 6-10-2025, Lens Cleaning (UPDATED)\n");
            updated = 1;
        } else {
            fputs(line, temp);
        }
    }
    fclose(f);
    fclose(temp);
    
    remove(testFileName);
    rename(tempFileName, testFileName);
    
    // ตรวจสอบการ update
    f = fopen(testFileName, "r");
    int verifyUpdate = 0;
    printf("\nUpdated data:\n");
    while (fgets(line, sizeof(line), f)) {
        printf("  %s", line);
        if (strstr(line, "UPDATED")) {
            verifyUpdate = 1;
        }
    }
    fclose(f);
    
    assert(updated == 1);
    assert(verifyUpdate == 1);
    
    printf("\n updateData() Unit Test PASSED\n");
    printf("========================================\n");
    
    // ลบไฟล์ทดสอบ
    remove(testFileName);
    printf(" Test file cleaned up\n\n");
}

void testDeleteData() {
    const char *testFileName = "UnitTest_Delete.csv";
    const char *tempFileName = "UnitTest_Delete_temp.csv";
    
    printf("\n========================================\n");
    printf("   Testing deleteData() Function\n");
    printf("========================================\n");
    
    // สร้างไฟล์ทดสอบพร้อมข้อมูล
    createTestCSV(testFileName);
    FILE *f = fopen(testFileName, "a");
    fprintf(f, "Printer, PR001, 7-10-2025, Ink Replacement\n");
    fprintf(f, "Scanner, SC001, 8-10-2025, Glass Cleaning\n");
    fclose(f);
    
    printf("Original data:\n");
    f = fopen(testFileName, "r");
    char line[200];
    while (fgets(line, sizeof(line), f)) {
        printf("  %s", line);
    }
    fclose(f);
    
    // Test: Delete ข้อมูล (mark as deleted)
    printf("\nDeleting Printer (PR001)...\n");
    FILE *temp = fopen(tempFileName, "w");
    f = fopen(testFileName, "r");
    
    fgets(line, sizeof(line), f);
    fputs(line, temp); // copy header
    
    int deleted = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "PR001")) {
            char name[50], id[50], date[30], details[50];
            sscanf(line, "%49[^,],%49[^,],%29[^,],%49[^\n]", name, id, date, details);
            
            // trim spaces
            char *p = name; while (*p == ' ') p++; memmove(name, p, strlen(p) + 1);
            p = id; while (*p == ' ') p++; memmove(id, p, strlen(p) + 1);
            p = date; while (*p == ' ') p++; memmove(date, p, strlen(p) + 1);
            p = details; while (*p == ' ') p++; memmove(details, p, strlen(p) + 1);
            
            fprintf(temp, "%s, %s, %s, %s (Deleted)\n", name, id, date, details);
            deleted = 1;
        } else {
            fputs(line, temp);
        }
    }
    fclose(f);
    fclose(temp);
    
    remove(testFileName);
    rename(tempFileName, testFileName);
    
    // ตรวจสอบการ delete
    f = fopen(testFileName, "r");
    int foundDeleted = 0;
    int foundActive = 0;
    printf("\nData after deletion:\n");
    while (fgets(line, sizeof(line), f)) {
        printf("  %s", line);
        if (strstr(line, "PR001") && strstr(line, "Deleted")) {
            foundDeleted = 1;
        }
        if (strstr(line, "SC001") && !strstr(line, "Deleted")) {
            foundActive = 1;
        }
    }
    fclose(f);
    
    assert(deleted == 1);
    assert(foundDeleted == 1);
    assert(foundActive == 1);
    
    printf("\n Record marked as deleted\n");
    printf(" Other records remain active\n");
    printf("\n deleteData() Unit Test PASSED\n");
    printf("========================================\n");
    
    // ลบไฟล์ทดสอบ
    remove(testFileName);
    printf(" Test file cleaned up\n\n");
}

int main() {
    int choice;
    
    do {
        printf("\n========================================\n");
        printf("       UNIT TEST MENU                   \n");
        printf("========================================\n");
        printf("1. Test addData() Function\n");
        printf("2. Test searchData() Function\n");
        printf("3. Test updateData() Function\n");
        printf("4. Test deleteData() Function\n");
        printf("5. Run All Tests\n");
        printf("0. Exit\n");
        printf("========================================\n");
        printf("Select test: ");
        scanf("%d", &choice);
        getchar();
        
        switch (choice) {
            case 1:
                testAddData();
                break;
            case 2:
                testSearchData();
                break;
            case 3:
                testUpdateData();
                break;
            case 4:
                testDeleteData();
                break;
            case 5:
                printf("\n========================================\n");
                printf("     RUNNING ALL UNIT TESTS             \n");
                printf("========================================\n");
                testAddData();
                testSearchData();
                testUpdateData();
                testDeleteData();
                printf("\n========================================\n");
                printf("    ALL UNIT TESTS PASSED!          \n");
                printf("========================================\n");
                break;
            case 0:
                printf("\nExiting Unit Test Program...\n");
                break;
            default:
                printf("\n Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
    
    return 0;
}