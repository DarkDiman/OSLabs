/* Simple shell, Dmitry Nekhocheninov */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
     char argv[16][80];
     char *orders[17];
     int i = 0; 
     int j = 0;

     putchar(62);
     while (1) {
          int ch = getchar();
          if (ch == ' ') {
               argv[i][j] = '\0';
               i++; j = 0;
          } else if (ch == '\n') {
               argv[i][j] = '\0';

               for(j=0;j<=i;j++) //creating orders through argv
                    orders[j] = argv[j];
               orders[j] = NULL;

               pid_t pid = fork();
               if (!pid) { // child branch
                    int rv = execvp(orders[0],orders);
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
          } else argv[i][j++] = ch; 
     }
     return EXIT_SUCCESS;
}