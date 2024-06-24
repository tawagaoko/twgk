#include <stdlib.h>
#include "command.h"

int
init_empty_command(Command * cmd)
{
    *cmd = (Command) { 0 };
    return 0;
}

int
init_sequence_command(Command * cmd)
{
    int error_mark = init_empty_command(cmd);
    if (error_mark) {
        return error_mark;
    }
    cmd->kind = KIND_SEQ;
    return 0;
}

int
append_command_to_sequence(Command *c, Command *cmd)
{
    c->seq_size += 1;
    Command * t = realloc(c->seq_commands, (c->seq_size) * sizeof*c);
    if (!t) {
        return -1;
    }
    c->seq_commands = t;
    c->seq_commands[c->seq_size - 1] = *cmd;
    return 0;
}

int
append_operation_to_sequence(Command * cmd, int operation)
{
    int * t = realloc(cmd->seq_operations, (cmd->seq_size) * sizeof operation);
    if (!t) {
        return -1;
    }
    cmd->seq_operations = t;
    cmd->seq_operations[cmd->seq_size - 1] = operation;
    return 0;
}

int
init_pipeline_command(Command *c)
{
    int error_mark = init_empty_command(c);
    if (error_mark) {
        return error_mark;
    }
    c->kind = KIND_PIPELINE;
    return 0;
}

int
append_to_pipeline(Command *c, Command *cmd)
{
    c->pipeline_size += 1;
    Command * t = realloc(c->pipeline_commands, (c->pipeline_size) * sizeof*c);
    if (!t) {
        return -1;
    }
    c->pipeline_commands = t;
    c->pipeline_commands[c->pipeline_size - 1] = *cmd;
    return 0;
}

int
init_redirect_command(Command *c)
{
    int error_mark = init_empty_command(c);
    if (error_mark) {
        return error_mark;
    }
    c->kind = KIND_REDIRECT;
    return 0;
}

int
set_rd_command(Command *c, Command *cmd)
{
    Command * error_mark = c->rd_command;
    if (error_mark) {
        return -1;
    }
    Command * t = malloc(sizeof*c);
    if (!t) {
        return -1;
    }
    c->rd_command = t;
    *t = *cmd;
    return 0;
}



int
init_simple_command(Command *c)
{
    int error_mark = init_empty_command(c);
    if (error_mark) {
        return error_mark;
    }
    c->kind = KIND_SIMPLE;
    char ** t = malloc(sizeof(char *));
    if (!t) {
        return -1;
    }
    c->argv = t;
    c->argv[0] = NULL;
    return 0;
}

int
append_word_simple_command(Command *c, char *arg)
{
    c->argc += 1;
    char ** t = realloc(c->argv, (c->argc + 1) * sizeof arg);
    if (!t) {
        return -1;
    }
    c->argv = t;
    c->argv[c->argc - 1] = arg;
    c->argv[c->argc] = NULL;
    return 0;

}

void free_command(Command *c)
{
    int kind_free = c->kind;
    switch (kind_free) {
        case KIND_SEQ: {
            for (int i = 0; i < c->seq_size; ++i) {
                free_command(c->seq_commands + i);
            }
            free(c->seq_commands);
            free(c->seq_operations);
            break;
        }
        case KIND_PIPELINE: {
            for (int i = 0; i < c->pipeline_size; ++i) {
                free_command(c->pipeline_commands + i);
            }
            free(c->pipeline_commands);
            break;
        }
        case KIND_REDIRECT: {
            free(c->rd_path);
            free_command(c->rd_command);
            free(c->rd_command);
            break;
        }
        case KIND_SIMPLE: {
            for (int i = 0; i < c->argc; ++i) {
                free(c->argv[i]);
            }
            free(c->argv);
            break;
        }
        default:
            abort();
            break;
    }
}


