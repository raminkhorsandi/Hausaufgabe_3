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
        if(last->prev != NULL){//NUR wenn (mindestens) ein Task vor das letzte Element sich befindet, soll diese veraendert werden
            last->prev->next = new_Elem;//last->prev ist NULL!!!
            new_Elem->prev = last->prev;
        }
        new_Elem->next = last;
        last->prev = new_Elem;
        return last;    //last ist immer noch dran
    }
    else{//Else ist last zu Ende bearbeitet -> free malloc!
        /*last->next = new_Elem;
        new_Elem->prev = last;*/
        if(last->prev != NULL){//Falls last nicht einzige Element der Queue
            last->prev->next = new_Elem;
            new_Elem->prev = last->prev;
        }
        last->prev = NULL;
        free(last); //last ist zu Ende->free malloc
        return new_Elem;
    }
}

ElementQueue* execLCFS(ElementQueue *last){
    if(execTask(last->task, 1) > 0){     //if Task 1-Tick succesfully executed
        if(isDone(last->task)){ //Nehme Element aus der Queue und free malloc
            //new Last
            if(last->prev != NULL){
                ElementQueue *pre;
                pre = last->prev;
                pre->next = NULL;
                last->prev = NULL;
                free(last);
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
        if(isDone(last->task)){ //Nehme Element aus der Queue und free malloc
            if(last->prev != NULL){//if 2 or more Elements in the Queue
                ElementQueue *pre;
                pre = last->prev;
                pre->next = NULL;
                last->prev = NULL;
                free(last);
                last = pre; //new Last
                return last;
            }
            else{   //Nur noch ein Element in der Queue
                last->prev = NULL;
                free(last);
                return NULL;
            }
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
            last->prev->next = new_Elem;//last->prev ist NULL!!!??
            new_Elem->prev = last->prev;
        }
        new_Elem->next = last;
        last->prev = new_Elem;
        
        new_Elem->wz = new_Elem->wz + 1; //new_Elem wartet eine Zeiteinheit wenn last immer noch dran isr
        
        return last;    //last ist immer noch dran
    }
    else{//falls last zu Ende bearbeitet hat -> nehme last aus der Queue und free malloc!
        //last->next = new_Elem;
        if(last->prev != NULL){
            last->prev->next = new_Elem;
            new_Elem->prev = last->prev;
        }
        free(last);
        return new_Elem; //returned element wird zu last
    }
}

ElementQueue* execHRRN(ElementQueue *last){
    
    ElementQueue *searcher = NULL;;     //elementQueue to search the queue for the highest rr
    int curr = -1;              // current response ratio
    int maxrr = -1;             //highest response ratio
    ElementQueue *max_Elem = NULL;     //elementQueue with the highest rr
    
    if(execTask(last->task, 1) > 0){     //if Task 1-Tick succesfully executed
        if(isDone(last->task)){ //if last-element is finished
            //find Task with the highest response rate = ((wz+bz)/bz) in the Queue
            if(last->prev != NULL){ //if last is NOT last Element of the Queue
                searcher = last;
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
                    max_Elem->next = NULL;
                    
                    last->prev = NULL;
                    free(last);
                    
                    last = max_Elem;//Element mit highest response-ratio ist jetzt last Element!
                }
                else{          //max_Elem hat kein prev
                    max_Elem->next->prev = NULL;
                        
                    if(last->prev != NULL && last->prev != max_Elem){ //if Queue hat mehr als 2 Eintraege
                        last->prev->next = max_Elem; //Hier ist der Fehler!! see wieso last->prev = NULL ist
                        max_Elem->prev = last->prev;
                    }
                    max_Elem->next = NULL;
                    
                    last->prev = NULL;
                    free(last);
                    
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
//Methoden SRTN----------------------------------------------------------------------------------------------
ElementQueue* addSRTN(ElementQueue *new_Elem, ElementQueue *last){//check Remaining Time Hierarchy!!
    ElementQueue *searcher = NULL;;     //elementQueue to search the queue for the shortest remaing time
    int curr = 99999999;              // current remaining time
    int min = 99999999;             //shorttest remaining time
    ElementQueue *max_Elem = NULL;     //elementQueue with the shortest remaining time
    if(!isDone(last->task)){//Falls last noch nicht zu Ende ist, check if new Element has shorter remaingn time!
        if(last->prev != NULL){ //if last is NOT last Element of the Queue
            searcher = last;
            while(searcher->prev != NULL){  //go over all Elements of the queue
                searcher = searcher->prev;//last is already finished, no need to check its remaining time
                curr = searcher->task->total_ticks - searcher->task->exec_ticks;//remaining time = total_ticks-exec_ticks
                if(curr < min){
                    min = curr; //set new remaining-time min
                    max_Elem = searcher;
                }
            }
            //now we have the element with the shortest remaing time in max_Elem and we know last->task is finished AND that the queue contains at least 2 Elements
            if(max_Elem->prev != NULL){ //max_Elem ist Nicht letztes Element des Queues
                max_Elem->prev->next = max_Elem->next;
                max_Elem->next->prev = max_Elem->prev;
                
                last->prev->next = max_Elem;
                max_Elem->prev = last->prev;
                max_Elem->next = NULL;
                
                last->prev = NULL;
                free(last);
                
                last = max_Elem;//Element mit highest response-ratio ist jetzt last Element!
            }
            else{          //max_Elem hat kein prev
                max_Elem->next->prev = NULL;
                
                if(last->prev != NULL && last->prev != max_Elem){ //if Queue hat mehr als 2 Eintraege
                    last->prev->next = max_Elem; //Hier ist der Fehler!! see wieso last->prev = NULL ist
                    max_Elem->prev = last->prev;
                }
                max_Elem->next = NULL;
                
                last->prev = NULL;
                free(last);
                
                last = max_Elem;
            }
            
            min = -1;     //reset highest response ratio
            max_Elem = NULL;//reset elementQueue with the highest rr
        }
        //free(last->next);
        //last->next = NULL;
        return last;
    }
    //in the case task is still working check if new Task has shorter-remaing time than current last-element
    //here new_Task-remaining time = total_ticks - 0 = total_ticks
    curr = last->task->total_ticks - last->task->exec_ticks; //curr == last-element remaing time = total_ticks - executed ticks
    if(curr <= new_Elem->task->total_ticks){   //if last-element remaining-time shorter than the new element's add new element to the queue (not in the last position)
        if(last->prev != NULL){
            last->prev->next = new_Elem;
            new_Elem->prev = last->prev;
        }
        last->prev = new_Elem;
        new_Elem->next = last;
    }
    else{   //else new element's remaining time is shorter than current last's
        last->next = new_Elem;
        new_Elem->prev = last;
        last = new_Elem;
    }
    curr = 0;//reset curr
    return last;
}

ElementQueue* execSRTN(ElementQueue *last){
    if(execTask(last->task, 1) > 0){     //if Task 1-Tick succesfully executed
        if(isDone(last->task)){ //Nehme Element aus der Queue und free malloc
            //new Last
            if(last->prev != NULL){
                ElementQueue *pre;
                pre = last->prev;
                pre->next = NULL;
                last->prev = NULL;
                free(last);
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
