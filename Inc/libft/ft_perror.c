/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalvez- <fgalvez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 21:16:53 by fgalvez-          #+#    #+#             */
/*   Updated: 2024/12/05 13:54:38 by fgalvez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_perror(const char *str, int code)
{
	const char	*prefix = "Error: ";
	const char	nl = '\n';

	if (str)
	{
		write(2, prefix, 7);
		write(2, str, ft_strlen(str));
		write(2, &nl, 1);
	}
	return (code);
}
