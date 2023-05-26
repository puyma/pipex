#include <stdio.h>

int	print_arr(int *arr, int len);

int
main(void)
{
	print_arr([0, 1, 2, 3], 4);
	return (0);
}

int
print_arr(int *arr, int len)
{
	int	i;

	i = 0;
	while (i++ < len)
		printf("arr[%d]: %d\n", i, arr[i]);
	return (0);
}
