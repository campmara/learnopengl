# CAMERA

A camera is comprised of the following values:
* camera position (P)
* camera direction (D) ```cameraDirection = (targetPosition - cameraPosition)``` (in openGL, these are reversed to be ```cameraDirection = cameraPosition - targetPosition```
* right axis vector (R) ```cameraRight = norm(cross(vec3(0, 1, 0), cameraDirection))```
* up axis vector (U) ```cross(cameraDirection, cameraRight)```

It uses these values in what's called a Look At matrix:
| x   | y   | z   | w   |
| --- | --- | --- | --- |
| Rx  | Ry  | Rz  | 0   |
| Ux  | Uy  | Uz  | 0   |
| Dx  | Dy  | Dz  | 0   |
| 0   | 0   | 0   | 1   |

multiplied by:
| x   | y   | z   | w   |
| --- | --- | --- | --- |
| 1   | 0   | 0   | -Px |
| 0   | 1   | 0   | -Py |
| 0   | 0   | 1   | -Pz |
| 0   | 0   | 0   | 1   |