/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilmahjou <ilmahjou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:15:16 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/09 16:36:25 by ilmahjou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// in command line execcution remember to reset the sigs on every fork and make sure that the parent waits for sigs of the child

#include "minishell.h"

void	estat(int i)
{
	exit_status = i;
}

void	ctrl_c(int sig)
{
	(void)sig;
	ft_printf(1,"\n");
	estat(130);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

// void	sigquit(int sig)
// {
// 	(void)sig;
// 	ft_printf(1, "Quit (core dumped)\n");
// 	estat(131);
// }

void	set_signals(void)
{
	signal(SIGINT, ctrl_c);
	signal(SIGQUIT, SIG_IGN);
}
