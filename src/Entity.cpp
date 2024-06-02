#include "Entity.h"
#include <cmath>
#include "Globals.h"

Entity::Entity(const Point& p, int w, int h) : position(p), dir({ 0,0 }), width(w), height(h), frame_width(w), frame_height(h), render(nullptr)
{
}
Entity::Entity(const Point& p, int w, int h, int frame_w, int frame_h) : position(p), dir({ 0,0 }), width(w), height(h), frame_width(frame_w), frame_height(frame_h), render(nullptr)
{
}
Entity::~Entity()
{
	if (render != nullptr)
	{
		delete render;
		render = nullptr;
	}
}
void Entity::SetPos(const Point& p)
{
	position = p;
}
void Entity::Update()
{
	position += dir;
}
AABB Entity::GetHitbox() const
{
	Point p(position.x, position.y - (height-1));
	AABB hitbox(p, width, height);
	return hitbox;
}
Point Entity::GetRenderingPosition() const
{
	Point p;
	p.x = position.x + width / 2 - frame_width / 2;
	p.y = position.y - (frame_height - 1);
	return p;
}
Point Entity::GetRenderingPositionPlayer() const
{
	Point p;
	p.x = position.x + width / 2 - ((frame_width / 2));
	p.y = position.y - height / 2 - ((frame_height /2) -1 );

	return p;
}
void Entity::Draw() const
{
	Point p = GetRenderingPosition();
	render->Draw(p.x, p.y);
}
void Entity::DrawPlayer() const
{
	Point p = GetRenderingPositionPlayer();
	render->Draw(p.x, p.y);
}
void Entity::DrawTint(const Color& col) const
{
	Point p = GetRenderingPosition();
	render->DrawTint(p.x, p.y, col);
}
void Entity::DrawHitbox(const Color& col) const
{
	Color c = col;
	c.a = 128;		//50% transparent

	render->DrawBox(position.x, position.y - (height - 1), width, height, c);
	render->DrawCorners(position.x, position.y - (height - 1), width, height);
}
void Entity::DrawHitbox(int x, int y, int w, int h, const Color& col) const
{
	Color c = col;
	c.a = 128;		//50% transparent

	render->DrawBox(x, y - (h - 1), w, h, c);
	render->DrawCorners(x, y - (h - 1), w, h);
}