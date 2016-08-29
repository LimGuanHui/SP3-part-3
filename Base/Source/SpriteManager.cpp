#include "SpriteManager.h"
#include "Mtx44.h"

SpriteManager::SpriteManager():
ortho_X(0),
ortho_Y(0)
{
}


SpriteManager::~SpriteManager()
{
}

void SpriteManager::Init(float ortho_X, float ortho_Y)
{

}

void SpriteManager::update(double dt)
{
    for (std::vector<SpriteAnimation*>::iterator it = SpriteAnimation_ListRender.begin(); it != SpriteAnimation_ListRender.end(); ++it)
    {
        SpriteAnimation* go = (SpriteAnimation*)*it;
        if (!go->m_anim->animActive)
        {
            SpriteAnimation_List.erase(it);
            continue;
        }
        go->Update(dt);
        
        /*SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_NET]);
        if (sa)
        {
            sa->Update(dt);
            sa->m_anim->animActive = true;
        }*/

    }
}

void SpriteManager::RenderSprite(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate, bool flip)
{    

    Render2DMesh(mesh, enableLight, size, x, y, rotate, flip);
}
void SpriteManager::NewSpriteAnimation(Mesh *mesh, Vector3 pos, Vector3 scale, int row, int col,
    int start, int end, float speed, int no_of_repeat, bool flip)
{
    SpriteAnimation* go = new SpriteAnimation(mesh->name, row, col);
    go->m_anim = new Animation();
    go->m_anim->Set(start, end, no_of_repeat, speed, false);
    sprite* N_sprite = new sprite(go, pos, scale, flip, mesh);
    spritelist.push_back(N_sprite);
    //SpriteAnimation_List.push_back(go);
    //return go;
}

//void SpriteManager::PlaySprite(SpriteAnimation* Sprite_Animation)
//{
//    //SpriteAnimation sp = *Sprite_Animation;
//    SpriteAnimation* go = new SpriteAnimation(*Sprite_Animation);
//    go->m_anim->animActive = true;
//    
//    SpriteAnimation_ListRender.push_back(go);
//}
void SpriteManager::spriteRender()
{
    for (std::vector<sprite*>::iterator it = spritelist.begin(); it != spritelist.end(); ++it)
    {
        sprite* go = (sprite*)*it;
        Render2DMesh(go->mesh, false, go->scale.x, go->scale.y, go->pos.x, go->pos.x, go->flip, false);
    }
}

//sprite
sprite::sprite(SpriteAnimation* spriteanim,
    Vector3 pos,
    Vector3 scale,
    bool flip, Mesh *mesh) :
    spriteanim(spriteanim),
    pos(pos),
    scale(scale),
    flip(flip),
    mesh(mesh)
{

}
sprite::~sprite()
{
    
}