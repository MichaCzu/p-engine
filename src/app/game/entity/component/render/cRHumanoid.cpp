#include "cRHumanoid.h"
#include "app/game/controller.h"

void CRHumanoid::render(pge::viewport::View& viewport)
{
	float scale = pge::draw::get_scale();

	if (lookDir.x != 0)
		dirRight = lookDir.x > 0;
	dirDown = lookDir.y >= 0;

	if (dirRight)
	{
		draw_larm(viewport, dirDown, dirRight, scale);
		draw_body(viewport, dirDown, dirRight, scale);
		draw_head(viewport, dirDown, dirRight, scale);
		draw_rarm(viewport, dirDown, dirRight, scale);
	}
	else 
	{
		draw_rarm(viewport, dirDown, dirRight, scale);
		draw_body(viewport, dirDown, dirRight, scale);
		draw_head(viewport, dirDown, dirRight, scale);
		draw_larm(viewport, dirDown, dirRight, scale);
	}
}

void CRHumanoid::draw_body(pge::viewport::View& viewport, bool down, bool right, float scale)
{
	pge::Rect anim = animator.getFrame();
	if (!down)
	{
		anim.x += 59;
	}

	pge::Pixel center = pge::Pixel{ 6.5, 11 };

	mainimg->drawExt(
		pge::viewport::get_xmoved(renderCopy.position.x, &viewport) * scale,
		pge::viewport::get_ymoved(renderCopy.position.y, &viewport) * scale,
		&anim, &center, 0.f, scale, scale, right ? GPU_FLIP_NONE : GPU_FLIP_HORIZONTAL);
}

void CRHumanoid::draw_head(pge::viewport::View& viewport, bool down, bool right, float scale)
{
	pge::Rect frame = pge::Rect{ down ? 0 : 59, 0,13,13 };
	pge::Pixel center = pge::Pixel{ 6.5, 19.5 };

	mainimg->drawExt(
		pge::viewport::get_xmoved(renderCopy.position.x, &viewport) * scale,
		pge::viewport::get_ymoved(renderCopy.position.y, &viewport) * scale,
		&frame, &center, 0.f, scale, scale, right ? GPU_FLIP_NONE : GPU_FLIP_HORIZONTAL);

	if (!down)
		return;

	frame = animeye.getFrame();
	center = pge::Pixel{ 3.5, 2 };

	mainimg->drawExt(
		pge::viewport::get_xmoved(renderCopy.position.x + lookDir.x, &viewport) * scale,
		pge::viewport::get_ymoved(renderCopy.position.y - 14.f + lookDir.y * 0.25, &viewport) * scale,
		&frame, &center, 0.f, scale, scale, dirRight ? GPU_FLIP_NONE : GPU_FLIP_HORIZONTAL);
}

void CRHumanoid::draw_rarm(pge::viewport::View& viewport, bool down, bool right, float scale)
{
	const float xpos = 2.5f;
	const float ypos = 8.5f;
	pge::Rect frame = pge::Rect{ right ? 0 : 10, 13, 10, 10 };
	pge::Pixel center = pge::Pixel{ 5, 5 };

	pge::Rect sframe = pge::Rect{ 0, 0, 5, right==down ? 10 : 12 };
	pge::Pixel scenter = pge::Pixel{ right ? -1.f : 6.f, right == down ? 9 : 11 };

	
	float time = animator.getTime();
	if (time >= 600)
		time = 700;
	else if (time >= 400)
		time = 400;
	else if (time >= 200)
		time = 100;
	else
		time = 0;
	time = time / 800.f * 6.283f;


	if (animator.id == 1)
		time = 0.f;

	if(right)
	swordimg->drawExt(
		pge::viewport::get_xmoved(renderCopy.position.x + (down ? -xpos : xpos), &viewport) * scale,
		pge::viewport::get_ymoved(renderCopy.position.y - ypos + SDL_sinf(pge::get_time_passed() / 800.f) * 0.25f, &viewport) * scale,
		&sframe, &scenter, SDL_sinf(time) * 25.f + (!right ? -80.f : 80.f), scale, scale, !right ? GPU_FLIP_HORIZONTAL : GPU_FLIP_NONE);

	mainimg->drawExt(
		pge::viewport::get_xmoved(renderCopy.position.x + (down ? -xpos : xpos), &viewport) * scale,
		pge::viewport::get_ymoved(renderCopy.position.y - ypos + SDL_sinf(pge::get_time_passed() / 800.f) * 0.25f, &viewport) * scale,
		&frame, &center, SDL_sinf(time)*25.f, scale, scale, right!=down ? GPU_FLIP_HORIZONTAL : GPU_FLIP_NONE );

	if (!right)
	swordimg->drawExt(
		pge::viewport::get_xmoved(renderCopy.position.x + (down ? -xpos : xpos), &viewport) * scale,
		pge::viewport::get_ymoved(renderCopy.position.y - ypos + SDL_sinf(pge::get_time_passed() / 800.f) * 0.25f, &viewport) * scale,
		&sframe, &scenter, SDL_sinf(time) * 25.f + (!right ? -80.f : 80.f), scale, scale, !right ? GPU_FLIP_HORIZONTAL : GPU_FLIP_NONE);
}

void CRHumanoid::draw_larm(pge::viewport::View& viewport, bool down, bool right, float scale)
{
	const float xpos = 3.f;
	const float ypos = 8.75f;
	pge::Rect frame = pge::Rect{ right ? 69 : 59, 13, 10, 10 };
	pge::Pixel center = pge::Pixel{ 5, 5 };

	pge::Rect sframe = pge::Rect{ right ? 10 : 0, 0, 10, 10 };
	pge::Pixel scenter = pge::Pixel{ right ? 2.f : 8.f, 6.f };

	float time = animator.getTime();
	if (time >= 600)
		time = 700;
	else if (time >= 400)
		time = 400;
	else if (time >= 200)
		time = 100;
	else
		time = 0;
	time = time / 800.f * 6.283f;
	if (animator.id == 1)
		time = 0.f;

	if(right)
		shieldimg->drawExt(
			pge::viewport::get_xmoved(renderCopy.position.x + (down ? xpos : -xpos), &viewport) * scale,
			pge::viewport::get_ymoved(renderCopy.position.y - ypos + SDL_sinf(pge::get_time_passed() / 800.f) * 0.25f, &viewport) * scale,
			&sframe, &scenter, SDL_sinf(time + 3.14f) * 25.f + (!right ? -80.f : 80.f), scale, scale, !right ? GPU_FLIP_HORIZONTAL : GPU_FLIP_NONE);

	mainimg->drawExt(
		pge::viewport::get_xmoved(renderCopy.position.x + (down ? xpos : -xpos), &viewport) * scale,
		pge::viewport::get_ymoved(renderCopy.position.y - ypos + SDL_sinf(pge::get_time_passed() / 800.f) * 0.25f, &viewport) * scale,
		&frame, &center, SDL_sinf(time+3.14f) * 25.f, scale, scale, right != down ? GPU_FLIP_HORIZONTAL : GPU_FLIP_NONE);

	if (!right)
		shieldimg->drawExt(
			pge::viewport::get_xmoved(renderCopy.position.x + (down ? xpos : -xpos), &viewport) * scale,
			pge::viewport::get_ymoved(renderCopy.position.y - ypos + SDL_sinf(pge::get_time_passed() / 800.f) * 0.25f, &viewport) * scale,
			&sframe, &scenter, SDL_sinf(time + 3.14f) * 25.f + (!right ? -80.f : 80.f), scale, scale, !right ? GPU_FLIP_HORIZONTAL : GPU_FLIP_NONE);
}