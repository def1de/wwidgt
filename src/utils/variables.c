#include "variables.h"
#include <stdlib.h>
#include <string.h>
#include "logging.h"

Variable variables[MAX_VARIABLES];
int variable_count = 0;

void parse_variables_node(xmlNode* node) {
    variable_count = 0;

    for (xmlNode* child = node->children; child; child = child->next) {
        if (child->type != XML_ELEMENT_NODE || strcmp((const char*)child->name, "var") != 0)
            continue;

        xmlChar* name = xmlGetProp(child, (const xmlChar*)"name");
        xmlChar* data = xmlGetProp(child, (const xmlChar*)"data");

        if (name && data && variable_count < MAX_VARIABLES) {
            variables[variable_count].name = strdup((const char*)name);
            variables[variable_count].value = strdup((const char*)data);
            variable_count++;
        }

        if (name) xmlFree(name);
        if (data) xmlFree(data);
    }
    if (variable_count == 0) {
        fprintf(stderr, "No variables found in <variables> node.\n");
    } else {
        log_printf("================\n");
        log_printf("Collected %d variables from <variables> node.\n", variable_count);
        for (int i = 0; i < variable_count; ++i) {
            log_printf("\tVariable %d: %s = %s\n", i+1, variables[i].name, variables[i].value);
        }
        log_printf("================\n");
    }
}

const char* get_variable_value(const char* name) {
    for (int i = 0; i < variable_count; ++i) {
        if (strcmp(variables[i].name, name) == 0)
            return variables[i].value;
    }
    return "";
}

int set_variable_value(const char* name, const char* value) {
    for (int i = 0; i < variable_count; ++i) {
        if (strcmp(variables[i].name, name) == 0) {
            free(variables[i].value);
            variables[i].value = strdup(value);
            log_printf("Variable %s set to %s\n", name, value);
            return 1;
        }
    }
    log_printf("Variable %s wasn't initialised at the config\n", name);
    return 0;
}

void free_variables(void) {
    for (int i = 0; i < variable_count; ++i) {
        free(variables[i].name);
        free(variables[i].value);
    }
    variable_count = 0;
}
