#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

struct ListItem {
    struct ListItem *next;
    int value;
};

struct ListItem *listHead = NULL;

void appendItem(int valie) {
    // ... implement this
    // append at the end of the list

    struct ListItem *newItem;
    newItem = malloc(sizeof(struct ListItem));
    newItem->value = valie;
    newItem->next = NULL;

    if (listHead->next == NULL) {
        listHead->next = newItem;
    } else {
        struct ListItem *temp = listHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newItem;
    }
    

}


int removeFirstItem() {
    // implement this
    // removes the first item from the list and returns its value; returns −1 if list is empty
    if (listHead->next == NULL) {
        return -1;

    } else{
        int temp = listHead->next->value;
        listHead->next = listHead->next->next;
        return temp;
    }
}


int containsItem(int value) {
    // implement this
    // return true (1) if list contains value, false (0) if not
    return 0;
}


int isEmpty() {
    // implement this
    // return true (1) if list is empty, false (0) otherweise
    return 0;
}



int main() {
    appendItem(42);
    appendItem(4711);
    removeFirstItem();
    appendItem(42);
    appendItem(4);
    for(int i=0; i<5; i++) printf("%d\n", removeFirstItem());
}