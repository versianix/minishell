#include "../include/utils.h"

char *trim_whitespaces(char *token) {
    // Remove leading whitespace characters
    while (*token && isspace(*token)) {
        token++;
    }

    // Remove trailing whitespace characters
    char* end = token + strlen(token) - 1;
    while (end > token && isspace(*end)) {
        end--;
    }
    *(end + 1) = '\0';

    return token;
}

void parse_command(char *cmd, t_command *parsed_cmd) {
    memset(parsed_cmd, 0, sizeof(t_command)); // inicialize struct with zeros

    char *token = strtok(cmd, " \n");
    int arg_index = 0;

    while (token != NULL && arg_index < MAX_ARG - 1) {
        if (strcmp(token, "<") == 0) {  // input redirection
            token = strtok(NULL, " \n");
            if (token) parsed_cmd->infile = strdup(token);
        }
        else if (strcmp(token, ">") == 0) {  // output redirection (truncate)
            token = strtok(NULL, " \n");
            if (token) {
                parsed_cmd->outfile = strdup(token);
                parsed_cmd->append = false;
            }
        }
        else if (strcmp(token, ">>") == 0) {  // output redirection (append)
            token = strtok(NULL, " \n");
            if (token) {
                parsed_cmd->outfile = strdup(token);
                parsed_cmd->append = true;
            }
        }
        else {  // command arguments
            parsed_cmd->argv[arg_index++] = token;
        }
        token = strtok(NULL, " \n");
    }
    parsed_cmd->argv[arg_index] = NULL;
}
