
#include <Servo.h>

#define LEDPIN 13
int Etat_barriere;
int pin_interrupt = 4;

//Variable relatif à la barrière
Servo servo_barriere;

void setup() {
  noInterrupts(); // désactiver toutes les interruptions

  pinMode(LEDPIN, OUTPUT); // ledpin juste pour voir si ca fonctionne.
  pinMode(pin_interrupt, INPUT);//Définir la pin de l'interrupteur en entrée
  digitalWrite(LEDPIN, HIGH);

  servo_barriere.attach(5); //Attache le servomoteur de la barriere au pin 5

  interrupts(); //Activer toutes les interruptions

  //Définir l'état initial du servo
  servo_barriere.write(0);
  
  delay(10000); //Attente de 2s avant de démarrer réellement le code

}


//void barriere(void){
//  servo_barriere.write(90);
//}

void loop() {
  Etat_barriere = digitalRead(pin_interrupt);
  if (Etat_barriere == HIGH){
    servo_barriere.write(90);
  }
  else{
    servo_barriere.write(10);
  } 

}
