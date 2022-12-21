/*
MIT License

Copyright (c) 2021 Mikel Irazabal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "lock_guard.h"

#include <stdio.h>

static
int dummy_func(pthread_mutex_t* m)
{
  lock_guard(m);
   for(int i =0; i < 10; ++i){
    printf("Value of the function = %d \n",i);
  }
   return 0;
}


int main()
{
  pthread_mutex_t mtx; // = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutexattr_t attr; 

  int rc = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
  assert(rc == 0);

  rc = pthread_mutex_init(&mtx, &attr);
  assert(rc == 0);


  dummy_func(&mtx);

  {
    lock_guard(&mtx);
  }

  lock_guard(&mtx);

//  rc = pthread_mutex_unlock(&mtx);
//  if(rc != 0){
//    fprintf(stdout, "Error while locking: %s\n", strerror(rc) ); 
//    exit(-1);
//   } 


  for(int i =0; i < 10; ++i){
    printf("Value of i = %d \n",i);
  }

  return 0;
}
