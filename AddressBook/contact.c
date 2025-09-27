#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#include "contact.h"
#include "file.h"
#include "populate.h"

void listContacts(AddressBook *addressBook) //,int sortCriteria) 
{
    // Sort contacts based on the chosen criteria
    printf("%-20s %-20s %-20s\n","NAME","PHONE","EMAIL");
    // Sort contacts based on the chosen criteria
    for(int i=0;i<addressBook->contactCount;i++)
    {
        printf("%-20s ",addressBook->contacts[i].name);
        printf("%-20s ",addressBook->contacts[i].phone);
        printf("%-20s\n",addressBook->contacts[i].email);
    }
}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    //populateAddressBook(addressBook);
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}
// ---------------- Validation Functions ----------------
int is_validate_name(char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') {
            return 0; // invalid
        }
    }
    return 1; // valid
}
int is_validate_phoneno(char *phone, AddressBook *addressBook) {
    // length check
    if (strlen(phone) != 10)
        return 0;
    // digits check
    for (int i = 0; phone[i] != '\0'; i++) {
        if (!isdigit(phone[i])) {
            return 0;
        }
    }
    return 1; // valid
}
int has_uppercase(const char *s) 
{
    for (int i = 0; s[i] != '\0'; i++) {
        if (isupper(s[i])) {
            return 1;
        }
    }
    return 0;
}
int is_validate_email(char *email, AddressBook *addressBook)
{
    if (has_uppercase(email)) {
        return 0;
    }
    char *at = strchr(email, '@');
    char *dot = strrchr(email, '.');
    if (!at || !dot || at > dot) {
        return 0; 
    }
    if (strcmp(dot, ".com") != 0) // check if ".com" exists at the end
    {
        return 0; // only allow .com for now
    }
    if (at == email || !isalnum(*(at - 1))) // check if character before '@' is alnum and check if char before ".com" is alnum.
    {
        return 0;
    }
    if (dot == email || !isalnum(*(dot - 1))) //check if char before ".com" is alnum
    {
        return 0;
    }
    return 1; // valid
}
void createContact(AddressBook *addressBook)
{
	/* Define the logic to create a Contacts */
    Contact *c = &addressBook->contacts[addressBook->contactCount];
    int valid = 0;
    // --- Name Validation ---
    while (!valid)
    {
        printf("Enter the Name: ");
        __fpurge(stdin);
        scanf(" %[^\n]", c->name);   // read name from user
        valid = is_validate_name(c->name); // assume valid
        if(!valid)
        {
            printf("Invalid name. Use only alphabets and spaces.\n");
        }       
    }
    // --- Phone Validation --
    valid = 0; 
    while (!valid)
    {
        printf("Enter the Phone.No : ");
        __fpurge(stdin); 
        scanf(" %[^\n]", c->phone);   // read phone no from user
        valid = is_validate_phoneno(c->phone,addressBook); // assume valid
        if(!valid)
        {
            printf("Invalid phone number.Must be 10 digits and Unique\n");
        } 
        // check duplicate phone number in existing contacts
         for (int i = 0; i < addressBook->contactCount && valid; i++) 
         {
             if (strcmp(addressBook->contacts[i].phone, c->phone) == 0)
            {
                 printf("Error: Phone number already exists.\n"); 
                 valid = 0; 
                 break; 
            } 
        } 
    }
    // --- Email Validation ---
    valid = 0;
    while (!valid)
    {
        printf("Enter the Email: ");
        __fpurge(stdin);
        scanf(" %[^\n]", c->email); //read Email from user
        if (has_uppercase(c->email))
        {
        printf("Error: Email must not contain uppercase letters.\n");
        valid = 0;
        continue;
        }
        valid = is_validate_email(c->email,addressBook);
        if(!valid)
        {
            printf("Invalid email .Email not in a proper format.\n");
        } 
        // check duplicate email in existing contacts
        for (int i = 0; i < addressBook->contactCount && valid; i++) 
        { 
            if (strcmp(addressBook->contacts[i].email, c->email) == 0)
            { 
                printf("Error: Email already exists.\n"); 
                valid = 0; 
                break; 
            } 
        } 
    }
    // Save contact
    addressBook->contactCount++;
    printf("Contact saved successfully!\n");
}
int foundcount = 0;
int contactIndex[100]; //To store matched indexes
void searchContact(AddressBook *addressBook) 
{
    /* Define the logic for search */
    int option;
    printf("--------Menu----------\n");
    printf("1.Search by Name\n");
    printf("2.Search by Phone No\n");
    printf("3.Search by Email\n");
    printf("----------------------\n");
    printf("Enter your option: ");
    __fpurge(stdin);
    scanf("%d", &option);
    char input[100]; 
    foundcount = 0;     //To how many matches found
    int valid =0;
    switch(option) {
        case 1: // search by name
        while(!valid)
        {
            printf("Enter name to search: ");
            scanf(" %[^\n]", input);
            valid = is_validate_name(input);
            if(!valid)
            {
                printf("Invalid name. Use only alphabets and spaces.\n");
            } 
        }
        for (int i = 0; i < addressBook->contactCount; i++) 
        {
            if (strcmp(addressBook->contacts[i].name, input) == 0)
            {
                contactIndex[foundcount++] = i; //To store index
            }
        }
        break;
        case 2: // search by phone
        while(!valid)
        {
            printf("Enter phone number to search: ");
            scanf(" %[^\n]", input);

            valid = is_validate_phoneno(input,addressBook); // assume valid
            if(!valid)
            {
                printf("Invalid phone number.Must be 10 digits and Unique\n");
            }
        }
        for (int i = 0; i < addressBook->contactCount; i++) 
        {
            if (strcmp(addressBook->contacts[i].phone, input) == 0)
            {
                contactIndex[foundcount++] = i;
            }
        }
        break;
        case 3: // search by email
        while(!valid)
        {
            printf("Enter email to search: ");
            scanf(" %[^\n]", input);
            if (has_uppercase(input))
            {
                printf("Error: Email must not contain uppercase letters.\n");
                continue;
            }
            valid = is_validate_email(input,addressBook);
            if(!valid)
            {
                printf("Invalid email .Email not in a proper format.\n");
            }
        } 
        for (int i = 0; i < addressBook->contactCount; i++) {
            if (strcmp(addressBook->contacts[i].email, input) == 0)
            {
                contactIndex[foundcount++] = i;
            }
        }
        break;
        default:
            printf("Invalid option!\n");
            return;
    }
    if (foundcount > 0) {
        printf("%d contacts are found\n", foundcount);
        for (int j = 0; j < foundcount; j++) {
            int index = contactIndex[j];
            printf("%d.%s %s %s\n",j+1,
                   addressBook->contacts[index].name,
                   addressBook->contacts[index].phone,
                   addressBook->contacts[index].email);
        }
    } else {
        printf("Contact not found.\n");
    }
}
void editContact(AddressBook *addressBook) {
    if (addressBook->contactCount == 0) {
        printf("No contacts available to edit!\n");
        return;
    }
    searchContact(addressBook);
    if (foundcount == 0) return;
    int sno = 1;
    if (foundcount > 1) {
        printf("Enter serial number to edit : ");
        scanf("%d", &sno);
        if (sno < 1 || sno > foundcount) 
        {
            printf("Invalid selection!\n");
            return;
        }
    }
    int Index = contactIndex[sno - 1];
    Contact *c = &addressBook->contacts[Index];
    int option;
    printf("--------Menu----------\n");
    printf("1.Edit Name\n");
    printf("2.Edit Phone No\n");
    printf("3.Edit Email\n");
    printf("----------------------\n");
    printf("Enter your option: ");
    __fpurge(stdin);
    scanf("%d", &option);
    int valid = 0;
    char new[100];
    switch(option) 
    {
        case 1: // Edit name
            do {
                printf("Enter new name: ");
                scanf(" %[^\n]", new);
                valid = is_validate_name(new);
                if (!valid) {
                    printf("Invalid name. Use only alphabets and spaces.\n");
                }
            } while (!valid);
            strcpy(c->name, new);
            printf("Name updated successfully!\n");
            break;
        case 2: // Edit phone
            do {
                printf("Enter new phone no: ");
                scanf(" %[^\n]", new);
                valid = is_validate_phoneno(new,addressBook);
                if (!valid) {
                    printf("Invalid phone number. Must be 10 digits and unique.\n");
                }
                if (valid) {
                    for (int i = 0; i < addressBook->contactCount; i++) {
                        if (i == Index) 
                        continue;
                        if (strcmp(addressBook->contacts[i].phone, new) == 0) {
                            printf("Error: Phone number already exists.try again.\n");
                            valid = 0;
                            break;
                        }
                    }
                }
            } while (!valid);
            strcpy(c->phone, new);
            printf("Phone updated successfully!\n");
            break;
        case 3: // Edit email
            do {
                printf("Enter new email: ");
                scanf(" %[^\n]", new);
                if (has_uppercase(new)) 
                {
                    printf("Error: Email must not contain uppercase letters.\n");
                    valid = 0;
                    continue;
                }
                valid = is_validate_email(new, addressBook);
                if (!valid) {
                    printf("Invalid email format. Try again.\n");
                }
                if (valid) {
                    for (int i = 0; i < addressBook->contactCount; i++) {
                        if (i == Index) 
                        continue;
                        if (strcmp(addressBook->contacts[i].email, new) == 0) {
                            printf("Error: Email already exists.Try again/n");
                            valid = 0;
                            break;
                        }
                    }
                }
            } while (!valid);
            strcpy(c->email, new);
            printf("Email updated successfully!\n");
            break;
        default:
            printf("Invalid option!\n");
    }
}

void deleteContact(AddressBook *addressBook)
{
	/* Define the logic for deletecontact */
    if (addressBook->contactCount <= 0) 
    {
        printf("Contact not yet created\n");
        return;
    }
    //Call searchContact
    searchContact(addressBook);
    if (foundcount == 0) {
        printf("No matching contacts found!\n");
        return;
    }
    int sno;
    if (foundcount == 1)
    {
        sno = 1;
        printf("One contact found, auto-selecting...\n");
    } 
    else 
    {
        printf("Enter Serial No of contact to delete: ");
        scanf("%d", &sno);
        if (sno < 1 || sno > foundcount) 
        {
            printf("Invalid serial number!\n");
            return;
        }
    }
    // Step 2: Convert serial no → index
    int delIndex = contactIndex[sno - 1];
    // Shift contacts left
    for (int i = delIndex; i < addressBook->contactCount - 1; i++)
     {
        strcpy(addressBook->contacts[i].name, addressBook->contacts[i + 1].name);
        strcpy(addressBook->contacts[i].phone, addressBook->contacts[i + 1].phone);
        strcpy(addressBook->contacts[i].email, addressBook->contacts[i + 1].email);
     }
    // Decrement the contact count
    addressBook->contactCount--;
    printf("Contact deleted successfully!\n");

}
