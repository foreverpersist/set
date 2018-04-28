# SET

# Design

	Just a sorted binary tree

# Structure

	set
		|-size
		|-node_size
		|-tree
		|-hash_func
		|-compare_func

	node
		|-hashcode
		|-collision
		|-entities
		|-parent
		|-left
		|-right

	entity
		|-value
		|-next

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

	Set hash function to generate hashcode to identity values.

> * `hash_func` is supposed to generate unique hashcode if possible.
> * Default `hash_func` always return 0.
> * Return 0 for success.

```
	int name##_set_set_hash_func(name##_set set, int (*hash_func)(value_type));
```

---

	Set compare function to distinguish values.

> * `compare_func` is required to return 0 when two values are the same, 
	and return non-zero when they are different.
> * Default `compare_func` always return `value1 - value2`.
> * Return 0 for success.

```
	int name##_set_set_compare_func(name##_set set, int (*compare_func)(value_type, value_type));
```

---

	Check whether `value` exists.
 
> * Return 0 for success.

```
	int name##_set_has(name##_set set, value_type value);
```

---

	Put an value.

> * Return 0 for success.

```
	int name##_set_put(name##_set set, value_type value);
```

---

	Remove an value.

> * Return 0 for success.

```
	int name##_set_remove(name##_set set, value_type value);
```

---

	Query set size

> * Return set size for success, return negative for failure.

```
	int name##_set_size(name##_set set);
```

---

	Query set collision mean.

> * Return collision mean for success, return negative for failure.

```
	double name##_set_collision_mean(name##_set set);
```

---

	Query set collision variance.

> * Return collision variance for success, return negative for failure.

```
	double name##_set_collision_variance(name##_set set);
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

	Visit all values with function `func` and extra parameter `args`.

> * If `func` is `NULL`, do nothing.
> * It will stop immediately if set size is changed.
> * Return 0 for success.

```
	int name##_set_foreach(name##_set set, int (func*)(value_type), void *args);
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

	Macro of function `name##_set_set_compare_func`

```
	int set_set_compare_func(name, set, compare_func)
```

---

	Macro of function `name##_set_has`

```
	int set_has(name, set, value);
```

---

	Macro of function `name##_set_put`

```
	int set_put(name, set, value);
```

---

	Macro of function `name_set_remove`

```
	int set_remove(name, set, value);
```

---

	Macro of function `name##_set_size`

```
	int set_size(name, set);
```

---

	Macro of function `name##_set_collision_mean`

```
	double set_collision_mean(name, set)
```

---

	Macro of function `name##_set_collision_variance`

```
	double set_collision_variance(name, set)
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

[example1.c](example1.c)

[example2.c](example2.c)