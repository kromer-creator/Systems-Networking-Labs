#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

//QUESTION 1: FORK
void callFork(){

    //Print title of function
    printf("FUNCTION 1: Call Fork\n");

    //Initialize access variable
    int x;
    x = 100;

    //Call fork
    int rc = fork();

    //Child process
    if (rc == 0){
        printf("Child process!\n");
        //Print initial x value
        printf("x = %d\n", x);
        //Change x and print its new value
        x = 50;
        printf("Now x = %d\n", x);
        //Exit child process
        exit(0);
    }

    //Parent process
    else if (rc > 0){
        //Wait for child process to complete
        wait(NULL);
        printf("Parent process!\n");
        //Print initial x value
        printf("x = %d\n", x);
        //Change x and print its new value
        x = 200;
        printf("Now x = %d\n", x);
    }

    //Print error message if fork fails
    else{
        fprintf(stderr, "Fork failed!");
    }

    /* QUESTION 1:
     * The value of x in the child process is 50.
     * The value of x in the parent process is 200.
     * When x is changed in both the parent and child process,
     * it adopts two different values, one for each process.
     */
}

//QUESTION 2: FORK AND OPEN
void forkOpen(){

    //Print title of function
    printf("FUNCTION 2: Fork and Open Write\n");

    //Open file
    int file;
    file = open("out.txt", O_CREAT|O_TRUNC|O_WRONLY, 0666);

    //Call fork
    int rc = fork();

    //Child process
    if (rc == 0){
        //Write to file and exit
        write(file, "Child writing to file\n", 22);
        exit(0);
    }

    //Parent process
    else if (rc > 0){
        //Wait for child process to complete
        wait(NULL);
        //Write to file and close file
        write(file, "Parent writing to file\n", 23);
        close(file);
    }

    //Print error message if fork fails
    else{
        fprintf(stderr, "Fork failed!\n");
    }

    /* QUESTION 2:
     * Both the child and parent can access the file descriptor returned by
     * open. If they are writing to the file concurrently (ie, wait(NULL) omitted),
     * then the parent writes first then the child. However, if the parent is told to
     * wait, then the child writes first.
     */

}

//QUESTION 3: FORK AND PRINT
void forkPrint() {

    //Print title of function
    printf("FUNCTION 3: Fork and Printing\n");

    //Call fork
    int rc = fork();

    //Child function
    if (rc == 0) {
        //Print hello then exit
        printf("Hello\n");
        exit(0);
    }

    //Parent function
    else if (rc > 0) {
        //Wait 500 microseconds for parent function to execute
        usleep(500);
        //Print goodbye
        printf("Goodbye\n");
    }

    //Print error message if fork fails
    else{
        fprintf(stderr, "Fork failed!\n");
    }

    /* QUESTION 3:
     * By using the "usleep()" function call, we can force the parent to wait for the child
     * to finish executing without using "wait()". However, this is less effective because,
     * on a slower machine, this may not be enough time. Moreover, it can mess up the
     * execution order of the other functions in the program. Therefore, it is best
     * to just use the "wait()" function call.
     */
}

//QUESTION 4: FORK AND EXEC
void forkExec(){

    //Print title of function
    printf("FUNCTION 4: Fork and Exec\n");

    //Call fork
    int rc = fork();

    //Child process
    if (rc == 0){
        //Print child process message
        printf("Child process!\n");

        //Initialize argv and envp
        char * const argv[] = {"ls", NULL};
        char * const envp[] = {NULL};

        //Call various exec commands -- ONLY UNCOMMENT ONE AT A TIME!!!!!
        execl("/bin/ls", "ls", NULL);
        //execlp("ls", "ls", NULL);
        //execle("/bin/ls", "ls", NULL, envp);
        //execv("/bin/ls", argv);
        //execvp("ls", argv);

        //Exit program
        exit(0);
    }

    //Parent process
    else if (rc > 0){
        //Wait for child to complete
        wait(NULL);
        //Print parent process message
        printf("Parent process!\n");
    }

    //Print error message if fork fails
    else{
        fprintf(stderr, "Fork failed!\n");
    }

    /* QUESTION 4:
     * There are many variations of exec because, according to
     * Reddit, they were added over time as new functionality was needed.
     * Each version of exec takes in different parameters, which
     * allows flexibility for programmers.
     */
}

//QUESTION 5: WAIT
void useWait(){

    //Print title of function
    printf("FUNCTION 5: Wait\n");

    //Call fork
    int rc = fork();

    //Child process
    if (rc == 0) {
        //Wait for parent process to complete
        int w = wait(NULL);
        //Print name and wait() return value
        printf("Child process!\n");
        printf("wait() return value: %d\n", w);
        //exit(0);
    }

    //Parent process
    else if (rc > 0){
        //Print parent process then exit
        printf("Parent process!\n");
        exit(0);
    }

    //Print parent process if fork fails
    else{
        fprintf(stderr, "Fork failed!\n");
    }

    /* QUESTION 5:
     * wait() returns -1. When you use wait() in the child,
     * you must use exit(0) in the parent, then the parent will run first
     * followed by the child.
     */
}

//QUESTION 6: WAITPID
void useWaitPid(){

    //Print title of function
    printf("FUNCTION 6: Waitpid\n");

    //Call fork
    int rc = fork();

    //Child function
    if (rc == 0){
        //Wait for parent process to execute
        //Before running child process
        int status;
        waitpid(rc, &status, 0);
        //Print child PID and exit
        printf("This is the child process. PID: %d\n", getpid());
    }

    //Parent function
    else if (rc > 0){
        //Print parent PID and exit
        printf("This is the parent process. PID: %d\n", getpid());
        exit(0);
    }

    //Print error message if fork fails
    else{
        fprintf(stderr, "Fork failed!\n");
    }

    /* QUESTION 6:
     * waitpid() would be useful when you want to wait until a specific
     * process to finish based on its PID before beginning a new one.
     */
}

//QUESTION 7: STANDARD OUT
void stdOutWrite(){

    //Print title of function
    printf("FUNCTION 7: Standard Out\n");

    //Call fork
    int rc = fork();

    //Child process
    if (rc == 0){
        //Child process closes standard output
        close(STDOUT_FILENO);
        //Calls printf then exits program
        printf("Child process!\n");
        exit(0);
    }

    //Parent process
    else if (rc > 0){
        //Wait for child process to finish then print
        wait(NULL);
        printf("Parent process!\n");
    }

    //Print error message if fork fails
    else{
        printf("Fork failed!\n");
    }

    /* QUESTION 7:
     * The print statement after the standard output closing command does
     * not execute. The parent print statement prints after the user
     * directory is once again brought up in the terminal.
     */

}

int main() {
    //Call question 1 function
    callFork();
    //Call question 2 function
    forkOpen();
    //Call question 4 function
    forkExec();
    //Call question 5 function
    useWait();
    //Call question 6 function
    useWaitPid();
    //Call question 7 function
    stdOutWrite();
    //Call question 3 function
    /* This function is called last because, as I explain in the answer to
     * question 3 within its function, the use of "usleep()" instead of "wait()"
     * can mess with the execution of the other functions in the program.
     */
    forkPrint();

    return 0;
}