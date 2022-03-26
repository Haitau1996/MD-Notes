#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
int main(int argc, char *argv[])
{
    char *msg = "Message in a pipe.\n";
    int msglen = strlen(msg);
    const int BUFSIZE = 1024;
    char buf[BUFSIZE];
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        fprintf (stderr, "Pipe failed.\n");
        return EXIT_FAILURE;
    }
    pid_t pid = fork();
    if (pid < 0) {
        fprintf (stderr, "Fork failed.\n");
        return EXIT_FAILURE;
    }
    if (pid != 0) {
        ssize_t writelen = write(pipe_fd[1], msg, msglen);
        printf("Parent: %s [%ld, %ld]\n", msg, msglen, writelen);
        close(pipe_fd[0]);
    } else {
        ssize_t readlen = read(pipe_fd[0], buf, BUFSIZE);
        printf("Child Rcvd: %s [%ld]\n", msg, readlen);
        close(pipe_fd[1]);
    }
}