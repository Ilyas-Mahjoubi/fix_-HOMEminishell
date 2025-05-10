/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilmahjou <ilmahjou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 19:55:30 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/05/09 21:43:29 by ilmahjou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../built-ins/minishell.h"

t_token *creat_token(char *content, t_token_type type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return NULL;
	new_token->content = ft_strdup(content);
	if (!new_token->content)
	{
		free(new_token);
		return NULL;
	}
	new_token->type = type;
	new_token->next = NULL;
	return new_token;
}
t_token *free_tokens(t_token *head)
{
	t_token *tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free (tmp->content);
		free(tmp);
	}
	return NULL;
}

t_token	*get_last_token(t_token *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

/* t_token *handle_env_variable(char *input, int *i, t_token *head, t_token *current)
{
	int     start;
	char    *var_name;
	t_token *new_token;

	(*i)++; // Skip the $

	// Handle special case for $? (last exit status)
	if (input[*i] == '?')
	{
		var_name = ft_strdup("?");
		if (!var_name)
			return (free_tokens(head));
		(*i)++;
		new_token = creat_token(var_name, TOKEN_VAR);  // Create a TOKEN_VAR for the special variable
		free(var_name);
	}
	else
	{
		// Parse the variable name
		start = *i;
		//while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
			(*i)++;

		if (*i == start)
			return (head); // No valid variable, just return
		var_name = ft_substr(input, start, *i - start);
		if (!var_name)
			return (free_tokens(head));
		// Create a token for the variable (e.g., HOME, USER)
		new_token = creat_token(var_name, TOKEN_VAR);
		free(var_name);
	}
	// Add the new token to the token list
	if (!new_token)
		return (free_tokens(head));

	if (!head)
		head = new_token;
	else
		current->next = new_token;
	return (head);
} */

/* t_token *handle_env_variable(char *input, int *i, t_token *head, t_token **current_word_token)
{
	int     start;
	char    *var_name;
	char    *var_value;
	t_token *new_token;

	(*i)++; // Skip the $

	// Handle special case for $? (last exit status)
	if (input[*i] == '?')
	{
		var_name = ft_strdup("?");
		if (!var_name)
			return (free_tokens(head));
		(*i)++;

		new_token = creat_token(var_name, TOKEN_VAR);  // Your friend will handle this
		free(var_name);

		if (!new_token)
			return (free_tokens(head));

		if (!head)
			head = new_token;
		else
			get_last_token(head)->next = new_token;

		*current_word_token = NULL; // Reset current word token
		return head;
	}

	// Parse variable name
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;

	if (*i == start)
		return head; // No valid variable, just return

	var_name = ft_substr(input, start, *i - start);
	if (!var_name)
		return (free_tokens(head));

	// Retrieve value from environment (or return empty string if not found)
	var_value = getenv(var_name);  // Get the value from the environment
	free(var_name);
	if (!var_value)
		var_value = ft_strdup("");
	else
		var_value = ft_strdup(var_value);  // Copy the value

	if (!var_value)
		return (free_tokens(head));

	// If we have a variable and the next character is a slash '/', concatenate it
	if (input[*i] == '/')
	{
		char *concatenated_value = ft_strjoin(var_value, "/Desktop");  // Append /Desktop
		free(var_value);  // Free the old value
		var_value = concatenated_value;  // Now var_value holds the full path
	}

	// Add the variable's value as a new token
	new_token = creat_token(var_value, TOKEN_VAR);
	free(var_value);

	if (!new_token)
		return (free_tokens(head));

	if (!head)
		head = new_token;
	else
		get_last_token(head)->next = new_token;

	*current_word_token = NULL;
	return head;
} */

/* t_token *handle_env_variable(char *input, int *i, t_token *head, t_token *current)
{
    int     start;
    char    *var_name;
    char    *var_with_path = NULL;
    t_token *new_token;

    (*i)++; // Skip the $

    // Handle special case for $? (last exit status)
    if (input[*i] == '?')
    {
        var_name = ft_strdup("?");
        if (!var_name)
            return (free_tokens(head));
        (*i)++;
        new_token = creat_token(var_name, TOKEN_VAR);
        free(var_name);
    }
    else
    {
        // Parse the variable name
        start = *i;
        while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
            (*i)++;

        if (*i == start)
            return (head); // No valid variable, just return

        var_name = ft_substr(input, start, *i - start);
        if (!var_name)
            return (free_tokens(head));

        // Check if the variable is followed by a path
        int path_start = *i;
        if (input[*i] == '/')
        {
            // Continue until we hit a space, pipe, redirection, quote or end of string
            while (input[*i] && !ft_strchr(" \t|><\'\"$", input[*i]))
                (*i)++;

            // Get the path component
            char *path_component = ft_substr(input, path_start, *i - path_start);
            if (!path_component)
            {
                free(var_name);
                return (free_tokens(head));
            }

            // Create a token that includes both the variable name and path
            var_with_path = ft_strjoin("$", var_name);
            if (!var_with_path)
            {
                free(var_name);
                free(path_component);
                return (free_tokens(head));
            }

            char *temp = var_with_path;
            var_with_path = ft_strjoin(var_with_path, path_component);
            free(temp);
            free(path_component);

            if (!var_with_path)
            {
                free(var_name);
                return (free_tokens(head));
            }

            new_token = creat_token(var_with_path, TOKEN_VAR);
            free(var_with_path);
            free(var_name);
        }
        else
        {
            // Create a token for just the variable name
            char *var_with_dollar = ft_strjoin("$", var_name);
            if (!var_with_dollar)
            {
                free(var_name);
                return (free_tokens(head));
            }

            new_token = creat_token(var_with_dollar, TOKEN_VAR);
            free(var_with_dollar);
            free(var_name);
        }
    }

    // Add the new token to the token list
    if (!new_token)
        return (free_tokens(head));

    if (!head)
        head = new_token;
    else
        current->next = new_token;

    return (head);
} */

/* t_token *handle_env_variable(char *input, int *i, t_token *head, t_token **current_word_token)
{
	int     start;
	char    *var_name;
	char    *var_value;

	(*i)++; // Skip the $

	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;

	if (*i == start)
		return head; // No valid variable name after $

	var_name = ft_substr(input, start, *i - start);
	if (!var_name)
		return free_tokens(head);

	var_value = getenv(var_name); // Or your custom env getter
	free(var_name);

	if (!var_value)
		var_value = ft_strdup(""); // Expand to empty if not found
	else
		var_value = ft_strdup(var_value); // Make a safe copy

	if (!var_value)
		return free_tokens(head);

	head = join_word_segment(var_value, head, current_word_token);
	return head;
} */

t_token *handle_env_variable(char *input, int *i, t_token *head, t_token **current_word_token, t_info *info)
{
	int		start;
	char	*var_name;
	char	*var_value;

	(*i)++; // Skip the $

	// Handle special case for $? (last exit status)
	if (input[*i] == '?')
	{
		(*i)++; // Skip the '?'

		// Use mdollar function to get exit status
		var_name = ft_strdup("?");
		if (!var_name)
			return free_tokens(head);

		var_value = mdollar(var_name, info);

		free(var_name);

		if (!var_value)
			var_value = ft_strdup(""); // Empty string if not found

		if (!var_value)
			return free_tokens(head);

		head = join_word_segment(var_value, head, current_word_token);
		return head;
	}
	// Handle regular environment variables
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;

	if (*i == start)
		return head; // No valid variable name after $

	var_name = ft_substr(input, start, *i - start);
	if (!var_name)
		return free_tokens(head);

	// Use mdollar function for variable expansion
	var_value = mdollar(var_name, info);
	free(var_name);

	if (!var_value)
		var_value = ft_strdup(""); // Expand to empty if not found

	if (!var_value)
		return free_tokens(head);

	head = join_word_segment(var_value, head, current_word_token);
	return head;
}

////////////////////////////////////////////////////////////////////////
// void free_commands(t_command *head)
// {
// 	t_command *tmp;

// 	while (head) {
// 		tmp = head;
// 		head = head->next;

// 		// Free arguments
// 		if (tmp->args) {
// 			for (int i = 0; tmp->args[i]; i++)
// 				free(tmp->args[i]);
// 			free(tmp->args);
// 		}

// 		// Free redirections (to be implemented)

// 		free(tmp);
// 	}
// }
