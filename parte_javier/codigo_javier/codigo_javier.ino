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
  generateFood();

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
    
    case 1://arriba
      if (snakePosY <0){///////JAVIER
        snakePosY +=1;///////JAVIER 
        showGameOverMessage();//AQUI VAN EL GAME OVER///////JAVIER
        return;///////JAVIER
      }else{///////JAVIER
        lc.setLed(numMatriz,snakePosY,snakePosX,true);
        delay(delaytime);
        lc.setLed(numMatriz,snakePosY+1,snakePosX,false);
      }
      break;
      
     case 2://abajo
     if (snakePosY >7){///////JAVIER
       snakePosY -=1;///////JAVIER
       showGameOverMessage();//AQUI VAN EL GAME OVER///////JAVIER
       return;///////JAVIER
     }///////JAVIER
     else{
      lc.setLed(numMatriz,snakePosY,snakePosX,true);
      delay(delaytime);
      lc.setLed(numMatriz,snakePosY-1,snakePosX,false);
     }
      break;
      
     case 3://izquierda
      if (numMatriz == 1 && snakePosX < 0){
        matrizTemp = numMatriz;
        numMatriz = 0;
        posTemp = snakePosX+1;
        snakePosX = 7;

      }else if(numMatriz == 0 && snakePosX < 0){///////JAVIER

       snakePosX +=1;///////JAVIER
       showGameOverMessage();//AQUI VAN EL GAME OVER///////JAVIER
       return;///////JAVIER

      }else{
        matrizTemp = numMatriz;
        posTemp = snakePosX+1;
      }
      
      lc.setLed(numMatriz,snakePosY,snakePosX,true);
      delay(delaytime);
      lc.setLed(matrizTemp,snakePosY,posTemp,false);
      break;
      
     case 4://derecha
      if (numMatriz == 0 && snakePosX > 7){
        matrizTemp = numMatriz;
        numMatriz = 1;
        posTemp = snakePosX-1;
        snakePosX = 0;

        }else if(numMatriz == 1 && snakePosX > 7){///////JAVIER

       snakePosX -=1;///////JAVIER
       showGameOverMessage();//AQUI VAN EL GAME OVER///////JAVIER
       return;///////JAVIER
       
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


//*************************************************************************************************************************************************************************************************************
//*************************************************************************************************************************************************************************************************************
//******************************************************************************************JAVIER ROBERTO ALFARO*******************************************************************************************
//*************************************************************************************************************************************************************************************************************
//*************************************************************************************************************************************************************************************************************

int puntos =0;
const short messageSpeed = 5;

//clase de la comida
struct Point {
  int row = 0, col = 0, matriz =0;
  Point(int row = 0, int col = 0,int matriz = 0): row(row), col(col), matriz(matriz) {}
};


// declaro la comida
Point food(-1, -1,-1);

// if there is no food, generate one, also check for victory

void generateFood() {
      delay(100);

// ver si cuadran la posicion de la serpiente con la de la comida (por ahora)
  if (food.row == -1 || food.col == -1||food.matriz==-1) { 
    food.col = random(8);//asigno col a comida
    food.row = random(8);//asigno fila a comida
    food.matriz = random(2);//asigno matrz a comida
    lc.setLed(food.matriz,food.row, food.col,true);

    //AQUI PUEDE IR EL +1 AL PUNTEO

  }else if(snakePosX == food.col && snakePosY ==food.row && numMatriz==food.matriz){ //si la cabeza se come la comida 
        food.col = -1;
        food.row = -1;
    }

  
}

// scrolls the 'game over' message around the matrix


const PROGMEM bool gameOverMessage[8][90] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

void showGameOverMessage() {
  [&] {
    int numMat=1; //****
    for (int d = 0; d < sizeof(gameOverMessage[0]) - 7; d++) {
      
      for (int col = 0; col < 8; col++) {
        numMat = col<8 ? 1 : 0; //****
        delay(messageSpeed);
        for (int row = 0; row < 8; row++) {
          
          // this reads the byte from the PROGMEM and displays it on the screen
          lc.setLed(numMat, row, col, pgm_read_byte(&(gameOverMessage[row][col + d]))); //****
        }
      }

    }
  }();

  lc.clearDisplay(0);
  lc.clearDisplay(1);


}
