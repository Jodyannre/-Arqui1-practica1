#ifndef PUNTO_H
#define PUNTO_H
#include "snake.h"

class punto : public snake{
    public:
        punto(int posX, int posY, int matriz);
        virtual ~punto();
};


#endif
