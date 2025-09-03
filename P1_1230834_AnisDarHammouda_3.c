// Name : Anis Asad DarHammouda  ID: 1230834  SEC: 3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// definition for LinkedList:
typedef struct node* Node; // for make it simple when define a pointer to node
struct node {
    int id;  // Task ID
    char name[100]; //Task Name
    char date[20];  //Task Date
    float duration; //Task Duration
    int isUndone; // a flag for the status of the Task if it is Undone or just unperformed
    Node next;
};
typedef Node List;

// Linked List functions

List makeEmpty(List L);
void deleteList(List L);
int isEmpty(List L);
int isLast(Node P, List L);
Node find (int id, List L); // search for a Task by ID
Node findByName (char name[], List L);  // search for a Task by Name
Node findPrevious(int x, List L);
void delete(int x,List L);
void insert(Node X, List L, Node P);
void insertLast(List L,Node P);
void insertFirst(List L,Node P);
void printList(List L);
int size(List L);
Node findLast(List L);
void printNode(Node P); // for print the task with it is details

// definition of stack
typedef Node Stack;

// stack functions:

Stack createStack();
void makeEmptyStack(Stack s);
int isEmptyStack(Stack s);
void pop(Stack s);
Node top(Stack s);
void push(Node p,Stack s);
void DisposeStack(Stack s);
Stack copyStack(Stack s);


// general functions
void viewMenu();
void LoadFile(List L,Stack s);
void addTask(List L,Stack s);
void deleteTask(List L);
void searchTask(List L);
void performTask(List L,Stack s);
void undoLastPerformedTask(List L,Stack s);
void viewPerformedTasks(Stack s);
void generateReport(List L,Stack s);

int main(void) {
    int choice;
    List L = NULL; // a LinkedList which will fill the tasks in (Unperformed and Undone Tasks)
    L = makeEmpty(L);  // to create the List
    Stack s = NULL; // a stack to store the performed tasks
    s = createStack();
    do {
        viewMenu(); // the function which print the choices for user
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                LoadFile(L,s);
            break;
            case 2:
                 addTask(L,s);
            break;
            case 3:
                deleteTask(L);
            break;
            case 4:
                searchTask(L);
            break;
            case 5:
                printf("Unperformed Tasks:\n");
                printList(L);
                printf("\n");
            break;
            case 6:
                performTask(L,s);
                printf("\n");
                break;
            case 7:
                undoLastPerformedTask(L,s);
                printf("\n");
                break;
            case 8:
                viewPerformedTasks(s);
                printf("\n");
               break;
            case 9:
                generateReport(L,s);
                printf("\n");
                break;
            case 10:
                printf("GoodBye.!");
                deleteList(L);
                DisposeStack(s);
                exit(0);
            default:
                printf("invalid Choice\n");
        }
    }while (choice != 10);

    return 0;
}

void viewMenu() {
    printf("\n");
    printf("=======|Main Menu|=======\n");
    printf("1. Load Tasks File\n");
    printf("2. Add a New Task\n");
    printf("3. Delete a Task\n");
    printf("4. Search for a Task\n");
    printf("5. View Unperformed Tasks\n");
    printf("6. Perform a Task\n");
    printf("7. Undo Last Preformed Task\n");
    printf("8. View Preformed Task\n");
    printf("9. Generate Summary Report\n");
    printf("10. Exit\n\n");
    printf("Enter your Choice: ");
}

void LoadFile(List L,Stack s) {
    FILE* file;
    file = fopen("tasks.txt", "r");  // C:\\Users\\asus\\CLionProjects\\Proj1\\tasks.txt

    if (file == NULL) {
        printf("File does not exist\n");
        return;
    }

    printf("Loading Tasks File...\n");

    char task[200]; // to get the data from the file , and take it as lines

    Node last = (Node)malloc(sizeof(struct node));
    last = findLast(L); // to point at the last task(node) in the list to insert tasks after it

    while (fgets(task,sizeof(task), file)) {   // while we does not reach the end of file the loop will not stop
        char *token = strtok(task, "#");    // make the # is the delimiter to cutting the line

        int id = atoi(token);  // atoi is a function to convert string to integer to get the ID

       int isFound = 0;    // a flag for checking if the ID is used in performed Task, because it may Undone it so it will cause an error which more than one task will has the same ID
        Stack temp = createStack(); // temp stack to check if there is a Task with the same ID enterd
        while (!isEmptyStack(s)) {
            push(top(s),temp);
            if (top(s)->id == id) {
                isFound = 1;
            }
            pop(s);
        }

        while (!isEmptyStack(temp)) {   // to return the data to the Performed Tasks stack
            push(top(temp),s);
            pop(temp);
        }

        if (isFound) {  // check the flag of Task ID if it in the performed tasks stack
            printf("There is a Performed Task with ID %d",id);
            continue;
        }

        if (find(id,L) != NULL) { // to check if there is a Task in the Unperformed tasks list to avoid adding two tasks with same ID
            printf("There is a Task with ID %d,, please choose another ID\n",id);
            continue;
        }


        token = strtok(NULL, "#");
        char name[100];
        strcpy(name, token);  // Second token after # is for the name of task

        while (!isEmptyStack(s)) {
            push(top(s),temp);
            if (strcmp(top(s)->name,name) ==0) {
                isFound = 1;
            }
            pop(s);
        }

        while (!isEmptyStack(temp)) {   // to return the data to the Performed Tasks stack
            push(top(temp),s);
            pop(temp);
        }

        if (findByName(name,L) != NULL || isFound) {
            printf("There is a Task with name ' %s ',, please choose another name\n",name);
            continue;
        }

        token = strtok(NULL, "#");

        char date[20];
        strcpy(date, token);  // Third token for the date
        token = strtok(NULL, "#");
        float duration = atof(token);  // atof function to convert string to float (for duration)

        Node newTask = (Node)malloc(sizeof(struct node));   // create a Node to add the data entered and insert it to the Tasks list
        newTask->id = id;
        strcpy(newTask->name, name);
        strcpy(newTask->date, date);
        newTask->duration = duration;
        newTask->next = NULL;
        newTask->isUndone =0;   // the default of the task added is Undone

        if (last == NULL) {     // if the list is empty it is no matter the insert last or first because it is the first element
            insertLast(L,newTask);
            last = newTask;
            continue;
        }
       insert(newTask,L,last);
        last = newTask;
    }
    printf("Tasks loaded successfully\n\n");
    fclose(file);
}

void addTask(List L,Stack s) {
    int id;
    char name[100];
    char date[20];
    float duration;
    printf("Enter Task ID: ");
    scanf("%d",&id);

    int isFound = 0;    // a flag for checking if the ID is used in performed Task, because it may Undone it so it will cause an error which more than one task will has the same ID
    Stack temp = createStack(); // temp stack to check if there is a Task with the same ID enterd
    while (!isEmptyStack(s)) {
        push(top(s),temp);
        if (top(s)->id == id) {
            isFound = 1;
        }
        pop(s);
    }

    while (!isEmptyStack(temp)) {   // to return the data to the Performed Tasks stack
        push(top(temp),s);
        pop(temp);
    }

    if (isFound) {  // check the flag of Task ID if it in the performed tasks stack
        printf("There is a Performed Task with ID %d,, please choose another ID\n",id);
        return;
    }

    if (find(id,L) != NULL) {   // check if the ID is used in unperformed tasks list
        printf("There is a Task with ID %d,, please choose another ID\n",id);
        return;
    }

    printf("Enter Task Name: ");
    gets(name); // just for avoid the error \n
    gets(name);
    if (findByName(name,L) != NULL) {
        printf("There is a Task with name ' %s ',, please choose another name\n",name);
       return;
    }

    while (!isEmptyStack(s)) {
        push(top(s),temp);
        if (strcmp(top(s)->name,name) ==0) {
            isFound = 1;
        }
        pop(s);
    }

    while (!isEmptyStack(temp)) {   // to return the data to the Performed Tasks stack
        push(top(temp),s);
        pop(temp);
    }

    if (findByName(name,L) != NULL || isFound) {
        printf("There is a Task with name ' %s ',, please choose another name\n",name);
        return;
    }

    printf("Enter Task Date (dd/mm/year): ");
    scanf("%s",&date);
    printf("Enter Task Duration: ");
    scanf("%f",&duration);
    Node newTask = (Node)malloc(sizeof(struct node));
    newTask->id = id;
    strcpy(newTask->name, name);
    strcpy(newTask->date, date);
    newTask->duration = duration;
    newTask->next = NULL;
    newTask->isUndone = 0;
    insertLast(L,newTask);
    printNode(newTask);
    printf("Task added successfully\n\n");
}

void deleteTask(List L) {
    int id;
    printf("Enter Task ID which you want delete: ");
    scanf("%d",&id);
    if (find(id,L) == NULL) {
        printf("Task not found or had been performed\n");
        return;
    }
    delete(id,L);
    printf("Task deleted successfully\n\n");
}

void searchTask(List L) {
    if (isEmpty(L)) {
        printf("There is no tasks added yet\n");
        return;
    }

    int ch,id = 0; // ch is for the user to choice way so search
    char name[100];
    printf("which way you want to search about the task:\n1. By Task ID\n2. By Task Name\n");
    scanf("%d",&ch);
    switch (ch) {
        case 1:
            printf("Enter Task ID which want search:\n");
            scanf("%d",&id);
            if (find(id,L) == NULL) {
                printf("Task not found or it has already performed\n\n");
            }
            else {
                printf("Task found:\n");
                printNode(find(id,L));
                printf("\n");
            }
        break;
        case 2:
            printf("Enter Task Name which want search: ");
            gets(name); // for \n to avoid error
            gets(name);
            if (findByName(name,L) == NULL) {
                printf("Task not found or it has already performed\n\n");
            }
            else {
                printf("Task Found:\n");
                printNode(findByName(name,L));
                printf("\n");
            }
        break;
        default:
            printf("invalid choice for search!\n");
        break;
    }

}

void performTask(List L,Stack s) {
    if (isEmpty(L)) {
        printf("There is no tasks added yet\n");
        return;
    }
    int id;
    printf("Enter Task ID to mark as performed: ");
    scanf("%d",&id);
    Node marked = find(id,L);   // make a pointer to point at the task we want delete

    if (marked == NULL) {
        printf("Task with ID %d not found in unperformed tasks list\n\n",id);
        return;
    }

    marked->isUndone = 0;
    push(marked,s);     // add this task to performed tasks Stack "s"
    delete(marked->id,L);   // delete this task from the main list (the linked list "L")
    printNode(top(s));
    printf("Task performed successfully\n\n");

}

void undoLastPerformedTask(List L,Stack s) {
    if (isEmpty(s)) {  // if there is no top for the performed tasks stack "s" (Empty)
        printf("there is no any task had performed yet\n\n");
        return;
    }
    Node lastTask = (Node)malloc(sizeof(struct node));  // last performed task is the top of the performed tasks stack "s"
    Node topStack = top(s); // make a pointer to point at the top of the performed tasks Stack
    lastTask->id = topStack->id;
    strcpy(lastTask->name, topStack->name);
    strcpy(lastTask->date, topStack->date);
    lastTask->duration = topStack->duration;
    lastTask->isUndone = 1;
    printNode(lastTask);
    pop(s);
    insertLast(L,lastTask);
    printf("Task undone successfully\n\n");
}

void viewPerformedTasks(Stack s) {
    if (isEmptyStack(s)) {
        printf("There is no performed tasks to display\n\n");
        return;
    }

    Stack temp = createStack();
    while (!isEmptyStack(s)) {  // transfer data to a temp stack
        push(top(s),temp);
        pop(s);
    }
    while (!isEmptyStack(temp)) {
        printNode(top(temp));   // print each task from temp stack (which will begin from first task performed)
        push(top(temp),s);  // return data to the performed tasks stack as order were
        pop(temp);
    }
}

void generateReport(List L,Stack s) { // Report.txt
    FILE *output = fopen("Report.txt","w");
    Node p = L->next;
    if (p == NULL) {    //  checking if the list is empty
        fprintf(output,"There is no unperformed or undone tasks\n");
    }
    else {
        fprintf(output,"Unperformed tasks:\n");
        while (p != NULL) {     // a loop for
            fprintf(output,"Task ID : %-5d Task Name: %-25s Task date : %-10s Task duration (in hours): %-4.2f\n", p->id, p->name, p->date, p->duration);
            p = p->next;
        }
        fprintf(output,"\n");
    }
    p = L->next;
        fprintf(output,"Undone tasks:\n");
        int thereIsUndone = 0;
        while (p != NULL) {
            if (p->isUndone == 1) {
                fprintf(output,"Task ID : %-5d Task Name: %-25s Task date : %-10s Task duration (in hours): %-4.2f\n", p->id, p->name, p->date, p->duration);
                thereIsUndone = 1;
            }
            p = p->next;
        }
        if (thereIsUndone == 0) {
            fprintf(output,"There is no Undone tasks\n");
        }
        fprintf(output,"\n");
    if (isEmptyStack(s)) {
        fprintf(output,"There is no performed tasks \n\n");
    }
    else {
        fprintf(output,"Performed tasks:\n");
        Stack temp = createStack();
        while (!isEmptyStack(s)) {
            push(top(s),temp);
            pop(s);
        }
        while (!isEmptyStack(temp)) {
            Node topTemp = top(temp);
            fprintf(output,"Task ID : %-5d Task Name: %-25s Task date : %-10s Task duration (in hours): %-4.2f\n", topTemp->id, topTemp->name, topTemp->date,topTemp->duration);
            push(top(temp),s);  // return data to the performed tasks stack as order were
            pop(temp);
        }
    }
    printf("report generated successfully");
    fclose(output);
}

// LinkedList functions
List makeEmpty(List L) {
    if (L!=NULL)
        deleteList(L);
    L = (List)malloc(sizeof(struct node));

    if (L==NULL) {
        printf("Out of space\n");
        exit(1);
    }

    L->next = NULL;
    return L;
}

void deleteList(List L) {
    struct node* p ,*temp;
    p = L->next;
    L->next = NULL;

    while (p != NULL) {
        temp = p->next;
        free(p);
        p = temp;
    }
}

int isEmpty(List L) {
    return L->next == NULL;
}

int isLast(Node P, Node L) {
    return P->next == NULL;
}

Node find (int id, List L) {
    struct node* p;
    p = L->next;
    while (p != NULL && p->id != id) {
        p = p->next;
    }
    return p;
}

Node findByName (char name[], List L) {
    struct node* p;
    p = L->next;
    while (p != NULL && (strcmp(p->name, name)) != 0) {
        p = p->next;
    }
    return p;
}

Node findPrevious(int x, List L) {
    struct node* p;
    p = L;
    while (p->next != NULL && p->next->id != x)
        p = p->next;

    return p;
}

void delete(int x, List L) {
    struct node *p, *temp;
    p = findPrevious(x, L);
    if (!isLast(p, L)) {
        temp = p->next;
        p->next = temp->next;
        free(temp);
    }

}

void insert(Node X, List L, Node P){ // will insert after node P
    X->next = P->next;
    P->next = X;
}

void insertLast(List L,Node P) { // will insert at the last fo the list
    Node current = L;
    if (P == NULL) {
        printf("Error: P is NULL\n");
        return;
    }
    while (current->next != NULL)
        current = current->next;
    current->next = P;
    P->next = NULL;
}

void printList(List L) {
    Node p = L;
    if (isEmpty(L))
        printf("List is empty\n");
    else {
        do {
            p = p->next;
            printNode(p);
        }while (!isLast(p, L));
        printf("\n");
    }
}

int size(List L) {
    int count = 0;
    struct node* p = L->next;
    while (p != NULL) {
        p = p->next;
        count++;
    }
    return count;
}

Node findLast(List L) {
    Node p = L->next;
    if (p == NULL) {
        return NULL;
    }
    while (p->next != NULL) {
        p = p->next;
    }
    return p;
}

void printNode(Node P) {
    printf("Task ID: %-5d Task Name: %-25s date: %-10s duration (in hours): %-4.2f", P->id, P->name, P->date, P->duration);
    if (P->isUndone == 1) {
        printf(" (Undone task)");
    }
    printf("\n");
}



// stack functions
Stack createStack() {
    Stack s;
    s = (Stack)malloc(sizeof(struct node));
    if (s == NULL) {
        printf("out of space\n");
    }
    s->next = NULL;
    makeEmpty(s);
    return s;
}

void makeEmptyStack(Stack s) {
    if (s == NULL)
        printf("out of space\n");
    else
        while (!isEmpty(s))
            pop(s);
}

int isEmptyStack(Stack s) {
    return s->next == NULL;
}

void pop(Stack s) {
    if (isEmptyStack(s))
        return;
    Node temp = s->next;
    s->next = temp->next;
    free(temp);
}

Node top(Stack s) {
    if (!isEmptyStack(s))
        return s->next;
    //printf("stack is empty\n");
    return NULL;
}
void push(Node p, Stack s) { // i had convert it to push nodes not data
    if (p == NULL){
        printf("NULL Node\n");
    }else {
        Node temp = (Node)malloc(sizeof(struct node));
        temp->id = p->id;
        temp->duration = p->duration;
        strcpy(temp->name, p->name);
        strcpy(temp->date, p->date);
        temp->next = s->next;
        s->next = temp;
    }
}

void DisposeStack(Stack s) {
    makeEmptyStack(s);
    free(s);
}

Stack copyStack(Stack s) {
    Stack copy = createStack();
    Stack temp = createStack();
    while (!isEmptyStack(s)) {
        push(top(s),temp);
        pop(s);
    }
    while (!isEmptyStack(temp)) {
        push(top(temp),s);
        push(top(temp),copy);
        pop(temp);
    }
    return copy;
}