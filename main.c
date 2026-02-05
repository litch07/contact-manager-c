#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>

#define CONTACT_FILE "All-Contact.txt"
#define NAME_MAX_LEN 50
#define PHONE_MAX_LEN 25
#define EMAIL_MAX_LEN 254

typedef struct {
    char name[NAME_MAX_LEN + 1];
    char phone[PHONE_MAX_LEN + 1];
    char email[EMAIL_MAX_LEN + 1];
} Contact;

bool isRunning = true;

void clear_screen(void)
{
    system("cls");
}

void clear_input(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void Menu()
{
    printf("\nContact Management System\n");
    printf("==========================\n");
    printf("Main Menu\n");
    printf("1. Add a new contact\n");
    printf("2. Show all contacts\n");
    printf("3. Search for a contact\n");
    printf("4. Edit a contact\n");
    printf("5. Delete a contact\n");
    printf("6. Delete all contacts\n");
    printf("i. User guide\n");
    printf("a. About\n");
    printf("0. Exit\n");
    printf("--------------------------\n");
    printf("Enter your choice: ");
}

void trim_newline(char *s)
{
    s[strcspn(s, "\r\n")] = '\0';
}

int equals_ignore_case(const char *a, const char *b)
{
    while (*a && *b)
    {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return 0;
        a++;
        b++;
    }
    return *a == '\0' && *b == '\0';
}

int load_contacts(Contact **out)
{
    FILE *fp = fopen(CONTACT_FILE, "r");
    if (fp == NULL)
    {
        *out = NULL;
        return 0;
    }

    Contact *list = NULL;
    int count = 0;
    char line[300];
    int state = 0;
    Contact current;

    while (fgets(line, sizeof(line), fp))
    {
        trim_newline(line);
        state++;

        if (state == 1)
            strcpy(current.name, line);
        else if (state == 2)
            strcpy(current.phone, line);
        else if (state == 3)
        {
            strcpy(current.email, line);
            Contact *next = realloc(list, sizeof(Contact) * (count + 1));
            if (!next)
            {
                free(list);
                fclose(fp);
                *out = NULL;
                return 0;
            }
            list = next;
            list[count++] = current;
            state = 0;
        }
    }

    fclose(fp);
    *out = list;
    return count;
}

void save_contacts(const Contact *list, int count)
{
    FILE *fp = fopen(CONTACT_FILE, "w");
    if (fp == NULL)
        return;

    for (int i = 0; i < count; i++)
    {
        fprintf(fp, "%s\n%s\n%s\n", list[i].name, list[i].phone, list[i].email);
    }
    fclose(fp);
}

int value_exists(const Contact *list, int count, const char *value, char type, int skipIndex)
{
    for (int i = 0; i < count; i++)
    {
        if (i == skipIndex)
            continue;

        if (type == 'p' && strcmp(list[i].phone, value) == 0)
            return 1;
        if (type == 'e' && strcmp(list[i].email, value) == 0)
            return 1;
    }
    return 0;
}

void p_line_space(char *Name, char ContactInfo)
{
    int i;
    int len = strlen(Name);

    if(Name[len-1] == '\n') {
        Name[len-1] = '\0';
        len--;
    }

    int TotalSpace = 0;
    if(ContactInfo == 'e')
        TotalSpace = 30 - len;
    else
        TotalSpace = 20 - len;

    if(TotalSpace < 0) TotalSpace = 0;

    printf("|%s", Name);
    for(i = 0; i < TotalSpace; i++)
        printf(" ");

    if(ContactInfo == 'e')
    {
        printf("|\n");
        printf("|--------------------|--------------------|------------------------------|\n");
    }
}

void restart()
{
    clear_screen();
}

void Exit()
{
    getchar();
    printf("Are you sure you want to exit? (Y/N): ");
    char choice;
    scanf("%c", &choice);
    if(choice == 'Y' || choice == 'y')
    {
        clear_screen();
        printf("Thank you for using Contact Manager.\n\n");
        isRunning = false;
    }
    else
    {
        restart();
    }
}

void GoBackOrExit()
{
    char option;
    getchar();
    printf("Press B to go back or 0 to exit: ");
    scanf("%c", &option);
    if(option == '0')
    {
        Exit();
    }
    else
    {
        clear_screen();
    }
}

void add()
{
    clear_screen();
    printf("\nAdd New Contact\n");
    printf("---------------\n\n");

    char Name[NAME_MAX_LEN + 1];
    char Phone[PHONE_MAX_LEN + 1];
    char Email[EMAIL_MAX_LEN + 1];

    printf("Enter name: ");
    scanf(" %[^\n]s", Name);

    if(strlen(Name) > NAME_MAX_LEN)
    {
        printf("\nName is too long. Maximum length is %d characters.\n", NAME_MAX_LEN);
        GoBackOrExit();
        return;
    }

    printf("Enter phone number: ");
    scanf(" %s", Phone);

    if(strlen(Phone) > PHONE_MAX_LEN)
    {
        printf("\nPhone number is too long. Maximum length is %d characters.\n", PHONE_MAX_LEN);
        GoBackOrExit();
        return;
    }

    printf("Enter email: ");
    scanf(" %s", Email);

    if(strlen(Email) > EMAIL_MAX_LEN)
    {
        printf("\nEmail is too long. Maximum length is %d characters.\n", EMAIL_MAX_LEN);
        GoBackOrExit();
        return;
    }

    Contact *list = NULL;
    int count = load_contacts(&list);
    if (value_exists(list, count, Phone, 'p', -1))
    {
        free(list);
        printf("\nA contact with this phone number already exists.\n");
        GoBackOrExit();
        return;
    }
    if (value_exists(list, count, Email, 'e', -1))
    {
        free(list);
        printf("\nA contact with this email already exists.\n");
        GoBackOrExit();
        return;
    }
    free(list);

    FILE *fp = fopen(CONTACT_FILE, "a");
    if (fp == NULL)
    {
        printf("\nUnable to open the contact file for writing.\n");
        GoBackOrExit();
        return;
    }
    fprintf(fp, "%s\n%s\n%s\n", Name, Phone, Email);
    fclose(fp);

    printf("\nContact added successfully.\n");
    GoBackOrExit();
}

void show()
{
    clear_screen();
    printf("\nAll Contacts\n");
    printf("------------\n\n");
    FILE* AllContactTextFile;
    int LineLength = 300;
    char Line[LineLength];

    printf("|====================|====================|==============================|\n");
    printf("|        Name        |    Phone Number    |          Email               |\n");
    printf("|====================|====================|==============================|\n");

    AllContactTextFile = fopen(CONTACT_FILE, "r");
    if(AllContactTextFile == NULL)
    {
        printf("\nNo contacts found.\n");
        GoBackOrExit();
        return;
    }
    int TotalContact = 0;
    int LineCount = 0;
    while(fgets(Line, LineLength, AllContactTextFile))
    {
        LineCount++;
        if(LineCount == 1)
        {
            p_line_space(Line, 'n');
        }
        else if(LineCount == 2)
        {
            p_line_space(Line, 'p');
        }
        else if(LineCount == 3)
        {
            p_line_space(Line, 'e');
            LineCount = 0;
            TotalContact++;
        }
    }
    printf("Total contacts: %d\n\n", TotalContact);
    fclose(AllContactTextFile);
    GoBackOrExit();
}

void search()
{
    clear_screen();
    printf("\nSearch Contact\n");
    printf("--------------\n\n");

    char Name[100];
    printf("Enter name: ");
    scanf(" %[^\n]s", Name);

    Contact *list = NULL;
    int count = load_contacts(&list);
    if (count == 0)
    {
        printf("No contacts found.\n");
        GoBackOrExit();
        return;
    }

    int found = 0;
    for (int i = 0; i < count; i++)
    {
        if (equals_ignore_case(list[i].name, Name))
        {
            found = 1;
            printf("\nContact found:\n");
            printf("Name : %s\n", list[i].name);
            printf("Phone: %s\n", list[i].phone);
            printf("Email: %s\n", list[i].email);
        }
    }

    if (!found)
        printf("\nNo contact found for \"%s\".\n", Name);

    free(list);
    GoBackOrExit();
}

void edit()
{
    clear_screen();
    printf("\nEdit Contact\n");
    printf("------------\n\n");

    char GivenName[NAME_MAX_LEN + 1];
    printf("Enter the name you want to edit: ");
    scanf(" %[^\n]s", GivenName);
    if(strlen(GivenName) > NAME_MAX_LEN)
    {
        printf("\nName is too long. Maximum length is %d characters.\n", NAME_MAX_LEN);
        GoBackOrExit();
        return;
    }

    Contact *list = NULL;
    int count = load_contacts(&list);
    if (count == 0)
    {
        printf("\nNo contacts found.\n");
        GoBackOrExit();
        return;
    }

    int *matches = malloc(sizeof(int) * count);
    if (!matches)
    {
        free(list);
        printf("\nUnable to allocate memory.\n");
        GoBackOrExit();
        return;
    }

    int matchCount = 0;
    for (int i = 0; i < count; i++)
    {
        if (equals_ignore_case(list[i].name, GivenName))
            matches[matchCount++] = i;
    }

    if (matchCount == 0)
    {
        printf("\nNo contact found for \"%s\".\n", GivenName);
        free(matches);
        free(list);
        GoBackOrExit();
        return;
    }

    printf("\nSelect a contact to edit:\n");
    for (int i = 0; i < matchCount; i++)
    {
        int idx = matches[i];
        printf("%d. %s | %s | %s\n", i + 1, list[idx].name, list[idx].phone, list[idx].email);
    }
    printf("0. Cancel\n");

    int choice = 0;
    printf("Choose an option: ");
    if (scanf("%d", &choice) != 1)
    {
        clear_input();
        free(matches);
        free(list);
        printf("\nInvalid input.\n");
        GoBackOrExit();
        return;
    }
    if (choice <= 0 || choice > matchCount)
    {
        free(matches);
        free(list);
        GoBackOrExit();
        return;
    }

    int selected = matches[choice - 1];

    char NewName[NAME_MAX_LEN + 1];
    char NewPhone[PHONE_MAX_LEN + 1];
    char NewEmail[EMAIL_MAX_LEN + 1];

    printf("New name (0 to keep): ");
    scanf(" %[^\n]s", NewName);
    if(strcmp(NewName, "0") != 0)
    {
        if(strlen(NewName) > NAME_MAX_LEN)
        {
            free(matches);
            free(list);
            printf("\nName is too long. Maximum length is %d characters.\n", NAME_MAX_LEN);
            GoBackOrExit();
            return;
        }
        strcpy(list[selected].name, NewName);
    }

    printf("New phone (0 to keep): ");
    scanf(" %s", NewPhone);
    if(strcmp(NewPhone, "0") != 0)
    {
        if(strlen(NewPhone) > PHONE_MAX_LEN)
        {
            free(matches);
            free(list);
            printf("\nPhone number is too long. Maximum length is %d characters.\n", PHONE_MAX_LEN);
            GoBackOrExit();
            return;
        }
        if (value_exists(list, count, NewPhone, 'p', selected))
        {
            free(matches);
            free(list);
            printf("\nA contact with this phone number already exists.\n");
            GoBackOrExit();
            return;
        }
        strcpy(list[selected].phone, NewPhone);
    }

    printf("New email (0 to keep): ");
    scanf(" %s", NewEmail);
    if(strcmp(NewEmail, "0") != 0)
    {
        if(strlen(NewEmail) > EMAIL_MAX_LEN)
        {
            free(matches);
            free(list);
            printf("\nEmail is too long. Maximum length is %d characters.\n", EMAIL_MAX_LEN);
            GoBackOrExit();
            return;
        }
        if (value_exists(list, count, NewEmail, 'e', selected))
        {
            free(matches);
            free(list);
            printf("\nA contact with this email already exists.\n");
            GoBackOrExit();
            return;
        }
        strcpy(list[selected].email, NewEmail);
    }

    save_contacts(list, count);
    printf("\nContact updated successfully.\n\n");

    free(matches);
    free(list);
    GoBackOrExit();
}

void delete_one()
{
    clear_screen();
    printf("\nDelete Contact\n");
    printf("--------------\n\n");

    char GivenName[NAME_MAX_LEN + 1];
    printf("Enter the name you want to delete: ");
    scanf(" %[^\n]s", GivenName);
    if(strlen(GivenName) > NAME_MAX_LEN)
    {
        printf("\nName is too long. Maximum length is %d characters.\n", NAME_MAX_LEN);
        GoBackOrExit();
        return;
    }

    Contact *list = NULL;
    int count = load_contacts(&list);
    if (count == 0)
    {
        printf("\nNo contacts found.\n");
        GoBackOrExit();
        return;
    }

    int *matches = malloc(sizeof(int) * count);
    if (!matches)
    {
        free(list);
        printf("\nUnable to allocate memory.\n");
        GoBackOrExit();
        return;
    }

    int matchCount = 0;
    for (int i = 0; i < count; i++)
    {
        if (equals_ignore_case(list[i].name, GivenName))
            matches[matchCount++] = i;
    }

    if (matchCount == 0)
    {
        printf("\nNo contact found for \"%s\".\n", GivenName);
        free(matches);
        free(list);
        GoBackOrExit();
        return;
    }

    printf("\nSelect a contact to delete:\n");
    for (int i = 0; i < matchCount; i++)
    {
        int idx = matches[i];
        printf("%d. %s | %s | %s\n", i + 1, list[idx].name, list[idx].phone, list[idx].email);
    }
    printf("0. Cancel\n");

    int choice = 0;
    printf("Choose an option: ");
    if (scanf("%d", &choice) != 1)
    {
        clear_input();
        free(matches);
        free(list);
        printf("\nInvalid input.\n");
        GoBackOrExit();
        return;
    }
    if (choice <= 0 || choice > matchCount)
    {
        free(matches);
        free(list);
        GoBackOrExit();
        return;
    }

    int selected = matches[choice - 1];
    for (int i = selected; i < count - 1; i++)
    {
        list[i] = list[i + 1];
    }
    count--;

    save_contacts(list, count);
    printf("\nContact deleted successfully.\n\n");

    free(matches);
    free(list);
    GoBackOrExit();
}

void delete_all()
{
    clear_screen();
    printf("\nDelete All Contacts\n");
    printf("-------------------\n\n");

    char Option;
    getchar();
    printf("Are you sure you want to delete all contacts? (Y/N): ");
    scanf("%c", &Option);
    if(Option == 'Y' || Option == 'y')
    {
        remove(CONTACT_FILE);
        FILE *AllContactTxtFile = fopen(CONTACT_FILE, "a");
        if (AllContactTxtFile != NULL)
            fclose(AllContactTxtFile);
        printf("\nAll contacts deleted successfully.\n\n");
    }
    GoBackOrExit();
}

void guideline()
{
    clear_screen();
    printf("\nUser Guide\n");
    printf("----------\n");
    printf("- Name matching is case-insensitive for search, edit, and delete.\n");
    printf("- If multiple contacts share a name, you can select which to edit or delete.\n");
    printf("- Maximum name length: 50 characters.\n");
    printf("- Maximum phone length: 25 characters.\n");
    printf("- Maximum email length: 254 characters.\n\n");
    GoBackOrExit();
}

void about()
{
    clear_screen();
    printf("\nAbout\n");
    printf("-----\n");
    printf("Developed by:\n\n");
    printf("Sadid Ahmed\n");
    printf("ID: 0112330154\n");
    printf("Telegram: @litch07\n\n");
    printf("GitHub: https://github.com/litch07\n\n");
    printf("For any queries, feel free to contact me.\n\n");
    GoBackOrExit();
}

int main(void)
{
    while(isRunning)
    {
        Menu();
        char str[100];
        char option;
        scanf("%s", str);
        option = str[0];
        switch(option)
        {
        case '0':
            Exit();
            break;
        case '1':
            add();
            break;
        case '2':
            show();
            break;
        case '3':
            search();
            break;
        case '4':
            edit();
            break;
        case '5':
            delete_one();
            break;
        case '6':
            delete_all();
            break;
        case 'I':
        case 'i':
            guideline();
            break;
        case 'A':
        case 'a':
            about();
            break;
        default:
            printf("\nInvalid choice. Please try again.\n\n");
            restart();
            break;
        }
    }
    return 0;
}
