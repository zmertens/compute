using System;
using Godot;

public partial class Plane
{
    Material material;
    Vector3 point;
    Vector3 normal;

    public Plane(Material material, Vector3 point, Vector3 normal)
    {
        this.material = material;
        this.point = point;
        this.normal = normal;
    }
}