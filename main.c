#include <stdio.h>
#include <string.h>

#define MAX_BOOKS 100
#define DATA_FILE "books.dat"

struct Book {
    int id;
    char title[100];
    char author[100];
    int total_copies;
    int available_copies;
};

// Save the entire array of books to a binary file
void saveBooks(struct Book library[], int count) {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (fp == NULL) {
        printf("Warning: Could not save data to file!\n");
        return;
    }
    fwrite(&count, sizeof(int), 1, fp);
    fwrite(library, sizeof(struct Book), count, fp);
    fclose(fp);
}

// Load saved books from the binary file at startup
void loadBooks(struct Book library[], int *count) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        *count = 0; // First time running, no file exists yet
        return;
    }
    fread(count, sizeof(int), 1, fp);
    fread(library, sizeof(struct Book), *count, fp);
    fclose(fp);
}

void addBook(struct Book library[], int *count) {
    if (*count >= MAX_BOOKS) {
        printf("Library is full! Cannot add more books.\n");
        return;
    }

    struct Book newBook;
    printf("Enter Book ID: ");
    scanf("%d", &newBook.id);
    getchar();

    for (int i = 0; i < *count; i++) {
        if (library[i].id == newBook.id) {
            printf("Error: A book with ID %d already exists!\n", newBook.id);
            return;
        }
    }

    printf("Enter Title: ");
    fgets(newBook.title, 100, stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0;

    printf("Enter Author: ");
    fgets(newBook.author, 100, stdin);
    newBook.author[strcspn(newBook.author, "\n")] = 0;

    printf("Enter Total Copies: ");
    scanf("%d", &newBook.total_copies);
    newBook.available_copies = newBook.total_copies;

    library[*count] = newBook;
    (*count)++;
    saveBooks(library, *count); // Save immediately
    printf("Book added and saved!\n");
}

void viewBooks(struct Book library[], int count) {
    if (count == 0) {
        printf("\nNo books in the library yet!\n");
        return;
    }

    printf("\n%-6s %-25s %-20s %-10s\n", "ID", "Title", "Author", "Available");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-6d %-25s %-20s %d/%d\n",
               library[i].id, library[i].title, library[i].author,
               library[i].available_copies, library[i].total_copies);
    }
}

void searchBook(struct Book library[], int count) {
    if (count == 0) {
        printf("\nNo books to search!\n");
        return;
    }

    int searchId;
    printf("Enter Book ID to search: ");
    scanf("%d", &searchId);

    for (int i = 0; i < count; i++) {
        if (library[i].id == searchId) {
            printf("\nBook Found!\n");
            printf("ID: %d | Title: %s | Author: %s | Available: %d/%d\n",
                   library[i].id, library[i].title, library[i].author,
                   library[i].available_copies, library[i].total_copies);
            return;
        }
    }
    printf("Book with ID %d not found.\n", searchId);
}

void issueBook(struct Book library[], int count) {
    int id;
    printf("Enter Book ID to issue: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (library[i].id == id) {
            if (library[i].available_copies > 0) {
                library[i].available_copies--;
                saveBooks(library, count); // Save updated count
                printf("Issued '%s'! Remaining copies: %d/%d\n",
                       library[i].title, library[i].available_copies, library[i].total_copies);
            } else {
                printf("Sorry, '%s' is currently out of stock (0 available).\n", library[i].title);
            }
            return;
        }
    }
    printf("Book with ID %d not found.\n", id);
}

void returnBook(struct Book library[], int count) {
    int id;
    printf("Enter Book ID to return: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (library[i].id == id) {
            if (library[i].available_copies < library[i].total_copies) {
                library[i].available_copies++;
                saveBooks(library, count); // Save updated count
                printf("Returned '%s'! Available copies: %d/%d\n",
                       library[i].title, library[i].available_copies, library[i].total_copies);
            } else {
                printf("Error: All copies of '%s' are already in the library!\n", library[i].title);
            }
            return;
        }
    }
    printf("Book with ID %d not found.\n", id);
}

int main() {
    struct Book library[MAX_BOOKS];
    int count = 0;
    int choice;

    loadBooks(library, &count); // Load saved records on startup

    while (1) {
        printf("\n=== Library Management System ===\n");
        printf("1. Add a Book\n");
        printf("2. View All Books\n");
        printf("3. Search Book by ID\n");
        printf("4. Issue a Book\n");
        printf("5. Return a Book\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(library, &count); break;
            case 2: viewBooks(library, count); break;
            case 3: searchBook(library, count); break;
            case 4: issueBook(library, count); break;
            case 5: returnBook(library, count); break;
            case 0: printf("Exiting... Goodbye!\n"); return 0;
            default: printf("Invalid choice, try again.\n");
        }
    }
}