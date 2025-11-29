# Material Reference Guide

## Material Types

### 1. LAMBERTIAN (Diffuse Materials)

**Physical Behavior**: Scatters light uniformly in all directions (cosine-weighted hemisphere)

**Use For**: 
- Matte surfaces
- Clay, plaster, concrete
- Unpolished wood
- Cloth, fabric
- Rough stone

**Parameters**:
```cpp
Sphere(
    center,                          // vec3
    radius,                          // float
    glm::vec3(r, g, b),             // Albedo color [0-1]
    MaterialType::LAMBERTIAN,
    0.0f,                           // fuzz (ignored)
    0.0f                            // refractiveIndex (ignored)
);
```

**Example Colors**:
- Red brick: `vec3(0.7, 0.2, 0.2)`
- Green grass: `vec3(0.3, 0.7, 0.2)`
- White chalk: `vec3(0.9, 0.9, 0.9)`
- Blue paint: `vec3(0.2, 0.3, 0.8)`

---

### 2. METAL (Reflective Materials)

**Physical Behavior**: Reflects rays with optional roughness (fuzz)

**Use For**:
- Polished metals (gold, silver, copper, aluminum)
- Mirrors
- Chrome surfaces
- Brushed metal

**Parameters**:
```cpp
Sphere(
    center,
    radius,
    glm::vec3(r, g, b),             // Albedo (metal tint)
    MaterialType::METAL,
    fuzz,                           // Roughness [0-1]
    0.0f                            // refractiveIndex (ignored)
);
```

**Fuzz Values**:
- `0.0` - Perfect mirror (sharp reflection)
- `0.1` - Polished metal
- `0.3` - Brushed metal
- `0.5` - Rough metal
- `1.0` - Very rough (almost diffuse)

**Example Metals**:
```cpp
// Gold
vec3(1.0, 0.84, 0.0), MaterialType::METAL, 0.05f

// Silver
vec3(0.95, 0.93, 0.88), MaterialType::METAL, 0.0f

// Copper
vec3(0.95, 0.64, 0.54), MaterialType::METAL, 0.1f

// Aluminum
vec3(0.91, 0.92, 0.92), MaterialType::METAL, 0.2f

// Bronze
vec3(0.8, 0.5, 0.2), MaterialType::METAL, 0.15f
```

---

### 3. DIELECTRIC (Glass/Transparent Materials)

**Physical Behavior**: Refracts and reflects based on refractive index and viewing angle (Fresnel effect)

**Use For**:
- Glass
- Water
- Crystals
- Ice
- Gems (diamond, ruby, sapphire)
- Plastic (clear)

**Parameters**:
```cpp
Sphere(
    center,
    radius,
    glm::vec3(r, g, b),             // Tint color (usually white)
    MaterialType::DIELECTRIC,
    0.0f,                           // fuzz (ignored)
    refractiveIndex                 // IOR [1.0 - 3.0]
);
```

**Refractive Indices**:
- `1.0` - Air/vacuum
- `1.33` - Water
- `1.5` - Standard glass
- `1.52` - Crown glass (windows)
- `1.9` - Sapphire
- `2.4` - Diamond
- `2.42` - Cubic zirconia

**Example Materials**:
```cpp
// Clear glass
vec3(1.0, 1.0, 1.0), MaterialType::DIELECTRIC, 0.0f, 1.5f

// Water
vec3(0.9, 0.95, 1.0), MaterialType::DIELECTRIC, 0.0f, 1.33f

// Diamond
vec3(1.0, 1.0, 1.0), MaterialType::DIELECTRIC, 0.0f, 2.4f

// Colored glass (blue)
vec3(0.7, 0.8, 1.0), MaterialType::DIELECTRIC, 0.0f, 1.5f

// Ice
vec3(0.95, 0.98, 1.0), MaterialType::DIELECTRIC, 0.0f, 1.31f
```

---

## Scene Composition Tips

### Classic "Three Sphere" Scene
```cpp
// Ground (large sphere)
Sphere(vec3(0, -1000, 0), 1000.0f, vec3(0.5, 0.5, 0.5), LAMBERTIAN, 0, 0);

// Left (metal)
Sphere(vec3(-4, 1, 0), 1.0f, vec3(0.8, 0.8, 0.8), METAL, 0.0f, 0);

// Center (glass)
Sphere(vec3(0, 1, 0), 1.0f, vec3(1.0, 1.0, 1.0), DIELECTRIC, 0, 1.5f);

// Right (diffuse)
Sphere(vec3(4, 1, 0), 1.0f, vec3(0.4, 0.2, 0.1), LAMBERTIAN, 0, 0);
```

### Interesting Combinations

**Glass sphere inside glass sphere** (hollow glass):
```cpp
Sphere(vec3(0, 1, 0), 1.0f, vec3(1, 1, 1), DIELECTRIC, 0, 1.5f);
Sphere(vec3(0, 1, 0), -0.9f, vec3(1, 1, 1), DIELECTRIC, 0, 1.5f); // Negative radius
```

**Foggy glass** (combine glass + diffuse):
Use low refractive index (~1.1) for milky glass effect.

**Colored metals**:
```cpp
// Blue metallic
vec3(0.2, 0.4, 0.8), METAL, 0.1f
```

---

## Color Theory for Raytracing

### Albedo Values
- **White** (0.9-1.0): Reflects most light (bright)
- **Mid-tone** (0.4-0.6): Balanced
- **Dark** (0.1-0.3): Absorbs most light
- **Black** (0.0-0.1): Absorbs nearly all light

### Physical Accuracy
Most real-world materials have albedo < 0.9:
- Fresh snow: ~0.9
- White paint: ~0.8
- Brick: ~0.3-0.5
- Asphalt: ~0.1
- Coal: ~0.04

### Complementary Colors
For colorful scenes, use color wheel opposites:
- Red (0.8, 0.2, 0.2) + Cyan (0.2, 0.8, 0.8)
- Blue (0.2, 0.2, 0.8) + Orange (0.8, 0.6, 0.2)
- Green (0.2, 0.8, 0.2) + Magenta (0.8, 0.2, 0.8)

---

## Debugging Materials

### Material Not Visible?
- Check `radius` is positive (negative = inside-out)
- Verify `center` is within camera frustum
- Ensure albedo is not black `vec3(0, 0, 0)`

### Glass Looks Wrong?
- Try refractive index 1.5 (standard glass)
- Use white tint `vec3(1, 1, 1)`
- Check if sphere is inside another object

### Metal Too Dark?
- Increase albedo (metals should be bright)
- Reduce fuzz (lower = more reflection)
- Check scene has light sources (sky gradient)

### Noisy Image?
- Increase `mTotalBatches` (more samples)
- Increase `mSamplesPerBatch` (slower but smoother)
- Wait for full convergence (all batches complete)

---

## Performance Tips

### Expensive Materials (Slowest to Fastest)
1. **DIELECTRIC** - Most bounces (refraction + reflection)
2. **METAL** (high fuzz) - Random scatter rays
3. **METAL** (low fuzz) - Direct reflections
4. **LAMBERTIAN** - Random scatter (but diffuse)

### Optimization
- Use fewer dielectric spheres for faster rendering
- Large ground plane as Lambertian (cheap)
- Mix materials (not all glass)
- Lower `MAX_BOUNCES` in shader (line 7) for speed

---

## Advanced: Modifying the Shader

### Change Max Bounces
`shaders/pathtracer.cs.glsl`, line 7:
```glsl
#define MAX_BOUNCES 8  // Increase for more realistic glass (10-12)
```

### Add Emissive Materials (Glowing Spheres)
After line 191 in `scatter()` function:
```glsl
if (hit.materialType == EMISSIVE) {
    attenuation = hit.albedo * 10.0;  // Emission strength
    return false;  // Stop bouncing
}
```

Then in C++:
```cpp
enum class MaterialType : uint32_t {
    LAMBERTIAN = 0,
    METAL = 1,
    DIELECTRIC = 2,
    EMISSIVE = 3  // New!
};
```

### Adjust Sky Color
`pathtracer.cs.glsl`, line 210:
```glsl
vec3 skyColor(vec3 direction) {
    float t = 0.5 * (normalize(direction).y + 1.0);
    // Sunset colors:
    return mix(vec3(1.0, 0.6, 0.3), vec3(0.3, 0.5, 0.9), t);
}
```

---

## Example Scenes

### Cornell Box Style
```cpp
// Red left wall
Sphere(vec3(-1001, 0, 0), 1000.0f, vec3(0.8, 0.1, 0.1), LAMBERTIAN, 0, 0);
// Green right wall
Sphere(vec3(1001, 0, 0), 1000.0f, vec3(0.1, 0.8, 0.1), LAMBERTIAN, 0, 0);
// White back wall
Sphere(vec3(0, 0, -1001), 1000.0f, vec3(0.8, 0.8, 0.8), LAMBERTIAN, 0, 0);
// Center glass sphere
Sphere(vec3(0, 1, 0), 1.0f, vec3(1, 1, 1), DIELECTRIC, 0, 1.5f);
```

### Jewelry Display
```cpp
// Gold ring
Sphere(vec3(-2, 0.5, 0), 0.5f, vec3(1.0, 0.84, 0.0), METAL, 0.05f, 0);
// Diamond
Sphere(vec3(0, 1, 0), 0.8f, vec3(1, 1, 1), DIELECTRIC, 0, 2.4f);
// Silver necklace
Sphere(vec3(2, 0.3, 0), 0.3f, vec3(0.95, 0.93, 0.88), METAL, 0.0f, 0);
```

### Abstract Art
```cpp
for (int i = 0; i < 20; i++) {
    float hue = (float)i / 20.0f;
    vec3 color = hsvToRgb(hue, 0.8f, 0.8f);  // Implement HSV conversion
    MaterialType type = (i % 3 == 0) ? LAMBERTIAN : 
                        (i % 3 == 1) ? METAL : DIELECTRIC;
    Sphere(..., color, type, ...);
}
```

---

*For more examples, see the reference implementation in `Compute.cpp::initPathTracer()`*

