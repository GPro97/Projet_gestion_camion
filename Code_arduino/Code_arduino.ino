
#include <Servo.h>

#define LEDPIN 13
#define Etat_barriere 4

//Variable relatif à la barrière
Servo servo_barriere;

void setup() {
  noInterrupts(); // désactiver toutes les interruptions

  pinMode(LEDPIN, OUTPUT); // ledpin juste pour voir si ca fonctionne.
  pinMode(Etat_barriere, INPUT);
  digitalWrite(LEDPIN, HIGH);

  servo_barriere.attach(5); //Attache le servomoteur de la barriere au pin 5
  pinMode(Etat_barriere, INPUT); //Définir Etat_barriere en entrée

  interrupts(); //Activer toutes les interruptions

  //Définir l'état initial du servo
  servo_barriere.write(0);

  delay(10000); //Attente de 2s avant de démarrer réellement le code

}


void barriere(void){
  servo_barriere.write(90);
}

void loop() {
//  servo_barriere.write(30);
//  delay(2000);
//  servo_barriere.write(60);
//  delay(2000);
//  servo_barriere.write(90);
//  delay(2000);
//  servo_barriere.write(120);
//  delay(2000);
//  servo_barriere.write(150);
//  delay(2000);
  servo_barriere.write(90);
  delay(2000);
  servo_barriere.write(170);
  delay(2000);
  servo_barriere.write(0);
  delay(2000);

}
