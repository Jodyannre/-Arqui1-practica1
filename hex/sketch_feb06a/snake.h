#ifndef SNAKE_H
#define SNAKE_H

class snake{
    public:
        int posX,posY,direccion,tamano;
        int posXant,posYant,matrizActual,matrizAnt;
        bool creado;
        snake();
        virtual ~snake();
};
#endif
