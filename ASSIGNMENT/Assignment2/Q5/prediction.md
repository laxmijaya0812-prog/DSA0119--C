# Q5 Prediction

The static `next` starts at 100. Three objects in `counter` call the constructor, followed by one `extra` object, so the counter becomes 104.

Predicted output:

```text
101 103 104 4
```

The constructor runs once for every object, regardless of whether the objects are created individually, in an array, in a loop, or inside a function.
