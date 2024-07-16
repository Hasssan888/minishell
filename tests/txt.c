#include <stdlib.h> // For malloc, free
#include <string.h> // For strdup

#include "../libraries/minishell.h"

t_command *get_command_with_args() {
    int i = 0;
    size_t args_size = get_args_size(data->_tokens_list);

    data->list_command->args = malloc((args_size + 2) * sizeof(char *));
    if (!data->list_command->args) {
        // Handle malloc failure
        return NULL;
    }

    while (data->_tokens_list != NULL && data->_tokens_list->type != PIPE) {
        data->list_command->args[i++] = ft_strdup(data->_tokens_list->value);
        data->list_command->args[i] = NULL;
        data->_tokens_list = free_node(data->_tokens_list);
        while (data->_tokens_list != NULL && (data->_tokens_list->type >= 2 && data->_tokens_list->type <= 5)) {
            data->_tokens_list = redirect_list(&data->head, &data->rdrct_head);
            if (data->syntax_error) {
                // Free allocated memory before returning
                free_command(data->list_command);
                return NULL;
            }
        }
    }
    return data->list_command;
}

void init_parser_var() {
    data->list_command = new_node(data->_tokens_list->type, ft_strdup(data->_tokens_list->value));
    if (!data->list_command) {
        // Handle malloc failure
        return;
    }
    data->list_command->args = NULL;
    data->rdrct_head = NULL;
}

t_command *parser_command(t_command *_tokens_list) {
    data->head = NULL;
    data->_tokens_list = _tokens_list;
    while (data->_tokens_list != NULL) {
        if (data->syntax_error) {
            return NULL;
        }
        init_parser_var();
        if (!data->list_command) {
            // Handle initialization failure
            clear_list(&data->head);
            return NULL;
        }
        if (data->_tokens_list->type == TOKEN) {
            if (!get_command_with_args()) {
                clear_list(&data->head);
                return NULL;
            }
        } else if (data->_tokens_list->type >= 2 && data->_tokens_list->type <= 5) {
            get_redirect_node(data->list_command);
        } else {
            data->_tokens_list = free_node(data->_tokens_list);
            if (!data->_tokens_list) {
                syntax_error(data->list_command, data->head);
                return NULL;
            }
        }
        add_back_list(&data->head, data->list_command);
        add_back_list(&data->head, data->rdrct_head);
    }
    return data->head;
}

void free_command(t_command *command) {
    if (command) {
        if (command->args) {
            for (int i = 0; command->args[i] != NULL; i++) {
                free(command->args[i]);
            }
            free(command->args);
        }
        free(command->value); // Assuming command has a value field
        free(command);
    }
}
