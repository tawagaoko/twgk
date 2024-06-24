#ifndef COMMAND_H
#define COMMAND_H

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
init_empty_command(Command *);

int
init_sequence_command(Command *);

int
append_command_to_sequence(Command *, Command *);

int
append_operation_to_sequence(Command *, int);

int
init_pipeline_command(Command *);

int
append_to_pipeline(Command *, Command * );

int
init_redirect_command(Command *);

int
set_rd_command(Command *, Command *);

int
init_simple_command(Command *);

int
append_word_simple_command(Command *, char *);

void
free_command(Command *);

#endif
