#include "snake.h"


snake::snake(){
    this->posX = 0;
    this->posY = 0;
    this->direccion = 4;
    this->matrizActual  = 0;
    this->matrizAnt = 0;
    this->creado = true;
}

snake::~snake(){

}