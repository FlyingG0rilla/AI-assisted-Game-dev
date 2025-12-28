
#pragma once
#include <SDL.h>

class Input {
public:
    bool w=false, a= false, s=false, d=false;
    int mouseX=0, mouseY=0;
    bool shoot=false;
    bool space = false;

    void update(bool& running);
};



