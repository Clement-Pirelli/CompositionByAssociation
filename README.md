### What this is

A small header-only composition by association utility targeting C++20, mostly made as an excuse to use C++20 more.
Uses the standard library quite a bit.

By "Composition by association" I mean that entities in this model are simply unique identifiers through which components may identify other components belonging to the same entity. 

For example, in the file `Example.cpp`, gravity components apply acceleration to movement components of the same entity like so:

```cpp
for (const Gravity &gravity : all_gravities)
{
	for (Movement &movement : all_movements.with_id_mut(gravity.id))
  {
    movement.velocity += gravity.direction * gravity.strength;
  }
}
```

As you can see, components are not in the same list owned by an Entity class of some sort. Instead, they're identified as being part of the same entity by their ID.

### Pros

This is pretty handy since you can then keep references to other components as IDs, meaning you can trivially serialize references. 
You can also safely hold a reference to a deleted object since you must call a cheap query every time you wish to access its data.
Iterating through components is fast, since they're simply stored in a vector, ordered by their ID.

### Cons

Of course, it also has downsides, notably that it takes time to query things. In `Example.cpp`, since the example is so trivial, it'd be much easier and much faster to have each movement component also have a velocity vector, which is applied every tick.
Inserting elements is quite slow, especially for huge amounts of components.

On a larger scale though the benefits of easy serialization, easy weak referencing and fast iteration are often worth these few downsides.

### todo list:
- Handle the deletion of a specific component
- Handle customization of the Components class, for example by specifying the expected amount of components and the expected rate of query as template arguments
- Make something cool with this to showcase it a bit better than a crummy example :D
