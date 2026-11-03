# KitRoom
 
**An equipment stock and lending checker for a student sports club, written in C.**
 
> **Status: version 0.4 (Assignment II).**
> This project is built in stages as the STU 1201 syllabus advances. Version 0.4 is a menu-driven program that keeps several equipment records in an array of structures and uses user-defined functions. Records are still kept in memory only. See the [Roadmap](#roadmap) for what comes next.
 
KitRoom helps an equipment officer keep track of shared sports equipment such as footballs, cone sets and bib sets. You add the equipment the club owns, then issue and return it as it is borrowed. The program keeps every record up to date, works out how many items are out, and warns you when stock is low.
 
This project is for **STU 1201: Introduction to Programming** at KCA University. Version 0.1 was CAT I (Program Implementation). Version 0.4 is **Assignment II: Multiple Records and Modular Programming**. Both come from the KitRoom design in Assignment 1, which was inspired by how equipment is tracked in a notebook at a student sports club.
 
## Contents
 
- [What it does now](#what-it-does-now)
- [Requirements](#requirements)
- [Getting started](#getting-started)
- [How to use it](#how-to-use-it)
- [Input checks (validation)](#input-checks-validation)
- [Trying it out](#trying-it-out)
- [Configuration](#configuration)
- [How the program is organised](#how-the-program-is-organised)
- [Project structure](#project-structure)
- [Known limitations](#known-limitations)
- [Roadmap](#roadmap)
- [Author](#author)
## What it does now
 
- Shows a **menu that repeats** until you choose Exit, so you can do as many transactions as you like in one run.
- **Adds** equipment records (up to 10). Each record has a one-letter code, a name (spaces allowed), the total quantity owned and the quantity available.
- **Displays** all records in a table with the quantity out.
- **Searches** for an item by its code and shows its stock level, including the percentage in the store.
- **Issues** equipment (subtracts from the available quantity) and **returns** equipment (adds back).
- Shows a **summary**: the number of item types, units owned, units in store, units out and a list of low-stock items.
- Calculates `out = total - available` and the percentage in store when they are needed, so these values can never disagree with the stored quantities.
- Checks every input and **asks again** instead of stopping when something is wrong.
## Requirements
 
- A C compiler such as **GCC**.
- Works on Linux, macOS and Windows with **WSL**. It only uses the standard `stdio.h` library.
## Getting started
 
### 1. Get the code
 
```bash
git clone https://github.com/lameck-otieno/kitroom.git
cd kitroom
```
 
### 2. Install a compiler (first time only)
 
On Ubuntu or WSL:
 
```bash
sudo apt update
sudo apt install build-essential
gcc --version
```
 
On macOS, run `xcode-select --install`.
 
### 3. Compile
 
```bash
gcc -Wall -Wextra -o kitroom kitroom.c
```
 
The `-Wall -Wextra` flags turn on extra compiler warnings, which help catch mistakes. If nothing is printed, it compiled with no warnings.
 
### 4. Run
 
```bash
./kitroom
```
 
### Running in WSL after downloading the file in Windows
 
Your Windows drives appear under `/mnt/c/` in WSL. For example:
 
```bash
cd /mnt/c/Users/YourName/Downloads
mkdir -p ~/kitroom
cp kitroom.c ~/kitroom/
cd ~/kitroom
gcc -Wall -Wextra -o kitroom kitroom.c
./kitroom
```
 
Replace `YourName` with your Windows username. Working inside the Linux file system (`~`) is faster than working in `/mnt/c`.
 
## How to use it
 
The program shows this menu and asks for a number from 0 to 6:
 
| Choice | What it does |
| --- | --- |
| `1` | **Add an item.** Asks for a code (one letter), a name, the total quantity and the quantity in the store. |
| `2` | **Display all items** in a table with the quantity out. |
| `3` | **Search for an item** by its code. |
| `4` | **Issue equipment.** Asks for a code and a quantity, and subtracts it from the available quantity. |
| `5` | **Return equipment.** Asks for a code and a quantity, and adds it back. |
| `6` | **Show summary** of all items. |
| `0` | **Exit.** |
 
Item codes can be capital or small letters. Small letters are changed to capitals, so `f` and `F` are the same item.
 
Records are lost when you exit, so add your items again each time you run the program.
 
### Example
 
```
=== KitRoom v0.4: Equipment Stock Checker ===
1. Add an item
2. Display all items
3. Search for an item
4. Issue equipment
5. Return equipment
6. Show summary
0. Exit
Choice (0 to 6): 1
Item code: C
Item name: Cone set
Total quantity (1 to 10000): 20
Quantity in the store (0 to 20): 2
Item added.
```
 
After adding `C` (Cone set, 20, 2), choosing `3` and searching for `C` gives:
 
```
Search code: C
Cone set: 2 of 20 in store, 18 out, 10.0% in store
WARNING: LOW STOCK! Only 2 left in the store.
```
 
Choosing `6` with a Football (10 owned, 7 in store, then 3 issued) and the Cone set gives:
 
```
LOW STOCK: Cone set (2 available)
Item types: 2
Units owned: 30
Units in store: 6
Units out: 24
Low-stock items: 1
```
 
## Input checks (validation)
 
If an input is wrong, the program shows an error message and **asks again**. Nothing is stored until the input is valid.
 
| Situation | What KitRoom does |
| --- | --- |
| Menu choice is not a number from 0 to 6 | "Error: please type a number." or "Error: the value must be from 0 to 6." and the choice is asked again |
| Item code is not a letter | "Error: the code must be a letter." and it asks again |
| Item code already exists | "Error: that item code already exists." and the record list is unchanged |
| Record list is full (10 items) | "Error: the record list is full." |
| Total is below 1 or above 10000 | "Error: the value must be from 1 to 10000." and it asks again |
| Quantity in the store is negative or more than the total | "Error: the value must be from 0 to ..." and it asks again |
| Letters typed instead of a number | "Error: please type a number." and it asks again |
| Name is longer than 19 characters | Only the first 19 characters are kept |
| Searching, issuing or returning with a code that does not exist | "No item with code ... was found." |
| Issuing when none are available | "Error: no units of ... are available to issue." |
| Issuing more than is available | "Error: the value must be from 1 to ..." and it asks again |
| Returning when none are out | "Error: no units of ... are out." |
| Returning more than is out | "Error: the value must be from 1 to ..." and it asks again |
| Searching, issuing, returning, displaying or summarising with no items | "No items recorded yet." |
 
## Trying it out
 
Start a fresh run with `./kitroom` and work down the table. Each row follows on from the one before, so the records build up. Press Enter after each value.
 
| # | What to enter | What you should see |
| --- | --- | --- |
| 1 | `1`, `F`, `Football`, `10`, `7`, then `2` | "Item added." The table shows `F  Football  10  7  3`. |
| 2 | `1`, `C`, `Cone set`, `20`, `2` | "Item added." The name keeps its space. |
| 3 | `1`, `B`, `Bibs set`, `15`, `15` | "Item added." Out is 0 for Bibs set. |
| 4 | `1`, `F` | "Error: that item code already exists." |
| 5 | `1`, `G`, `Gloves`, `0` | "Error: the value must be from 1 to 10000." It asks for the total again. Enter `10`, `10` to finish. |
| 6 | `1`, `H`, `Hats`, `5`, `9` | "Error: the value must be from 0 to 5." It asks again. Enter `5` to finish. |
| 7 | `1`, `J`, `Jerseys`, `abc` | "Error: please type a number." It asks again. Enter `8`, `8` to finish. |
| 8 | `x` at the menu | "Error: please type a number." and the menu choice is asked again. |
| 9 | `3`, `f` | "Football: 7 of 10 in store, 3 out, 70.0% in store" and "Stock level is fine." |
| 10 | `3`, `Z` | "No item with code Z was found." |
| 11 | `3`, `C` | "Cone set: 2 of 20 in store, 18 out, 10.0% in store" and "WARNING: LOW STOCK! Only 2 left in the store." |
| 12 | `4`, `F`, `7` | "Equipment issued: 7". Football now has 0 available and 10 out. |
| 13 | `5`, `F`, `11`, then `10` | 11 is rejected with "Error: the value must be from 1 to 10." and it asks again. 10 gives "Equipment returned: 10". |
| 14 | `4`, `B`, `0` | "Error: the value must be from 1 to 15." and it asks again. |
| 15 | `6` | A summary of all items, including a "LOW STOCK" line for the Cone set. |
| 16 | `0` | "Goodbye." and the program ends. |
 
Rows 5 to 7 add three extra items, so the summary in row 15 will include them. To check the summary against fixed numbers, restart the program and add only the three items from rows 1 to 3. The summary should then show 3 item types, 45 units owned, 24 in store, 21 out and 1 low-stock item (the Cone set).
 
## Configuration
 
These constants are at the top of `kitroom.c`. Change them and compile again.
 
| Constant | Default | Meaning |
| --- | --- | --- |
| `MAX_ITEMS` | `10` | The most records the array can hold |
| `NAME_LEN` | `20` | The size of the name field, including the end-of-string character (so names can have 19 characters). If you change it, also change the `19` in `readName()` to `NAME_LEN - 1`. |
| `MAX_STOCK` | `10000` | The biggest total allowed for one item |
| `LOW_STOCK` | `2` | A warning appears when the available quantity is this number or less and some items are still out |
 
## How the program is organised
 
The records are held in an array of structures, one structure for each type of equipment:
 
```c
struct Item
{
    char code;
    char name[NAME_LEN];
    int total;
    int available;
};
```
 
`main()` shows the menu and calls one function for each choice. Each function has one job:
 
| Function | Purpose |
| --- | --- |
| `addItem` | Add a new validated record and return the new count |
| `displayItems` | Show all records in a table |
| `findItem` | Return the position of an item by code, or -1 |
| `searchItem` | Ask for a code and show one record and its stock level |
| `calcOut` | Calculate the units currently out |
| `calcPercent` | Calculate the percentage in the store |
| `issueItem` | Issue available equipment |
| `returnItem` | Return equipment that is out |
| `showSummary` | Show totals and low-stock items |
| `readInt` | Read a whole number within a range, asking again if it is wrong |
| `readCode` | Read a letter and change it to a capital |
| `readName` | Read a name that may contain spaces |
| `clearInput` | Throw away the rest of the current input line |
 
## Project structure
 
```
kitroom/
├── kitroom.c       # the whole program
└── README.md       # this file
```
 
The compiled program is created when you compile, so it is best not to commit it. Add this to a `.gitignore` file:
 
```
kitroom
```
 
## Known limitations
 
These are the current limits of version 0.4. Most of them are planned to be fixed in later versions.
 
- **No saving.** The records are kept in memory only, so they are lost when the program ends.
- **No borrower names or history.** The program does not record who borrowed what or when.
- **At most 10 item types**, each with a one-letter code, so no more than 26 item types are possible even if `MAX_ITEMS` is increased.
- **Names are limited to 19 characters.** Longer names are cut.
- **No way to edit or delete a record.** To fix a mistake in a record, restart the program.
## Roadmap
 
The project grows with the syllabus. Each stage adds one new idea, and the order follows the unit's lecture topics. The plan may change if the syllabus changes.
 
| Version | Status | Syllabus topic | Features |
| --- | --- | --- | --- |
| **0.1** | Done (CAT I) | C program structure, lexical elements, data types, variables, constants, operators, input/output, selection | One item and one transaction per run, validation with `if...else`, low-stock warning |
| **0.2** | Done (in 0.4) | Program control structures (loops) | A menu that repeats until Exit, asking again after invalid input instead of ending, several transactions per run |
| **0.3** | Partly done (in 0.4) | Secondary data types (arrays and strings) | Several items tracked at once in an array of structures, with names stored as text. Borrower names are still planned. |
| **0.4** | **Current** (Assignment II) | Functions | Code split into functions (add, display, search, issue, return, summary, input checking) |
| **0.5** | Planned | File handling | Stock and borrowing history saved to text files and loaded when the program starts, plus a "view history" option |
 
Assignment II combined versions 0.2 to 0.4 into one step, so the program jumped from version 0.1 to 0.4. The remaining plan is to save the records and keep a borrowing history.
 
## Author
 
Lameck Odhiambo Otieno, Registration No. 17/00207.
STU 1201: Introduction to Programming, KCA University, September-December 2026.