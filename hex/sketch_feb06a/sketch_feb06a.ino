#include <MatrizLed.h>
#include <LedControl.h>

//Definición de los pines utilizados
const int butPinUp = 7;
const int butPinDown = 6;
const int butPinLeft = 5;
const int butPinRight = 4;
const int ledPin = 3;

//Creación de variable que manejará la matriz
LedControl lc = LedControl(12, 11, 10, 2);

//Variable para manejar el delay de actualización y velocidad de la snake
unsigned long delaytime = 700;

//Variables de estados de los botones
int butUpState = 0;
int butDownState = 0;
int butLeftState = 0;
int butRightState = 0;

//Variables para el manejo de la snake y sus sectores
byte ultimaX = 0;
byte ultimaY = 0;
byte ultimaMat = 0;


//Comida
//clase de la comida
struct Point {
  int row = 0, col = 0, matriz = 0;
  Point(int row = 0, int col = 0, int matriz = 0): row(row), col(col), matriz(matriz) {}
};

//Clase snake
struct Cabeza {
  int y = 0, x = 0, matriz = 0, dir = 0;
  Cabeza(int y = 0, int x = 0, int matriz = 0, int dir = 0): y(y), x(x), matriz(matriz), dir(dir) {}
};

//Variables de estados del juego
int puntos = 0;
bool gameOver = false;
bool snakeCrecio = false;

//matriz para guardar el cuerpo de la snake
byte cuerpo[3][128];
byte longitud;


//declaro el 1.er objeto comida //JAVIER
Point food(-1, -1, -1); //JAVIER
Cabeza snake(0, 0, 0, 0);
Cabeza cola(0,0,0,0);

//tablero
int tablero[7][15];


void setup() {
  //Inicializando los pines de los botones como entradas
  pinMode(butPinUp, INPUT);
  pinMode(butPinDown, INPUT);
  pinMode(butPinLeft, INPUT);
  pinMode(butPinRight, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

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
  generarPosicionInicial();
  generateFood();
  longitud = 0;
  lc.setLed(snake.matriz, snake.y, snake.x, true);
  cuerpo[0][0]=snake.x;
  cuerpo[1][0]=snake.y;
  cuerpo[2][0]=snake.matriz;
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
  actualizarPosicion();
  mover();
  generateFood();
}


//Método para inicializar y generar la posición aleatoría de la cabeza de la serpiente
void generarPosicionInicial() {
  snake.y = random(1, 8);
  snake.x = random(1, 8);
  snake.dir = random(1, 5);
  snake.matriz = random(1, 2);
  cola.y= snake.y;
  cola.x= snake.x;
  cola.dir= snake.dir;
  cola.matriz= snake.matriz;
}


//Método que actualiza la dirección de la snake
void actualizarDireccion() {
  if (butUpState == HIGH) {
    //Se enciende el led
    digitalWrite(ledPin, HIGH);
    snake.dir = 1;
  } else if (butDownState) {
    digitalWrite(ledPin, HIGH);
    snake.dir = 2;
  } else if (butLeftState) {
    digitalWrite(ledPin, HIGH);
    snake.dir = 3;
  } else if (butRightState) {
    digitalWrite(ledPin, HIGH);
    snake.dir = 4;
  } else {
    //Se mantiene apagado el led
    digitalWrite(ledPin, LOW);
  }
}


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
        //Pierde
      }
      break;
    case 2:
      snake.y++;
      if (snake.y > 7) {
        //Pierde
      }
      break;
    case 3:
      snake.x--;
      if (snake.x < 0 && snake.matriz == 0) {
        //Pierde
      } else if (snake.x < 0 && snake.matriz == 1) {
        snake.matriz = 0;
        snake.x = 7;
      } else {
        //snake.matriz = ultimaMatriz;
      }
      break;
    case 4:
      snake.x++;
      if (snake.x > 7 && snake.matriz == 0) {
        snake.x = 0;
        snake.matriz = 1;
      } else if (snake.x > 7 && snake.matriz == 1) {
        //Pierde
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
    cuerpo[0][j] = cuerpo[0][j-1];
    cuerpo[1][j] = cuerpo[1][j-1];
    cuerpo[2][j] = cuerpo[2][j-1];
  }
  //Actualizar cabeza
  cuerpo[0][0] = snake.x;
  cuerpo[1][0] = snake.y;
  cuerpo[2][0] = snake.matriz;
}

// Método que repinta la matriz con las nuevas posiciones
void mover() {
  lc.setLed(snake.matriz, snake.y, snake.x, true);
  /*
  if (snakeCrecio){
    lc.setLed(ultimaMat,ultimaY,ultimaX,false);
    snakeCrecio = false;
  }
  if (snake.x == food.col && snake.y ==food.row && snake.matriz == food.matriz){
    snakeCrecio = true;
  }else{
    lc.setLed(cola.matriz,cola.y,cola.x, false);
    
  } 
  */
  lc.setLed(cola.matriz,cola.y,cola.x, false);
  if (longitud>0){pintarCuerpo();}  

  //Recuperar las posiciones de la cola
  cola.x = cuerpo[0][longitud];
  cola.y = cuerpo[1][longitud];
  cola.matriz = cuerpo[2][longitud];

  delay(delaytime);
}

void pintarCuerpo(){
  for (byte i=longitud;i>0;i--){
    lc.setLed(cuerpo[2][i],cuerpo[1][i],cuerpo[0][i],true);
  }
}

//Método para la generación de la comida
void generateFood() {
  delay(100);
  if (gameOver == false) {
    lc.setLed(food.matriz, food.row, food.col, true);
  } else {
    return;
  }

  // ver si cuadran la posicion de la serpiente con la de la comida (por ahora)
  if (food.row == -1 || food.col == -1 || food.matriz == -1) {
    food.col = random(1, 7); //asigno col a comida
    food.row = random(8);//asigno fila a comida
    food.matriz = random(2);//asigno matrz a comida
    lc.setLed(food.matriz, food.row, food.col, true);


    //Serial.println(food.row);
    //Serial.println(food.col);
    //Serial.println(food.matriz);
    //Serial.println("------------------------");

    //AQUI PUEDE IR EL +1 AL PUNTEO


  } else if (snake.x == food.col && snake.y ==food.row && snake.matriz == food.matriz) { //si la cabeza se come la comida
    //Actualización de la snake
    ultimaX = cola.x;
    ultimaY = cola.y;
    ultimaMat = cola.matriz;
    cola.x = food.col;
    cola.y = food.row;
    cola.matriz = food.matriz;
    longitud++;
    cuerpo[0][longitud]= cola.x;
    cuerpo[1][longitud]= cola.y;
    cuerpo[2][longitud]= cola.matriz;
    //snakeCrecio = true;
    //Actualización de comida
    food.col = -1;
    food.row = -1;
    puntos += 1;

  }
}
