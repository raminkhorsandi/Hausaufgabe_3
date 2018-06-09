#include "LCFS.h"
#include "../Tasks/cJSON.h"
#include "../Tasks/ElementQueue.h"

void schedule_LCFS(const TaskPool *task_pool){
    //LCFS ist nicht Verdraengend!
    //Define new Task
    
    int currTick = 0;
    
    ElementQueue* last = NULL;//Queue empty
    ElementQueue* oldLast = NULL;
    
    while(!allDone(task_pool)){
        
        if(checkArrivals(task_pool,currTick)==NULL){//if keine Arrivals at current Tick
            currTick++;
        }
        else{ //else Task arrives at the current tick
            
            if(last == NULL){//If Queue empty
                last = createElem(checkArrivals(task_pool,currTick));   // create new Queue Element
            }
            else{//If Queue not empty
                last = addLCFS(createElem(checkArrivals(task_pool,currTick)), last); //add new Element to the Queue with the constraint LCFS
            }
            currTick++;
        }
        if(last != NULL){ //if Queue not empty
            last = execLCFS(last);
        }
    }
    //Last check if all Tasks were succesfully executed
    if(!allDone(task_pool)){
        printf("Taskpool didn't complete all Tasks!");
    }
    printf("\n");
    return;
}
