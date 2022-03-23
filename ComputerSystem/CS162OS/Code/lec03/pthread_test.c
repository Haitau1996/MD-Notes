#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int common = 162;
void *threadfun(void *threadid){
    long tid = (long)threadid;
    printf("thread #%lx stack : %ld common %lx (%d)\n",tid,(unsigned long)&tid, (unsigned long)&common, common++);
    pthread_exit(NULL);
}
int main(int argc, char *argv[]){
    long t;
    int nthreads = 2;
    if(argc > 1){
        nthreads = atoi(argv[1]);
    }
    pthread_t *threads = malloc(nthreads * sizeof(pthread_t));
    printf("main stack : %ld ,common :%lx (%d)\n", (unsigned long)&t, (unsigned long)&common, common);
    for(t = 0; t < nthreads; ++t){
        int rc = pthread_create(&threads[t],NULL, threadfun,(void*)t);
        if(rc){
            printf("ERROR with return code %d \n",rc);
            exit(-1);
        }
    }
    for(t = 0;  t < nthreads; t++){
        pthread_join(threads[t], NULL);
    }
    pthread_exit(NULL);
}