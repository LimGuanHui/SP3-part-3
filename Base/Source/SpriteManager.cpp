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
    if (spritelist.size() != 0)
    {
        for (std::vector<sprite*>::iterator it = spritelist.begin(); it != spritelist.end(); ++it)
        {
            sprite* go = (sprite*)*it;
            if (!go->spriteanim->m_anim->animActive)
            {
                delete go->spriteanim->m_anim;
                go->spriteanim->m_anim = NULL;
                delete go->spriteanim;
                go->spriteanim = NULL;
                spritelist.erase(it);
                if (spritelist.size() == 0)
                    break;
                 continue;
            }
            go->spriteanim->Update(dt);
            
        }
    }
    
}

void SpriteManager::RenderSprite(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate, bool flip)
{    
    Render2DMesh(mesh, enableLight, size, x, y, rotate, flip);
}
//void SpriteManager::NewSpriteAnimation(Mesh *mesh, Vector3 pos, Vector3 scale, int row, int col,
//    int start, int end, float speed, int no_of_repeat, bool flip)
//{
//    
//    SpriteAnimation *go = dynamic_cast<SpriteAnimation*>(mesh);
//    go->m_anim = new Animation();
//    go->m_anim->Set(start, end, no_of_repeat, speed, true);
//    sprite* N_sprite = new sprite(go, pos, scale, flip, mesh);
//    spritelist.push_back(N_sprite);
//    //SpriteAnimation_List.push_back(go);
//    //return go;
//}

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