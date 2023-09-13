#pragma once
#include"Obj.h"

class CMonster : public CObj
{
public:
	CMonster();
	~CMonster();

    HRESULT Initialize(void) override;
    int Update(void) override;
    void Late_Update(void) override;
    void Render(void) override;
    void Release(void) override;

public:

private:


};

