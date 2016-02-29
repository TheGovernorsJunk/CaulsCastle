using UnityEngine;
using System.Collections;

public class ElisaAnimation : MonoBehaviour
{
	Animator animator;
	GroundCheck groundCheck;

	int groundedHash = Animator.StringToHash("grounded");
	int attackHash = Animator.StringToHash("attack");
	int blockingHash = Animator.StringToHash("blocking");
	int speedHash = Animator.StringToHash("speed");

	void Awake()
	{
		animator = GetComponent<Animator>();
		groundCheck = GetComponentInChildren<GroundCheck>();
	}

	void Update()
	{
		animator.SetBool(groundedHash, groundCheck.Grounded);

		if (Input.GetButtonDown("Fire1"))
			animator.SetTrigger(attackHash);

		animator.SetBool(blockingHash, (int)Input.GetAxisRaw("Fire3") == 1);

		float horizontal = Input.GetAxisRaw("Horizontal");
		animator.SetFloat(speedHash, horizontal);
		if (horizontal != 0f)
			transform.localScale = new Vector3(
				Mathf.Abs(transform.localScale.x) * horizontal,
				transform.localScale.y,
				transform.localScale.z);
	}
}
