#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include "Mesh.h"
#include "SpriteAnimation.h"
class SpriteManager
{
public:
    SpriteManager();
    ~SpriteManager();

    void Init(float ortho_X, float ortho_Y);
    void update(double dt);

    void RenderSprite(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate, bool flip);
    SpriteAnimation* NewSpriteAnimation(Mesh *mesh, Vector3 pos, int row, int col,
        int start, int end, float speed, int no_of_repeat);

    void PlaySprite(SpriteAnimation* SpriteAnimation);

    std::vector<SpriteAnimation* > SpriteAnimation_ListRender;
    std::vector<SpriteAnimation* > SpriteAnimation_List;
    float ortho_X, ortho_Y;
};

#endif