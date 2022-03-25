#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>

int main(){
    printf("Beginning of line ");
    sleep(5);
    printf("end of the line\n");

    write(STDOUT_FILENO, "Beginning of line ",18);
    sleep(5);
    write(STDOUT_FILENO,"and end of line \n",16);
}