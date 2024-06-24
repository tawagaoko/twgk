#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

enum {
    KIND_SIMPLE = 1,
    KIND_PIPELINE, // = 2
    KIND_SEQ, // = 3
    OP_SEQ, // = 4
    KIND_REDIRECT, // = 5
    RD_OUTPUT, // = 6
    RD_INPUT, // = 7
    OP_CONJUNCT, // = 8
    OP_BACKGROUND, // = 9
    RD_APPEND, // = 10
    OP_DISJUNCT, // = 11
    PERMISSIONS = 0666
};

typedef struct Command {
    int kind;
    union {
        struct {
            int argc;
            char ** argv;
        };
        struct {
            int pipeline_size;
            struct Command * pipeline_commands;
        };
        struct {
            int seq_size;
            struct Command * seq_commands;
            int * seq_operations;
        };
        struct {
            int rd_mode;
            char * rd_path;
            struct Command * rd_command;
        };
    };
} Command;

int
run_command(Command *);

int
f_seq(Command * c)
{
    int pid = 0, status = 0, no_exec = 0;
    for (int i = 0; i < c->seq_size; ++i) {
        switch (c->seq_operations[i]) {
        case OP_SEQ: {
            if (no_exec == 0) {
                pid = run_command(c->seq_commands + i);
                waitpid(pid, NULL, 0);
            }
            else {
                no_exec = 0;
            }
            break;
        }
        case OP_CONJUNCT: {
            if (no_exec == 0) {
                pid = run_command(c->seq_commands + i);
                waitpid(pid, &status, 0);
            }
            no_exec = (!(WIFEXITED(status) && (WEXITSTATUS(status) == 0)));
            break;
        }
        case OP_DISJUNCT: {
            if (no_exec == 0) {
                pid = run_command(c->seq_commands + i);
                waitpid(pid, &status, 0);
            }
            no_exec = (WIFEXITED(status) && (WEXITSTATUS(status) == 0));
            break;
        }
        case OP_BACKGROUND: {
            if (no_exec == 0) {
                run_command(c->seq_commands + i);
            }
            else {
                no_exec = 0;
            }
            break;
        }
        default:
            break;
        }
    }
    while (wait(NULL) != -1) {
        ;
    }
    return pid;
}

int
f_pipeline(Command *);

int
f_simple(Command * c)
{
    int pid;
    /*
        We believe that the process table is sufficient to run
        all the users commands (fork() == -1).
    */
    if ((pid = fork()) == 0) {
        execvp(c->argv[0], c->argv);
        exit(1);
    }
    return pid;
}

int
f_redirect(Command * c)
{
    int pid = 0, f, buf = 0, deckr = 0;
    switch (c->rd_mode) {
    case RD_OUTPUT: {
        buf = dup(1);
        f = open(c->rd_path, O_WRONLY | O_CREAT | O_TRUNC, PERMISSIONS);
        dup2(f, 1);
        deckr = 1;
        close(f);
        break;
    }
    case RD_APPEND: {
        buf = dup(1);
        f = open(c->rd_path, O_WRONLY | O_CREAT | O_APPEND, PERMISSIONS);
        dup2(f, 1);
        deckr = 1;
        close(f);
        break;
    }
    case RD_INPUT: {
        buf = dup(0);
        f = open(c->rd_path, O_RDONLY);
        dup2(f, 0);
        deckr = 0;
        close(f);
        break;
    }
    default:
        break;
    }
    pid = run_command(c->rd_command);
    dup2(buf, deckr);
    return pid;
}

int
run_command(Command * c)
{
    int pid = 0;
    switch (c->kind) {
    case KIND_SEQ: {
        pid = f_seq(c);
        return pid;
    }
    case KIND_SIMPLE: {
        pid = f_simple(c);
        return pid;
    }
    case KIND_REDIRECT: {
        pid = f_redirect(c);
        return pid;
    }
    case KIND_PIPELINE: {
        pid = f_pipeline(c);
        return pid;
    }
    default:
        return pid;
    }
}

int
f_pipeline(Command * c)
{
    int fd[2], buf = 0, i = 0, original_input = dup(0), original_output = dup(1), pid = 0;
    for (i = 0; i < c->pipeline_size; ++i) {
        if (i != 0) {
            dup2(buf, 0);
            close(buf);
        }
        if (i == c->pipeline_size - 1) {
            dup2(original_output, 1);
        }
        else {
            pipe(fd);
            dup2(fd[1], 1);
            close(fd[1]);
        }
        pid = run_command(c->pipeline_commands + i);
        buf = fd[0];
    }
    dup2(original_input, 0);
    if (buf == 0) {
        close(buf);
    }
    return pid;
}

int
main(void)
{
    return 0;
}
