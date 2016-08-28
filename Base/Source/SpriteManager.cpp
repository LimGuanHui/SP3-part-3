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
        
        /*SpriteAnimation *sa = static_cast<SpriteAnimation*>(meshList[GEO_NET]);
        if (sa)
        {
            sa->Update(dt);
            sa->m_anim->animActive = true;
        }*/

    }
}

//void SpriteManager::RenderSprite(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate, bool flip)
//{
//    for (std::vector<SpriteAnimation*>::iterator it = SpriteAnimation_List.begin(); it != SpriteAnimation_List.end(); ++it)
//    {
//        SpriteAnimation* go = (SpriteAnimation*)*it;
//        Render2DMesh()
//    }
//            
//}
void SpriteManager::NewSpriteAnimation(Mesh *mesh, Vector3 pos, int row, int col, 
    int start, int end, float speed, int no_of_repeat)
{
    SpriteAnimation* go = new SpriteAnimation(mesh->name, row, col);
    go->m_anim = new Animation();
    go->m_anim->Set(start, end, no_of_repeat, speed, false);
    SpriteAnimation_List.push_back(go);
    
}

//void SpriteManager::PlaySprite(SpriteAnimation* Sprite_Animation)
//{
//    SpriteAnimation sp = *Sprite_Animation;
//    SpriteAnimation* go = new SpriteAnimation(*sp);
//}

