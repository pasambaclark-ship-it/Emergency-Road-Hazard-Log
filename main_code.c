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

/* Member 1: collect a report and append it to hazards.txt */
void addHazard(void)
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
    char again;
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
    scanf(" %c", &again);
    getchar();  //Remove lang 'yong Enter Key from the input hihi

    } while (again == 'y' || again == 'Y');
}

void viewHazards(void){
    char line[LINE_SIZE];
    FILE *fp;

    fp = fopen("hazard.txt", "r");
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

void searchHazards(void) {
    char searchText[LOCATION_SIZE];
    char line[LINE_SIZE];
    FILE *fp;

    int found=0;

    printf("Enter a Location or Hazard to search: ");
    fgets(searchText, sizeof(searchText), stdin);
    searchText[strcspn(searchText, "\n")]='\0';

    fp= fopen("hazard.txt", "r");
    if(fp==NULL){
        printf("No hazard reports found.\n");
        return;
    }

    printf("\n===== SEARCH RESULTS =====\n");

    while(fgets(line, sizeof(line), fp) !=NULL){
        if(strstr(line, searchText) !=NULL){
            printf("%s", line);
            found=1;
        }
    }

    if(found==0){
        printf("No matching hazard report found.\n");
    }
    fclose(fp);
}

// BRAXTENN 
void updateHazardStatus(void)
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

    int found = 0;
    int validDate;
    int i;

    /* TODO: Find the report by location. */
    /* TODO: Write all records to a temporary file, updating the matching record. */
    /* TODO: Set status to Cleared and save the date cleared. */
    /* TODO: Close files and replace the original file with the temporary file. */

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

    temp = fopen("temp.txt", "w");

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
                        remove("temp.txt");
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
        remove("temp.txt");
        return;
    }

    if (remove(HAZARD_FILE) != 0)
    {
        printf("Could not remove the original file.\n");
        return;
    }

    if (rename("temp.txt", HAZARD_FILE) != 0)
    {
        printf("Could not replace the original file.\n");
        return;
    }

    printf("\nHazard records updated successfully!\n");
}


int main(void)
{
    // Add the menu here when the team combines the functions
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