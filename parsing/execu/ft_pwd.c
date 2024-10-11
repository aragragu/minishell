/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:33:05 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/11 00:21:44 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		ft_fprintf(2, "getcwd() : failed\n");
		g_es(1, 0);
		return ;
	}
	ft_fprintf(2, "%s\n", cwd);
	g_es(0, 0);
}
