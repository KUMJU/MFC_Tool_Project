#pragma once
#include"Obj.h"

class CEnvironment : public CObj
{
public:

    CEnvironment();
    ~CEnvironment();

    HRESULT Initialize(void) override;
    int Update(void) override;
    void Late_Update(void) override;
    void Render(void) override;
    void Release(void) override;

public:

    void SetImgCount(int _count) { m_iImgCount = _count; }

private:

    int m_iImgCount;

};

