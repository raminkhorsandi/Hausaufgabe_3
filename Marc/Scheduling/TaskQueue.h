#include <sys/queue.h>
#include "Task.h"

struct list_entry {
  Task *task;

  TAILQ_ENTRY(tailq_entry) entries;
}
