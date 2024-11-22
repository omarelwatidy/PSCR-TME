#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int tubeDesc[2];
    pid_t pid_fils;

    // Locate the pipe ('|') symbol in arguments
    int pipe_index = -1;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "|") == 0) {
            pipe_index = i;
            break;
        }
    }

    // Ensure '|' symbol is present
    if (pipe_index == -1) {
        fprintf(stderr, "Error: '|' symbol missing in the command\n");
        return 1;
    }

    // Construct argcmd1 (before '|') and argcmd2 (after '|')
    char* argcmd1[pipe_index + 1];
    char* argcmd2[argc - pipe_index];
    for (int i = 1; i < pipe_index; ++i) {
        argcmd1[i - 1] = argv[i];
    }
    argcmd1[pipe_index - 1] = NULL; // NULL-terminate argcmd1

    for (int i = pipe_index + 1; i < argc; ++i) {
        argcmd2[i - pipe_index - 1] = argv[i];
    }
    argcmd2[argc - pipe_index - 1] = NULL; // NULL-terminate argcmd2
    for (int i = 0; argcmd1[i] != NULL; ++i) {
        printf("argcmd1[%d]: %s\n", i, argcmd1[i]);
    }
    for (int i = 0; argcmd2[i] != NULL; ++i) {
        printf("argcmd2[%d]: %s\n", i, argcmd2[i]);
    }
    // Create pipe
    if (pipe(tubeDesc) == -1) {
        perror("pipe");
        exit(1);
    }

    // Fork the first child
    if ((pid_fils = fork()) == -1) {
        perror("fork");
        exit(2);
    }

    if (pid_fils == 0) {
        // First child: run argcmd1, redirect output to pipe
        dup2(tubeDesc[1], STDOUT_FILENO); // Redirect stdout to pipe write-end
        close(tubeDesc[0]);              // Close unused read-end
        close(tubeDesc[1]);              // Close write-end after dup2
        execvp(argcmd1[0], argcmd1);     // Execute first command
        perror("execvp");                // Handle error
        exit(1);
    }

    // Fork the second child
    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        return 1;
    }

    if (pid2 == 0) {
        // Second child: run argcmd2, redirect input from pipe
        dup2(tubeDesc[0], STDIN_FILENO); // Redirect stdin to pipe read-end
        close(tubeDesc[1]);             // Close unused write-end
        close(tubeDesc[0]);             // Close read-end after dup2
        execvp(argcmd2[0], argcmd2);    // Execute second command
        perror("execvp");               // Handle error
        exit(1);
    }

    // Parent: close pipe and wait for both children
    close(tubeDesc[0]);
    close(tubeDesc[1]);
    waitpid(pid_fils, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
