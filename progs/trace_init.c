// Spawn strace in a fork and exec into /sbin/init
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int pid_t_char_count(pid_t pid) {
    // {{{
    size_t char_count = 0;

    if (pid < 0)
        char_count += 1;

    while (pid != 0) {
        char_count += 1;
        pid = pid / 10;
    }

    return char_count;
} // }}}

int main(int argc, char *argv[]) {
    pid_t pid = vfork();

    if (pid == 0) {
        // Child
        pid_t parent_pid = getppid();

        size_t char_count = pid_t_char_count(parent_pid);
        // "--attach=" + len(parent_pid) + "\0"
        const int attach_arg_len = 9 + char_count + 1;
        char attach_arg[attach_arg_len];
        {
            int write_len = snprintf(attach_arg, attach_arg_len, "--attach=%d", parent_pid);
            if (write_len > attach_arg_len) {
                printf("[E]: Wrote more than buffer length!\n");
                exit(1);
            }
        }

        char *strace_args[] = {
            "strace",
            "--follow-forks",
            "--quiet",
            "--trace=/^exec",
            "--signal=!all",
            attach_arg,
            NULL
        };

        execv("/bin/strace", strace_args);

    } else if (pid > 0) {
        // Parent
        char *args[argc + 1];
        char *prog = "/sbin/init";

        args[0] = prog;
        for (int i = 1; i < argc; i++)
            args[i] = argv[i];
        args[argc + 1] = NULL;

        execv(prog, args);

    } else {
        // Error
        printf("[E]: Fork failed!\n");
        exit(1);
    }
}
