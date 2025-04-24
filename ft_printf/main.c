#include <stdio.h>
#include "ft_printf.h"
#include <limits.h>

int main()
{
	ft_printf("Test 1: %%c and %%s:\n");
	int		x;
	int		y;
	char	*s1 = NULL;
	char	*s2 = "Hello world!";
	char	*s3 = "\0test\0";
	char	*s4 = "%c %s %p %i %d %x %X %%";

	x = ft_printf("My: c's: %c %c %c %c | s's: %s %s %s %s\n", 'a', 42, '^', 198, s1, s2, s3, s4);
	y =    printf("Cs: c's: %c %c %c %c | s's: %s %s %s %s\n", 'a', 42, '^', 198, s1, s2, s3, s4);

	ft_printf("\n");

	ft_printf("Test 2: %%d/i and %%u:\n");
	
	x += ft_printf("My: di's: %d %i %d %i | u's: %u %u %u %u\n", 42, 9999, INT_MAX, INT_MIN, (unsigned int)-1, (unsigned int)(size_t)-1, (unsigned int)((unsigned int)INT_MAX + 9999), 0);
	y += printf   ("Cs: di's: %d %i %d %i | u's: %u %u %u %u\n", 42, 9999, INT_MAX, INT_MIN, (unsigned int)-1, (unsigned int)(size_t)-1, (unsigned int)((unsigned int)INT_MAX + 9999), 0);

	ft_printf("\n");

	ft_printf("Test 3: %%x and %% X\n");

	x += ft_printf("My: %x %X %x %X %x %X %x %X\n", x, x + 1, x + 2, x + 3, -1, 0, INT_MAX, INT_MIN);
	y +=    printf("Cs: %x %X %x %X %x %X %x %X\n", y, y + 1, y + 2, y + 3, -1, 0, INT_MAX, INT_MIN);
	
	ft_printf("\n");

	ft_printf("Test 4: %%p\n");

	x += ft_printf("My: %p %p %p %p\n", NULL, &x, &y, s4);
	y +=    printf("Cs: %p %p %p %p\n", NULL, &x, &y, s4);

	printf("My output: %d\nCs output: %d\n", x, y);
	return 0;
}
