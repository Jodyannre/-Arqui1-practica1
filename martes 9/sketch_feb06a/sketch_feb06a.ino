#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <LedControl.h>
//-------------------------------------------------------------------------------------------------------------------------------------
//Definición de los pines utilizados
const int butPinUp = 7;
const int butPinDown = 6;
const int butPinLeft = 5;
const int butPinRight = 4;
const int ledPin = 3;
int Cont = 0;
//-------------------------------------------------------------------------------------------------------------------------------------


///////********************************INSERCCION 1 DE CODIGO LETRERO***********************************************///////////////////////
//# DE PIN PARA CADA BOTON
const int btnIn1 = 11;
const int btnIn2 = 10;
const int btnIn3 = 9;
//ESTADO INICIAL DE LOS BOTONES
int btnIn1State = 0;
int btnIn2State = 0;
int btnIn3State = 0;


// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define  MAX_DEVICES 2
#define CLK_PIN  52
#define DATA_PIN 51
#define CS_PIN   45

// HARDWARE SPI
MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);
// SOFTWARE SPI
//MD_Parola P = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);


uint8_t  frameDelay; //VELOCIDAD DE SCROLL

// Global message buffers shared by Serial and Scrolling functions
#define BUF_SIZE  80
uint16_t WAIT_TIME;
char curMessage[BUF_SIZE];
char newMessage[BUF_SIZE];
bool newMessageAvailable = false;

//***************************************FINALIZA INSERCION 1************************************************************************///





//-------------------------------------------------------------------------------------------------------------------------------------
//Creación de variable que manejará la matriz
LedControl lc = LedControl(51, 52, 45, 2);
//-------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------------
//Variable para manejar el delay de actualización y velocidad de la snake
unsigned long delaytime = 700;
//-------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------------
//Variables de estados de los botones
int butUpState = 0;
int butDownState = 0;
int butLeftState = 0;
int butRightState = 0;
//-------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------------
//Variables para el manejo de la cola de la snake
byte ultimaX = 0;
byte ultimaY = 0;
byte ultimaMat = 0;
//-------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------------
//clase de la comida
struct Point {
  int row = 0, col = 0, matriz = 0;
  Point(int row = 0, int col = 0, int matriz = 0): row(row), col(col), matriz(matriz) {}
};
//-------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------------
//Clase snake
struct Cabeza {
  int y = 0, x = 0, matriz = 0, dir = 0;
  Cabeza(int y = 0, int x = 0, int matriz = 0, int dir = 0): y(y), x(x), matriz(matriz), dir(dir) {}
};
//-------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------------
//Variables de estados del juego
int puntos = 0;
const short messageSpeed = 0;
bool snakeCrecio = false;
//-------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------------
//Matriz para guardar el cuerpo de la snake
byte cuerpo[3][128];
byte longitud;
//-------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------------
//Declaración de comida, cabeza y cola de la snake
Point food(-1, -1, -1);
Cabeza snake(0, 0, 0, 0);
Cabeza cola(0, 0, 0, 0);
//-------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------------
//Valores para start y pause
int special = 8;
int specialState = 0;
//-------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------------
//Nuevas variables para medir tiempo
unsigned long duration;
long tiempoJavier = 0;
bool pausabool = false;
bool gameOver = false;
bool modoPractica = false;
long time;
//-------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------------
//Método setup, inicialización de variables
void setup() {

  //***********************************INSERCION 2 DE CODIGO LETRERO*********************************************************************//
  //DEFINICION DE LOS BOTONES COMO ENTRADA EN PINES
  pinMode(btnIn1, INPUT);
  pinMode(btnIn2, INPUT);
  pinMode(btnIn3, INPUT);

  // P.begin(); /////////************************POSIBLE PROBLEMA CON INTERFERENCIA DE LIBRERIAS *********************************////

  //  P.displayClear();
  //P.displaySuspend(false);
  //***********************************FINALIZA INSERCION 2 ******************************************************************************//

  Serial.begin(9600);

  //Inicializando los pines de los botones como entradas
  pinMode(butPinUp, INPUT);
  pinMode(butPinDown, INPUT);
  pinMode(butPinLeft, INPUT);
  pinMode(butPinRight, INPUT);
  pinMode(ledPin, OUTPUT);
  // pulso de buttom
  pinMode(special, INPUT);//JAVIER



  //Inicialización de la matriz
  //Obtener cantidad de matrices
  //Cancelar modo ahorro
  for (int i = 0; i < 2; i++) {
    lc.shutdown(i, false);
    //Configurar brillo
    lc.setIntensity(i, 8);
    //Limpiar la matriz
    lc.clearDisplay(i);
  }

  Serial.println(">>>>>>>presione (START) por 3 seg/veces! para iniciar<<<<<<<<)");//JAVIER

  time = 0;




  //delay(delaytime); //Limpiando delays, este no parece afectar nada
}
//-------------------------------------------------------------------------------------------------------------------------------------





//-------------------------------------------------------------------------------------------------------------------------------------
//Método LOOP
void loop() {


  if (modoPractica == false) {
    Letrero();
    BIFROST();// sin esto no se puede empesar el juego lo q hace es si start no se oprime por 3 seg o veces no inicial el juego esto mismo se hace despues del game over
    //********************************************INSERCCION CODIGO 3 TABLERO*************************************************************//

  }



  //modoPractica se habilita en metodo BIFROST


  if (modoPractica == true) {
    // P.displayClear();

    /* for (int i = 0; i < 2; i++) {
       lc.shutdown(i, false);
       //Configurar brillo
       lc.setIntensity(i, 8);
       //Limpiar la matriz
       lc.clearDisplay(i);
      }
       time = 0;

    */

    generateFood();
    specialState = digitalRead(special);//JAVIER para el boton de START
    //Leer los estados de los botones
    butUpState = digitalRead(butPinUp);
    butDownState = digitalRead(butPinDown);
    butLeftState = digitalRead(butPinLeft);
    butRightState = digitalRead(butPinRight);

    actualizarDireccion();

    //Pintar y actualizar posición snake --> si pausa esta activa la serpiente no pinta ni se mueve
    if (pausabool == false && gameOver == false) { //<-falta condición
      //Actualizar posiciones y pintar la matriz

      actualizarPosicion();

    }

    //Generar nueva comida

  }


}
//-------------------------------------------------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------------------------------------------------
//Método para inicializar y generar la posición aleatoría de la cabeza de la serpiente
void generarPosicionInicial() {
  //snake.y = random(1, 8);
  snake.y = 0;
  snake.x = random(1, 8);
  snake.dir = random(1, 5);
  snake.matriz = random(1, 2);
  cola.y = snake.y;
  cola.x = snake.x;
  cola.dir = snake.dir;
  cola.matriz = snake.matriz;
  cuerpo[0][0] = snake.x;
  cuerpo[1][0] = snake.y;
  cuerpo[2][0] = snake.matriz;
}
//-------------------------------------------------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------------------------------------------------
//Método que actualiza la dirección de la snake
void actualizarDireccion() {
  if (butUpState == HIGH && pausabool == false && gameOver == false) {
    //Se enciende el led
    digitalWrite(ledPin, HIGH);
    snake.dir = 1;
  } else if (butDownState && pausabool == false && gameOver == false) {
    digitalWrite(ledPin, HIGH);
    snake.dir = 2;
  } else if (butLeftState && pausabool == false && gameOver == false) {
    digitalWrite(ledPin, HIGH);
    snake.dir = 3;
  } else if (butRightState && pausabool == false && gameOver == false) {
    digitalWrite(ledPin, HIGH);
    snake.dir = 4;
  } else if (specialState == HIGH) { // si START FUE PRECIONADO tengo q calcular si es pause o endgame su funcion es la de contar el n numero de seg de presion
    duration = pulseIn(special, HIGH);

    if (duration == 0) {
      tiempoJavier += 1;
      Serial.println(tiempoJavier);
      return;

    }
  } else {
    //Se mantiene apagado el led
    digitalWrite(ledPin, LOW);


    //Validación de pausa

    if (tiempoJavier == 1) { //JAVIER aqui valido el n numero de segundos q calcule anterior

      pausabool = !pausabool;//JAVIER cambio de estado automaticamente si es pausa
      pausa();//JAVIER llamo al metodo de desplique de mensaje de score o que reanima el juego
      tiempoJavier = 0; //JAVIER
      //return;//JAVIER

    } else if (tiempoJavier >= 3) { //JAVIER

      Serial.println("modo EXIT");//JAVIER  *******************MODO EXIT
      tiempoJavier = 0; //JAVIER
      showGameOverMessage();//AQUI VAN EL GAME OVER///////JAVIER
      //modoPractica = false;

      //return;//JAVIER
    }//JAVIER
    //tiempoJavier=0;
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------------------------------------------------
//Método que actualiza la posición de la cabeza
void actualizarPosicion() {
  if (!snakeCrecio) {

  } else {
    snakeCrecio = false;
  }

  switch (snake.dir) {
    case 1:
      snake.y--;
      if (snake.y < 0) {
        showGameOverMessage();//AQUI VAN EL GAME OVER///////JAVIER
        return;///////JAVIER
      }
      break;
    case 2:
      snake.y++;
      if (snake.y > 7) {
        showGameOverMessage();//AQUI VAN EL GAME OVER///////JAVIER
        return;///////JAVIER
      }
      break;
    case 3:
      snake.x--;
      if (snake.x < 0 && snake.matriz == 1) {
        showGameOverMessage();//AQUI VAN EL GAME OVER///////JAVIER
        return;///////JAVIER
      } else if (snake.x < 0 && snake.matriz == 0) {
        snake.matriz = 1;
        snake.x = 7;
      } else {
        //snake.matriz = ultimaMatriz;
      }
      break;
    case 4:
      snake.x++;
      if (snake.x > 7 && snake.matriz == 1) {
        snake.x = 0;
        snake.matriz = 0;
      } else if (snake.x > 7 && snake.matriz == 0) {
        showGameOverMessage();//AQUI VAN EL GAME OVER///////JAVIER
        return;///////JAVIER
      } else {
        //snake.matriz = ultimaMatriz;
      }
      break;
  }
  if (snake.x == food.col && snake.y == food.row && snake.matriz == food.matriz) {
    snakeCrecio = true;
  }
  //Fila 1 para X y fila 2 para Y, fila 3 para matriz
  //Recuperar las posiciones de la cola
  cola.x = cuerpo[0][longitud];
  cola.y = cuerpo[1][longitud];
  cola.matriz = cuerpo[2][longitud];

  for (byte j = longitud; j > 0; j--) {
    cuerpo[0][j] = cuerpo[0][j - 1];
    cuerpo[1][j] = cuerpo[1][j - 1];
    cuerpo[2][j] = cuerpo[2][j - 1];
  }
  //Actualizar cabeza
  cuerpo[0][0] = snake.x;
  cuerpo[1][0] = snake.y;
  cuerpo[2][0] = snake.matriz;
  mover();
}
//-------------------------------------------------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------------------------------------------------
// Método que repinta la matriz con las nuevas posiciones
void mover() {
  lc.setLed(snake.matriz, snake.y, snake.x, true);

  //Verificar si la snake se come a sí misma

  for (byte i = longitud; i > 0; i--) {
    if (snake.x == cuerpo[0][i] && snake.y == cuerpo[1][i] && snake.matriz == cuerpo[2][i]) {
      showGameOverMessage();
      return;
    }
  }

  lc.setLed(cola.matriz, cola.y, cola.x, false);
  if (longitud > 0) {
    pintarCuerpo();
  }

  //Recuperar las posiciones de la cola
  cola.x = cuerpo[0][longitud];
  cola.y = cuerpo[1][longitud];
  cola.matriz = cuerpo[2][longitud];

  delay(delaytime);
}
//-------------------------------------------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------------------------------------------
//Método que actualiza y pinta las partes de la snake
void pintarCuerpo() {
  for (byte i = longitud; i > 0; i--) {
    lc.setLed(cuerpo[2][i], cuerpo[1][i], cuerpo[0][i], true);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------------------------------------------
//Método para ocultar snake
void ocultarSnake() {
  for (byte i = longitud; i > -1; i--) {
    lc.setLed(cuerpo[2][i], cuerpo[1][i], cuerpo[0][i], false);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------------------------------------------
//Método para la generación de la comida y para evaluar si la snake comió
void generateFood() {
  //P.displayReset();

  bool posicionCorrecta = false;
  //delay(50);<-------------------------------Probando sin este delay
  if (gameOver == false && pausabool == false) {
    lc.setLed(food.matriz, food.row, food.col, true);
  } else {

    return;
  }

  // ver si cuadran la posicion de la serpiente con la de la comida (por ahora)
  if (food.row == -1 || food.col == -1 || food.matriz == -1) {


    //Crear comida aleatoria y verificar que la posición sea correcta
    while (!posicionCorrecta) {
      food.col = random(1, 7); //asigno col a comida
      food.row = random(8);//asigno fila a comida
      food.matriz = random(2);//asigno matrz a comida

      for (byte i = longitud; i > -1; i--) {
        if (food.col == cuerpo[0][i] && food.row == cuerpo[1][i] && food.matriz == cuerpo[2][i]) {
          posicionCorrecta = false;
        } else {
          posicionCorrecta = true;
          break;
        }
      }
    }

    lc.setLed(food.matriz, food.row, food.col, true);

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


  } else if (snake.x == food.col && snake.y == food.row && snake.matriz == food.matriz) { //si la cabeza se come la comida
    //Actualización de la snake
    ultimaX = cola.x;
    ultimaY = cola.y;
    ultimaMat = cola.matriz;
    cola.x = food.col;
    cola.y = food.row;
    cola.matriz = food.matriz;
    longitud++;
    cuerpo[0][longitud] = cola.x;
    cuerpo[1][longitud] = cola.y;
    cuerpo[2][longitud] = cola.matriz;
    //snakeCrecio = true;
    //Actualización de comida
    food.col = -1;
    food.row = -1;
    delaytime = delaytime - 20; //COMO COMIO LE AUMENTO LA VELOCIDAD
    Serial.print(">>>velocidad:");
    Serial.print(delaytime);
    puntos += 1;
    Serial.print(">>>puntos:");
    Serial.print(puntos);
    Serial.println();

  }
}
//-------------------------------------------------------------------------------------------------------------------------------------





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
    int numMat = 1; //****

    gameOver = true;

    for (int d = 0; d < sizeof(gameOverMessage[0]) - 7; d++) {

      for (int col = 0; col < 8; col++) {
        numMat = col < 8 ? 1 : 0; //****
        delay(messageSpeed);
        for (int row = 0; row < 8; row++) {

          // this reads the byte from the PROGMEM and displays it on the screen
          lc.setLed(0, row, col, pgm_read_byte(&(gameOverMessage[row][col + d]))); //****
          lc.setLed(1, row, col, pgm_read_byte(&(gameOverMessage[row][col - 7 + d]))); //****

        }
      }

    }
    showScoreMessage(puntos);//AQUI MUESTRO EL SCORE LUEGO Q SE DESPLEGUE GAME OVER
    //reset de puntos
    puntos = 0;
  }();

  lc.setLed(food.matriz, food.row, food.col, false); // RECORDAR CUANDO SE RESETEE LA PARTIDA SE DEBE DE INICIALUZAR NUEVAMENTE LA COMIDA
  Serial.println(">>>>>>>presione START por 3 seg/veces! para iniciar el juego<<<<<<<<)");//JAVIER
  lc.clearDisplay(0);
  lc.clearDisplay(1);


  modoPractica = false;
  //Serial.println(">>>>>>><<<<<<<<)");//JAVIER
  //Letrero();//EDIN
  BIFROST();
  return;

}


// scrolls the 'score' message with numbers around the matrix
void showScoreMessage(int score) {

  /* specialState = digitalRead(special);//JAVIER para el boton de START para saber cuando se esta pulsando

    if (specialState == HIGH) {
     modoPractica = false;
     return;
     }
  */


  //**************************************************8
  if (score < 0 || score > 99) return;

  // specify score digits
  int second = score % 10;
  int first = (score / 10) % 10;

  [&] {
    for (int d = 0; d < sizeof(scoreMessage[0]) + 2 * sizeof(digits[0][0]); d++) {

      for (int col = 0; col < 8; col++) {

        delay(messageSpeed);

        while (digitalRead(specialState) == 1) {

          duration = pulseIn(special, HIGH);

          if (duration == 0) {
            tiempoJavier += 1;
            Serial.println(tiempoJavier);
            return;

          }

          if (tiempoJavier >= 3) { //JAVIER

            Serial.println("FFFF");//JAVIER  *******************MODO EXIT
            tiempoJavier = 0; //JAVIER
            modoPractica = false;
            return;
          }//JAVIER


        }

        for (int row = 0; row < 8; row++) {

          if (d <= sizeof(scoreMessage[0]) - 8) {
            lc.setLed(0, row, col, pgm_read_byte(&(scoreMessage[row][col + d])));
            lc.setLed(1, row, col, pgm_read_byte(&(scoreMessage[row][col - 7 + d]))); //****
          }

          int c = col + d - sizeof(scoreMessage[0]) + 6; // move 6 px in front of the previous message

          // if the score is < 10, shift out the first digit (zero)
          if (score < 10) c += 8;

          if (c >= 0 && c < 8) {
            if (first > 0) {
              lc.setLed(0, row, col, pgm_read_byte(&(digits[first][row][c])));
              // lc.setLed(0, row, col, pgm_read_byte(&(digits[first][row][c]))); //****

            }// show only if score is >= 10 (see above)
          } else {
            c -= 8;
            if (c >= 0 && c < 8) {
              lc.setLed(0, row, col, pgm_read_byte(&(digits[second][row][c]))); // show always
              //lc.setLed(0, row, col, pgm_read_byte(&(digits[second][row][c]))); //****

            }
          }
        }
      }

    }
  }();

  lc.clearDisplay(0);
  lc.clearDisplay(1);


}


//***********************************************************************

//METODO PARA PAUSA NECESITA DE UN BOOL LLAMADO pausabool si es true entra en pausa y apago la comida como la cabeza he imprimo el score actual.// si pausabool es false reactivo la comida como el led de la cabeza de la snake
void pausa() {

  if (pausabool == true) {
    Serial.println("modo PAUSA ON");//JAVIER  *******************MODO PAUSA on
    lc.setLed(food.matriz, food.row, food.col, false); //PARA QUE NO SE MIRE LA COMIDA
    //lc.setLed(numMatriz, snakePosY, snakePosX, false); //PARA Q NO SE MIRE LA SNAKE
    //ocultarSnake();//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


    showScoreMessage(puntos);//AQUI MUESTRO EL SCORE LUEGO Q entre en modo pausa

  } else {
    //CUANDO SE QUITE EL MODO PAUSA SE TIENE Q VER DE NUEVO LA COMIDA Y LA SERPIENTE
    Serial.println("modo PAUSA OFF");//JAVIER  *******************MODO PAUSA off
    lc.setLed(food.matriz, food.row, food.col, true); //PARA QUE SE MIRE LA COMIDA
    //lc.setLed(snake.matriz, snake.y, snake.x, true); //PARA Q SE MIRE LA SNAKE
    mover();
  }

}

void BIFROST() {

  specialState = digitalRead(special);//JAVIER para el boton de START para saber cuando se esta pulsando

  if (specialState == HIGH) { //JAVIER si START FUE PRECIONADO su funcion es la de contar el n numero de seg de presion
    duration = pulseIn(special, HIGH); //JAVIER

    if (duration == 0) { //JAVIER
      tiempoJavier += 1; //JAVIER
      Serial.println(tiempoJavier);//JAVIER
    }//JAVIER

    if (tiempoJavier >= 3) { //JAVIER
      Serial.println("iniciando juego SNAKE.....");//JAVIER  *******************MODO INICIAL
      tiempoJavier = 0; //JAVIER

      //restablesco todas las variables nuevamente
      food.col = -1;
      food.row = -1;
      puntos = 0;
      //snake.x = 0;
      //snake.y = 0;
      //snake.matriz = 1;
      specialState = 0; //JAVIER
      duration = 0; //JAVIER
      tiempoJavier = 0; //JAVIER
      pausabool = false; //JAVIER
      gameOver = false; //Javier

      //Reiniciando cuerpo de la serpiente
      for (int i = longitud; i > -1; i--) {
        cuerpo[0][i] = 0;
        cuerpo[1][i] = 0;
        cuerpo[2][i] = 0;
      }
      //Generando nueva snake
      longitud = 0;
      generarPosicionInicial();

      //hacer ver a la serpiente inicialmente

      lc.setLed(snake.matriz, snake.y, snake.x, true); // posicion inicial de snake JOEL

      //paso de modo de mensaje a modo del juego habilitando el bool
      modoPractica = true;
    }
  }
}


void seAutoDestruye() {

}





//***********************************************************METODOS TABLERO***********************************************************************//
void ImprimirLetraPorLetra() {
  P.print("T    ");
  delay(WAIT_TIME);
  P.print("P    ");
  delay(WAIT_TIME);
  P.print("1    ");
  delay(WAIT_TIME);
  P.print("-    ");
  delay(WAIT_TIME);
  P.print("G    ");
  delay(WAIT_TIME);
  P.print("R    ");
  delay(WAIT_TIME);
  P.print("U    ");
  delay(WAIT_TIME);
  P.print("P    ");
  delay(WAIT_TIME);
  P.print("O    ");
  delay(WAIT_TIME);
  P.print("1    ");
  delay(WAIT_TIME);
  P.print("2    ");
  delay(WAIT_TIME);
  P.print("-    ");
  delay(WAIT_TIME);
  P.print("S    ");
  delay(WAIT_TIME);
  P.print("E    ");
  delay(WAIT_TIME);
  P.print("C    ");
  delay(WAIT_TIME);
  P.print("C    ");
  delay(WAIT_TIME);
  P.print("I    ");
  delay(WAIT_TIME);
  P.print("O    ");
  delay(WAIT_TIME);
  P.print("N    ");
  delay(WAIT_TIME);
  P.print("A    ");
  delay(WAIT_TIME);
  P.print("*    ");
  delay(WAIT_TIME);
}

void Velocidad() {
  if (digitalRead(btnIn3) == HIGH) {
    frameDelay = 30;
    WAIT_TIME = 300;
  }
  else {
    frameDelay = 80;
    WAIT_TIME = 700;
  }
}


void Letrero() {

  showScoreMessage(puntos);
  /*
       //se lee el estado de los botones
    int btnIn1State = digitalRead(btnIn1);
    int btnIn2State = digitalRead(btnIn2);
    int btnIn3State = digitalRead(btnIn3);
    Velocidad();//metodo para revisar velocidad cada vez que empieza mensaje

    if (P.displayAnimate())
    {
    //si esta en 0 bt1 es movimiento
    if (btnIn1State == LOW)
    {
      //si esta en 0 bt2 derecha a izquierda
      if (btnIn2State == HIGH)
      {
        P.displayScroll(curMessage, PA_LEFT, PA_SCROLL_LEFT, frameDelay); //movimiento hacia la izquierda
        strcpy(curMessage, "TP1 - GRUPO 12 - SECCION A*");
      }
      else
      {
        P.displayScroll(curMessage, PA_LEFT, PA_SCROLL_RIGHT, frameDelay); //movimiento hacia la dereacha
        strcpy(curMessage, "TP1 - GRUPO 12 - SECCION A*");
      }
    }
    else
    {
      ImprimirLetraPorLetra(); //para movimiento estatico
      BIFROST();
    }
    P.displayReset();
    }
      //*************************************************FINALIZA INSERCION*******************************************************************

  */
}
//******************************************************************************************FIN METODOS TABLERO*********************************************************************//
