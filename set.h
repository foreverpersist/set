#ifndef __SET_H
#define __SET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 0
#define ERR_SYS -1
#define ERR_USER -2
#define ERR_EMPTY -3

#define set_log(stream, format, args...) \
	fprintf(stream, "%s: %d: %s: " format, __FILE__, __LINE__, __func__, ##args); 

#define set_error(format, args...) set_log(stderr, format, ##args)

#define set(name) name##_set
#define iter(name) name##_iter

#define set_create(name) \
	name##_set_create()
#define set_destroy(name, set) \
	name##_set_destroy(&set)
#define set_set_hash_func(name, set, hash_func) \
	name##_set_set_hash_func(set, hash_func)
#define set_compare_func(name, set, compare_func) \
	name##_set_set_compare_func(set, compare_func)
#define set_has(name, set, value) \
	name##_set_has(set, value)
#define set_put(name, set, value) \
	name##_set_put(set, value)
#define set_remove(name, set, value) \
	name##_set_remove(set, value)
#define set_size(name, set) \
	name##_set_size(set)
#define set_collision_mean(name, set) \
	name##_set_collision_mean(set)
#define set_collision_variance(name, set) \
	name##_set_collision_variance(set)
#define set_iter(name, set) \
	name##_set_iter(set)
#define set_next(name, set, it) \
	name##_set_next(set, it)
#define set_iter_get(name, set, it, pvalue) \
	name##_set_iter_get(set, it, pvalue)
#define set_foreach(name, set, func, args) \
	name##_set_foreach(set, func, args)

/* All structures and functions are implemented here, 
   because maybe only `macro` can be used for any data type.
*/
#define SET_DEFINE(name, value_type) \
	\
	/* Data Entity, member 'next' is a Data Entity pointer. */\
	typedef struct name##_ENTITY \
	{ \
		value_type value; \
		struct name##_ENTITY *next; \
	} name##_entity, *name##_iter; \
	\
	/* Binary Tree Node */\
	typedef struct name##_NODE \
	{ \
		int hashcode; \
		size_t collision; \
		name##_iter entities; \
		struct name##_NODE *parent; \
		struct name##_NODE *left; \
		struct name##_NODE *right; \
	} name##_node, *name##_tree; \
	\
	/* Set Entity */\
	typedef struct \
	{ \
		size_t size; \
		size_t node_size; \
		name##_tree tree; \
		int (*hash_func)(value_type); \
		int (*compare_func)(value_type, value_type); \
	} name##_set_entity, *name##_set; \
	\
	/* Generate hashcode from raw value.
	   If function `hash_func` is given, just use it. Otherwise, use `0`.
	 */\
	inline static int name##_real_hash(int (*hash_func)(value_type), \
		value_type value) \
	{ \
		if (hash_func) \
			return hash_func(value); \
		/* set_error("Error [%2d]: Can not find valid hash function, use `0` instead\n", ERR_USER) */ \
		return 0;\
	} \
	\
	/* Compare two values
	   If function `compare_func` is given, just use it. Otherwise, use `value1 - value2`.
	 */\
	inline static int name##_real_compare(int (*compare_func)(value_type, value_type), \
		value_type value1, value_type value2) \
	{ \
		if (compare_func) \
			return compare_func(value1, value2); \
		/* set_error("Error [%2d]: Can not find valid compare function, use `value1 - value2` instead\n", ERR_USER) */ \
		return value1 - value2; \
	} \
	\
	/* Set hash function.
	   It requires set is clean.
	 */\
	int name##_set_set_hash_func(name##_set set, \
		int (*hash_func)(value_type)) \
	{ \
		if (!set) \
		{ \
			set_error("Error [%2d]: Invalid set\n", ERR_USER) \
		} \
		if (set->tree && set->size > 0) \
		{ \
			set_error("Error [%2d]: Set is not clean\n", ERR_USER) \
		} \
		set->hash_func = hash_func; \
		return OK; \
	} \
	\
	/* Set compare function.
	   If requires set is clean.
	 */\
	int name##_set_set_compare_func(name##_set set, \
		int (*compare_func)(value_type, value_type)) \
	{ \
		if (!set) \
		{ \
			set_error("Error [%2d]: Invalid set\n", ERR_USER) \
		} \
		if (set->tree && set->size > 0) \
		{ \
			set_error("Error [%2d]: Set is not clean\n", ERR_USER) \
		} \
		set->compare_func = compare_func; \
		return OK; \
	} \
	\
	/* Find min tree node
	 */\
	static name##_tree find_min(name##_tree tree) \
	{ \
		if (!tree) \
			return NULL; \
		name##_tree tr = tree; \
		name##_tree parent = NULL; \
		while (tr) \
		{ \
			parent = tr; \
			tr = tr->left; \
		} \
		return parent; \
	} \
	\
	/* Find max tree node */\
	static name##_tree find_max(name##_tree tree) \
	{ \
		if (!tree) \
			return NULL; \
		name##_tree tr = tree; \
		name##_tree parent = NULL; \
		while (tr) \
		{ \
			parent = tr; \
			tr = tr->right; \
		} \
		return parent; \
	} \
	\
	/* Find next tree node
	 */\
	static name##_tree find_next(name##_tree tree) \
	{ \
		if (!tree) \
			return NULL; \
		if (tree->right) \
			return find_min(tree->right); \
		name##_tree next = tree->parent; \
		while (next) \
		{ \
			if (next->hashcode > tree->hashcode) \
				break; \
			next = next->parent; \
		} \
		return next; \
	} \
	\
	/* Create set.
	 */\
	name##_set name##_set_create() \
	{ \
		name##_set set = NULL; \
		set = (name##_set) malloc(sizeof(name##_set_entity)); \
		if (!set) \
		{ \
			set_error("Error [%2d]: Can not allocate memory for new set\n",ERR_SYS) \
			return NULL; \
		} \
		set->tree = NULL; \
		set->size = 0; \
		set->node_size = 0; \
		set->hash_func = NULL; \
		return set; \
	} \
	\
	/* Destroy set.
	 */\
	int name##_set_destroy(name##_set *pset) \
	{ \
		if (!(*pset)) \
		{ \
			set_error("Error [%2d]: Invalid set\n", ERR_USER) \
			return ERR_USER; \
		} \
		if (!(*pset)->tree || !(*pset)->size) \
		{ \
			free(*pset); \
			*pset = NULL; \
			return OK; \
		} \
		name##_tree trs[(*pset)->node_size]; \
		name##_tree tr = find_min((*pset)->tree); \
		size_t i = 0; \
		while (tr) \
		{ \
			trs[i] = tr; \
			i++; \
			tr = find_next(tr); \
		} \
		name##_iter it; \
		name##_iter prev; \
		for (i = 0; i < (*pset)->node_size; i++) \
		{ \
			it = trs[i]->entities; \
			while (it) \
			{ \
				prev = it; \
				it = it->next; \
				free(prev); \
			} \
			free(trs[i]); \
		} \
		free(*pset); \
		*pset = NULL; \
		return OK; \
	} \
	\
	/* Check whether `value` exists.
	 */\
	int name##_set_has(name##_set set, value_type value) \
	{ \
		if (!set) \
		{ \
			set_error("Error  [%2d]: Invalid set\n", ERR_USER) \
			return ERR_USER; \
		} \
		int hashcode = name##_real_hash(set->hash_func, value); \
		name##_tree tr = set->tree; \
		name##_iter it; \
		while (tr) \
		{ \
			if (hashcode == tr->hashcode) \
			{ \
				it = tr->entities; \
				while (it) \
				{ \
					if (!name##_real_compare(set->compare_func, it->value, value)) \
						return OK; \
					it = it->next; \
				} \
				return ERR_EMPTY; \
			} \
			else if (hashcode < tr->hashcode) \
				tr = tr->left; \
			else \
				tr = tr->right; \
		} \
		return ERR_EMPTY; \
	} \
	\
	/* Put an value.
	 */\
	int name##_set_put(name##_set set, value_type value) \
	{ \
		if (!set) \
		{ \
			set_error("Error  [%2d]: Invalid set\n", ERR_USER) \
			return ERR_USER; \
		} \
		int hashcode = name##_real_hash(set->hash_func, value); \
		name##_tree tr = set->tree; \
		name##_tree parent = NULL; \
		name##_iter prev; \
		name##_iter it; \
		while (tr) \
		{ \
			if (hashcode == tr->hashcode) \
			{ \
				it = tr->entities; \
				while (it) \
				{ \
					if (!name##_real_compare(set->compare_func, it->value, value)) \
						return ERR_USER; \
					prev = it; \
					it = it->next; \
				} \
				it = (name##_iter) malloc(sizeof(name##_entity)); \
				it->value = value; \
				it->next = NULL; \
				prev->next = it; \
				tr->collision++; \
				set->size++; \
				return OK; \
			} \
			else if (hashcode < tr->hashcode) \
			{ \
				parent = tr; \
				tr = tr->left; \
			} \
			else \
			{ \
				parent = tr; \
				tr = tr->right; \
			} \
		} \
		it = (name##_iter) malloc(sizeof(name##_entity)); \
		it->value = value; \
		it->next = NULL; \
		tr = (name##_tree) malloc(sizeof(name##_node)); \
		tr->hashcode = hashcode; \
		tr->entities = it; \
		tr->collision++; \
		tr->parent = parent; \
		tr->left = NULL; \
		tr->right = NULL; \
		if (!parent) \
			set->tree = tr; \
		else if (hashcode < parent->hashcode) \
			parent->left = tr; \
		else \
			parent->right = tr; \
		set->size++; \
		set->node_size++; \
		return OK; \
	} \
	\
	/* Remove an value
	 */\
	int name##_set_remove(name##_set set, value_type value) \
	{ \
		if (!set) \
		{ \
			set_error("Error  [%2d]: Invalid set\n", ERR_USER) \
			return ERR_USER; \
		} \
		int hashcode = name##_real_hash(set->hash_func, value); \
		name##_tree tr = set->tree; \
		name##_tree parent = NULL; \
		name##_tree cur = NULL; \
		name##_iter it; \
		name##_iter prev = NULL; \
		while (tr) \
		{ \
			if (hashcode == tr->hashcode) \
			{ \
				it = tr->entities; \
				while (it) \
				{ \
					if (!name##_real_compare(set->compare_func, it->value, value)) \
					{ \
						/* OK, `value` exists. */ \
						if (prev) \
						{ \
							/* The tree node has other entities left. Keep it*/ \
							prev->next = it->next; \
							tr->collision--; \
						} \
						else \
						{ \
							/* The tree node has nothing left. Remove it*/ \
							parent = tr->parent; \
							if (tr->left && !tr->right) \
							{ \
								/* it has only left child */ \
								printf("[%d] has only left child\n", tr->hashcode); \
								cur = tr->left; \
							} \
							else if (!tr->left && tr->right) \
							{ \
								/* it has only right child */ \
								printf("[%d] has only right child\n", tr->hashcode); \
								cur = tr->right; \
							} \
							else if (tr->left && tr->right) \
							{ \
								/* it has two children */ \
								printf("[%d] has two children\n", tr->hashcode); \
								cur = find_max(tr->left); \
								if (cur != tr->left) \
								{ \
									cur->parent->right = cur->left; \
									if (cur->left) \
										cur->left->parent = cur->parent; \
									tr->left->parent = cur; \
									tr->right->parent = cur; \
									cur->left = tr->left; \
								} \
								cur->right = tr->right; \
							} \
							if (cur) \
							{ \
								cur->parent = parent; \
								printf("Find cur: %d\n", cur->hashcode); \
							} \
							if (!parent) \
							{ \
								set->tree = cur; \
								printf("Put cur as root\n"); \
							} \
							else if (hashcode < parent->hashcode) \
							{ \
								parent->left = cur; \
								printf("Put cur left of parent[%d]\n", parent->hashcode); \
							} \
							else \
							{ \
								parent->right = cur; \
								printf("Put cur right of parent[%d]\n", parent->hashcode); \
							} \
							free(tr); \
							set->node_size--; \
						} \
						free(it); \
						set->size--; \
						return OK; \
					} \
					prev = it; \
					it = it->next; \
				} \
				break; \
			} \
			else if (hashcode < tr->hashcode) \
				tr = tr->left; \
			else \
				tr = tr->right; \
		} \
		return ERR_USER; \
	} \
	\
	/* Get set size.
	 */\
	int name##_set_size(name##_set set) \
	{ \
		if (!set) \
		{ \
			set_error("Error  [%2d]: Invalid set\n", ERR_USER) \
			return ERR_USER; \
		} \
		return set->size; \
	} \
	\
	/* Get set collision mean
	 */\
	double name##_set_collision_mean(name##_set set) \
	{ \
		if (!set) \
		{ \
			set_error("Error [%2d]: Invalid set\n", ERR_USER) \
		} \
		if (!set->node_size) \
			return 0.0; \
		long total = 0; \
		name##_tree tr = set->tree; \
		while (tr) \
		{ \
			total += tr->collision - 1; \
			tr = find_next(tr); \
		} \
		return 1.0 * total / set->node_size; \
	} \
	\
	/* Get set collision variance
	 */\
	double name##_set_collision_variance(name##_set set) \
	{ \
		if (!set) \
		{ \
			set_error("Error [%2d]: Invalid set\n", ERR_USER) \
		} \
		if (!set->node_size) \
			return 0.0; \
		long sum = 0; \
		double mean; \
		double diff; \
		double total = 0; \
		size_t i = 0; \
		double collisions[set->node_size]; \
		name##_tree tr = set->tree; \
		while (tr) \
		{ \
			collisions[i] = tr->collision - 1; \
			sum += collisions[i]; \
			i++; \
			tr = find_next(tr); \
		} \
		mean = 1.0 * sum / set->node_size; \
		for (i = 0; i < set->node_size; i++) \
		{ \
			diff = collisions[i] - mean; \
			total += diff * diff; \
		} \
		return total / set->node_size; \
	} \
	\
	/* Get set iterator.
	 */\
	name##_iter name##_set_iter(name##_set set) \
	{ \
		if (!set) \
		{ \
			set_error("Error  [%2d]: Invalid set\n", ERR_USER) \
			return NULL; \
		} \
		name##_tree tr = find_min(set->tree); \
		if (!tr) \
			return NULL; \
		return tr->entities; \
	} \
	\
	/* Get next iterator.
	   [Warn] If current iterator has been freed, unkown error will happen.
	 */\
	name##_iter name##_set_next(name##_set set, name##_iter it) \
	{ \
		if (!set || !it) \
		{ \
			set_error("Error  [%2d]: Invalid set\n", ERR_USER) \
			return NULL; \
		} \
		if (it->next) \
			return it->next; \
		int hashcode = name##_real_hash(set->hash_func, it->value); \
		name##_tree tr = set->tree; \
		name##_tree next; \
		while (tr) \
		{ \
			if (hashcode == tr->hashcode) \
				break; \
			else if (hashcode < tr->hashcode) \
				tr = tr->left; \
			else \
				tr = tr->right; \
		} \
		tr = find_next(tr); \
		if (!tr) \
			return NULL; \
		return tr->entities; \
	} \
	\
	/* Get value of current iterator.
	   [Warn] If current iterator has been freed, unkonwn error will happen.
	 */\
	name##_set_iter_get(name##_set set, name##_iter it, value_type *pvalue) \
	{ \
		if (!it) \
		{ \
			set_error("Error [%2d]: Invalid iterator\n", ERR_USER) \
			return ERR_USER; \
		} \
		if (pvalue) \
			*pvalue = it->value; \
		return OK; \
	} \
	\
	/* Do foreach with specific function `func` and optional extral arguments.
	   Exec `func(entity, args)` for each entity.
	   If set size if changed during foreach, it will stop immediately.
	 */\
	int name##_set_foreach(name##_set set, \
		int (*func)(value_type, void *), void *args) \
	{ \
		if (!set) \
		{ \
			set_error("Error  [%2d]: Invalid set\n", ERR_USER) \
			return ERR_USER; \
		} \
		if (!func) \
		{ \
			set_error("Error [%2d]: Invalid func\n", ERR_USER); \
		} \
		name##_tree tr = find_min(set->tree); \
		name##_tree next; \
		name##_iter it; \
		size_t size = set->size; \
		while (tr) \
		{ \
			it = tr->entities; \
			while (it) \
			{ \
				if (func(it->value, args) || set->size != size) \
				{ \
					set_error("Error [%2d]: User requests to quit, or hashmap has been changed\n", ERR_USER) \
					return ERR_USER; \
				} \
				it = it->next; \
			} \
			tr = find_next(tr); \
		} \
		return OK; \
	} \

/* Some default functions */
int str_hashcode(const char *value)
{
	char *p = (char *)value;
	int hash = 0;
	for (p; *p; p++)
	{
		hash += *p;
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

#endif
