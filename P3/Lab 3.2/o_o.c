#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

struct ListItem {
    struct ListItem *next;
    int value;
};

struct ListItem *listHead = NULL;


void appendItem(int value) {
    // append at the end of the list

    struct ListItem *newItem;
    newItem = malloc(sizeof(struct ListItem));
    newItem->value = value;
    newItem->next = NULL;

    if (listHead == NULL) {
        listHead = newItem;
    } else {
        struct ListItem *current = listHead;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newItem;
    }
}


int removeFirstItem() {
    // removes the first item from the list and returns its value; returns −1 if list is empty

    if (listHead == NULL) {
        return -1;

    } else{
        struct ListItem *tbr = listHead;
        listHead = listHead->next;
        int value = tbr->value;
        free(tbr);
        return value;
    }
}


int containsItem(int value) {
    // return true (1) if list contains value, false (0) if not

    struct ListItem *current = listHead;
    while (current != NULL) {
        if (current->value == value) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}


int isEmpty() {
    // return true (1) if list is empty, false (0) otherweise    

    return listHead == NULL ? 1 : 0;
}



int main() {
    appendItem(42);
    appendItem(4711);
    removeFirstItem();
    appendItem(42);
    appendItem(4);
    for(int i=0; i<5; i++) printf("%d\n", removeFirstItem());
}