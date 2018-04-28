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
#define set_has(name, set, entity) \
	name##_set_has(set, entity)
#define set_put(name, set, entity) \
	name##_set_put(set, entity)
#define set_remove(name, set, entity) \
	name##_set_remove(set, entity)
#define set_size(name, set) \
	name##_set_size(set)
#define set_iter(name, set) \
	name##_set_iter(set)
#define set_next(name, set, it) \
	name##_set_next(set, it)
#define set_iter_get(name, set, it, pentity) \
	name##_set_iter_get(set, it, pentity)
#define set_foreach(name, set, func, args) \
	name##_set_foreach(set, func, args)

/* All structures and functions are implemented here, 
   because maybe only `macro` can be used for any data type.
*/
#define SET_DEFINE(name, entity_type) \
	\
	/* Binary Tree Node */\
	typedef struct name##_NODE \
	{ \
		int hashcode; \
		entity_type entity; \
		struct name##_NODE *parent; \
		struct name##_NODE *left; \
		struct name##_NODE *right; \
	} name##_node, *name##_iter; \
	\
	/* Set Entity */\
	typedef struct \
	{ \
		size_t size; \
		name##_node *tree; \
		int (*hash_func)(entity_type); \
	} name##_set_entity, *name##_set; \
	\
	/* Set hash function.
	   It requires set is clean.
	 */\
	int name##_set_set_hash_func(name##_set set, \
		int (*hash_func)(entity_type)) \
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
	static name##_iter find_min(name##_iter tree) \
	{ \
		if (!tree) \
			return NULL; \
		name##_iter it = tree; \
		name##_iter parent = NULL; \
		while (it) \
		{ \
			parent = it; \
			it = it->left; \
		} \
		return parent; \
	} \
	\
	static name##_iter find_max(name##_iter tree) \
	{ \
		if (!tree) \
			return NULL; \
		name##_iter it = tree; \
		name##_iter parent = NULL; \
		while (it) \
		{ \
			parent = it; \
			it = it->right; \
		} \
		return parent; \
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
		if (!(*pset)->hash_func || !(*pset)->tree || !(*pset)->size) \
		{ \
			free(*pset); \
			*pset = NULL; \
			return OK; \
		} \
		name##_iter its[(*pset)->size]; \
		name##_iter it = find_min((*pset)->tree); \
		size_t i = 0; \
		while (it) \
		{ \
			its[i] = it; \
			i++; \
			if (it->right) \
				it = find_min(it->right); \
			else if (it->parent && it->parent->hashcode > it->hashcode) \
				it = it->parent; \
			else \
				break; \
		} \
		for (i = 0; i < (*pset)->size; i++) \
		{ \
			free(its[i]); \
		} \
		free(*pset); \
		*pset = NULL; \
		return OK; \
	} \
	\
	/* Check whether `entity` exists.
	 */\
	int name##_set_has(name##_set set, entity_type entity) \
	{ \
		if (!set || !set->hash_func) \
		{ \
			set_error("Error  [%2d]: Invalid set\n", ERR_USER) \
			return ERR_USER; \
		} \
		int hashcode = set->hash_func(entity); \
		name##_iter it = set->tree; \
		while (it) \
		{ \
			if (hashcode == it->hashcode) \
				return OK; \
			else if (hashcode < it->hashcode) \
				it = it->left; \
			else \
				it = it->right; \
		} \
		return ERR_EMPTY; \
	} \
	\
	/* Put an entity.
	 */\
	int name##_set_put(name##_set set, entity_type entity) \
	{ \
		if (!set || !set->hash_func) \
		{ \
			set_error("Error  [%2d]: Invalid set\n", ERR_USER) \
			return ERR_USER; \
		} \
		int hashcode = set->hash_func(entity); \
		name##_iter it = set->tree; \
		name##_iter parent = NULL; \
		while (it) \
		{ \
			if (hashcode == it->hashcode) \
				return ERR_USER; \
			else if (hashcode < it->hashcode) \
			{ \
				parent = it; \
				it = it->left; \
			} \
			else \
			{ \
				parent = it; \
				it = it->right; \
			} \
		} \
		name##_iter p = (name##_node *) malloc(sizeof(name##_node)); \
		p->hashcode = hashcode; \
		p->entity = entity; \
		p->parent = parent; \
		p->left = NULL; \
		p->right = NULL; \
		if (!parent) \
			set->tree = p; \
		else if (hashcode < parent->hashcode) \
			parent->left = p; \
		else \
			parent->right = p; \
		set->size++; \
		return OK; \
	} \
	\
	/* Remove an entity
	 */\
	int name##_set_remove(name##_set set, entity_type entity) \
	{ \
		if (!set || !set->hash_func) \
		{ \
			set_error("Error  [%2d]: Invalid set\n", ERR_USER) \
			return ERR_USER; \
		} \
		int hashcode = set->hash_func(entity); \
		name##_iter it = set->tree; \
		name##_iter parent = NULL; \
		name##_iter cur; \
		while (it) \
		{ \
			if (hashcode == it->hashcode) \
			{ \
				parent = it->parent; \
				if (it->left && !it->right) \
				{ \
					/* it has only left child */ \
					printf("[%d] has only left child\n", it->hashcode); \
					cur = it->left; \
				} \
				else if (!it->left && it->right) \
				{ \
					/* it has only right child */ \
					printf("[%d] has only right child\n", it->hashcode); \
					cur = it->right; \
				} \
				else if (it->left && it->right) \
				{ \
					/* it has two children */ \
					printf("[%d] has two children\n", it->hashcode); \
					cur = find_max(it->left); \
					if (cur != it->left) \
					{ \
						cur->parent->right = cur->left; \
						if (cur->left) \
							cur->left->parent = cur->parent; \
						it->left->parent = cur; \
						it->right->parent = cur; \
						cur->left = it->left; \
					} \
					cur->right = it->right; \
				} \
				cur->parent = parent; \
				printf("Find cur: %d\n", cur->hashcode); \
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
				free(it); \
				set->size--; \
				return OK; \
			} \
			else if (hashcode < it->hashcode) \
				it = it->left; \
			else \
				it = it->right; \
		} \
		return ERR_USER; \
	} \
	\
	/* Get set size.
	 */\
	int name##_set_size(name##_set set) \
	{ \
		if (!set || !set->hash_func) \
		{ \
			set_error("Error  [%2d]: Invalid set\n", ERR_USER) \
			return ERR_USER; \
		} \
		return set->size; \
	} \
	\
	/* Get set iterator.
	 */\
	name##_iter name##_set_iter(name##_set set) \
	{ \
		if (!set || !set->hash_func) \
		{ \
			set_error("Error  [%2d]: Invalid set\n", ERR_USER) \
			return NULL; \
		} \
		return find_min(set->tree); \
	} \
	\
	/* Get next iterator.
	   [Warn] If current iterator has been freed, unkown error will happen.
	 */\
	name##_iter name##_set_next(name##_set set, name##_iter it) \
	{ \
		if (!set || !set->hash_func || !it) \
		{ \
			set_error("Error  [%2d]: Invalid set\n", ERR_USER) \
			return NULL; \
		} \
		if (it->right) \
			return find_min(it->right); \
		if (it->parent && it->parent->hashcode > it->hashcode) \
			return it->parent; \
		return NULL; \
	} \
	\
	/* Get entity of current iterator.
	   [Warn] If current iterator has been freed, unkonwn error will happen.
	 */\
	name##_set_iter_get(name##_set set, name##_iter it, entity_type *pentity) \
	{ \
		if (!it) \
		{ \
			set_error("Error [%2d]: Invalid iterator\n", ERR_USER) \
			return ERR_USER; \
		} \
		if (pentity) \
			*pentity = it->entity; \
		return OK; \
	} \
	\
	/* Do foreach with specific function `func` and optional extral arguments.
	   Exec `func(entity, args)` for each entity.
	   If set size if changed during foreach, it will stop immediately.
	 */\
	int name##_set_foreach(name##_set set, \
		int (*func)(entity_type, void *), void *args) \
	{ \
		if (!set || !set->hash_func) \
		{ \
			set_error("Error  [%2d]: Invalid set\n", ERR_USER) \
			return ERR_USER; \
		} \
		if (!func) \
		{ \
			set_error("Error [%2d]: Invalid func\n", ERR_USER); \
		} \
		name##_iter it = find_min(set->tree); \
		size_t size = set->size; \
		while (it) \
		{ \
			if (func(it->entity, args) || set->size != size) \
			{ \
				set_error("Error [%2d]: User requests to quit, or hashmap has been changed\n", ERR_USER) \
				return ERR_USER; \
			} \
			if (it->right) \
				it = find_min(it->right); \
			else if (it->parent && it->parent->hashcode > it->hashcode) \
				it = it->parent; \
			else \
				break; \
		} \
		return OK; \
	} \

#endif
