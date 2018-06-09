#include "ElementQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

ElementQueue* createElem(Task *ta){
    ElementQueue *new_Elem;
    
    new_Elem = (ElementQueue *) malloc(sizeof(ElementQueue));
    //Standarswerte einsetzten
    //new_Elem->last = false;
    new_Elem->task = ta;
    new_Elem->prev = NULL;
    new_Elem->next = NULL;
    
    return new_Elem;
}
//Anfang Methoden LCFS--------------------------------------------
ElementQueue* addLCFS(ElementQueue *new_Elem, ElementQueue *last){
    last->next = new_Elem;
    new_Elem->prev = last;
    return new_Elem;
}

ElementQueue* execLCFS(ElementQueue *last){
    if(execTask(last->task, 1) > 0){     //if Task 1-Tick succesfully executed
        if(isDone(last->task)){ //Nehme Element aus der Queue
            last = last->prev; //new Last
            //free(last->next);
            last->next = NULL;
            return last;
        }
        return last;
    }
    else{
        printf("%sERROR:%s Something went wrong with execLCFS(const *ElementQueue last) See line 35 of ElementQueue.c.\n", COLOR_RED, COLOR_RESET);
        return NULL; //not succesfull
    }
}


