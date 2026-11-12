/*
   Program    : KitRoom - Equipment Stock and Lending Checker (version 0.5)
   Unit       : STU 1201 Introduction to Programming
   Assessment : Assignment III - File Handling and System Integration
   Student    : Lameck Odhiambo Otieno (Reg No: 17/00207)

   What the program does:
   KitRoom extends the CAT I program. The equipment officer of a student
   sports club keeps several equipment records (up to MAX_ITEMS) in an array
   of structures. A repeating menu lets the officer add an item, display all
   items, search for an item by code, issue equipment, return equipment and
   show a summary.

   NEW in version 0.5: the records can be saved to a text file
   (kitroom_data.txt) and are loaded again when the program starts, so they
   are still there after the program is closed. File format: one record per
   line, fields separated by |   (example line:  C|Cone set|20|2)

   Each record holds a one-letter code, a name (spaces allowed), the total
   quantity owned and the quantity available. The quantity out
   (out = total - available) and the percentage in the store are calculated
   when they are needed and are never stored.

   Project page: https://github.com/lameck-otieno/kitroom
   Roadmap:      https://github.com/lameck-otieno/kitroom#roadmap
*/

#include <stdio.h>

/* Constants */
#define MAX_ITEMS 10       /* most records the array can hold */
#define NAME_LEN 20        /* name size, including the end-of-string character */
#define MAX_STOCK 10000    /* biggest total allowed for one item */
#define LOW_STOCK 2        /* warn when available is this number or less */
#define DATA_FILE "kitroom_data.txt"   /* the permanent storage file */

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
int issueItem(struct Item items[], int count);
int returnItem(struct Item items[], int count);
void showSummary(struct Item items[], int count);
int saveItems(struct Item items[], int count);
int loadItems(struct Item items[]);
void showFile(void);



int main()
{
    struct Item items[MAX_ITEMS];
    int count;           /* how many records are stored */
    int oldCount;        /* count before an add, to see if it changed */
    int loaded;          /* result of loading from the file */
    int dirty = 0;       /* 1 = memory has changes that are not in the file */
    int choice = -1;     /* menu choice; -1 so the loop starts */

    /* Load the saved records from the file when the program starts.
       Memory then matches the file, so dirty stays 0. */
    count = loadItems(items);
    if (count == -1)
    {
        printf("Starting with an empty list.\n");
        count = 0;
    }

    while (choice != 0)
    {
        printf("\n=== KitRoom v0.5: Equipment Stock Checker ===\n");
        printf("1. Add an item\n");
        printf("2. Display all items\n");
        printf("3. Search for an item\n");
        printf("4. Issue equipment\n");
        printf("5. Return equipment\n");
        printf("6. Show summary\n");
        printf("7. Save records to file\n");
        printf("8. Load records from file\n");
        printf("9. Show file contents\n");
        printf("0. Exit\n");
        choice = readInt("Choice", 0, 9);

        if (choice == 1)
        {
            oldCount = count;
            count = addItem(items, count);
            if (count != oldCount)
            {
                dirty = 1;               /* a record was really added */
            }
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
            if (issueItem(items, count) == 1)
            {
                dirty = 1;               /* stock was really issued */
            }
        }
        else if (choice == 5)
        {
            if (returnItem(items, count) == 1)
            {
                dirty = 1;               /* stock was really returned */
            }
        }
        else if (choice == 6)
        {
            showSummary(items, count);
        }
        else if (choice == 7)
        {
            if (saveItems(items, count) == 1)
            {
                dirty = 0;               /* file now matches memory */
            }
        }
        else if (choice == 8)
        {
            loaded = loadItems(items);
            if (loaded != -1)
            {
                count = loaded;
                dirty = 0;               /* memory was just copied from the file */
            }
        }
        else if (choice == 9)
        {
            showFile();
        }
        else if (choice == 0)
        {
            /* Ask only if there are changes that would be lost. */
            if (dirty == 1)
            {
                printf("You have unsaved changes.\n");
                if (readInt("Save before exiting? 1 = yes, 0 = no", 0, 1) == 1)
                {
                    if (saveItems(items, count) == 1)
                    {
                        dirty = 0;
                    }
                    else
                    {
                        /* Saving failed: do not close and lose the changes. */
                        printf("WARNING: your changes were NOT saved, so KitRoom has not closed.\n");
                        printf("Fix the problem and choose 7 to save, or choose 0 and answer 0 to exit without saving.\n");
                        choice = -1;     /* stay in the menu */
                    }
                }
            }
            if (choice == 0)
            {
                printf("Goodbye.\n");
            }
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

/* Take quantity out of the store (never more than is available).
   Returns 1 if stock was issued, 0 if nothing changed. */
int issueItem(struct Item items[], int count)
{
    char code;
    int pos;
    int quantity;

    if (count == 0)
    {
        printf("No items recorded yet.\n");
        return 0;
    }
    code = readCode("Issue code");
    pos = findItem(items, count, code);
    if (pos == -1)
    {
        printf("No item with code %c was found.\n", code);
        return 0;
    }
    if (items[pos].available < 1)
    {
        printf("Error: no units of %s are available to issue.\n",
               items[pos].name);
        return 0;
    }
    quantity = readInt("Quantity to issue", 1, items[pos].available);
    items[pos].available = items[pos].available - quantity;
    printf("Equipment issued: %d\n", quantity);
    return 1;
}

/* Put quantity back in the store (never more than is out).
   Returns 1 if stock was returned, 0 if nothing changed. */
int returnItem(struct Item items[], int count)
{
    char code;
    int pos;
    int quantity;
    int out;

    if (count == 0)
    {
        printf("No items recorded yet.\n");
        return 0;
    }
    code = readCode("Return code");
    pos = findItem(items, count, code);
    if (pos == -1)
    {
        printf("No item with code %c was found.\n", code);
        return 0;
    }
    out = calcOut(items[pos]);
    if (out < 1)
    {
        printf("Error: no units of %s are out.\n", items[pos].name);
        return 0;
    }
    quantity = readInt("Quantity to return", 1, out);
    items[pos].available = items[pos].available + quantity;
    printf("Equipment returned: %d\n", quantity);
    return 1;
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

/* Save every record to the data file, one line per record.
   fopen mode "w" creates the file, or replaces its old contents.
   Returns 1 if the save worked, 0 if it failed. */
int saveItems(struct Item items[], int count)
{
    FILE *fp;
    int i;

    fp = fopen(DATA_FILE, "w");
    if (fp == NULL)
    {
        printf("Error: could not open %s for writing.\n", DATA_FILE);
        return 0;
    }
    for (i = 0; i < count; i++)
    {
        fprintf(fp, "%c|%s|%d|%d\n",
                items[i].code, items[i].name,
                items[i].total, items[i].available);
    }
    /* fclose returns 0 on success; anything else means the data may not
       have reached the disk (for example, the disk is full). */
    if (fclose(fp) != 0)
    {
        printf("Error: could not finish writing %s.\n", DATA_FILE);
        return 0;
    }
    printf("Saved %d record(s) to %s.\n", count, DATA_FILE);
    return 1;
}

/* Read the records from the data file into the array (fopen mode "r").
   Returns the number of records loaded, or -1 if the file could not be
   opened (the records already in the array are not touched then). */
int loadItems(struct Item items[])
{
    FILE *fp;
    int count = 0;

    fp = fopen(DATA_FILE, "r");
    if (fp == NULL)
    {
        printf("Error: could not open %s for reading. "
               "The file may not exist yet (it is created when you save).\n",
               DATA_FILE);
        return -1;
    }
    /* fscanf returns 4 when all four fields of one record were read */
    while (count < MAX_ITEMS &&
           fscanf(fp, " %c|%19[^|]|%d|%d",
                  &items[count].code, items[count].name,
                  &items[count].total, &items[count].available) == 4)
    {
        count++;
    }
    fclose(fp);
    printf("Loaded %d record(s) from %s.\n", count, DATA_FILE);
    return count;
}

/* Print the file exactly as it is stored on disk (fopen mode "r"). */
void showFile(void)
{
    FILE *fp;
    char buffer[100];
    int lines = 0;

    fp = fopen(DATA_FILE, "r");
    if (fp == NULL)
    {
        printf("Error: could not open %s for reading. "
               "The file may not exist yet (it is created when you save).\n",
               DATA_FILE);
        return;
    }
    printf("\nContents of %s (what is stored on disk):\n", DATA_FILE);
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        lines++;
        printf("%d: %s", lines, buffer);
    }
    if (lines == 0)
    {
        printf("The file is empty.\n");
    }
    fclose(fp);
}
