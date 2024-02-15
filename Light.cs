using System;
using Godot;

public partial class Light
{
    Vector4 center;
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;

    public Light(Vector4 center, Vector3 ambient, Vector3 diffuse, Vector3 specular)
    {
        this.center = center;
        this.ambient = ambient;
        this.diffuse = diffuse;
        this.specular = specular;
    }
}