#include <MatrizLed.h>
#include <LedControl.h>
#include "punto.h"
#include "snake.h"

//Definición de los pines utilizados
const int butPinUp = 7;
const int butPinDown = 6;
const int butPinLeft = 5;
const int butPinRight = 4;
const int ledPin = 3;

//Creación de variable que manejará la matriz
LedControl lc=LedControl(12,11,10,2);

//Variable para manejar el delay de actualización y velocidad de la snake
unsigned long delaytime=700;

//Variables de estados de los botones
int butUpState = 0;
int butDownState = 0;
int butLeftState = 0;
int butRightState = 0;

//Variables para el manejo de la snake y sus sectores
int ultimaPosX = 0;
int ultimaPosY = 0;
int ultimaMatriz = 0;

//Snake, matriz y sectores
snake cabeza;
snake sector;
snake* matriz[129];

//Comida
//clase de la comida 
struct Point {
  int row = 0, col = 0, matriz =0;
  Point(int row = 0, int col = 0,int matriz = 0): row(row), col(col), matriz(matriz) {}
};

//Variables de estados del juego
int puntos = 0;
bool gameOver = false;
bool snakeCrecio = false;


//declaro el 1.er objeto comida //JAVIER
Point food(-1, -1,-1);//JAVIER




void setup() { 
  //Inicializando los pines de los botones como entradas
  pinMode(butPinUp, INPUT);
  pinMode(butPinDown,INPUT);
  pinMode(butPinLeft,INPUT);
  pinMode(butPinRight,INPUT);
  pinMode(ledPin,OUTPUT);
  Serial.begin(9600);

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
  generarPosicionInicial();
  generateFood();
  lc.setLed(cabeza.matrizActual,cabeza.posY,cabeza.posX,true);
  matriz[0]=&cabeza;
  matriz[1]=&sector;
  delay(delaytime);
}

void loop() {
  //Leer los estados de los botones
  butUpState = digitalRead(butPinUp);
  butDownState = digitalRead(butPinDown);
  butLeftState = digitalRead(butPinLeft);
  butRightState = digitalRead(butPinRight);
  //Actualizar posiciones y pintar la matriz
  actualizarDireccion();
  //if (cabeza.tamano == 2){crearSector();}
  actualizarPosicion();
  buscarUltimoSector();
  generateFood();
  mover();
}


//Método para inicializar y generar la posición aleatoría de la cabeza de la serpiente
void generarPosicionInicial(){
    cabeza.posX = random(1,8);
    cabeza.posY = random(1,8);
    cabeza.matrizActual = random(2);
    cabeza.direccion = random(1,5);
    cabeza.posXant = cabeza.posX;
    cabeza.posYant = cabeza.posY;
    cabeza.matrizAnt = cabeza.matrizActual;
    cabeza.creado = true;
    cabeza.tamano = 1;
}


//Método que actualiza la dirección de la snake
void actualizarDireccion(){
  if (butUpState == HIGH){
    //Se enciende el led
    digitalWrite(ledPin,HIGH);
    cabeza.direccion = 1;
  }else if (butDownState){
    digitalWrite(ledPin,HIGH);
    cabeza.direccion = 2;
  }else if (butLeftState){
    digitalWrite(ledPin,HIGH);
    cabeza.direccion = 3;
  }else if (butRightState){
    digitalWrite(ledPin,HIGH);
    cabeza.direccion = 4;
  }else{
    //Se mantiene apagado el led
    digitalWrite(ledPin,LOW);
  }  
}


//Método que actualiza la posición de la cabeza
void actualizarPosicion(){
  switch(cabeza.direccion){
    case 1:
      cabeza.posYant = cabeza.posY;
      cabeza.posY--;
      cabeza.posXant = cabeza.posX;
      cabeza.matrizAnt = cabeza.matrizActual;
      if (cabeza.posY < 0){
        //Pierde
      }
      break;
    case 2:
      cabeza.posYant = cabeza.posY;
      cabeza.posY++;
      cabeza.posXant = cabeza.posX;
      cabeza.matrizAnt = cabeza.matrizActual;
      if (cabeza.posY > 7){
        //Pierde
      }
      break;
    case 3:
      cabeza.posXant = cabeza.posX;
      cabeza.posYant = cabeza.posY;
      cabeza.posX--;
      if (cabeza.posX < 0 && cabeza.matrizActual == 0){
        //Pierde  
      }else if (cabeza.posX < 0 && cabeza.matrizActual == 1){
        cabeza.matrizActual = 0;
        cabeza.matrizAnt = 1;
        cabeza.posXant = 0;        
        cabeza.posX = 7;       
      }else{
        cabeza.matrizAnt = cabeza.matrizActual;
      }
      break;
    case 4:
      cabeza.posXant = cabeza.posX;
      cabeza.posYant = cabeza.posY;
      cabeza.posX++;
      if (cabeza.posX > 7 && cabeza.matrizActual == 0){
        cabeza.matrizActual = 1;
        cabeza.matrizAnt = 0;
        cabeza.posXant = 7;      
        cabeza.posX = 0;
      }else if (cabeza.posX > 7 && cabeza.matrizActual == 1){
        //Pierde
      }else{
        cabeza.matrizAnt = cabeza.matrizActual;
      }
      break;
  }
}


//Método que encuentra el último sector existente
void buscarUltimoSector(){
  for (int i=1;i<129;i++){
     if (matriz[i]->creado){
        Serial.println("Si existe");
        matriz[i]->posXant = matriz[i]->posX;
        matriz[i]->posYant = matriz[i]->posY;
        matriz[i]->matrizAnt = matriz[i]->matrizActual;
        matriz[i]->posX = matriz[i-1]->posXant;
        matriz[i]->posY = matriz[i-1]->posYant;
        matriz[i]->matrizActual = matriz[i-1]->matrizAnt;
     }else{
        ultimaPosX = matriz[i-1]->posXant;
        ultimaPosY = matriz[i-1]->posYant;
        ultimaMatriz = matriz[i-1]->matrizAnt;
        if (snakeCrecio){
          crearSector(i);
        }
        break;
     }
  }
}


// Método que repinta la matriz con las nuevas posiciones
void mover(){
  lc.setLed(cabeza.matrizActual,cabeza.posY,cabeza.posX,true);
  if (cabeza.tamano==1){
    lc.setLed(cabeza.matrizAnt,cabeza.posYant,cabeza.posXant,false);
  }else{
    if (snakeCrecio){
      snakeCrecio = false; //Se vuelve a colocar en false para el próximo movimiento
    }else{
      lc.setLed(ultimaMatriz,ultimaPosY,ultimaPosX,false);
    }
  }
  delay(delaytime);
}

//Crea nuevos sectores después de encontrar comida
void crearSector(int posicion){
  Serial.println("Creando sector");
  snake nSector;
  snake vacio;
  /*
  snake nSector1;
  snake nSector2;
  */
  
  nSector.creado = true;
  nSector.posX = ultimaPosX;
  nSector.posY = ultimaPosY;
  nSector.matrizActual = ultimaMatriz;
  nSector.posXant = ultimaPosX;
  nSector.posYant = ultimaPosY;
  nSector.matrizAnt = ultimaMatriz;

  /*
  nSector1.creado = true;
  nSector1.posX = 1;
  nSector1.posY = 0;
  nSector1.matrizActual = 0;
  nSector1.posXant = 0;
  nSector1.posYant = 0;
  nSector1.matrizAnt = 0;

  
  nSector2.creado = true;
  nSector2.posX = 0;
  nSector2.posY = 0;
  nSector2.matrizActual = 0;
  nSector2.posXant = 0;
  nSector2.posYant = 0;
  nSector2.matrizAnt = 0;
  */
  
  delete matriz[posicion];
  matriz[posicion]=&nSector;
  //matriz[2]=&nSector1;
  //matriz[3]=&nSector2;
  matriz[posicion+1]=&vacio;
  //lc.setLed(0,0,0,true);
  //lc.setLed(0,0,1,true);
  //lc.setLed(0,0,2,true);
  delay(delaytime);
  cabeza.tamano++;
}


//Método para la generación de la comida
void generateFood() {
      delay(100);     
      if(gameOver == false){
      lc.setLed(food.matriz,food.row, food.col,true);
      }else{
        return;
        }

// ver si cuadran la posicion de la serpiente con la de la comida (por ahora)
  if (food.row == -1 || food.col == -1||food.matriz==-1) { 
    food.col = random(1,7);//asigno col a comida
    food.row = random(8);//asigno fila a comida
    food.matriz = random(2);//asigno matrz a comida
    lc.setLed(food.matriz,food.row, food.col,true);


  Serial.println(food.row);
  Serial.println(food.col);
  Serial.println(food.matriz);
  Serial.println("------------------------");

    //AQUI PUEDE IR EL +1 AL PUNTEO
    

  }else if(cabeza.posX == food.col && cabeza.posY ==food.row && cabeza.matrizActual==food.matriz){ //si la cabeza se come la comida 
        food.col = -1;
        food.row = -1;
        puntos+=1;
        Serial.print(">>>puntos:");
        Serial.print(puntos);
        Serial.println();
        snakeCrecio = true;
    }

  
}
