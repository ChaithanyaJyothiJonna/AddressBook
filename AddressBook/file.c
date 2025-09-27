#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) {
  FILE *fptr=fopen("contact.csv","w");
    if(fptr==NULL)
    {
        printf("Opening file falied");
        return ;
    }
    fprintf(fptr,"%d\n",addressBook->contactCount);
    for(int i=0;i<addressBook->contactCount;i++)
    {
        fprintf(fptr,"%s,%s,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    fclose(fptr);
}

void loadContactsFromFile(AddressBook *addressBook) 
{
    FILE *fptr = fopen("contact.csv","r");
    if(fptr == NULL)
    {
        printf("Failed to open fail");
        return;
    }
    fscanf(fptr,"%d\n",&addressBook->contactCount);
    for(int i =0;i<addressBook->contactCount;i++)
    {
        fscanf(fptr,"%[^,],%[^,],%[^\n]\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    fclose(fptr);
}
