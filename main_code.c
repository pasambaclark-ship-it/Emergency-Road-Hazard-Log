#include <stdio.h>
#include <string.h>


#define HAZARD_FILE "hazards.txt"
#define LOCATION_SIZE 100
#define TYPE_SIZE 50
#define DESCRIPTION_SIZE 200
#define NAME_SIZE 50
#define DATE_SIZE 20
#define STATUS_SIZE 20
#define LINE_SIZE 512

//Yanna
void addHazard()
{
    char location[LOCATION_SIZE];
    char hazardType[TYPE_SIZE];
    char description[DESCRIPTION_SIZE];
    char reportedBy[NAME_SIZE];
    char dateReported[DATE_SIZE];
    char status[STATUS_SIZE];
    char dateCleared[DATE_SIZE];
    char choiceText[10];
    int choice;
    char again[10];
    FILE *fp;

    do
    {
        printf("\n===== REPORT ROAD HAZARD =====\n");

    printf("Enter location: ");
    fgets(location, sizeof(location), stdin);
    location[strcspn(location, "\n")] = '\0';

    printf("Enter hazard type: ");
    fgets(hazardType, sizeof(hazardType), stdin);
    hazardType[strcspn(hazardType, "\n")] = '\0';

    printf("Enter description: ");
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = '\0';

    printf("Enter reporter name: ");
    fgets(reportedBy, sizeof(reportedBy), stdin);
    reportedBy[strcspn(reportedBy, "\n")] = '\0';

    printf("Enter date reported (YYYY-MM-DD): ");
    fgets(dateReported, sizeof(dateReported), stdin);
    dateReported[strcspn(dateReported, "\n")] = '\0';

    printf("\nChoose the report status:\n");
    printf("1. Blocked (active hazard or closure)\n");
    printf("2. Cleared (already reopened)\n");
    printf("Enter 1 or 2: ");
    fgets(choiceText, sizeof(choiceText), stdin);
    choice = choiceText[0] - '0';

    switch (choice)
    {
        case 1:
            strcpy(status, "Blocked");
            strcpy(dateCleared, "N/A");
            break;
        case 2:
            strcpy(status, "Cleared");
            printf("Enter date cleared (YYYY-MM-DD): ");
            fgets(dateCleared, sizeof(dateCleared), stdin);
            dateCleared[strcspn(dateCleared, "\n")] = '\0';
            break;
        default:
            printf("Invalid choice. The report will be saved as Blocked.\n");
            strcpy(status, "Blocked");
            strcpy(dateCleared, "N/A");
            break;
    }

    if (strchr(location, '|') != NULL || strchr(hazardType, '|') != NULL ||
        strchr(description, '|') != NULL || strchr(reportedBy, '|') != NULL ||
        strchr(dateReported, '|') != NULL || strchr(dateCleared, '|') != NULL)
    {
        printf("Do not use the | character in any answer. Report not saved.\n");
        return;
    }

    fp = fopen(HAZARD_FILE, "a");
    if (fp == NULL)
    {
        printf("Could not open hazards.txt. The report was not saved.\n");
        return;
    }

    fprintf(fp, "%s|%s|%s|%s|%s|%s|%s\n",
            location, hazardType, description, reportedBy,
            dateReported, status, dateCleared);

    fclose(fp);
    printf("Hazard report saved.\n");

    printf("\nWould you like to add another report? (Y/N): ");
    fgets(again, sizeof(again), stdin);

    } while (again[0] == 'y' || again[0] == 'Y');
}


//Clark
void viewHazards(){
    char line[LINE_SIZE];
    FILE *fp;

    fp = fopen(HAZARD_FILE, "r");
    if(fp==NULL){
        printf("No hazard reports found.\n");
        return;
    }

    printf("\n===== ALL HAZARD REPORTS =====\n");

    while(fgets(line, sizeof(line), fp) !=NULL){
        printf("%s", line);
    }
    fclose(fp);
}

//Clark
void searchHazards()
{
    char searchText[LOCATION_SIZE];
    char line[LINE_SIZE];
    FILE *fp;
    int found = 0;

    printf("Enter a Location or Hazard to search: ");
    fgets(searchText, sizeof(searchText), stdin);
    searchText[strcspn(searchText, "\n")] = '\0';

    if (searchText[0] == '\0')
    {
        printf("Please enter something to search for.\n");
        return;
    }

    fp = fopen(HAZARD_FILE, "r");
    if (fp == NULL)
    {
        printf("No hazard reports found.\n");
        return;
    }

    printf("\n===== SEARCH RESULTS =====\n");

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (strstr(line, searchText) != NULL)
        {
            printf("%s", line);
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("No matching hazard report found.\n");
    }

    fclose(fp);
}

// BRAXTENN 
void updateHazardStatus()
{
    char searchLocation[LOCATION_SIZE];
    char dateCleared[DATE_SIZE];
    char line[LINE_SIZE];

    char location[LOCATION_SIZE];
    char hazardType[TYPE_SIZE];
    char description[DESCRIPTION_SIZE];
    char reportedBy[NAME_SIZE];
    char dateReported[DATE_SIZE];
    char status[STATUS_SIZE];
    char oldDateCleared[DATE_SIZE];

    FILE *fp;
    FILE *temp;
    FILE *check;

    int found = 0;
    int validDate;
    int i;

    printf("\n===== UPDATE HAZARD STATUS =====\n");

    printf("Enter location of the hazard to update: ");
    fgets(searchLocation, sizeof(searchLocation), stdin);
    searchLocation[strcspn(searchLocation, "\n")] = '\0';

    if (strchr(searchLocation, '|') != NULL)
    {
        printf("Location cannot contain the | character.\n");
        return;
    }

    fp = fopen(HAZARD_FILE, "r");

    if (fp == NULL)
    {
        printf("No hazard reports found.\n");
        return;
    }

    check = fopen("hazards_update_temp.tmp", "r");
    if (check != NULL)
    {
        fclose(check);
        fclose(fp);
        printf("Temporary file already exists. No files were changed.\n");
        return;
    }

    temp = fopen("hazards_update_temp.tmp", "w");

    if (temp == NULL)
    {
        printf("Could not create temporary file.\n");
        fclose(fp);
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line,
            "%99[^|]|%49[^|]|%199[^|]|%49[^|]|%19[^|]|%19[^|]|%19[^\n]",
            location, hazardType, description, reportedBy,
            dateReported, status, oldDateCleared) != 7)
        {
            fprintf(temp, "%s", line);
            continue;
        }

        if (strcmp(location, searchLocation) == 0)
        {
            found = 1;

            printf("\n===== HAZARD REPORT FOUND =====\n");
            printf("Location: %s\n", location);
            printf("Hazard Type: %s\n", hazardType);
            printf("Description: %s\n", description);
            printf("Reported By: %s\n", reportedBy);
            printf("Date Reported: %s\n", dateReported);
            printf("Current Status: %s\n", status);

            if (strcmp(status, "Cleared") == 0)
            {
                printf("\nThis hazard is already Cleared.\n");
                fprintf(temp, "%s", line);
            }
            else
            {
                do
                {
                    validDate = 1;
                    printf("\nEnter date cleared (YYYY-MM-DD): ");

                    if (fgets(dateCleared, sizeof(dateCleared), stdin) == NULL)
                    {
                        fclose(fp);
                        fclose(temp);
                        remove("hazards_update_temp.tmp");
                        return;
                    }

                    dateCleared[strcspn(dateCleared, "\n")] = '\0';

                    if (strlen(dateCleared) != 10)
                    {
                        validDate = 0;
                    }
                    else
                    {
                        for (i = 0; i < 10; i++)
                        {
                            if (i == 4 || i == 7)
                            {
                                if (dateCleared[i] != '-')
                                {
                                    validDate = 0;
                                }
                            }
                            else if (dateCleared[i] < '0' || dateCleared[i] > '9')
                            {
                                validDate = 0;
                            }
                        }
                    }

                    if (validDate == 0)
                    {
                        printf("Invalid date format. Use YYYY-MM-DD.\n");
                    }
                } while (validDate == 0);

                strcpy(status, "Cleared");

                fprintf(temp, "%s|%s|%s|%s|%s|%s|%s\n",
                        location, hazardType, description,
                        reportedBy, dateReported, status, dateCleared);
            }
        }
        else
        {
            fprintf(temp, "%s", line);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found == 0)
    {
        printf("\nNo matching hazard report found.\n");
        remove("hazards_update_temp.tmp");
        return;
    }

    check = fopen("hazards_update_backup.bak", "r");
    if (check != NULL)
    {
        fclose(check);
        remove("hazards_update_temp.tmp");
        printf("Backup file already exists. No files were changed.\n");
        return;
    }

    if (rename(HAZARD_FILE, "hazards_update_backup.bak") != 0)
    {
        printf("Could not protect the original file.\n");
        remove("hazards_update_temp.tmp");
        return;
    }

    if (rename("hazards_update_temp.tmp", HAZARD_FILE) != 0)
    {
        printf("Could not replace the original file. Restoring backup.\n");
        rename("hazards_update_backup.bak", HAZARD_FILE);
        remove("hazards_update_temp.tmp");
        return;
    }

    remove("hazards_update_backup.bak");
    printf("\nHazard records updated successfully!\n");
}


int main()
{
    int choice;
    char choiceText[10];

    do
    {
        printf("\n====================================\n");
        printf("           ROAD HAZARD LOG\n");
        printf("====================================\n");

        printf("[1] Add Hazard Report\n");
        printf("[2] View All Hazard Reports\n");
        printf("[3] Search Hazard Report\n");
        printf("[4] Update Hazard Status\n");
        printf("[5] Exit\n");

        printf("------------------------------------\n");
        printf("Enter your choice: ");

        fgets(choiceText, sizeof(choiceText), stdin);
        choice = choiceText[0] - '0';

        switch (choice)
        {
            case 1:
                addHazard();
                break;

            case 2:
                viewHazards();
                break;

            case 3:
                searchHazards();
                break;

            case 4:
                updateHazardStatus();
                break;

            case 5:
                printf("\nThank you for using Road Hazard Log!\n");
                break;

            default:
                printf("\nInvalid choice. Please try again.\n");
        }

    } while (choice != 5);

    return 0;
}








