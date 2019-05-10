//This library are used for the lcd screen and the motor
#include <Servo.h>
#include <LiquidCrystal.h>

#define LEDPIN 13

int Etat_barriere;
int pin_interrupt = 2;
int led_rouge = 6;
int potentiometre = A0;
int pot_binaire = 0;
float tempo;

const int temps_barriere_entre_2degrees = 40;
const int temps_barriere_attente = 2000;
long t_actuel;
long t_led_rouge = 0;
int position_barriere = 89;
bool flag_barriere_montee = false;
bool flag_barriere_descente = false;
bool flag_barriere_attente = false;
bool flag_maintien_barriere = false;
bool etat_led = false;

//Variable relatif à la barrière
Servo servo_barriere;

LiquidCrystal lcd(7,8,9,10,11,12);

void setup() {

  //Then we can assign each pin to input or output.
                                  //Reminder:
  pinMode(LEDPIN, OUTPUT);        //Pin 13
  pinMode(pin_interrupt, INPUT);  //Pin 2
  pinMode(led_rouge, OUTPUT);     //Pin 6
  pinMode(potentiometre, INPUT);  //Pin A0

 
  servo_barriere.attach(3);//The servo-motor has to be attach to a pin. Here the pin 3
  servo_barriere.write(89);//Because of the way the servo motor is put, we have to tilt the barrier to 89° so that it is straight
  lcd.begin(16,2);//This is a configuration of the screen's size
  
  delay(2000); //When the setup is done, we wait 2 seconds. A lot of forums and website give this recommendation.
}


void barriere(void){
  
  lcd.setCursor(0,0);
  lcd.write("Bienvenue");
  
  if(!flag_barriere_montee && !flag_barriere_attente && !flag_barriere_descente){
    if (millis() - t_actuel >= temps_barriere_entre_2degrees) {
      position_barriere -- ;
      servo_barriere.write(position_barriere);
      t_actuel = millis();
      if (position_barriere == 20){
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
      position_barriere ++ ;
      servo_barriere.write(position_barriere);
      t_actuel = millis();
      if (position_barriere == 89){
        flag_barriere_descente = true;
      }
    }
  }

  if(flag_barriere_montee && flag_barriere_attente && flag_barriere_descente){
    flag_barriere_montee = false;
    flag_barriere_attente = false;
    flag_barriere_descente = false;
    flag_maintien_barriere = false;
    lcd.clear();
    digitalWrite(led_rouge, LOW);
  }
}

void clignotement(float temps_clignotement){
  if(millis() - t_led_rouge >= temps_clignotement){
    t_led_rouge = millis();
    etat_led = digitalRead(led_rouge);
    if(etat_led == LOW){
      digitalWrite(led_rouge, HIGH);
    }else{
      digitalWrite(led_rouge, LOW);
    }
  }
}

void loop() {
  digitalWrite(LEDPIN, HIGH);                     //When the program is looping, the small led is alight.

  pot_binaire = analogRead(potentiometre);        //The value of the analog potentiometer is recovered.
  float frequence = map(pot_binaire,0,1005,1,6);  //This instruction gives a value include between 1 and 6. It's the frequency of the red LED.
  tempo = (1/frequence) * 1000;                   //The frequency is converted to seconds.
  
  Etat_barriere = digitalRead(pin_interrupt);     //The value of the input pin is read

  //Here, we check two condition. Firstly, if the raspberry detect a number plate. And secondly, if the gate is closed. If only one is HIGH, the condition is confirm.
  if (Etat_barriere == HIGH | flag_maintien_barriere == HIGH){
    flag_maintien_barriere = true;//This flag authorise to pass if the gate is in movement
    barriere();
    clignotement(tempo);
  }
}
