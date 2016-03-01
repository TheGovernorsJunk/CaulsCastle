using UnityEngine;
using System.Collections;

public class CameraScript : MonoBehaviour {

	public GameObject subject;
	public Vector2 offset = new Vector2(0, 0);
	public Tiled2Unity.TiledMap map;

	Camera camera;

	void Start ()
	{
		camera = GetComponent<Camera>();
	}
	
	void Update ()
	{
		Vector3 subPosition = new Vector3(
			subject.transform.position.x + offset.x,
			subject.transform.position.y + offset.y,
			transform.position.z);
		transform.position = subPosition;

		if (map != null)
		{
			BottomLimit();
			LeftLimit();
			TopLimit();
			RightLimit();
		}
	}

	void BottomLimit()
	{
		Vector3 cameraPosition = transform.position;
		Vector3 mapPosition = map.transform.position;
		float cameraHalfHeight = camera.orthographicSize;
		float diff = (mapPosition.y - map.NumTilesHigh * map.TileHeight * map.ExportScale) - (cameraPosition.y - cameraHalfHeight);
		if (diff > 0F)
		{
			transform.position = new Vector3(
				transform.position.x,
				transform.position.y + diff,
				transform.position.z);
		}
	}

	void LeftLimit()
	{
		Vector3 cameraPosition = transform.position;
		Vector3 mapPosition = map.transform.position;
		float cameraHalfWidth = camera.orthographicSize * camera.aspect;
		float diff = (mapPosition.x) - (cameraPosition.x - cameraHalfWidth);
		if (diff > 0F)
		{
			transform.position = new Vector3(
				transform.position.x + diff,
				transform.position.y,
				transform.position.z);
		}
	}

	void TopLimit()
	{
		Vector3 cameraPosition = transform.position;
		Vector3 mapPosition = map.transform.position;
		float cameraHalfHeight = camera.orthographicSize;
		float diff = (cameraPosition.y + cameraHalfHeight) - mapPosition.y;
		if (diff > 0F)
		{
			transform.position = new Vector3(
				transform.position.x,
				transform.position.y - diff,
				transform.position.z);
		}
	}

	void RightLimit()
	{
		Vector3 cameraPosition = transform.position;
		Vector3 mapPosition = map.transform.position;
		float cameraHalfWidth = camera.orthographicSize * camera.aspect;
		float diff = (cameraPosition.x + cameraHalfWidth) - (mapPosition.x + map.NumTilesWide * map.TileWidth * map.ExportScale);
		if (diff > 0F)
		{
			transform.position = new Vector3(
				transform.position.x - diff,
				transform.position.y,
				transform.position.z);
		}
	}
}
