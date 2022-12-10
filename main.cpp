#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void do_command(char* argv[])
{
    int child = fork();

    if (child == 0)
    {
        int a = execvp(argv[0], argv);
        if (a < 0) {
            perror("exec failed");
            exit(EXIT_FAILURE);
        }
        exit(0);
    }
    else if (child < 0) {
        perror("fork failed");
        exit(1);
    }


    int exit_code;
    waitpid(child, &exit_code, 0);

    if (WIFEXITED(exit_code))
    {
        std::cout << "Child terminated normally with status code " << WEXITSTATUS(exit_code) << "\n";
    }
    else if (WIFSIGNALED(exit_code))
    {
        std::cout << "Child process was terminated by force.\n";
    }
}

int main()
{
    char* program[4];
    program[0] = "cat";
    program[1] = "/etc/lsb-release";
    program[2] = nullptr;
    do_command(program);
}
