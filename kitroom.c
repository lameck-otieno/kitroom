/*
   Program    : KitRoom - Equipment Stock and Lending Checker (version 0.4)
   Unit       : STU 1201 Introduction to Programming
   Assessment : Assignment II - Multiple Records and Modular Programming
   Student    : Lameck Odhiambo Otieno (Reg No: 17/00207)

   What the program does:
   KitRoom extends the CAT I program. The equipment officer of a student
   sports club keeps several equipment records (up to MAX_ITEMS) in an array
   of structures. A repeating menu lets the officer add an item, display all
   items, search for an item by code, issue equipment, return equipment and
   show a summary. Records are kept only while the program is running, so
   nothing is saved when the program ends.

   Each record holds a one-letter code, a name (spaces allowed), the total
   quantity owned and the quantity available. The quantity out
   (out = total - available) and the percentage in the store are calculated
   when they are needed and are never stored.

   New since CAT I:
   - Structure and array of structures (struct Item, items[MAX_ITEMS])
   - User-defined functions with parameters and return values
   - A repeating menu (while loop) with if...else if selection
   - Input checking that asks again instead of stopping the program

   Project page: https://github.com/lameck-otieno/kitroom
   Roadmap:      https://github.com/lameck-otieno/kitroom#roadmap
*/

#include <stdio.h>

/* Constants */
#define MAX_ITEMS 10       /* most records the array can hold */
#define NAME_LEN 20        /* name size, including the end-of-string character */
#define MAX_STOCK 10000    /* biggest total allowed for one item */
#define LOW_STOCK 2        /* warn when available is this number or less */

/* One equipment record */
struct Item
{
    char code;
    char name[NAME_LEN];
    int total;
    int available;
};

/* Function prototypes */
int addItem(struct Item items[], int count);
void displayItems(struct Item items[], int count);
int calcOut(struct Item it);
void clearInput(void);
int readInt(char prompt[], int min, int max);
char readCode(char prompt[]);
void readName(char prompt[], char name[]);
int findItem(struct Item items[], int count, char code);
float calcPercent(struct Item it);
void searchItem(struct Item items[], int count);
void issueItem(struct Item items[], int count);
void returnItem(struct Item items[], int count);
void showSummary(struct Item items[], int count);



int main()
{
    struct Item items[MAX_ITEMS];
    int count = 0;       /* how many records are stored */
    int choice = -1;     /* menu choice; -1 so the loop starts */

    while (choice != 0)
    {
        printf("\n=== KitRoom v0.4: Equipment Stock Checker ===\n");
        printf("1. Add an item\n");
        printf("2. Display all items\n");
        printf("3. Search for an item\n");
        printf("4. Issue equipment\n");
        printf("5. Return equipment\n");
        printf("6. Show summary\n");
        printf("0. Exit\n");
        choice = readInt("Choice", 0, 6);   

        if (choice == 1)
        {
            count = addItem(items, count);
        }
        else if (choice == 2)
        {
            displayItems(items, count);
        }
        else if (choice == 3)
        {
            searchItem(items, count);
        }
        else if (choice == 4)
        {
            issueItem(items, count);
        }
        else if (choice == 5)
        {
            returnItem(items, count);
        }
        else if (choice == 6)
        {
            showSummary(items, count);
        }
        else if (choice == 0)
        {
            printf("Goodbye.\n");
        }
    }

    return 0;
}

/* Totals for all records, and a list of low-stock items. */
void showSummary(struct Item items[], int count)
{
    int i;
    int totalAll = 0;
    int availableAll = 0;
    int lowCount = 0;

    if (count == 0)
    {
        printf("No items recorded yet.\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        totalAll = totalAll + items[i].total;
        availableAll = availableAll + items[i].available;

        if (calcOut(items[i]) > 0 && items[i].available <= LOW_STOCK)
        {
            printf("LOW STOCK: %s (%d available)\n",
                   items[i].name, items[i].available);
            lowCount++;
        }
    }

    printf("Item types: %d\n", count);
    printf("Units owned: %d\n", totalAll);
    printf("Units in store: %d\n", availableAll);
    printf("Units out: %d\n", totalAll - availableAll);
    printf("Low-stock items: %d\n", lowCount);
}


/* Take quantity out of the store (never more than is available). */
void issueItem(struct Item items[], int count)
{
    char code;
    int pos;
    int quantity;

    if (count == 0)
    {
        printf("No items recorded yet.\n");
        return;
    }

    code = readCode("Issue code");
    pos = findItem(items, count, code);

    if (pos == -1)
    {
        printf("No item with code %c was found.\n", code);
        return;
    }

    if (items[pos].available < 1)
    {
        printf("Error: no units of %s are available to issue.\n",
               items[pos].name);
        return;
    }

    quantity = readInt("Quantity to issue", 1, items[pos].available);
    items[pos].available = items[pos].available - quantity;
    printf("Equipment issued: %d\n", quantity);
}

/* Put quantity back in the store (never more than is out). */
void returnItem(struct Item items[], int count)
{
    char code;
    int pos;
    int quantity;
    int out;

    if (count == 0)
    {
        printf("No items recorded yet.\n");
        return;
    }

    code = readCode("Return code");
    pos = findItem(items, count, code);

    if (pos == -1)
    {
        printf("No item with code %c was found.\n", code);
        return;
    }

    out = calcOut(items[pos]);

    if (out < 1)
    {
        printf("Error: no units of %s are out.\n", items[pos].name);
        return;
    }

    quantity = readInt("Quantity to return", 1, out);
    items[pos].available = items[pos].available + quantity;
    printf("Equipment returned: %d\n", quantity);
}


/* Percentage of the stock in the store (the cast avoids integer division). */
float calcPercent(struct Item it)
{
    return (float) it.available / it.total * 100;
}

/* Search by item code and show the record and its stock level. */
void searchItem(struct Item items[], int count)
{
    char code;
    int pos;
    int out;

    if (count == 0)
    {
        printf("No items recorded yet.\n");
        return;
    }

    code = readCode("Search code");
    pos = findItem(items, count, code);

    if (pos == -1)
    {
        printf("No item with code %c was found.\n", code);
        return;
    }

    out = calcOut(items[pos]);
    printf("%s: %d of %d in store, %d out, %.1f%% in store\n",
           items[pos].name, items[pos].available, items[pos].total,
           out, calcPercent(items[pos]));

    if (out == 0)
    {
        printf("All items are back in the store.\n");
    }
    else if (items[pos].available <= LOW_STOCK)
    {
        printf("WARNING: LOW STOCK! Only %d left in the store.\n",
               items[pos].available);
    }
    else
    {
        printf("Stock level is fine.\n");
    }
}


/* Throw away the rest of the current input line. */
void clearInput(void)
{
    int ch;

    ch = getchar();
    while (ch != '\n' && ch != EOF)
    {
        ch = getchar();
    }
}

/* Ask until the user types a whole number from min to max. */
int readInt(char prompt[], int min, int max)
{
    int value = 0;
    int ok = 0;

    while (ok == 0)
    {
        printf("%s (%d to %d): ", prompt, min, max);

        if (scanf("%d", &value) != 1)
        {
            printf("Error: please type a number.\n");
            clearInput();
        }
        else if (value < min || value > max)
        {
            printf("Error: the value must be from %d to %d.\n", min, max);
            clearInput();
        }
        else
        {
            clearInput();
            ok = 1;
        }
    }

    return value;
}

/* Ask until the user types a letter; return it as a capital letter. */
char readCode(char prompt[])
{
    char code = ' ';

    while (code < 'A' || code > 'Z')
    {
        printf("%s: ", prompt);
        scanf(" %c", &code);
        clearInput();

        if (code >= 'a' && code <= 'z')
        {
            code = code - 'a' + 'A';     /* small letter to capital */
        }

        if (code < 'A' || code > 'Z')
        {
            printf("Error: the code must be a letter.\n");
        }
    }

    return code;
}

/* Read a name that may contain spaces (at most NAME_LEN - 1 characters). */
void readName(char prompt[], char name[])
{
    printf("%s: ", prompt);
    scanf(" %19[^\n]", name);      /* 19 = NAME_LEN - 1 */
    clearInput();
}

/* Return the position of the item with this code, or -1 if not found. */
int findItem(struct Item items[], int count, char code)
{
    int i;

    for (i = 0; i < count; i++)
    {
        if (items[i].code == code)
        {
            return i;
        }
    }

    return -1;
}


/* Add one new record. Returns the new number of records. */
int addItem(struct Item items[], int count)
{
    char code;

    if (count >= MAX_ITEMS)
    {
        printf("Error: the record list is full.\n");
        return count;
    }

    code = readCode("Item code");

    if (findItem(items, count, code) != -1)
    {
        printf("Error: that item code already exists.\n");
        return count;
    }

    items[count].code = code;
    readName("Item name", items[count].name);
    items[count].total = readInt("Total quantity", 1, MAX_STOCK);
    items[count].available =
        readInt("Quantity in the store", 0, items[count].total);

    printf("Item added.\n");
    return count + 1;
}


/* Show every record in a table. */
void displayItems(struct Item items[], int count)
{
    int i;

    if (count == 0)
    {
        printf("No items recorded yet.\n");
        return;
    }

    printf("\n%-5s %-19s %5s  %9s  %3s\n",
           "Code", "Name", "Total", "Available", "Out");

    for (i = 0; i < count; i++)
    {
        printf("%-5c %-19s %5d  %9d  %3d\n",
               items[i].code, items[i].name,
               items[i].total, items[i].available,
               calcOut(items[i]));
    }
}

/* Units currently out = total - available. */
int calcOut(struct Item it)
{
    return it.total - it.available;
}