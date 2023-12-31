#pragma once
#include "CObj.h"
#include "CAssetMgr.h"

class CPlatform :
    public CObj
{
    GENERATED_OBJECT(CObj);
protected:
    class CCollider* m_Collider;
    FFrame imagePos;
    class CTexture* m_Image;

public:
    void setImagePos(Vec2 _vLeftTop, Vec2 _vCutSize) { imagePos.vLeftTop = _vLeftTop; imagePos.vCutSize = _vCutSize; }
    void setTexture(const wstring& _strTextureName, const wstring& _strRelativePath) { m_Image = CAssetMgr::GetInst()->LoadTexture(_strTextureName, _strRelativePath); }

public:
    virtual void tick(float _DT) override;
    virtual void render(HDC _dc) override;
    virtual void Overlap(CCollider* _OwnCol, CObj* _OtherObj, CCollider* _OtherCol) override;
    virtual void BeginOverlap(CCollider* _OwnCol, CObj* _OtherObj, CCollider* _OtherCol) override;


public:
    CLONE(CPlatform);
    CPlatform();
    CPlatform(const CPlatform& _Origin);
    ~CPlatform();
};

