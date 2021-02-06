#include <MatrizLed.h>
#include <LedControl.h>

//Definición de los pines utilizados
const int butPinUp = 7;
const int butPinDown = 6;
const int butPinLeft = 5;
const int butPinRight = 4;
const int ledPin = 3;

//Creación de variable que manejará la matriz
LedControl lc=LedControl(12,11,10,2);

//Variable para manejar el delay de actualización
unsigned long delaytime=100;

//Estados de los botones
int butUpState = 0;
int butDownState = 0;
int butLeftState = 0;
int butRightState = 0;

//Estado de serpiente
int snakePosX = 0;
int snakePosY = 0;

//Estado de matriz
int numMatriz = 0;




void setup() { 
  //Inicializando los pines de los botones como entradas
  pinMode(butPinUp, INPUT);
  pinMode(butPinDown,INPUT);
  pinMode(butPinLeft,INPUT);
  pinMode(butPinRight,INPUT);
  pinMode(ledPin,OUTPUT);


  //Inicialización de la matriz
  //Obtener cantidad de matrices
  //Cancelar modo ahorro
  for (int i = 0;i<2;i++){
    lc.shutdown(i,false);
    //Configurar brillo
    lc.setIntensity(i,8);
    //Limpiar la matriz
    lc.clearDisplay(i);    
  }
  lc.setLed(0,0,0,true);
}

void loop() {
  //Leer los estados de los botones
  butUpState = digitalRead(butPinUp);
  butDownState = digitalRead(butPinDown);
  butLeftState = digitalRead(butPinLeft);
  butRightState = digitalRead(butPinRight);

  //Comportamiento de los botones
  if (butUpState == HIGH){
    //Se enciende el led
    digitalWrite(ledPin,HIGH);
    snakePosY-=1;
    actualizarPosSnake(1);
  }else if (butDownState){
    digitalWrite(ledPin,HIGH);
    snakePosY+=1;
    actualizarPosSnake(2);
  }else if (butLeftState){
    digitalWrite(ledPin,HIGH);
    snakePosX-=1;
    actualizarPosSnake(3);
  }else if (butRightState){
    digitalWrite(ledPin,HIGH);
    snakePosX+=1;
    actualizarPosSnake(4);
  }else{
    //Se mantiene apagado el led
    digitalWrite(ledPin,LOW);
  }

}


void actualizarPosSnake(int horientacion){
  int posTemp, matrizTemp;
  switch(horientacion){
    case 1:
      lc.setLed(numMatriz,snakePosY,snakePosX,true);
      delay(delaytime);
      lc.setLed(numMatriz,snakePosY+1,snakePosX,false);
      break;
     case 2:
      lc.setLed(numMatriz,snakePosY,snakePosX,true);
      delay(delaytime);
      lc.setLed(numMatriz,snakePosY-1,snakePosX,false);
      break;
     case 3:
      if (numMatriz == 1 && snakePosX < 0){
        matrizTemp = numMatriz;
        numMatriz = 0;
        posTemp = snakePosX+1;
        snakePosX = 7;
      }else{
        matrizTemp = numMatriz;
        posTemp = snakePosX+1;
      }
      lc.setLed(numMatriz,snakePosY,snakePosX,true);
      delay(delaytime);
      lc.setLed(matrizTemp,snakePosY,posTemp,false);
      break;
     case 4:
      if (numMatriz == 0 && snakePosX > 7){
        matrizTemp = numMatriz;
        numMatriz = 1;
        posTemp = snakePosX-1;
        snakePosX = 0;
      }else{
        posTemp = snakePosX-1;
        matrizTemp = numMatriz;
      }
      lc.setLed(numMatriz,snakePosY,snakePosX,true);
      delay(delaytime);
      lc.setLed(matrizTemp,snakePosY,posTemp,false);
      break;
  }
  delay(delaytime);
}
