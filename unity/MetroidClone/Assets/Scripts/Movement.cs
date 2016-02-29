using UnityEngine;
using System.Collections;

public class Movement : MonoBehaviour
{

	public float walkSpeed = 100f;
	public float runSpeed = 175f;
	public float jumpForce = 5f;

	Rigidbody2D rigidbody;
	GroundCheck groundCheck;
	Animator animator;

	int blockHash = Animator.StringToHash("Block");
	int attackHash = Animator.StringToHash("Attack");

	void Start()
	{
		rigidbody = GetComponent<Rigidbody2D>();
		groundCheck = GetComponentInChildren<GroundCheck>();
		animator = GetComponentInChildren<Animator>();
	}

	void FixedUpdate()
	{
		bool grounded = groundCheck.Grounded;

		int isRunning = (int)Input.GetAxisRaw("Fire4");
		int horizontal = (int)Input.GetAxisRaw("Horizontal");

		AnimatorStateInfo info = animator.GetCurrentAnimatorStateInfo(0);
		if (info.shortNameHash != blockHash && (info.shortNameHash != attackHash || !grounded))
		{
			rigidbody.velocity = new Vector2(
				horizontal * (isRunning == 1 ? runSpeed : walkSpeed) * Time.deltaTime,
				rigidbody.velocity.y);
		}
		else
		{
			rigidbody.velocity = new Vector2(0, rigidbody.velocity.y);
		}

		int vertical = (int)Input.GetAxisRaw("Fire2");
		if ((vertical == 1) && grounded)
		{
			Vector2 vel = rigidbody.velocity;
			vel.y = 0;
			rigidbody.velocity = vel;
			rigidbody.AddForce(new Vector2(0, jumpForce), ForceMode2D.Impulse);
		}
	}
}
