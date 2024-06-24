#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "command.h"

int
init_parser(FILE *);

void
free_parser(void);

int
next_command(Command *);

#endif
