#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player(Location location, Dimension dimension, const char* name);
	~Player();

public:
	int Health = 100;

	void Attack() override;
	void Jump() override;

	bool IsInAir() const { return Jumping || Falling; }
	bool IsAttacking() const { return is_attacking; }
	bool IsDead() const { return (Health <= 0); }
	bool IsHurt() const { return is_hurt; }

	void Update() override;
	void Move() override;
	void Idle() override;

	void TakeDamage();

	int GetKills();

private:
	const char* FilePath = "./res/img/PlayerImages.path";

	//Idle
	//----------------
	int Frames = 0;

	//Attack
	//----------------
	bool is_attacking = false;

	//Damage && Death
	//----------------
	int DamageTime = 0;
	bool is_hurt = false;

	// Jump & falling
	//--------------
	double Gravity;
	bool Jumping = false;
	bool Falling = false;

	void SetDamageTimeOnce();
	void ResetDoOnce() { do_once = true; }
	bool do_once = true;

	void MoveToLeft();
	void MoveToRight();
	void CauseDamage();
};

