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
//Methoden HRRN----------------------------------------------------------------------------------------------
ElementQueue* createHRRN(Task *ta){
    ElementQueue *new_Elem;
    
    new_Elem = (ElementQueue *) malloc(sizeof(ElementQueue));
    //Standarswerte einsetzten
    //new_Elem->last = false;
    new_Elem->task = ta;
    new_Elem->prev = NULL;
    new_Elem->next = NULL;
    new_Elem->wz = 0;
    
    return new_Elem;
}

ElementQueue* addHRRN(ElementQueue *new_Elem, ElementQueue *last){
    if(!isDone(last->task)){//Falls last noch nicht zu Ende ist, ist es immer noch das letzte Element der Queue da nicht Verdraengend!
        if(last->prev != NULL){//NUR wenn ein Task vor das letzte Element sich befindet, soll diese veraendert werden
            last->prev->next = new_Elem;//last->prev ist NULL!!!
            new_Elem->prev = last->prev;
        }
        new_Elem->next = last;
        last->prev = new_Elem;
        return last;    //last ist immer noch dran
    }
    else{//falls last zu Ende bearbeitet hat -> nehme last aus der Queue
        //last->next = new_Elem;
        if(last->prev != NULL){
            last->prev->next = new_Elem;
            new_Elem->prev = last->prev;
        }
        return new_Elem; //returned element wird zu last
    }
}

ElementQueue* execHRRN(ElementQueue *last){
    
    ElementQueue *searcher;     //elementQueue to search the queue for the highest rr
    int curr = -1;              // current response ratio
    int maxrr = -1;             //highest response ratio
    ElementQueue *max_Elem;     //elementQueue with the highest rr
    
    if(execTask(last->task, 1) > 0){     //if Task 1-Tick succesfully executed
        if(isDone(last->task)){ //if last-element is finished
            //find Task with the highest response rate = ((wz+bz)/bz) in the Queue
            if(last->prev != NULL){ //if last is NOT last Element of the Queue
                while(searcher->prev != NULL){  //go over all Elements of the queue
                    searcher = searcher->prev;//last is already finished, no need to check its response ratio
                    curr = (searcher->wz + searcher->task->total_ticks) / searcher->task->total_ticks;                  //response-ratio= ((currentTick-Ankunft+1)+Bedienzeit)/Bedienzeit = (wz+bz)/bz <- 2 Alternatives to implement this!
                    if(curr > maxrr){
                        maxrr = curr; //set new rr-maximum
                        max_Elem = searcher;
                    }
                }
                //now we have the element with the highest response-rate in max_Elem and we know last->task is finished AND that the queue contains at least 2 Elements
                if(max_Elem->prev != NULL){ //max_Elem ist Nicht letztes Element des Queues
                    max_Elem->prev->next = max_Elem->next;
                    max_Elem->next->prev = max_Elem->prev;
                        
                    last->prev->next = max_Elem;
                    max_Elem->prev = last->prev;
                        
                    last = max_Elem;//Element mit highest response-ratio ist jetzt last Element!
                }
                else{          //max_Elem ist letztes Element des Queues
                    max_Elem->next->prev = NULL;
                        
                    if(last->prev!=NULL){ //if Queue hat mehr als 2 Eintraege
                        last->prev->next = max_Elem;
                        max_Elem->prev = last->prev;
                    }
                    last = max_Elem;
                }
                
                maxrr = -1;     //reset highest response ratio
                max_Elem = NULL;//reset elementQueue with the highest rr
            }
            //free(last->next);
            //last->next = NULL;
            return last;
        }
        //in the case task is still working return same task to continue working and add wz+1 to every task in the queue that isin't last
        searcher = last;
        while(searcher->prev != NULL){  //go over all Elements of the queue
            searcher = searcher->prev;//no need to see last-element
            searcher->wz++;    //Wartezeit+1 fuer jedes Element das nicht gerade durchgefuehrt wird
        }
        searcher = NULL; //reset searcher
        return last;
    }
    else{
        printf("%sERROR:%s Something went wrong with execLCFS(const *ElementQueue last) See line 134 of ElementQueue.c.\n", COLOR_RED, COLOR_RESET);
        return NULL; //not succesfull
    }
}
