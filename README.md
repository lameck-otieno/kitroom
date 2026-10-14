# KitRoom

**An equipment stock and lending checker for a student sports club, written in C.**

> **Status: work in progress (version 0.1).**
> This project is being built in stages as the STU 1201 syllabus advances. Version 0.1 uses only the C features covered in class so far, so it is deliberately small. More advanced features will be added as each topic is taught. See the [Roadmap](#roadmap) for the plan.

KitRoom helps an equipment officer keep track of shared sports equipment (footballs, cone sets and bib sets). In version 0.1 you enter the stock of one item, record one transaction (issue or return), and the program updates the available quantity, shows how many items are still out, and warns you when stock is low.

This project is for **STU 1201: Introduction to Programming** (CAT 1: Program Implementation) at KCA University. It implements the core processing part of the KitRoom concept from Assignment 1, which was inspired by how equipment is tracked in a notebook at a student sports club.

## Contents

- [What it does now](#what-it-does-now)
- [Requirements](#requirements)
- [Getting started](#getting-started)
- [How to use it](#how-to-use-it)
- [Input checks (validation)](#input-checks-validation)
- [Trying it out](#trying-it-out)
- [Configuration](#configuration)
- [Project structure](#project-structure)
- [Known limitations](#known-limitations)
- [Roadmap](#roadmap)
- [Author](#author)

## What it does now

- Lets you choose one of three items: **Football**, **Cone set** or **Bibs set**.
- Asks for the total quantity the club owns and how many are in the store now.
- **Issues** equipment (subtracts from the available quantity) or **returns** equipment (adds back).
- Calculates how many items are out: `out = total - available`.
- Shows the percentage of the stock that is in the store.
- Shows one of three messages about the stock level: a **LOW STOCK** warning, "all items are back", or "stock level is fine".
- Checks the input and shows a clear error message when something is wrong, including trying to issue more than is available or return more than is out.

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
gcc -Wall -Wextra kitroom.c -o kitroom
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
gcc -Wall -Wextra kitroom.c -o kitroom
./kitroom
```

Replace `YourName` with your Windows username. Working inside the Linux file system (`~`) is faster than working in `/mnt/c`.

## How to use it

The program asks its questions in this order:

1. **Item:** `F` (Football), `C` (Cone set) or `B` (Bibs set). Capital or small letters both work.
2. **Total quantity** the club owns, from 1 to 10000.
3. **Quantity in the store now**, from 0 up to the total.
4. **Action:** `1` to issue or `2` to return.
5. **Quantity** for this transaction.

Then it shows the updated stock. Each run handles **one transaction**. To record another one, run the program again.

### Example

```
=== KitRoom: Equipment Stock Checker ===

Item (F = Football, C = Cone set, B = Bibs set): C
You chose: Cone set
Total quantity the club owns (1 to 10000): 5
Quantity in the store now (0 to 5): 5
Currently out: 0

Action (1 = Issue, 2 = Return): 1
Quantity: 3

Equipment issued: 3

--- Updated stock ---
Total:     5
Available: 2
Out:       3
In store:  40.0%
WARNING: LOW STOCK! Only 2 left in the store.
```

## Input checks (validation)

If an input is wrong, the program shows an error message and ends. Run it again to try again.

| Situation | What KitRoom does |
| --- | --- |
| Item is not F, C or B | "Error: the item must be F, C or B." |
| Total is below 1 or above 10000 | "Error: the total must be from 1 to 10000." |
| Quantity in the store is negative or more than the total | "Error: the quantity in the store must be from 0 to ..." |
| Action is not 1 or 2 | "Error: the action must be 1 or 2." |
| Quantity is less than 1 | "Error: the quantity must be at least 1." |
| Issuing more than is in the store | "Error: only 4 in the store, so 6 cannot be issued." |
| Returning more than is out | "Error: only 2 is out, so 5 cannot be returned." |
| Letters typed instead of a number | The number is not accepted, so one of the errors above is shown |

## Trying it out

Run `./kitroom` and enter the values below, one at a time, pressing Enter after each. Each row is a separate run of the program.

| # | What to enter (in order) | What you should see |
| --- | --- | --- |
| 1 | `F`, `10`, `10`, `1`, `3` | "Equipment issued: 3". Available 7, Out 3, In store 70.0%. "Stock level is fine." |
| 2 | `C`, `5`, `5`, `1`, `3` | Available 2, Out 3. "WARNING: LOW STOCK! Only 2 left in the store." |
| 3 | `B`, `4`, `1`, `2`, `3` | "Equipment returned: 3". Available 4, Out 0. "All items are back in the store." |
| 4 | `F`, `10`, `4`, `1`, `6` | "Error: only 4 in the store, so 6 cannot be issued." |
| 5 | `F`, `10`, `8`, `2`, `5` | "Error: only 2 is out, so 5 cannot be returned." |
| 6 | `X` | "Error: the item must be F, C or B." |
| 7 | `F`, `0` | "Error: the total must be from 1 to 10000." |
| 8 | `F`, `10`, `10`, `3` | "Error: the action must be 1 or 2." |
| 9 | `F`, `10`, `10`, `1`, `0` | "Error: the quantity must be at least 1." |

Rows 1 to 3 show the three different stock-level messages. Rows 4 to 9 show the input checks.

## Configuration

These constants are at the top of `kitroom.c`. Change them and compile again.

| Constant | Default | Meaning |
| --- | --- | --- |
| `LOW_STOCK` | `2` | A warning appears when the available quantity is this number or less |
| `MAX_STOCK` | `10000` | The biggest total allowed for one item |

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

These are the current limits of version 0.1. Most of them are planned to be fixed in later versions.

- **One transaction per run.** There is no menu that repeats.
- **No saving.** The stock is typed in each time, and nothing is stored when the program ends.
- **One item per run.** You choose the item first, and the program does not track all three items at once.
- **No borrower names or history.** The program does not record who borrowed what.
- **Wrong input ends the program.** It does not ask again.

## Roadmap

The project grows with the syllabus. Each stage adds one new idea, and the order follows the unit's lecture topics. The plan may change if the syllabus changes.

| Version | Status | Syllabus topic | Planned features |
| --- | --- | --- | --- |
| **0.1** | Current | C program structure, lexical elements, data types, variables, constants, operators, input/output, selection | One item and one transaction per run, validation with `if...else`, low-stock warning |
| **0.2** | Planned | Program control structures (loops) | A menu that repeats until Exit, asking again after invalid input instead of ending, several transactions per run |
| **0.3** | Planned | Secondary data types (arrays and strings) | All three items tracked at once with arrays, borrower names stored as text |
| **0.4** | Planned | Functions | Code split into functions (issue, return, show stock, input checking) |
| **0.5** | Planned | File handling | Stock and borrowing history saved to text files and loaded when the program starts, plus a "view history" option |

The final goal is the full KitRoom design from Assignment 1: a menu-driven program with saved stock, a borrowing history, and checks against over-issuing and over-returning.

## Author

Lameck Odhiambo Otieno, Registration No. 17/00207.
STU 1201: Introduction to Programming, KCA University, September-December 2026.