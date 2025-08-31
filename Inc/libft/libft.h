/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalvez- <fgalvez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 18:55:21 by fgalvez-          #+#    #+#             */
/*   Updated: 2024/12/05 14:53:53 by fgalvez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <unistd.h>
# include <limits.h>

# define E_EMPTY "La cadena esta vacía"

int		ft_isdigit(int x);
int		ft_atoi(const char *nptr);
long	ft_atol(const char *nptr);
int		ft_perror(const char *str, int code);
void	ft_putstr(char *str);
int		is_number(char *av);
size_t	ft_strlen(const char *str);

#endif
