#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// Function to split the input into individual commands based on the pipe character '|'
void split_commands(char *input, char **commands) {
    int i = 0;
    char *command = strtok(input, "|");
    while (command != NULL) {
        commands[i++] = command;
        command = strtok(NULL, "|");
    }
    // commands[i] = NULL; // Terminate the commands array with NULL
}

// // Function to execute a command
// void execute_command(char *command, int input_fd, int output_fd) {
//     pid_t pid = fork();

//     if (pid == 0) { // Child process
//         if (input_fd != 0) { // Redirect input if needed
//             dup2(input_fd, STDIN_FILENO);
//             close(input_fd);
//         }

//         if (output_fd != 1) { // Redirect output if needed
//             dup2(output_fd, STDOUT_FILENO);
//             close(output_fd);
//         }

//         char *args[100];
//         int i = 0;
//         char *arg = strtok(command, " ");
//         while (arg != NULL) {
//             args[i++] = arg;
//             arg = strtok(NULL, " ");
//         }
//         args[i] = NULL;

//         execvp(args[0], args); // Execute the command
//         perror("execvp failed"); // If execvp returns, there was an error
//         exit(1);
//     } else if (pid < 0) {
//         perror("fork failed");
//         exit(1);
//     }

//     wait(NULL); // Parent process waits for the child process to finish
// }

// // Function to execute a pipeline of commands
// void execute_pipeline(char **commands) {
//     int i = 0;
//     int input_fd = 0; // The input file descriptor, starts with STDIN

//     while (commands[i] != NULL) {
//         int pipefd[2];
//         pipe(pipefd); // Create a pipe

//         // If it's the last command, the output should go to STDOUT
//         if (commands[i + 1] == NULL) {
//             execute_command(commands[i], input_fd, STDOUT_FILENO);
//         } else {
//             execute_command(commands[i], input_fd, pipefd[1]);
//         }

//         close(pipefd[1]); // Close the write end of the pipe

//         // The input for the next command comes from the read end of the pipe
//         input_fd = pipefd[0];

//         i++;
//     }
// }

// // int main() {
// //     char input[1024];
// //     printf("Enter a command: ");
// //     fgets(input, sizeof(input), stdin);

// //     // Remove the newline character from the input
// //     input[strlen(input) - 1] = '\0';

// //     char *commands[100];
// //     split_commands(input, commands);

// //     execute_pipeline(commands);

// //     return 0;
// // }
