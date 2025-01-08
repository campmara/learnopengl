# LIGHTING MAPS

Having only color properties for specifying our materials isn't very robust, and thus we can introduce diffuse and specular maps into the struct! These are basically textures.

## Diffuse Maps

Diffuse maps use a texture image to represent all of an object's diffuse colors. This can be multiplied with a color value in what's known as a *tint*.

## Specular Maps

We can specify another texture for the specular highlighting, whose color values will allow us to effectively "mask" the specular light to certain parts of the texture.