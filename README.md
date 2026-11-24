# Library Management System (C)

A command-line Library Management System built in C featuring inventory tracking and persistent binary file storage.

## Features
- **Add Books:** Store book ID, title, author, and copy count with duplicate ID validation.
- **View Inventory:** Display all books and live availability (`available/total`) in a formatted table.
- **Search by ID:** Quickly look up book details using its unique ID.
- **Issue & Return:** Automatically updates available stock with boundary checks.
- **Persistent Storage:** Automatically saves and loads records using a local binary file (`books.dat`).

## How to Compile and Run

```bash
gcc main.c -o library
./library