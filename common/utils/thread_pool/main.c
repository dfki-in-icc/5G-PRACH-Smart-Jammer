#include <assert.h>
#include <stdlib.h>
#include "task_manager.h"
#include "../common_task.h"

int main()
{
  task_manager_t man;
  init_task_manager(&man, NUM_THREADS);

  pair_t* arr = calloc(NUM_JOBS, sizeof(pair_t));
  assert(arr != NULL);

//  wake_and_spin_task_manager(&man);

  for(int i = 0; i < NUM_JOBS; ++i){
    arr[i] = fill_pair(i); 
    task_t t = {.args = &arr[i], t.func = do_work};
    async_task_manager(&man, t);
  }

//  stop_spin_task_manager(&man);
  wait_all_spin_task_manager(&man); 
  free(arr);
  free_task_manager(&man, NULL);

  return EXIT_SUCCESS;
}

