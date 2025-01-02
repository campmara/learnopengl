# CAMERA

A camera is comprised of the following values:
* camera position
* camera direction ```cameraDirection = (targetPosition - cameraPosition)``` (in openGL, these are reversed to be ```cameraDirection = cameraPosition - targetPosition```
* right axis vector ```cameraRight = norm(cross(vec3(0, 1, 0), cameraDirection))```
* up axis vector ```cross(cameraDirection, cameraRight)```

It uses these values in what's called a Look At matrix:
| x   | y   | z   | w   |
| --- | --- | --- | --- |
| Rx  | Ry  | Rz  | 0   |
| Ux  | Uy  | Uz  | 0   |
| Dx  | Dy  | Dz  | 0   |
| 0   | 0   | 0   | 1   |