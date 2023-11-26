#include <Servo.h>

#define IN1 12
#define IN2 13
#define ENA 11

#define IN3 2
#define IN4 4
#define ENB 5

#define pistonPin 10
#define servoLPin 6
#define servoRPin 9

#define potAngle 0
#define potSpeed 1

Servo piston;
Servo servoL;
Servo servoR;

int speed0 = 255;
int tExtend = 150;
int tRetract = 500;

int t = 0;
int tnext = 0;

int pAngulo;
int pSpeed;

float angle = 0;
int angleNext = 0;
int tPistonMove = 50;
int minAngle = 0;
int maxAngle = 180;

int loopDelay = 10;

unsigned long prevTime = millis();
int intervalPrint = 1000;

void setup(){
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  servoL.attach(servoLPin);
  servoR.attach(servoRPin);
  piston.attach(pistonPin);

  // forward(speed0);
}

void loop(){
  // //Lee y escribir el ángulo de inclinación
  pAngulo = map(analogRead(potAngle), 0, 1023, 0, 90);
  servoL.write(pAngulo);
  servoR.write(180-pAngulo);

  // //Lee y escribe la velocidad de los motores
  pSpeed = map(analogRead(potSpeed), 0, 1023, 100, 255);
  forward(pSpeed);

  //Movimiento del piston
  t = tnext;
  if(t == 0){ //Se retrae
    piston.write(minAngle);
    tnext += 1;
  }if(0 < t && t < tExtend){ //Espera cerrado
    if(tExtend-tPistonMove < t){
      angle += float((loopDelay*(maxAngle-minAngle)))/float(10*tPistonMove);
      piston.write(angle);
    }
    tnext += 1;
  }if(t == tExtend){ //Se retrae
    angle = maxAngle;
    tnext += 1;
    piston.write(maxAngle);
  }if(tExtend < t && t < tExtend + tRetract){ //Espera retraido
    if(tExtend + tRetract-tPistonMove < t){
      angle -= float((loopDelay*(maxAngle-minAngle)))/float(10*tPistonMove);
      piston.write(angle);
    }
    tnext += 1;
  }if(t == tExtend + tRetract){ //Reinicia el ciclo
    angle = minAngle;
    tnext = 0;
  }

  // Imprimir datos
  unsigned long currentTime = millis();
  if(currentTime - prevTime > intervalPrint){
    Serial.print("Ángulo: ");
    Serial.print(90-pAngulo);
    Serial.print(" Velocidad: ");
    Serial.println(pSpeed);
    prevTime = currentTime;
  }
  delay(loopDelay);
}

void forward(int speed){
  analogWrite(ENA, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  analogWrite(ENB, speed);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}