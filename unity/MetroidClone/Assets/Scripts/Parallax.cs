using UnityEngine;
using System.Collections;

public class Parallax : MonoBehaviour
{
	public float parallaxFactorX = 1f;
	public float parallaxFactorY = 1f;
	public SpriteRenderer parallaxBackground;

	private Camera camera;

	void Start()
	{
		camera = GetComponent<Camera>();
	}

	void Update ()
	{
		Vector3 position = new Vector3(
			parallaxFactorX * camera.transform.position.x,
			parallaxFactorY * camera.transform.position.y);
		position.z = parallaxBackground.transform.position.z;
		parallaxBackground.transform.position = position;
	}
}
