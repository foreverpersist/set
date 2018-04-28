# SET

# Design

	Just a sorted binary tree

# Structure

	set
		|-size
		|-tree
		|-hash_func

	node
		|-hashcode
		|-entity
		|-left
		|-right

# API

## Raw API

	Create set.

> * Return 0 for success.

```
	name##_set name##_set_create();
```

---

	Destroy set.

> * Return 0 for success.

```
	int name##_set_destroy(name##_set *pset);
```

---

	Set hash function to generate unique hashcode to identity entities.

> * You have to invoke it to set valid `hash_func`.
> * `hash_func` must generate unique hashcode for different entities. Otherwise set can't distinguish them.
> * Return 0 for success.

```
	int name##_set_set_hash_func(name##_set set, int (*hash_func)(entity_type));
```

---

	Check whether `entity` exists.

> * Return 0 for success.

```
	int name##_set_has(name##_set set, entity_type entity);
```

---

	Put an entity.

> * Return 0 for success.

```
	int name##_set_put(name##_set set, entity_type entity);
```

---

	Remove an entity.

> * Return 0 for success.

```
	int name##_set_remove(name##_set set, entity_type entity);
```

---

	Query set size

> * Return set size for success, return negative for failure.

```
	int name##_set_size(name##_set set);
```

---

	Get set iterator.

> * Return iterator for success, return `NULL` for failure.

```
	name##_iter name##_set_iter(name##_set set);
```

---

	Get next set iterator.

> * Return next iterator for success, return `NULL` for failure.

```
	name##_iter name##_set_next(name##_set set, name##_iter it);
```

---

	Visit all entities with function `func` and extra parameter `args`.

> * If `func` is `NULL`, do nothing.
> * It will stop immediately if set size is changed.
> * Return 0 for success.

```
	int name##_set_foreach(name##_set set, int (func*)(entity_type), void *args);
```

## Macro API

	Macro of function `name##_set_create`

```
	set(name) set_create(name);
```

---

	Macro of function `name##_set_destroy`

```
	int set_destroy(name, pset);
```

---

	Macro of function `name##_set_set_hash_func`

```
	int set_set_hash_func(name, set, hash_func);
```

---

	Macro of function `name##_set_has`

```
	int set_has(name, set, entity);
```

---

	Macro of function `name##_set_put`

```
	int set_put(name, set, entity);
```

---

	Macro of function `name_set_remove`

```
	int set_remove(name, set, entity);
```

---

	Macro of function `name##_set_size`

```
	int set_size(name, set);
```

---

	Macro of function `name##_set_iter`

```
	iter(name) set_iter(name, set)
```

---

	Macro of funciton `name##_set_next`

```
	iter(name) set_next(name, set, it)
```

---

	Macro of function `name##_set_foreach`

```
	int set_foreach(name, set, func, args);
```

# Example

[example.c](example.c)