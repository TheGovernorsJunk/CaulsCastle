using UnityEngine;
using System.Collections;

public class Parallax : MonoBehaviour
{
	public float parallaxFactorX = 1f;
	public float parallaxFactorY = 1f;
	public GameObject parallaxBackground;

	void Update ()
	{
		Vector3 position = new Vector3(
			parallaxFactorX * transform.position.x,
			parallaxFactorY * transform.position.y,
			parallaxBackground.transform.position.z);
		parallaxBackground.transform.position = position;
	}
}
