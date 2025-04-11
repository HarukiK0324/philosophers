#include "philo.h"

int str_len(char *str)
{
    int	i;

    i = 0;
    if(!str)
        return 0;
    while (str[i] != '\0')
        i++;
    return (i);
}

int	to_int(char *str)
{
	int	i;
	int	itr;
	long num;

    if(str_len(str) > 10)
        return (-1);
	num = 0;
	i = 0;
	itr = 1;
	if (str[0] == '-')
	{
		itr = -1;
		i++;
	}
	while (str[i] != '\0')
	{
		num = num * 10 + itr * (str[i] - '0');
		i++;
	}
    if(num > INT32_MAX)
        return (-1);
	return (num);
}

int check_num(char *str)
{
    int i;

    i = 0;
    if(!str)
        return (-1);
    while (str[i] != '\0')
    {
        if(str[i] < '0' || str[i] > '9')
            return (-1);
        i++;
    }
    return (0);
}
