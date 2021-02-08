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

//clase de la comida //JAVIER
struct Point {//JAVIER
  int row = 0, col = 0, matriz =0;//JAVIER
  Point(int row = 0, int col = 0,int matriz = 0): row(row), col(col), matriz(matriz) {}//JAVIER
};//JAVIER


//declaro el 1.er objeto comida //JAVIER
Point food(-1, -1,-1);//JAVIER


int special = 8;//JAVIER
int specialState =0;//JAVIER

//nuevas variables para medir tiempo no tocar
unsigned long duration;//JAVIER
long tiempoJavier =0;//JAVIER
bool pausabool =false;//JAVIER
bool gameOver =false;//Javier
bool modoPractica =false;

void setup() { 

  Serial.begin(9600);
  
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
  //lc.setLed(0,0,0,true);// posicion inicial de snake JOEL

  Serial.println(">>>>>>>presione START por 3 seg/veces! para iniciar<<<<<<<<)");//JAVIER

 

  // pulso de buttom
  pinMode(special, INPUT);//JAVIER
  
}

void loop() {

  BIFROST();// sin esto no se puede empesar el juego lo q hace es si start no se oprime por 3 seg o veces no inicial el juego esto mismo se hace despues del game over
  //modoPractica se habilita en metodo BIFROST
  if(modoPractica ==true){
  generateFood();//JAVIER

  specialState = digitalRead(special);//JAVIER para el boton de START

  //Leer los estados de los botones
  butUpState = digitalRead(butPinUp);
  butDownState = digitalRead(butPinDown);
  butLeftState = digitalRead(butPinLeft);
  butRightState = digitalRead(butPinRight);

  //Comportamiento de los botones
  if (butUpState == HIGH && pausabool==false && gameOver ==false){//JAVIER para q cuando entre en pause no se mueva la serpiente coloque la nueva condicion
    //Se enciende el led
    digitalWrite(ledPin,HIGH);
    snakePosY-=1;
    actualizarPosSnake(1);
  }else if (butDownState && pausabool==false && gameOver ==false){//JAVIER para q cuando entre en pause no se mueva la serpiente coloque la nueva condicion
    digitalWrite(ledPin,HIGH);
    snakePosY+=1;
    actualizarPosSnake(2);
  }else if (butLeftState && pausabool==false && gameOver ==false){//JAVIER para q cuando entre en pause no se mueva la serpiente coloque la nueva condicion
    digitalWrite(ledPin,HIGH);
    snakePosX-=1;
    actualizarPosSnake(3);
  }else if (butRightState && pausabool==false && gameOver ==false){//JAVIER para q cuando entre en pause no se mueva la serpiente coloque la nueva condicion
    digitalWrite(ledPin,HIGH);
    snakePosX+=1;
    actualizarPosSnake(4);
    
  }else if(specialState == HIGH){//JAVIER si START FUE PRECIONADO tengo q calcular si es pause o endgame su funcion es la de contar el n numero de seg de presion
    duration = pulseIn(special,HIGH);//JAVIER
    
    if(duration==0){//JAVIER
      tiempoJavier+=1;//JAVIER
      Serial.println(tiempoJavier);//JAVIER
    }//JAVIER
  
  }else{
    //Se mantiene apagado el led
    digitalWrite(ledPin,LOW);

  if(tiempoJavier==1){//JAVIER aqui valido el n numero de segundos q calcule anterior
    
    pausabool = !pausabool;//JAVIER cambio de estado automaticamente si es pausa
    pausa();//JAVIER llamo al metodo de desplique de mensaje de score o que reanima el juego
    tiempoJavier=0;//JAVIER
    return;//JAVIER
    
    }else if(tiempoJavier>=3){//JAVIER
    Serial.println("modo EXIT");//JAVIER  *******************MODO EXIT
    tiempoJavier=0;//JAVIER
    showGameOverMessage();//AQUI VAN EL GAME OVER///////JAVIER

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>       AQUI TIENE Q HABER ALGO PARA REGRESAR AL MODO "MENU" Y AL EMPEZAR OTRA PARTIDA RESETEAR:
/*

Point food(-1, -1,-1);//JAVIER
specialState =0;//JAVIER
duration=0;//JAVIER
tiempoJavier =0;//JAVIER
pausabool =false;//JAVIER
gameOver =false;//Javier

*/
    
    return;//JAVIER
    }//JAVIER
    
    //tiempoJavier=0;
  }

}//entre modos
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

const short messageSpeed = 0; // default era 5, pero le baje con el fin de que sean mas rapidos los mensajes de game over y score

// if there is no food, generate one, also check for lose

void generateFood() {
      delay(100);
      
      //aqui estaba
      if(gameOver == false && pausabool==false){
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

  Serial.print("ComidaRow:");
  Serial.print(food.row);
  Serial.println("");
  Serial.print("ComidaCol:");
  Serial.print(food.col);
  Serial.println("");
  Serial.print("ComidaMatriz:");
  Serial.print(food.matriz);
  Serial.println("");
  Serial.println("------------------------");

    //AQUI PUEDE IR EL +1 AL PUNTEO
    

  }else if(snakePosX == food.col && snakePosY ==food.row && numMatriz==food.matriz){ //si la cabeza se come la comida 
        food.col = -1;
        food.row = -1;
        delaytime=delaytime-10;//COMO COMIO LE AUMENTO LA VELOCIDAD
        Serial.print(">>>velocidad:");
        Serial.print(delaytime);
        puntos+=1;
        Serial.print(">>>puntos:");
        Serial.print(puntos);
        Serial.println();
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

const PROGMEM bool scoreMessage[8][58] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const PROGMEM bool digits[][8][8] = {
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 1, 0},
    {0, 1, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 0, 1, 1, 0, 0},
    {0, 1, 0, 0, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  }
};

void showGameOverMessage() {
  [&] {
    int numMat=1; //****
    
    gameOver = true;
    
    for (int d = 0; d < sizeof(gameOverMessage[0]) - 7; d++) {
      
      for (int col = 0; col < 8; col++) {
        numMat = col<8 ? 1 : 0; //****
        delay(messageSpeed);
        for (int row = 0; row < 8; row++) {
          
          // this reads the byte from the PROGMEM and displays it on the screen
          lc.setLed(1, row, col, pgm_read_byte(&(gameOverMessage[row][col + d]))); //****
          lc.setLed(0, row, col, pgm_read_byte(&(gameOverMessage[row][col-7 + d]))); //****
          
        }
      }

    }
    showScoreMessage(puntos);//AQUI MUESTRO EL SCORE LUEGO Q SE DESPLEGUE GAME OVER
    //reset de puntos
    puntos=0;
  }();
  
  lc.setLed(food.matriz,food.row, food.col,false);// RECORDAR CUANDO SE RESETEE LA PARTIDA SE DEBE DE INICIALUZAR NUEVAMENTE LA COMIDA
  Serial.println(">>>>>>>presione START por 3 seg/veces! para iniciar el juego<<<<<<<<)");//JAVIER
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  modoPractica =false;


}


// scrolls the 'score' message with numbers around the matrix
void showScoreMessage(int score) {
  if (score < 0 || score > 99) return;

  // specify score digits
  int second = score % 10;
  int first = (score / 10) % 10;

  [&] {
    for (int d = 0; d < sizeof(scoreMessage[0]) + 2 * sizeof(digits[0][0]); d++) {
      for (int col = 0; col < 8; col++) {
        delay(messageSpeed);
        for (int row = 0; row < 8; row++) {
          if (d <= sizeof(scoreMessage[0]) - 8) {
            lc.setLed(1, row, col, pgm_read_byte(&(scoreMessage[row][col + d])));
            lc.setLed(0, row, col, pgm_read_byte(&(scoreMessage[row][col-7 + d]))); //****
          }

          int c = col + d - sizeof(scoreMessage[0]) + 6; // move 6 px in front of the previous message

          // if the score is < 10, shift out the first digit (zero)
          if (score < 10) c += 8;

          if (c >= 0 && c < 8) {
            if (first > 0) {
              lc.setLed(1, row, col, pgm_read_byte(&(digits[first][row][c]))); 
             // lc.setLed(0, row, col, pgm_read_byte(&(digits[first][row][c]))); //****
            
            }// show only if score is >= 10 (see above)
          } else {
            c -= 8;
            if (c >= 0 && c < 8) {
              lc.setLed(1, row, col, pgm_read_byte(&(digits[second][row][c]))); // show always
              //lc.setLed(0, row, col, pgm_read_byte(&(digits[second][row][c]))); //****
              
            }
          }
        }
      }

/*  //CREO Q HAY Q VALIDAR Q NO SE MUEVA LA SERPIENTE EN ESTOS 2 TIPOS DE MENSAJES
      // if the joystick is moved, exit the message
      if (analogRead(Pin::joystickY) < joystickHome.y - joystickThreshold
              || analogRead(Pin::joystickY) > joystickHome.y + joystickThreshold
              || analogRead(Pin::joystickX) < joystickHome.x - joystickThreshold
              || analogRead(Pin::joystickX) > joystickHome.x + joystickThreshold) {
        return; // return the lambda function
      }*/
    }
  }();

  lc.clearDisplay(0);
  lc.clearDisplay(1);


  //  // wait for joystick co come back
  //  while (analogRead(Pin::joystickY) < joystickHome.y - joystickThreshold
  //          || analogRead(Pin::joystickY) > joystickHome.y + joystickThreshold
  //          || analogRead(Pin::joystickX) < joystickHome.x - joystickThreshold
  //          || analogRead(Pin::joystickX) > joystickHome.x + joystickThreshold) {}

}


//***********************************************************************

//METODO PARA PAUSA NECESITA DE UN BOOL LLAMADO pausabool si es true entra en pausa y apago la comida como la cabeza he imprimo el score actual.// si pausabool es false reactivo la comida como el led de la cabeza de la snake
void pausa(){

  if(pausabool==true){
    Serial.println("modo PAUSA ON");//JAVIER  *******************MODO PAUSA on
      lc.setLed(food.matriz,food.row, food.col,false);//PARA QUE NO SE MIRE LA COMIDA
      lc.setLed(numMatriz,snakePosY,snakePosX,false);//PARA Q NO SE MIRE LA SNAKE
      showScoreMessage(puntos);//AQUI MUESTRO EL SCORE LUEGO Q entre en modo pausa   
  
    }else{
      //CUANDO SE QUITE EL MODO PAUSA SE TIENE Q VER DE NUEVO LA COMIDA Y LA SERPIENTE
      Serial.println("modo PAUSA OFF");//JAVIER  *******************MODO PAUSA off
      lc.setLed(food.matriz,food.row, food.col,true);//PARA QUE SE MIRE LA COMIDA
      lc.setLed(numMatriz,snakePosY,snakePosX,true);//PARA Q SE MIRE LA SNAKE
      }  

}

void BIFROST(){

  specialState = digitalRead(special);//JAVIER para el boton de START para saber cuando se esta pulsando
  
  if(specialState == HIGH){//JAVIER si START FUE PRECIONADO su funcion es la de contar el n numero de seg de presion
    duration = pulseIn(special,HIGH);//JAVIER
    
    if(duration==0){//JAVIER
      tiempoJavier+=1;//JAVIER
      Serial.println(tiempoJavier);//JAVIER
    }//JAVIER

    if(tiempoJavier>=3){//JAVIER
    Serial.println("iniciando juego SNAKE.....");//JAVIER  *******************MODO INICIAL
    tiempoJavier=0;//JAVIER

//restablesco todas las variables nuevamente
  food.col = -1;
  food.row = -1;
  puntos =0;
  snakePosX = 0;
  snakePosY = 0;
  numMatriz = 0;
  specialState =0;//JAVIER
  duration=0;//JAVIER
  tiempoJavier =0;//JAVIER
  pausabool =false;//JAVIER
  gameOver =false;//Javier


  //hacer ver a la serpiente
  lc.setLed(0,0,0,true);// posicion inicial de snake JOEL

    //paso de modo de mensaje a modo del juego habilitando el bool
    modoPractica =true;




    
    }

  }
  
  
  }
