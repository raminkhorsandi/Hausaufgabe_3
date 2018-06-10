#include "LCFS-PR.h"

void schedule_LCFS_PR(const TaskPool *task_pool){
    //LCFS ist Verdraengend!
    //Define new Task
    
    int currTick = 0;
    
    ElementQueue* last = NULL;//Queue empty
    
    while(!allDone(task_pool)){
        
        if(checkArrivals(task_pool,currTick)==NULL){//if keine Arrivals at current Tick
        }
        else{ //else Task arrives at the current tick
            
            if(last == NULL){//If Queue empty
                last = createElem(checkArrivals(task_pool,currTick));   // create new Queue Element
            }
            else{//If Queue not empty
                last = addLCFS_PR(createElem(checkArrivals(task_pool,currTick)), last); //add new Element to the Queue with the constraint LastComeFirstServed
            }
        }
        if(last != NULL){ //if Queue not empty
            last = execLCFS_PR(last);
        }
        currTick++;
    }
    //Last check if all Tasks were succesfully executed
    if(!allDone(task_pool)){
        printf("Taskpool didn't complete all Tasks!");
    }
    printf("\n");
    return;
}
