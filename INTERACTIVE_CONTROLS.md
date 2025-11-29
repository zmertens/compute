# Interactive Controls - Complete Guide

## 🎮 Controls Summary

Your PBR path tracer now has **fully interactive camera controls** with automatic progressive refinement!

---

## 🖱️ Mouse Controls

### **Camera Rotation**
- **Hold Left Mouse Button** + Move Mouse
  - Rotate camera view in any direction
  - Real-time response
  - Accumulation automatically resets on camera movement

### **Alternative: Mouse Lock Mode**
- **Press TAB** to toggle mouse lock
  - When locked: mouse movement rotates camera (no button needed)
  - When unlocked: normal mouse behavior
  - Press TAB again to unlock

### **Field of View (Zoom)**
- **Mouse Wheel Up**: Decrease FOV (zoom in)
- **Mouse Wheel Down**: Increase FOV (zoom out)
- Range: 1° to maximum FOV
- Smooth adjustment

---

## ⌨️ Keyboard Controls

### **Movement (Two Options)**

#### **WASD Keys** (Primary)
- **W**: Move forward
- **S**: Move backward
- **A**: Strafe left
- **D**: Strafe right

#### **Arrow Keys** (Alternative)
- **↑ (Up Arrow)**: Move forward
- **↓ (Down Arrow)**: Move backward
- **← (Left Arrow)**: Strafe left
- **→ (Right Arrow)**: Strafe right

### **Special Keys**
- **TAB**: Toggle mouse lock mode
- **ESC**: Exit application
- **Mouse Wheel**: Adjust field of view

---

## 🔄 Progressive Refinement Behavior

### **Automatic Reset on Movement**
When the camera moves or rotates, the path tracer automatically:
1. **Detects camera change** (position, yaw, or pitch)
2. **Resets accumulation** to batch 0
3. **Clears previous samples** 
4. **Starts fresh** progressive refinement from new viewpoint

### **Thresholds**
- **Position threshold**: 0.01 units
- **Rotation threshold**: 0.1 degrees
- Prevents jitter from tiny floating-point changes
- Smooth experience without unnecessary resets

### **During Movement**
- Image will be **noisy** with few samples
- **Stop moving** to let it converge
- After ~4 seconds (250 batches), full 1000 samples reached
- Image becomes **smooth and photorealistic**

---

## 🎯 Usage Tips

### **Exploring the Scene**
1. **Hold left mouse button** and move mouse to look around
2. Use **WASD** to fly through the scene
3. **Stop moving** when you find a good view
4. **Wait 4 seconds** for full quality (1000 samples)

### **Finding Good Angles**
- Move camera to different heights (W/S)
- Circle around objects (A/D + mouse rotation)
- Get close to glass spheres to see refraction
- Look at metal spheres from different angles to see reflections

### **Performance**
- **Moving/rotating**: 4-10 samples (noisy but fast)
- **Stationary**: Progressively refines to 1000 samples
- **60 FPS** maintained during refinement
- GPU-accelerated compute shader

---

## 🎬 Recommended Workflow

### **Quick Exploration**
```
1. Hold Left Mouse Button
2. Move mouse to look around quickly
3. Release when you find something interesting
4. Wait for convergence
```

### **Precise Positioning**
```
1. Press TAB to lock mouse
2. Use WASD for fine adjustments
3. Use mouse for rotation (no clicking needed)
4. Press TAB to unlock when done
```

### **Screenshot Workflow**
```
1. Position camera with controls
2. Stop all movement
3. Wait for "100.0% (250/250 batches)" in console
4. Take screenshot (OS screenshot tool)
5. Move to next position
```

---

## 📊 Console Feedback

### **Progress Logs**
```
Path tracing progress: 4.0% (10/250 batches, 40 samples)
Path tracing progress: 8.0% (20/250 batches, 80 samples)
...
Path tracing progress: 100.0% (250/250 batches, 1000 samples)
```

### **FPS Counter**
```
FPS: 59 | time (ms) / frame: 16.68
```
- Updates every second
- Also displayed in window title

### **Camera Info (at startup)**
```
Camera initialized at position: (0.00, 50.00, 200.00)
Camera target direction: (-0.00, -0.17, -0.98)
Camera far plane: 500.00
```

---

## 🔧 Technical Details

### **Camera Movement Implementation**
- Uses first-person camera system
- Target vector for forward/backward
- Right vector for strafing
- Movement speed: 220 units/second
- Smooth interpolation

### **Rotation Implementation**
- Yaw (left/right): -180° to 180°
- Pitch (up/down): -89° to 89° (prevents gimbal lock)
- Mouse sensitivity: 1.0
- Euler angle-based rotation

### **Accumulation Reset Logic**
```cpp
if (position_changed || yaw_changed || pitch_changed) {
    mCurrentBatch = 0;  // Reset to first batch
    glClearTexImage(accumTex);  // Clear accumulation
}
```

### **Frame Timing**
- Fixed timestep: 60 FPS target
- Delta time accumulator
- Input/Update/Render loop
- VSync enabled (SDL_GL_SetSwapInterval(1))

---

## 🎨 Visual Effects During Movement

### **1-10 samples (Moving)**
- Very noisy/grainy
- Can see general shapes and colors
- Fast updates (60 FPS)

### **40-100 samples (Slowing)**
- Noise reducing
- Details emerging
- Still some grain

### **200-500 samples (Refining)**
- Smooth gradients
- Clear reflections
- Minor noise in shadows

### **1000 samples (Converged)**
- Photorealistic
- Smooth everywhere
- Accurate caustics
- Perfect glass refraction

---

## 🚀 Advanced Usage

### **Camera Reset**
To return to starting position:
- Restart application
- Or manually fly back to `(0, 50, 200)`

### **Changing Starting Position**
Edit `Compute.cpp`, line 12:
```cpp
mCamera(glm::vec3(x, y, z), yaw, pitch, fov, near, far)

// Example: Start closer
mCamera(glm::vec3(0.0f, 10.0f, 50.0f), -90.0f, -10.0f, 65.0f, 0.1f, 500.0f)
```

### **Changing Movement Speed**
Edit `Player.cpp`, line 10:
```cpp
float Player::scMovementScalar = 220.0f;  // Default
float Player::scMovementScalar = 500.0f;  // Faster
float Player::scMovementScalar = 100.0f;  // Slower
```

### **Changing Mouse Sensitivity**
Edit `Player.cpp`, line 9:
```cpp
const float Player::scMouseSensitivity = 1.0f;  // Default
const float Player::scMouseSensitivity = 2.0f;  // More sensitive
const float Player::scMouseSensitivity = 0.5f;  // Less sensitive
```

---

## 🐛 Troubleshooting

### **Controls Not Responding**
- ✅ Check window has focus (click on it)
- ✅ Try both WASD and arrow keys
- ✅ For rotation: hold left mouse button OR press TAB

### **Camera Rotation Too Sensitive**
- Reduce `scMouseSensitivity` in `Player.cpp`
- Or use smaller mouse movements

### **Movement Too Fast/Slow**
- Adjust `scMovementScalar` in `Player.cpp`
- Default is 220 units/second

### **Image Keeps Resetting**
- This is normal when moving!
- Stop moving to let it converge
- If resetting while stationary, check threshold values

### **ESC Not Working**
- Make sure window has focus
- Try clicking window first, then ESC
- Or use window close button (X)

---

## 📝 Control Summary Card

```
═══════════════════════════════════════════════════
               CAMERA CONTROLS
═══════════════════════════════════════════════════

MOVEMENT
  WASD or ↑↓←→     Move camera (fly mode)

ROTATION  
  Left Mouse + Drag   Rotate camera view
  TAB + Mouse Move    Mouse lock mode

ZOOM
  Mouse Wheel      Adjust field of view

SPECIAL
  ESC              Exit application
  TAB              Toggle mouse lock

═══════════════════════════════════════════════════
TIP: Stop moving to see full quality (1000 samples)
═══════════════════════════════════════════════════
```

---

## ✨ What Makes This Special

Your interactive path tracer now features:

1. **Real-time interaction** at 60 FPS
2. **Progressive refinement** that adapts to movement
3. **Dual input modes** (click-drag or mouse lock)
4. **Automatic quality adjustment** (noisy while moving, smooth when still)
5. **Physically accurate rendering** with interactive exploration
6. **Production-quality controls** matching professional 3D applications

This combines the best of both worlds:
- **Interactive** like a game engine
- **Photorealistic** like offline renderers

---

**Enjoy exploring your PBR path-traced scene!** 🎨✨

