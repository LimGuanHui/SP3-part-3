
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue)
: type(typeValue),
scale(1, 1, 1),
active(false),
mass(1.f),
normal(0, 0, 0),
name("nameless")

{
}

GameObject::~GameObject()
{
}

void GameObject::Init()
{
    type = GameObject::GO_BALL;
    scale = Vector3(1, 1, 1);
    active = false;
    mass = 1.f;
    normal = Vector3(0, 0, 0);
    name = string("nameless");
}
