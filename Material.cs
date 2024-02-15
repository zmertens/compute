using System;
using Godot;

public partial class Material
{
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
    float shiny;
    float reflectivity;
    float refractivity;

    public Material(Vector3 ambient, Vector3 diffuse, Vector3 specular, float shiny, float reflectivity, float refractivity)
    {
        this.ambient = ambient;
        this.diffuse = diffuse;
        this.specular = specular;
        this.shiny = shiny;
        this.reflectivity = reflectivity;
        this.refractivity = refractivity;
    }
}