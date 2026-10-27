/*
   Program    : KitRoom - Equipment Stock and Lending Checker (version 0.1)
   Unit       : STU 1201 Introduction to Programming
   Assessment : CAT 1 - Program Implementation
   Student    : Lameck Odhiambo Otieno (Reg No: 17/00207)

   What the program does:
   This program implements the KitRoom design from Assignment 1. The
   equipment officer of a student sports club enters the stock of ONE item,
   then records ONE transaction (issue or return). The program updates the
   available quantity, works out how many items are still out
   (out = total - available), shows the percentage still in the store, and
   warns when the stock is low.

   Items:    F = Football    C = Cone set    B = Bibs set
   Actions:  1 = Issue equipment    2 = Return equipment

   Work in progress: this version only uses what has been covered in class
   so far. It handles one transaction per run, and nothing is saved when the
   program ends. More features will be added as the syllabus advances
   (see the roadmap in README.md).

   Project page: https://github.com/lameck-otieno/kitroom
   Roadmap:      https://github.com/lameck-otieno/kitroom#roadmap

   C concepts used:
   - Lexical elements: keywords, identifiers, constants, operators, comments
   - Constants:        #define LOW_STOCK and #define MAX_STOCK
   - Data types:       int, char and float
   - Variables:        declared with a starting value
   - Operators:        arithmetic (- + * /), relational (< > <= >= == !=),
                       logical (|| &&), assignment (=) and a typecast (float)
   - Input/output:     printf() and scanf()
   - Selection:        if, else if, else and nested if
   - Program exit:     return 0 (normal end) and return 1 (stopped after an
                       error)
*/

#include <stdio.h>

/* Constants */
#define LOW_STOCK 2        /* warn when available is this number or less */
#define MAX_STOCK 10000    /* biggest total allowed for one item */

int main()
{
    /* Variables. Each one starts at a value that FAILS its check below, so if
       the user types something that is not a number, the value stays the same
       and the program shows an error message. */
    char item = ' ';       /* F, C or B */
    int total = 0;         /* total quantity the club owns */
    int available = -1;    /* quantity in the store now */
    int action = 0;        /* 1 = issue, 2 = return */
    int quantity = 0;      /* quantity in this transaction */
    int out = 0;           /* quantity still out (total - available) */
    float percent = 0;     /* percentage of the stock in the store */

    printf("=== KitRoom: Equipment Stock Checker ===\n\n");

    /* ---------- Step 1: choose the item ---------- */
    printf("Item (F = Football, C = Cone set, B = Bibs set): ");
    scanf(" %c", &item);   /* the space before %c skips any leftover Enter */

    /* Selection: three valid outcomes, otherwise an error */
    if (item == 'F' || item == 'f')
    {
        printf("You chose: Football\n");
    }
    else if (item == 'C' || item == 'c')
    {
        printf("You chose: Cone set\n");
    }
    else if (item == 'B' || item == 'b')
    {
        printf("You chose: Bibs set\n");
    }
    else
    {
        printf("Error: the item must be F, C or B.\n");
        return 1;          /* stop the program because the input is wrong */
    }

    /* ---------- Step 2: enter the current stock (with validation) ---------- */
    printf("Total quantity the club owns (1 to %d): ", MAX_STOCK);
    scanf("%d", &total);

    if (total < 1 || total > MAX_STOCK)
    {
        printf("Error: the total must be from 1 to %d.\n", MAX_STOCK);
        return 1;
    }

    printf("Quantity in the store now (0 to %d): ", total);
    scanf("%d", &available);

    if (available < 0 || available > total)
    {
        printf("Error: the quantity in the store must be from 0 to %d.\n",
               total);
        return 1;
    }

    out = total - available;
    printf("Currently out: %d\n\n", out);

    /* ---------- Step 3: choose the action ---------- */
    printf("Action (1 = Issue, 2 = Return): ");
    scanf("%d", &action);

    if (action != 1 && action != 2)
    {
        printf("Error: the action must be 1 or 2.\n");
        return 1;
    }

    printf("Quantity: ");
    scanf("%d", &quantity);

    if (quantity < 1)
    {
        printf("Error: the quantity must be at least 1.\n");
        return 1;
    }

    /* ---------- Step 4: process the transaction ---------- */
    if (action == 1)
    {
        /* Issue: we cannot give out more than is in the store */
        if (quantity > available)
        {
            printf("Error: only %d in the store, so %d cannot be issued.\n",
                   available, quantity);
            return 1;
        }
        available = available - quantity;
        printf("\nEquipment issued: %d\n", quantity);
    }
    else
    {
        /* Return: we cannot take back more than is out */
        if (quantity > out)
        {
            printf("Error: only %d is out, so %d cannot be returned.\n",
                   out, quantity);
            return 1;
        }
        available = available + quantity;
        printf("\nEquipment returned: %d\n", quantity);
    }

    /* ---------- Step 5: show the updated stock ---------- */
    out = total - available;
    percent = (float) available / total * 100;   /* typecast to keep decimals */

    printf("\n--- Updated stock ---\n");
    printf("Total:     %d\n", total);
    printf("Available: %d\n", available);
    printf("Out:       %d\n", out);
    printf("In store:  %.1f%%\n", percent);

    /* Selection: three different outcomes for the stock level.
       "All back" is checked first, so a small stock with nothing out is not
       reported as low. */
    if (out == 0)
    {
        printf("All items are back in the store.\n");
    }
    else if (available <= LOW_STOCK)
    {
        printf("WARNING: LOW STOCK! Only %d left in the store.\n", available);
    }
    else
    {
        printf("Stock level is fine.\n");
    }

    return 0;
}