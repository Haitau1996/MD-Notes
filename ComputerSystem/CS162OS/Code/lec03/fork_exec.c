#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
    int i;
    pid_t cpid = fork();
    if (cpid > 0) {
        // do nothing 
    } else if (cpid == 0) {
        char *args[]= {"ls", "-l", NULL};
        execv("/bin/ls", args);

        // execv 执行后不返回，所以成功的话下面部分不执行
        perror("execv failed");
        exit(-1);
    }
}