
#include <Servo.h>

#define LEDPIN 13
int Etat_barriere;
int pin_interrupt = 2;

const int temps_barriere_entre_2degrees = 15;
const int temps_barriere_attente = 2000;
long t_actuel;
int position_barriere = 0;
bool flag_barriere_montee = false;
bool flag_barriere_descente = false;
bool flag_barriere_attente = false;
bool flag_maintien_barriere = false;

//Variable relatif Ã  la barriÃ¨re
Servo servo_barriere;

void setup() {
  noInterrupts(); // dÃ©sactiver toutes les interruptions

  pinMode(LEDPIN, OUTPUT); // ledpin juste pour voir si ca fonctionne.
  pinMode(pin_interrupt, INPUT);//DÃ©finir la pin de l'interrupteur en entrÃ©e

  servo_barriere.attach(3); //Attache le servomoteur de la barriere au pin 5

  interrupts(); //Activer toutes les interruptions

  //DÃ©finir l'Ã©tat initial du servo
  servo_barriere.write(0);
  Serial.begin(9600);
  
  delay(2000); //Attente de 2s avant de dÃ©marrer rÃ©ellement le code

}


void barriere(void){
  if(!flag_barriere_montee && !flag_barriere_attente && !flag_barriere_descente){
    if (millis() - t_actuel >= temps_barriere_entre_2degrees) {
      position_barriere ++ ;
      servo_barriere.write(position_barriere);
      t_actuel = millis();
      if (position_barriere == 90){
        flag_barriere_montee = true;
      }
    }
  }

  if(flag_barriere_montee && !flag_barriere_attente && !flag_barriere_descente){
    if(millis() - t_actuel >= temps_barriere_attente){
      flag_barriere_attente = true;
    }
  }

  if(flag_barriere_montee && flag_barriere_attente && !flag_barriere_descente){
    if (millis() - t_actuel >= temps_barriere_entre_2degrees) {
      position_barriere -- ;
      servo_barriere.write(position_barriere);
      t_actuel = millis();
      if (position_barriere == 0){
        flag_barriere_descente = true;
      }
    }
  }

  if(flag_barriere_montee && flag_barriere_attente && flag_barriere_descente){
    flag_barriere_montee = false;
    flag_barriere_attente = false;
    flag_barriere_descente = false;
    flag_maintien_barriere = false;
  }
}

void loop() {
  digitalWrite(LEDPIN, HIGH);
  Etat_barriere = digitalRead(pin_interrupt);
  if (Etat_barriere == HIGH | flag_maintien_barriere == HIGH){
    flag_maintien_barriere = true;
    barriere();
  }
}

