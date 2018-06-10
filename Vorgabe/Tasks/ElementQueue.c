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

//Methoden LCFS---------------------------------------------------------------------------------------
ElementQueue* addLCFS(ElementQueue *new_Elem, ElementQueue *last){
    if(!isDone(last->task)){//Falls last noch nicht zu Ende ist,ist es immer noch das letzte Element der Queue da nicht Verdraengend!
        if(last->prev != NULL){//NUR wenn ein Task vor das letzte Element sich befindet, soll diese veraendert werden
            last->prev->next = new_Elem;//last->prev ist NULL!!!
            new_Elem->prev = last->prev;
        }
        new_Elem->next = last;
        last->prev = new_Elem;
        return last;    //last ist immer noch dran
    }
    else{//Else ist last zu Ende bearbeitet
        last->next = new_Elem;
        new_Elem->prev = last;
        return new_Elem;
    }
}

ElementQueue* execLCFS(ElementQueue *last){
    /*if(isDone(last->task)){//Wenn Task zu Ende bearbeitet ist, nehme naehste Task zum bearbeiten
        ElementQueue *pre;
        pre = last->prev;
        pre->next = NULL;
        last->prev = NULL;
        last = prev;
    }*/
    if(execTask(last->task, 1) > 0){     //if Task 1-Tick succesfully executed
        if(isDone(last->task)){ //Nehme Element aus der Queue
            //new Last
            if(last->prev != NULL){
                ElementQueue *pre;
                pre = last->prev;
                pre->next = NULL;
                last->prev = NULL;
                last = pre;
            }
            //free(last->next);
            //last->next = NULL;
            return last;
        }
        return last;
    }
    else{
        printf("%sERROR:%s Something went wrong with execLCFS(const *ElementQueue last) See line 35 of ElementQueue.c.\n", COLOR_RED, COLOR_RESET);
        return NULL; //not succesfull
    }
}

//Methoden LCFS-PR----------------------------------------------------------------------------------------------
ElementQueue* addLCFS_PR(ElementQueue *new_Elem, ElementQueue *last){
    last->next = new_Elem;
    new_Elem->prev = last;
    return new_Elem;
}

ElementQueue* execLCFS_PR(ElementQueue *last){
    if(execTask(last->task, 1) > 0){     //if Task 1-Tick succesfully executed
        if(isDone(last->task)){ //Nehme Element aus der Queue
            last = last->prev; //new Last
            //free(last->next);
            //last->next = NULL;
            return last;
        }
        return last;
    }
    else{
        printf("%sERROR:%s Something went wrong with execLCFS(const *ElementQueue last) See line 35 of ElementQueue.c.\n", COLOR_RED, COLOR_RESET);
        return NULL; //not succesfull
    }
}


