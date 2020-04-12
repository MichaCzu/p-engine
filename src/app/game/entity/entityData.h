#pragma once

enum EEDamageGroup
{
	eed_player,
	eed_friendly,
	eed_neutral,
	eed_enemy,
};

struct EntityData
{
	pge::Pixel position = pge::Pixel::Zero();
	float radius = 0;
	bool collidable = false;
};
