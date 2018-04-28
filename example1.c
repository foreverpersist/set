#include "set.h"

int hash_int(int entity)
{
	return entity;
}

int print_int(int entity, void *args)
{
	printf("[Entity]: %d\n", entity);
	return 0;
}

SET_DEFINE(int, int)

int main(int argc, char *argv[])
{
	int i;
	int size;
	iter(int) it;
	int entity;
	int status;
	double mean;
	double variance;

	printf("Init\n");
	set(int) set = set_create(int);
	set_set_hash_func(int, set, hash_int);

	printf("Put\n");
	for (i = 0; i < 20; i++)
	{
		entity = rand() % 10;
		status = set_put(int, set, entity);
		printf("[Put] entity: %d, status: %d\n", entity, status);	
	}

	printf("Remove\n");

	set_remove(int, set, 3);
	printf("[Remove] entity: %d\n", 3);

	printf("Size\n");
	size = set_size(int, set);
	printf("[Size] size: %d\n", size);

	printf("Collision\n");
	mean = set_collision_mean(int, set);
	variance = set_collision_variance(int, set);
	printf("[Collision] mean: %f, variance:%f\n", mean, variance);

	printf("Iterate\n");
	i = 0;
	for (it = set_iter(int, set); it != NULL; it = set_next(int, set, it))
	{
		set_iter_get(int, set, it, &entity);
		printf("[Iterate] No.%d entity: %d\n", i, entity);
		i++;
		if (i >= 2 * size)
			break;
	}

	printf("Foreach\n");
	set_foreach(int, set, print_int, NULL);

	printf("Has\n");
	status = set_has(int, set, 1);
	printf("[Has] entity: %d, status: %d\n", 1, status);
	status = set_has(int, set, 3);
	printf("[Has] entity: %d, status: %d\n", 3, status);
	status = set_has(int, set, 10);
	printf("[Has] entity: %d, status: %d\n", 10, status);

	printf("Destroy\n");
	set_destroy(int, set);

	return 0;
}