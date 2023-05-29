#include "libft.h"
#include <stdio.h>

int
main(void)
{
	char *src = "abc def";
	char	*test = ft_strndup(src, 3);
	printf("%s\n", test);
	return (0);
}
