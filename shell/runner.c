#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "runner.h"

static int
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
                return -1;
                break;
        }
    }
    return pid;
}

static int
f_pipeline(Command *);

static int
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

static int
f_redirect(Command * c)
{
    int pid = 0, f, buf = 0, deckr = 0;
    switch (c->rd_mode) {
    case RD_OUTPUT: {
        buf = dup(1);
        f = open(c->rd_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        dup2(f, 1);
        deckr = 1;
        close(f);
        break;
    }
    case RD_APPEND: {
        buf = dup(1);
        f = open(c->rd_path, O_WRONLY | O_CREAT | O_APPEND, 0666);
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

static int
f_pipeline(Command * c)
{
    int fd[2], i = 0, original_input = dup(0), original_output = dup(1), pid = 0;
    for (i = 0; i < c->pipeline_size; ++i) {
    
        if (i > 0) {
            dup2(fd[0], 0);
            close(fd[0]);
        }
        
        if (pipe(fd) != 0) {
            return -1;
        }
        
        if (i == c->pipeline_size - 1) {
            dup2(original_output, 1);
            close(fd[1]);
        }
        else {
            dup2(fd[1], 1);
            close(fd[1]);
        }
        
        if (c->pipeline_size == 1) {
            pid = run_command(c->pipeline_commands);
            close(fd[0]);
        }
        else if ((pid = fork()) == 0) {
            //Each command in a multi-command pipeline, where pipes are created,
            //is executed in its own subshell, which is a separate process.
            pid = run_command(c->pipeline_commands + i);
            close(fd[0]);
            close(original_input);
            close(original_output);
            exit(0);
        }
    }
    close(fd[0]);
    dup2(original_input, 0);
    close(original_input);
    close(original_output);
    return pid;
}
