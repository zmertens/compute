using System;
using Godot;

public partial class Sphere
{
    Vector3 center;
    float radius;
    Material material;

    public Sphere(Vector3 center, float radius, Material material)
    {
        this.center = center;
        this.radius = radius;
        this.material = material;
    }
}