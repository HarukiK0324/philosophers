/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:48:07 by haruki            #+#    #+#             */
/*   Updated: 2025/04/11 09:43:38 by hkasamat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

int		ft_printf(const char *format, ...);
int		ft_print_char(int c);
int		ft_print_string(char *c);
int		ft_strlen(const char *s);
int		ft_print_num(int num);
int		ft_print_pointer(void *ptr);
int		ft_print_unsigned_num(unsigned int num);
int		ft_print_hex(unsigned int num,char c);
int		hex_size(unsigned int num);
int		ft_digits(int num);
int		ft_unsigned_digits(unsigned int num);
void	ft_print_hex_pointer(unsigned long ptr);
int		ft_pointer_digits(unsigned long ptr);

#endif
