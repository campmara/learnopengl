# TRANSFORMATIONS (VECTORS AND MATRICES)

## Vectors

Length of a vector is calculated with the pythagoras theorem!
given Vector (x, y)
length = sqrt(pow(x, 2) + pow(y, 2));

Normalizing vector involves taking the vector and dividing it by it's length, this makes the length 1!

## Matrices

Addition and subtraction is done on a per-element basis.

Scalar multiplication is also a per-element operation.

Multiplication is weird.
1. You can only multiply two matrices if the number of columns on the left hand side matrix is equal to the number of rows on the right hand side matrix.
2. It's not commutative, A * B != B * A

## Scaling

Scaling a vector by a matrix works by taking the scalar value and constructing an identity matrix from it (multiplying each value by the scalar), and then performing multiplication with that on the vector.

Translation works by taking an identity matrix and setting the first three rows of the last column to the translation vector's corresponding values. Then, a multiplication of that matrix can occur on the target vector and the desired translation will take place.

## Homogenous Coordinates

The w component of a vector is known as a homogenous coordinate. We usually divide a vector by its w value to get the 3D vector (usually don't notice this). It's useful because is lets us do matrix transformations on vectors.

When w is 0, this means the vector is known as a *direction* vector since a vector with that w coordinate cannot be translated.