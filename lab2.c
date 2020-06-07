/* Simple shell + I/O redirection, Dmitry Nekhocheninov */ 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int main() {

     char argv[16][80];
     char *orders[17];
     int i = 0; 
     int j = 0;
     int import = 0;
     int export = 0;

     putchar(62);
     while (1) {
          int ch = getchar();
          if (ch == ' ') {
               argv[i][j] = '\0';
               i++; j = 0;
          } else if (ch == '<') {
              import = i;
              argv[i][j] = '\0';
              i++; j = 0;
          } else if (ch == '>') {
              export = i;
              argv[i][j++] = '\0';
              i++; j = 0;
          } else if (ch == '\n') {
               argv[i][j] = '\0';

               for(j=0;j<=i;j++) //creating orders through argv
                    orders[j] = argv[j];
               orders[j] = NULL;

               pid_t pid = fork();
               if (!pid) { // child branch
                    if (import != 0) { // import file
                        int fd = open(argv[++import], O_RDONLY);
                        if (fd == -1) {
                            perror("open import");
                            exit(EXIT_FAILURE);
                        }
                        if (dup2(fd, STDIN_FILENO) == -1) {
                            perror("dup2 import");
                            exit(EXIT_FAILURE);
                        }
			            orders[import] = NULL;
                    }
                    if (export != 0) { // export file
                        int fd2 = open(argv[++export], O_WRONLY | O_CREAT | O_TRUNC,0666);
                        if (fd2 == -1) {
                            perror("open export");
                            exit(EXIT_FAILURE);
                        }
                        if (dup2(fd2, STDOUT_FILENO) == -1) {
                            perror("dup2 export");
                            exit(EXIT_FAILURE);
                        }
			            orders[export] = NULL;
                    }
                    int rv = execvp(orders[0],orders); // command execution
                    if (rv == -1) {
                         perror("execvp");
                         exit(EXIT_FAILURE);
                    }
               }
               pid = wait(NULL); // parent branch
               if (pid == -1) {
                    perror("wait");
                    exit(EXIT_FAILURE);
               }
               putchar(62);
               i = 0; j = 0;
               import = 0; export = 0;
          } else argv[i][j++] = ch; 
     }
     return EXIT_SUCCESS;
}
