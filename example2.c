#include "set.h"

int print_str(const char *value, void *args)
{
	printf("[Foreach] value: %s\n", value);
	return 0;
}

SET_DEFINE(str, const char *)

int main(int argc, char *argv[])
{
	int i;
	int size;
	iter(str) it;
	const char *value;
	int status;
	double mean;
	double variance;

	char *test_value[] = {
		"a", "b", "c", "d", "e", "f", "g", 
		"h", "i", "j", "k", "l", "m", "n", 
		"o", "p", "q", "r", "s", "t", 
		"u", "v", "w", "x", "y", "z"};

	printf("Init\n");
	set(str) set = set_create(str);
	set_set_hash_func(str, set, str_hashcode);

	printf("Put\n");
	for (i = 0; i < 26; i++)
	{
		status = set_put(str, set, test_value[i]);
		printf("[Put] value: %s, status: %d\n", test_value[i], status);	
	}

	printf("Remove\n");

	set_remove(str, set, test_value[2]);
	printf("[Remove] value: %s\n", test_value[2]);
	set_remove(str, set, test_value[22]);
	printf("[Remove] value: %s\n", test_value[22]);

	printf("Size\n");
	size = set_size(str, set);
	printf("[Size] size: %d\n", size);

	printf("Collision\n");
	mean = set_collision_mean(str, set);
	variance = set_collision_variance(str, set);
	printf("[Collision] mean: %f, variance:%f\n", mean, variance);

	printf("Iterate\n");
	i = 0;
	for (it = set_iter(str, set); it != NULL; it = set_next(str, set, it))
	{
		set_iter_get(str, set, it, &value);
		printf("[Iterate] No.%d value: %s\n", i, value);
		i++;
		if (i > 2 * size)
			break;
	}

	printf("Foreach\n");
	set_foreach(str, set, print_str, NULL);

	printf("Has\n");
	status = set_has(str, set, test_value[1]);
	printf("[Has] value: %s, status: %d\n", test_value[1], status);
	status = set_has(str, set, test_value[2]);
	printf("[Has] value: %s, status: %d\n", test_value[2], status);
	status = set_has(str, set, test_value[3]);
	printf("[Has] value: %s, status: %d\n", test_value[3], status);

	printf("Destroy\n");
	set_destroy(str, set);

	return 0;
}