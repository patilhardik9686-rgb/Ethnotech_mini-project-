#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book
{
    int id;
    char title[50];
    char author[50];
    int quantity;
    int available;
};

struct Issue
{
    int bookId;
    int studentId;
    int issueDay;
};

void addBook();
void displayBooks();
void searchBook();
void issueBook();
void returnBook();
void viewIssuedBooks();

int main()
{
    int choice;

    while (1)
    {
        printf("\n====== LIBRARY MANAGEMENT SYSTEM ======\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Search Book\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. View Issued Books\n");
        printf("7. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addBook();
                break;

            case 2:
                displayBooks();
                break;

            case 3:
                searchBook();
                break;

            case 4:
                issueBook();
                break;

            case 5:
                returnBook();
                break;

            case 6:
                viewIssuedBooks();
                break;

            case 7:
                printf("Exiting Program...\n");
                exit(0);

            default:
                printf("Invalid Choice!\n");
        }
    }

    return 0;
}

void addBook()
{
    FILE *fp;
    struct Book b, temp;
    int duplicate = 0;

    fp = fopen("books.dat", "ab+");

    printf("\nEnter Book ID: ");
    scanf("%d", &b.id);

    rewind(fp);

    while (fread(&temp, sizeof(temp), 1, fp))
    {
        if (temp.id == b.id)
        {
            duplicate = 1;
            break;
        }
    }

    if (duplicate)
    {
        printf("Book ID already exists!\n");
        fclose(fp);
        return;
    }

    printf("Enter Book Title: ");
    scanf(" %[^\n]", b.title);

    printf("Enter Author Name: ");
    scanf(" %[^\n]", b.author);

    printf("Enter Quantity: ");
    scanf("%d", &b.quantity);

    b.available = b.quantity;

    fwrite(&b, sizeof(b), 1, fp);

    printf("Book Added Successfully!\n");

    fclose(fp);
}

void displayBooks()
{
    FILE *fp;
    struct Book b;

    fp = fopen("books.dat", "rb");

    if (fp == NULL)
    {
        printf("No Books Found!\n");
        return;
    }

    printf("\n===== BOOK LIST =====\n");

    while (fread(&b, sizeof(b), 1, fp))
    {
        printf("\nBook ID       : %d", b.id);
        printf("\nTitle         : %s", b.title);
        printf("\nAuthor        : %s", b.author);
        printf("\nQuantity      : %d", b.quantity);
        printf("\nAvailable     : %d\n", b.available);
    }

    fclose(fp);
}

void searchBook()
{
    FILE *fp;
    struct Book b;
    int id, found = 0;

    fp = fopen("books.dat", "rb");

    if (fp == NULL)
    {
        printf("No Books Found!\n");
        return;
    }

    printf("Enter Book ID to Search: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(b), 1, fp))
    {
        if (b.id == id)
        {
            found = 1;

            printf("\nBook Found!\n");
            printf("Book ID   : %d\n", b.id);
            printf("Title     : %s\n", b.title);
            printf("Author    : %s\n", b.author);
            printf("Quantity  : %d\n", b.quantity);
            printf("Available : %d\n", b.available);

            break;
        }
    }

    if (!found)
    {
        printf("Book Not Found!\n");
    }

    fclose(fp);
}

void issueBook()
{
    FILE *fp, *tempfp, *issuefp;
    struct Book b;
    struct Issue issue, tempIssue;

    int bookId, studentId, found = 0, duplicate = 0;

    printf("Enter Book ID: ");
    scanf("%d", &bookId);

    printf("Enter Student ID: ");
    scanf("%d", &studentId);

    issuefp = fopen("issue.dat", "ab+");

    rewind(issuefp);

    while (fread(&tempIssue, sizeof(tempIssue), 1, issuefp))
    {
        if (tempIssue.bookId == bookId &&
            tempIssue.studentId == studentId)
        {
            duplicate = 1;
            break;
        }
    }

    if (duplicate)
    {
        printf("This student already issued this book!\n");
        fclose(issuefp);
        return;
    }

    fp = fopen("books.dat", "rb");
    tempfp = fopen("temp.dat", "wb");

    while (fread(&b, sizeof(b), 1, fp))
    {
        if (b.id == bookId)
        {
            found = 1;

            if (b.available > 0)
            {
                b.available--;

                issue.bookId = bookId;
                issue.studentId = studentId;

                printf("Enter Issue Day Number: ");
                scanf("%d", &issue.issueDay);

                fwrite(&issue, sizeof(issue), 1, issuefp);

                printf("Book Issued Successfully!\n");
            }
            else
            {
                printf("No Copies Available!\n");
            }
        }

        fwrite(&b, sizeof(b), 1, tempfp);
    }

    fclose(fp);
    fclose(tempfp);
    fclose(issuefp);

    remove("books.dat");
    rename("temp.dat", "books.dat");

    if (!found)
    {
        printf("Book Not Found!\n");
    }
}

void returnBook()
{
    FILE *fp, *tempfp, *issuefp, *tempissuefp;
    struct Book b;
    struct Issue issue;

    int bookId, studentId;
    int returnDay;
    int found = 0;

    printf("Enter Book ID: ");
    scanf("%d", &bookId);

    printf("Enter Student ID: ");
    scanf("%d", &studentId);

    issuefp = fopen("issue.dat", "rb");
    tempissuefp = fopen("tempissue.dat", "wb");

    while (fread(&issue, sizeof(issue), 1, issuefp))
    {
        if (issue.bookId == bookId &&
            issue.studentId == studentId)
        {
            found = 1;

            printf("Enter Return Day Number: ");
            scanf("%d", &returnDay);

            int days = returnDay - issue.issueDay;
            int fine = 0;

            if (days > 7)
            {
                fine = (days - 7) * 5;
            }

            printf("Fine Amount = Rs. %d\n", fine);
        }
        else
        {
            fwrite(&issue, sizeof(issue), 1, tempissuefp);
        }
    }

    fclose(issuefp);
    fclose(tempissuefp);

    remove("issue.dat");
    rename("tempissue.dat", "issue.dat");

    if (!found)
    {
        printf("Issue Record Not Found!\n");
        return;
    }

    fp = fopen("books.dat", "rb");
    tempfp = fopen("temp.dat", "wb");

    while (fread(&b, sizeof(b), 1, fp))
    {
        if (b.id == bookId)
        {
            b.available++;
        }

        fwrite(&b, sizeof(b), 1, tempfp);
    }

    fclose(fp);
    fclose(tempfp);

    remove("books.dat");
    rename("temp.dat", "books.dat");

    printf("Book Returned Successfully!\n");
}

void viewIssuedBooks()
{
    FILE *fp;
    struct Issue issue;

    fp = fopen("issue.dat", "rb");

    if (fp == NULL)
    {
        printf("No Issued Books Found!\n");
        return;
    }

    printf("\n===== ISSUED BOOKS =====\n");

    while (fread(&issue, sizeof(issue), 1, fp))
    {
        printf("\nBook ID      : %d", issue.bookId);
        printf("\nStudent ID   : %d", issue.studentId);
        printf("\nIssue Day    : %d\n", issue.issueDay);
    }

    fclose(fp);
}
