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

void runEndToEndTest() {
    const char *testFileName = "E2ETest.csv";
    const char *tempFileName = "E2ETest_temp.csv";
    
    printf("\n=========================================\n");
    printf("    Running End-to-End Test             \n");
    printf("=========================================\n");
    
    // สร้างไฟล์ทดสอบใหม่
    FILE *testFile = fopen(testFileName, "w");
    fprintf(testFile, "AssetName, AssetID, MaintenanceDate(DD-MM-YYYY), MaintenanceDetails\n");
    fclose(testFile);
    
    // ===== Test 1: ADD =====
    printf("\n[Test 1] Testing ADD functionality...\n");
    printf("=========================================\n");
    testFile = fopen(testFileName, "a");
    fprintf(testFile, "TestLaptop, T001, 15-3-2025, Screen Repair\n");
    fprintf(testFile, "TestPrinter, T002, 20-5-2025, Ink Replacement\n");
    fprintf(testFile, "TestRouter, T003, 10-8-2025, Firmware Update\n");
    fclose(testFile);
    printf(" Added 3 test records\n");
    
    // ตรวจสอบว่าข้อมูลถูกเพิ่มจริง
    testFile = fopen(testFileName, "r");
    int lineCount = 0;
    char buffer[200];
    while (fgets(buffer, sizeof(buffer), testFile)) {
        lineCount++;
    }
    fclose(testFile);
    assert(lineCount == 4); // header + 3 records
    printf(" Verified: 3 records added successfully\n");
    printf(" ADD Test PASSED\n");
    
    // ===== Test 2: SEARCH =====
    printf("\n[Test 2] Testing SEARCH functionality...\n");
    printf("=========================================\n");
    testFile = fopen(testFileName, "r");
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
    printf(" SEARCH Test PASSED\n");
    
    // ===== Test 3: UPDATE =====
    printf("\n[Test 3] Testing UPDATE functionality...\n");
    printf("=========================================\n");
    FILE *temp = fopen(tempFileName, "w");
    testFile = fopen(testFileName, "r");
    
    fgets(buffer, sizeof(buffer), testFile);
    fputs(buffer, temp); // copy header
    
    int updated = 0;
    while (fgets(buffer, sizeof(buffer), testFile)) {
        if (strstr(buffer, "T001")) {
            fprintf(temp, "TestLaptop-Updated, T001, 15-3-2025, Screen Repair (Updated)\n");
            updated = 1;
            printf(" Updated record T001\n");
        } else {
            fputs(buffer, temp);
        }
    }
    fclose(testFile);
    fclose(temp);
    
    remove(testFileName);
    rename(tempFileName, testFileName);
    assert(updated == 1);
    
    // ตรวจสอบการอัพเดต
    testFile = fopen(testFileName, "r");
    int verifyUpdate = 0;
    while (fgets(buffer, sizeof(buffer), testFile)) {
        if (strstr(buffer, "Updated")) {
            verifyUpdate = 1;
            printf(" Verified update: %s", buffer);
            break;
        }
    }
    fclose(testFile);
    assert(verifyUpdate == 1);
    printf(" UPDATE Test PASSED\n");
    
    // ===== Test 4: DELETE =====
    printf("\n[Test 4] Testing DELETE functionality...\n");
    printf("=========================================\n");
    temp = fopen(tempFileName, "w");
    testFile = fopen(testFileName, "r");
    
    fgets(buffer, sizeof(buffer), testFile);
    fputs(buffer, temp); // copy header
    
    int deletedCount = 0;
    while (fgets(buffer, sizeof(buffer), testFile)) {
        if (strstr(buffer, "T002")) {
            // เพิ่ม (Deleted) แทนการลบทิ้ง
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
    
    remove(testFileName);
    rename(tempFileName, testFileName);
    assert(deletedCount == 1);
    printf(" DELETE Test PASSED\n");
    
    // ===== Test 5: VERIFICATION =====
    printf("\n[Test 5] Testing SEARCH after DELETE...\n");
    printf("=========================================\n");
    testFile = fopen(testFileName, "r");
    int foundDeleted = 0;
    int foundActive = 0;
    fgets(buffer, sizeof(buffer), testFile); // skip header
    
    printf("Current database state:\n");
    while (fgets(buffer, sizeof(buffer), testFile)) {
        printf("  %s", buffer);
        
        // เช็คว่ามี T002 และมี Deleted
        if (strstr(buffer, "T002")) {
            if (strstr(buffer, "Deleted")) {
                foundDeleted = 1;
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
    
    assert(foundDeleted == 1);
    assert(foundActive == 1);
    printf("\n Deleted record marked correctly\n");
    printf(" Active records still accessible\n");
    printf(" VERIFICATION Test PASSED\n");
    
    // ===== สรุปผลการทดสอบ =====
    printf("\n=========================================\n");
    printf("   End-to-End Test Summary              \n");
    printf("=========================================\n");
    printf("   ADD Test:         PASSED           \n");
    printf("   SEARCH Test:      PASSED           \n");
    printf("   UPDATE Test:      PASSED           \n");
    printf("   DELETE Test:      PASSED           \n");
    printf("   VERIFICATION:     PASSED           \n");
    printf("=========================================\n");
    printf("   All E2E Tests PASSED!            \n");
    printf("=========================================\n");
    
    // ลบไฟล์ทดสอบ
    remove(testFileName);
    printf("\n E2E test file '%s' cleaned up\n", testFileName);
    printf(" Original AssetData.csv remains unchanged\n");
}

int main() {
    int choice;
    
    do {
        printf("\n=========================================\n");
        printf("     END-TO-END TEST MENU               \n");
        printf("=========================================\n");
        printf("1. Run End-to-End Test\n");
        printf("0. Exit\n");
        printf("=========================================\n");
        printf("Select option: ");
        scanf("%d", &choice);
        getchar();
        
        switch (choice) {
            case 1:
                runEndToEndTest();
                break;
            case 0:
                printf("\nExiting E2E Test Program...\n");
                break;
            default:
                printf("\n Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
    
    return 0;
}