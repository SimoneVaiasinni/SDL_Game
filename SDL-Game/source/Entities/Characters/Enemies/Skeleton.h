#pragma once
#include "../Character.h"

class Skeleton : public Character {

public:
	Skeleton(Location location, Dimension dimension, const char* name);
	~Skeleton();

	int Health = 100;

	void Render()override;
	void Update() override;
	void Move() override;
	void Idle() override;
	void Attack() override;

	bool IsAttacking() const { return is_attacking; }
	bool IsDead() const { return (Health <= 0); }
	bool IsHurt() const { return hit_react; }

	void TakeDamage();
private:
	const char* FilePath = "./res/img/SkeletonImages.path";

	//Idle
	//----------------
	int FramesIdle = 0;

	//Moving
	//----------------
	bool is_moving = false;
	double velocity = 0;

	//Attack
	//----------------
	int FramesAttack = 0;
	bool is_attacking = false;

	//Damage && Death
	//----------------
	int FramesHit = 0;
	int FramesDeath = 0;
	double TimeDeath = 0;

	bool hit_react = false;
	
	bool DamageOnce = true;
	bool DeathOnce = true;

	void MoveToLeft();
	void MoveToRight();

	void CauseDamage();
};