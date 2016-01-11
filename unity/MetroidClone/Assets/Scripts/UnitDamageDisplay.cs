using UnityEngine;
using System.Collections;

public class UnitDamageDisplay : MonoBehaviour {

	public TextMesh textPrototype;

	void Start() {}
	void OnDamageTaken(int damage)
	{
		TextMesh existingText = GetComponentInChildren<TextMesh>();
		if (existingText != null)
		{
			int existingDamage = int.Parse(existingText.text);
			existingText.text = string.Format("{0}", damage + existingDamage);
			existingText.GetComponent<TextDie>().Reset();
		}
		else
		{
			TextMesh text = (TextMesh)Instantiate(textPrototype);
			text.text = string.Format("{0}", damage);
			text.transform.parent = transform;
			text.transform.localPosition = text.transform.position;
		}
	}
}
