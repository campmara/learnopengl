# Light Casters

Time to learn about all the different ways in which light can be cast!

## Directional Light (the sun, for example!)

A light source that is modeled to be *infinitely* far away, so its presence is always felt in a certain direction.

All the light 'rays' are parallel, so we don't need to worry about the direction of individual objects relative to the position of the sun. Lighting calculations will therefore be similar for every object in a scene.

In the fragment shader:
```c++
vec3 lightDir = normalize(-light.direction);
```

## Point Light

A light at a point in space that illuminates in all directions. The light fades out over distance, this is called attenuation.

The attenuation of a light *could* be linear, but this wouldn't produce very realistic results! Instead, we use a formula that looks like this:

```
Fatt = 1.0 / (Kconstant + Klinear * distance + Kquadratic * distanceSquared)
```

Kconstant:
* Usually kept at 1.0.
* There to make sure the denominator never gets smaller than 1 since it would then boost light intensity at certain distances!

Klinear:
* multiplied with the distance value and reduces intensity linearly

Kquadratic:
* multiplied with distance squared (the quadrant of the distance) and sets a quadratic decrease of intensity for the light source. Will grow in significance as distance increases.

The combination of linear and quatratic terms in this formula will ensure that light will diminish in a mostly linear fashion until the distance becomes large enough for the quadratic term to surpass the linear, making light intensity decrease much slower.

What you end up seeing is the light is super bright up close and then loses that intensity quickly but not completely, slowly draining that remaining intensity over a longer time.

## Spotlight

Like a point light, but instead of shooting light in all directions, it shoots light in a specific direction. In OpenGL, a spotlight is represented with a position, a direction, and a cutoff angle that specifies the radius of the spotlight. To determine if a fragment is lit, we calculate whether or not the point is between the cutoff directions specified by the spotlight.

The calculation looks like this:
```c++
vec3 lightDir = normalize(light.position - FragPos);
float theta = dot(lightDir, normalize(-light.direction));

if (theta > light.cutoff)
{
    // do normal lighting calculations
}
else
{
    // still light things with a certain amount of ambient lighting
    FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
}
```

This only leaves us with a harsh cone of light, and in actuality the light sort of attenuates softly outwards in the case of a flashlight, for example. We can represent this with an inner and outer cone, and gradually decrease light intensity outside of the inner cone.

So instead, we can change all that control flow to the following:
```c++
// do normal lighting calculations...

// spotlight (soft edges)
float theta = dot(lightDir, normalize(-light.direction)); 
float epsilon = (light.cutOff - light.outerCutOff);
float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
diffuse  *= intensity;
specular *= intensity;

// do attenuation calculations...
```