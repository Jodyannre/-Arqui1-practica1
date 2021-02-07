#include "punto.h"


punto::punto(int posX, int posY, int matriz){
    this->posX = posX;
    this->posY = posY;
    this->posXant = posX;
    this->posYant = posY;
    this->matrizActual = matriz;
    this->matrizAnt = matriz;
}

punto::~punto(){

}