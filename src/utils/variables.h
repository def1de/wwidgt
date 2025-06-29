#ifndef VARIABLES_H
#define VARIABLES_H

#include <libxml/parser.h>

/// Structure for a variable
typedef struct {
    char* name;
    char* value;
} Variable;

#define MAX_VARIABLES 128

extern Variable variables[MAX_VARIABLES];
extern int variable_count;

/// Accessor for variable value
const char* get_variable_value(const char* name);
/// Free all allocated memory
void free_variables(void);
/// Parse <variables> node from layout
void parse_variables_node(xmlNode* variables_node);

#endif //VARIABLES_H
