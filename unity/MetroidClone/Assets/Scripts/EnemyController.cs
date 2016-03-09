using UnityEngine;
using System.Collections;

public class EnemyController : MonoBehaviour
{
	public Transform approachStart;
	public Transform approachEnd;
	public Transform attackPoint;
	public LayerMask attackMask;

	public float moveForce = 50f;
	public float maxSpeed = 0.5f;

	Rigidbody2D rigidbody;
	Animator animator;

	int attackHash = Animator.StringToHash("attack");
	int attackAnimHash = Animator.StringToHash("Attack");
	int speedHash = Animator.StringToHash("speed");

	void Start() {} 

	void Awake()
	{
		rigidbody = GetComponent<Rigidbody2D>();
		animator = GetComponent<Animator>();
	}
	
	void Update()
	{
	}

	void FixedUpdate()
	{
		bool isAttacking = animator.GetCurrentAnimatorStateInfo(0).shortNameHash == attackAnimHash;
		RaycastHit2D approachHit = Physics2D.Linecast(approachStart.position, approachEnd.position, attackMask);

		if (Physics2D.OverlapPoint(attackPoint.position, attackMask) && !isAttacking)
		{
			animator.SetTrigger(attackHash);
		}
		else if (approachHit && !isAttacking)
		{
			float direction = Mathf.Sign(approachHit.point.x - transform.position.x);
			rigidbody.AddForce(new Vector2(direction * moveForce, rigidbody.velocity.y));
			transform.localScale = new Vector3(Mathf.Abs(transform.localScale.x) * direction, transform.localScale.y, transform.localScale.z);
			animator.SetFloat(speedHash, direction);
		}

		if (Mathf.Abs(rigidbody.velocity.x) > maxSpeed)
		{
			rigidbody.velocity = new Vector2(Mathf.Sign(rigidbody.velocity.x) * maxSpeed, rigidbody.velocity.y);
		}
		if (!approachHit)
		{
			animator.SetFloat(speedHash, 0f);
		}
	}
}
