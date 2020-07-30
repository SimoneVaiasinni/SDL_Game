#pragma once
#include "Pawn.h"
#include <vector>

class FirePlace : public Pawn {

public:
	FirePlace(Location location,Dimension dimension, const char* name);
	~FirePlace();

	void Update() override;
	void Idle() override;

private:
	const char* FilePath = "./res/img/FirePlaceImages.path";
	int Frames = 0;

	bool IsWeak;

	std::vector<std::string> Weak = 
	{
		"./res/img/FirePlace/FP_1.png",
		"./res/img/FirePlace/FP_2.png",
		"./res/img/FirePlace/FP_3.png",
		"./res/img/FirePlace/FP_4.png",
		"./res/img/FirePlace/FP_5.png",
		"./res/img/FirePlace/FP_6.png"
	};

	std::vector<std::string> Strong =
	{
		"./res/img/FirePlace/FP_7.png",
		"./res/img/FirePlace/FP_8.png",
		"./res/img/FirePlace/FP_9.png",
		"./res/img/FirePlace/FP_10.png",
		"./res/img/FirePlace/FP_11.png",
		"./res/img/FirePlace/FP_12.png"
	};
};
