#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H


#include "SpriteAnimation.h"
#include "SceneBase.h"

class sprite
{
public:
    sprite(SpriteAnimation* spriteanim,
        Vector3 pos,
        Vector3 scale,
        bool flip, Mesh *mesh);
    ~sprite();

    SpriteAnimation* spriteanim;
    Vector3 pos;
    Vector3 scale;
    bool flip;
    Mesh *mesh;
private:

};

class SpriteManager : public SceneBase
{
public:
    SpriteManager();
    ~SpriteManager();

    void Init(float ortho_X, float ortho_Y);
    void update(double dt);

    void RenderSprite(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate, bool flip);
    void NewSpriteAnimation(Mesh *mesh, Vector3 pos, Vector3 scale, int row, int col,
        int start, int end, float speed, int no_of_repeat, bool flip);

    //void PlaySprite(SpriteAnimation* Sprite_Animation);
    void spriteRender();

    std::vector<SpriteAnimation* > SpriteAnimation_ListRender;
    std::vector<SpriteAnimation* > SpriteAnimation_List;

    std::vector<sprite* >spritelist;
    float ortho_X, ortho_Y;
};



#endif