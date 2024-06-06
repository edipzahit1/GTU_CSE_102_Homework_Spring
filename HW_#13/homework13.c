#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LENGTH 100

typedef enum
{
    CLASS,
    ORDER,
    FAMILY,
    GENUS,
    SPECIES
} InfoType;

typedef struct
{
    char *class;
    char *order;
    char *family;
    char *genus;
    char *species;
} CreatureInfo;

typedef struct _Creature
{
    CreatureInfo info;
    struct _Creature *next;
} Creature;

void get_string_dynm(char **p);
Creature *add_creature(Creature **head);
void get_creature_dynm(Creature *creature);
Creature *delete_creature(Creature **head, const char *species);
int modify_creature(Creature *head, const char *species);
void get_and_update_field(char **field, char *prompt);
void print_list(Creature *head);
void sort_list(Creature **head, InfoType by_what);
void free_list(Creature *head);
void free_creature(Creature *creature);

void sort_list(Creature **head, InfoType by_what)
{
    if (*head == NULL)
        return;
    Creature *i, *j;
    for (i = *head; i != NULL; i = i->next)
    {
        for (j = i->next; j != NULL; j = j->next)
        {
            int result;
            switch (by_what)
            {
            case CLASS: result = strcmp(i->info.class, j->info.class);
                break;
            case ORDER: result = strcmp(i->info.order, j->info.order);
                break;
            case FAMILY: result = strcmp(i->info.family, j->info.family);
                break;
            case GENUS: result = strcmp(i->info.genus, j->info.genus);
                break;
            case SPECIES: result = strcmp(i->info.species, j->info.species);
                break;
            default: result = 0;
                break;
            }
            if (result > 0)
            {
                CreatureInfo temp = i->info;
                i->info = j->info;
                j->info = temp;
            }
        }
    }
}

void get_string_dynm(char **p) 
{
    char buffer[MAX_STR_LENGTH];
    int i = 0;
    char c;  
    while ((c = getchar()) != '\n' && i < sizeof(buffer) - 1) 
        buffer[i++] = c;
    buffer[i] = '\0'; 

    *p = (char *)malloc((i + 1) * sizeof(char)); 
    if (*p != NULL) 
        strcpy(*p, buffer);
}

void get_creature_dynm(Creature *newCreature)
{
    printf("Enter class: ");
    get_string_dynm(&newCreature->info.class);
    printf("Enter order: ");
    get_string_dynm(&newCreature->info.order);
    printf("Enter family: ");
    get_string_dynm(&newCreature->info.family);
    printf("Enter genus: ");
    get_string_dynm(&newCreature->info.genus);
    printf("Enter species: ");
    get_string_dynm(&newCreature->info.species);
    newCreature->next = NULL;
}

Creature *add_creature(Creature **head)
{
    Creature *newCreature = (Creature *)malloc(sizeof(Creature));
    if (newCreature == NULL)
    {
        printf("Error allocating memory.\n");
        return NULL;
    }
    get_creature_dynm(newCreature);
    newCreature->next = *head;
    *head = newCreature;
    return newCreature;
}

Creature *delete_creature(Creature **head, const char *species)
{
    if (*head == NULL)
    {
        printf("The list is empty.\n");
        return NULL;
    }
    Creature *current = *head;
    Creature *previous = NULL;

    while (current != NULL)
    {
        if (strcmp(current->info.species, species) == 0)
        {
            if (previous == NULL)   /*If the node to be deleted is head node*/
                *head = current->next;
            else
                previous->next = current->next; /*connect the previous node to the next node of current, skip the node between*/
            
            free_creature(current);
            return *head;
        }
        previous = current;
        current = current->next;
    }
    printf("No such creature.\n");
    return NULL;
}

void get_and_update_field(char **field, char *prompt)
{
    char buffer[MAX_STR_LENGTH];
    printf("%s", prompt);
    fgets(buffer, MAX_STR_LENGTH, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0)
    {
        free((*field));
        (*field) = (char *)malloc(sizeof(strlen(buffer + 1)));
        if ((*field) != NULL)
            strcpy((*field), buffer);
    }
}

int modify_creature(Creature *head, const char *species)
{
    if (head == NULL)
    {
        printf("The list is empty.\n");
        return 0;
    }
    Creature *temp = head;
    while (temp != NULL)
    {
        if (strcmp(temp->info.species, species) == 0)
        {
            printf("Enter new information: \n");
            get_and_update_field(&temp->info.class, "New Class: ");
            get_and_update_field(&temp->info.order, "New Order: ");
            get_and_update_field(&temp->info.family, "New Family: ");
            get_and_update_field(&temp->info.genus, "New Genus: ");
            get_and_update_field(&temp->info.species, "New Species: ");
            return 1;
        }
        temp = temp->next;
    }   
    printf("No such species named \"%s\"\n", species);
    return 0;
}

void print_list(Creature *head) 
{
    Creature *current;
    for (current = head; current != NULL; current = current->next) 
    {
        printf("Class: %-10s, Order: %-10s, Family: %-10s, Genus: %-10s, Species: %-10s\n",
               current->info.class, current->info.order, current->info.family,
               current->info.genus, current->info.species);
    }
}

void free_list(Creature *head) 
{
    Creature *current;
    while (head != NULL) 
    {
        current = head;
        head = head->next;
        free_creature(current);
    }
}

void free_creature(Creature *creature)
{
    if (creature != NULL)
    {
        free(creature->info.class);
        free(creature->info.order);
        free(creature->info.family);
        free(creature->info.genus);
        free(creature->info.species);
        free(creature);
    }
}

int main()
{
    Creature *head = NULL;
    int choice;
    char species[MAX_STR_LENGTH];
    InfoType sort_type;

    while (1) 
    {
        printf("1. Add Creature\n");
        printf("2. Delete Creature\n");
        printf("3. Modify Creature\n");
        printf("4. Print List\n");
        printf("5. Sort List\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%i", &choice);
        getchar();

        switch (choice) 
        {
            case 1:
                add_creature(&head);
                break;
            case 2:
                printf("Enter species to delete: ");
                fgets(species, sizeof(species), stdin);
                species[strcspn(species, "\n")] = '\0';
                delete_creature(&head, species);
                break;
            case 3:
                printf("Enter species to modify: \n");
                printf("Old species: ");
                fgets(species, sizeof(species), stdin);
                species[strcspn(species, "\n")] = '\0';
                if(modify_creature(head, species))
                {
                    printf("Species modified succesfully.\n");
                    print_list(head);
                }    
                break;
            case 4:
                print_list(head);
                break;
            case 5:
                printf("Sort by: (Class, Order, Family, Genus, Species) : ");
                char bywhat[MAX_STR_LENGTH];
                fgets(bywhat, sizeof(bywhat), stdin);
                bywhat[strcspn(bywhat, "\n")] = '\0';
                if      (strcmp(bywhat, "Class") == 0)   sort_type = CLASS;  
                else if (strcmp(bywhat, "Order") == 0)   sort_type = ORDER;
                else if (strcmp(bywhat, "Family") == 0)  sort_type = FAMILY;
                else if (strcmp(bywhat, "Genus") == 0)   sort_type = GENUS;
                else if (strcmp(bywhat, "Species") == 0) sort_type = SPECIES;
                else                                     
                {
                    printf("Invalid criteria. Enter again.\n");
                    break;
                }
                sort_list(&head, sort_type);
                break;
            case 6:
                free_list(head);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } 
}
