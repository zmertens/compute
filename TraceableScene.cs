using Godot;
using System;
using System.Collections.ObjectModel;
using System.Linq;

public partial class TraceableScene : Node2D
{
	RenderingDevice rd = RenderingServer.CreateLocalRenderingDevice();
	Rid bufferRid;
	Rid shaderRid;

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		this.shaderRid = InitShader();
		var tf = InitTexture();
		var spheres = InitSpheres();
		var plane = InitPlane();

		// var bytesOfSpheres = new byte[spheres.Count * ];
		// Buffer.BlockCopy(spheres, 0, bytesOfSpheres, 0, bytesOfSpheres.Length);
		// this.bufferRid = rd.StorageBufferCreate((uint) inputBytes.Length, inputBytes);

		// Uniforms
		// var storageUniform = new RDUniform
		// {
		// 	UniformType = RenderingDevice.UniformType.StorageBuffer,
		// 	Binding = 0
		// };
		// storageUniform.AddId(tf.rid);
		// var uniformSet = rd.UniformSetCreate(new Godot.Collections.Array<RDUniform>{storageUniform},
		// 	shaderRid, 0);

		var imageUniform = new RDUniform
		{
			UniformType = RenderingDevice.UniformType.ImageBuffer,
			Binding = 0
		};
		imageUniform.AddId(bufferRid);
		var uniformSet = rd.UniformSetCreate(new Godot.Collections.Array<RDUniform>{imageUniform}, shaderRid, 0);

		var pipeline = rd.ComputePipelineCreate(shaderRid);
		var computeList = rd.ComputeListBegin();
		rd.ComputeListBindComputePipeline(computeList, pipeline);
		rd.ComputeListBindUniformSet(computeList, uniformSet, 0);
		rd.ComputeListDispatch(computeList, xGroups: 5, yGroups: 1, zGroups: 1);
		rd.ComputeListEnd();

		rd.Submit();
		rd.Sync();

		var shaderMaterial = GetNode<TextureRect>("CanvasLayer/TextureRect").Material as ShaderMaterial;
		if (shaderMaterial == null)
		{
			GD.PrintErr("Error getting shaderMaterial");
		}
		shaderMaterial.SetShaderParameter("base", new Vector4(GD.Randf(), GD.Randf(), GD.Randf(), GD.Randf()));
	}

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _Process(double delta)
	{

	}

    public override void _ExitTree()
    {
        base._ExitTree();
		rd.FreeRid(shaderRid);
		// rd.FreeRid(textureRid);
    }

    private Collection<Sphere> InitSpheres()
	{
		const int TOTAL_SPHERES = 20;
		Collection<Sphere> spheres = new Collection<Sphere>();
		for (int i = 0; i < TOTAL_SPHERES; i++)
		{
			Vector3 ambientRandom = new Vector3(GD.Randf(), GD.Randf(), GD.Randf());
			Vector3 diffuseRandom = new Vector3(GD.Randf(), GD.Randf(), GD.Randf());
			Vector3 specularRandom = new Vector3(GD.Randf(), GD.Randf(), GD.Randf());
			float shiny = (float) GD.RandRange(10.0, 300.0);
			float refl = (float) GD.RandRange(0.05, 1.0);

			Material material = new Material(ambientRandom, diffuseRandom, specularRandom, shiny, refl, 0);

			float radius = (float) GD.RandRange(5.0, 25.0);
			float centerX = (float) GD.RandRange(1.0, 100.0);
			Vector3 center = new Vector3(centerX, 1.0f, 1.0f);

			spheres.Add(new Sphere(center, radius, material));
		}

		// GD.Print(spheres.Count);

		return spheres;
	}

	private Plane InitPlane()
	{
		Vector3 ambientRandom = new Vector3(GD.Randf(), GD.Randf(), GD.Randf());
		Vector3 diffuseRandom = new Vector3(GD.Randf(), GD.Randf(), GD.Randf());
		Vector3 specularRandom = new Vector3(GD.Randf(), GD.Randf(), GD.Randf());
		float shiny = (float) GD.RandRange(10.0, 300.0);
		float refl = (float) GD.RandRange(0.05, 1.0);

		Material material = new Material(ambientRandom, diffuseRandom, specularRandom, shiny, refl, 0);

		Plane plane = new Plane(material, new Vector3(0.0f, 0.0f, 0.0f), new Vector3(0.0f, 1.0f, 0.0f));
		return plane;
	}

	private Rid InitShader()
	{
		var shaderFile = GD.Load<RDShaderFile>("res://tracer.glsl");
		var shaderBytecode = shaderFile.GetSpirV();
		var shaderRid = rd.ShaderCreateFromSpirV(shaderBytecode);
		return shaderRid;
	}

	private RDTextureFormat InitTexture()
	{
		RDTextureFormat tf = new RDTextureFormat();
		tf.Format = RenderingDevice.DataFormat.R32G32B32A32Sfloat;
		tf.TextureType = RenderingDevice.TextureType.Type2D;
		tf.Width = (uint) this.GetWindow().Size.X;
		tf.Height = (uint) this.GetWindow().Size.Y;
		tf.Depth = 1;
		tf.ArrayLayers = 1;
		tf.Mipmaps = 1;
		tf.UsageBits = RenderingDevice.TextureUsageBits.SamplingBit
			| RenderingDevice.TextureUsageBits.StorageBit
			| RenderingDevice.TextureUsageBits.CanUpdateBit
			| RenderingDevice.TextureUsageBits.CanCopyToBit;
		return tf;
	}

}
