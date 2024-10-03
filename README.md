# hull2d-startup


2d convex hull startup code.  

How to run:
Does compile as is. Just hit type in make and run ./hull2d n where n is the number of points. 

What is it?
Finds the convex hull of 10 sets of points. The code should handle all degenerate cases including an array of identical points. Collinearity is handled by only adding points to the hull that are strictly to the left. It appears that my code also handled the case of lines. However, I am unsure if this is due to the numerical instability of the left function. 