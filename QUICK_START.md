# Quick Start Guide - PBR Path Tracer

## 🚀 Running the Path Tracer

### 1. Build the Project
```powershell
cd C:\Users\zachm\Desktop\zmertens-compute\build
cmake --build . --config Debug
```

### 2. Run the Application
```powershell
cd Debug
.\compute.exe
```

### 3. What to Expect
- **Window opens** with black screen
- **Progressive refinement** over ~16 seconds (250 batches @ 60fps)
- **Console logs** progress every 10 batches
- **Final image** will show:
  - Large gray ground plane
  - Center glass sphere (transparent)
  - Left brown diffuse sphere
  - Right gold metal sphere
  - 16 random colored spheres in circular pattern

---

## ⚙️ Configuration

### Switch Rendering Mode

Edit `GLRaytracer/Compute.cpp`, line 19:
```cpp
mUsePathTracer(true)   // PBR path tracer (new)
mUsePathTracer(false)  // Legacy Phong raytracer (old)
```

### Adjust Quality

Edit `GLRaytracer/Compute.cpp`, lines 17-18:
```cpp
// Fast preview (200 samples)
mSamplesPerBatch(2)
mTotalBatches(100)

// Balanced (1000 samples) - DEFAULT
mSamplesPerBatch(4)
mTotalBatches(250)

// High quality (5000 samples)
mSamplesPerBatch(10)
mTotalBatches(500)
```

**Formula**: Total samples = `mSamplesPerBatch × mTotalBatches`

### Change Max Ray Bounces

Edit `shaders/pathtracer.cs.glsl`, line 7:
```glsl
#define MAX_BOUNCES 8    // Default
#define MAX_BOUNCES 12   // Better glass realism (slower)
#define MAX_BOUNCES 4    // Faster (less realistic glass)
```

---

## 🎨 Creating Custom Scenes

### Edit Scene Setup

Open `GLRaytracer/Compute.cpp`, find `initPathTracer()` function (line ~302)

### Add a Sphere

```cpp
spheres.emplace_back(
    glm::vec3(x, y, z),              // Position
    radius,                           // Size
    glm::vec3(r, g, b),              // Color [0-1]
    MaterialType::LAMBERTIAN,         // Type
    0.0f,                             // Fuzz (for metals)
    0.0f                              // Refractive index (for glass)
);
```

### Example: Add Red Metal Sphere

After line 345, add:
```cpp
spheres.emplace_back(
    glm::vec3(8.0f, 2.0f, 0.0f),     // Right side, elevated
    1.5f,                             // Large sphere
    glm::vec3(0.9f, 0.1f, 0.1f),     // Red color
    MaterialType::METAL,              // Reflective
    0.2f,                             // Slightly rough
    0.0f
);
```

### Example: Add Blue Glass Sphere

```cpp
spheres.emplace_back(
    glm::vec3(-8.0f, 1.5f, 0.0f),
    1.2f,
    glm::vec3(0.8f, 0.9f, 1.0f),     // Blue tint
    MaterialType::DIELECTRIC,
    0.0f,
    1.5f                              // Glass IOR
);
```

---

## 🐛 Troubleshooting

### Black Screen / No Output

**Check 1**: Verify shader files exist
```
shaders/
  - raytracer.vert.glsl
  - raytracer.frag.glsl
  - pathtracer.cs.glsl
```

**Check 2**: Console output
- Should see: "Using PBR Path Tracer"
- Should see: "Path tracer initialized with 20 spheres"
- Should see: Progress logs (10%, 20%, ...)

**Check 3**: OpenGL version
- Requires OpenGL 4.5+
- Check console: "GL Version: X.X"

### Shader Compilation Error

**Symptom**: Window opens, then crashes

**Fix**: Check console for shader errors:
```
ERROR: Shader compilation failed
```

Common issues:
- Missing random functions → Fixed in pathtracer.cs.glsl
- Syntax error → Check GLSL version 450

### Spheres Not Visible

**Cause**: Camera position/orientation

**Fix**: Edit `Compute.cpp`, line 12:
```cpp
mCamera(
    glm::vec3(0.0f, 50.0f, 200.0f),  // Position (X, Y, Z)
    -90.0f,  // Yaw (look left/right)
    -10.0f,  // Pitch (look up/down)
    65.0f,   // FOV
    0.1f,    // Near plane
    500.0f   // Far plane
)
```

**Test**: Move camera closer:
```cpp
glm::vec3(0.0f, 5.0f, 20.0f)  // Much closer view
```

### Very Noisy Image

**Cause**: Not enough samples

**Fix 1**: Wait longer (let all batches complete)

**Fix 2**: Increase samples:
```cpp
mSamplesPerBatch(10)
mTotalBatches(1000)  // 10,000 total samples
```

**Fix 3**: Reduce sphere count (faster):
```cpp
#define TOTAL_SPHERES 10  // In Compute.cpp, line 14
```

### Slow Performance

**Cause**: Too many samples or complex materials

**Fix 1**: Reduce quality:
```cpp
mSamplesPerBatch(2)
mTotalBatches(50)  // 100 samples (fast preview)
```

**Fix 2**: Reduce bounces:
```glsl
#define MAX_BOUNCES 4  // In pathtracer.cs.glsl
```

**Fix 3**: Fewer glass spheres:
```cpp
// In initPathTracer(), change line 375:
if (matChoice < 0.95f) {  // More diffuse, less glass
```

---

## 📊 Console Output Explained

### Startup
```
Using PBR Path Tracer
Path tracer initialized with 20 spheres
Camera initialized at position: (0.00, 50.00, 200.00)
```
✅ Path tracer loaded successfully

### During Rendering
```
Path tracing progress: 10.0% (25/250 batches, 100 samples)
Path tracing progress: 20.0% (50/250 batches, 200 samples)
...
Path tracing progress: 100.0% (250/250 batches, 1000 samples)
```
✅ Progressive refinement in progress

### Performance
```
FPS: 60 | time (ms) / frame: 16.67
```
- **60 FPS** = Smooth rendering
- **<30 FPS** = Reduce quality or sphere count

---

## 🎯 Next Steps

### 1. Experiment with Materials
See `MATERIAL_GUIDE.md` for examples:
- Different metal colors (gold, silver, copper)
- Glass with different refractive indices
- Colored glass (tinted)
- Rough vs smooth metals

### 2. Modify Scene Layout
- Change sphere positions
- Add more spheres (up to 20)
- Create patterns (grid, spiral, random)

### 3. Adjust Rendering Quality
- Test different sample counts
- Find balance between speed and quality
- Try different bounce counts

### 4. Change Camera
- Move closer/farther
- Look at different angles
- Adjust FOV for wide/narrow view

---

## 📝 Quick Reference

### File Locations
- **Main config**: `GLRaytracer/Compute.cpp` (lines 12-20)
- **Scene setup**: `GLRaytracer/Compute.cpp` `initPathTracer()` (~line 302)
- **Shader**: `shaders/pathtracer.cs.glsl`
- **Material types**: `GLRaytracer/Material.hpp`

### Important Defines
```cpp
// Compute.cpp
#define TOTAL_SPHERES 20
#define TOTAL_LIGHTS 5

// pathtracer.cs.glsl
#define MAX_SPHERES 20
#define MAX_BOUNCES 8
#define EPSILON 0.001
```

### Material Type Values
```cpp
MaterialType::LAMBERTIAN   // 0 - Diffuse
MaterialType::METAL        // 1 - Reflective
MaterialType::DIELECTRIC   // 2 - Glass
```

---

## 💡 Tips

### For Best Results
1. ✅ Wait for 100% completion (all batches)
2. ✅ Use at least 1000 samples for good quality
3. ✅ Mix material types (not all glass)
4. ✅ Place camera to see all spheres

### For Fast Iteration
1. ✅ Use 100-200 samples for quick preview
2. ✅ Reduce MAX_BOUNCES to 4
3. ✅ Test with fewer spheres (10-15)
4. ✅ Increase samples for final render

### For Debugging
1. ✅ Enable DEBUG_COMPUTE (Compute.cpp, line 1)
2. ✅ Check sphere_data.txt or pathtracer_spheres.txt
3. ✅ Output constant color to verify shader runs
4. ✅ Check console for OpenGL errors

---

## 🔍 Testing Checklist

- [ ] Project builds without errors
- [ ] Window opens (1080×720)
- [ ] Console shows "Using PBR Path Tracer"
- [ ] Console shows progress logs
- [ ] Image refines over time (gets less noisy)
- [ ] Can see spheres clearly
- [ ] Glass sphere is transparent
- [ ] Metal sphere shows reflections
- [ ] FPS is acceptable (>30)

---

## 📚 Documentation Files

1. **REFACTORING_SUMMARY.md** - Complete implementation details
2. **MATERIAL_GUIDE.md** - Material examples and recipes
3. **IMPLEMENTATION_COMPARISON.md** - Reference code comparison
4. **QUICK_START.md** - This file

---

## 🆘 Still Having Issues?

### Check Build Output
```powershell
cmake --build . --config Debug 2>&1 | Select-String "error"
```

### Verify OpenGL Support
The application requires:
- OpenGL 4.5+ with compute shaders
- GLAD loader (included)
- SDL3 (included)

### Reset to Legacy Raytracer
If path tracer doesn't work:
```cpp
mUsePathTracer(false)  // Use old working raytracer
```

### Start Minimal
Test with just 4 spheres:
```cpp
// Comment out the random sphere generation loop
// Keep only the 4 main spheres (ground + 3 objects)
```

---

**Ready to render!** 🎨

Run the application and watch your scene progressively refine into a photorealistic image!

