//This library are used for the lcd screen and the servomotor
#include <Servo.h>
#include <LiquidCrystal.h>

#define LEDPIN 13

int action_barriere;

//Here, some pin numbers are assigned to a variable name
int pot_binaire = 0;
int com_raspberry = 2;
int led_rouge = 6;
int potentiometre = A0;

//we can control the servo motor with a single instruction, but the movement is too fast for the gate. In this program, we control the speed of the gate movement.
const int temps_barriere_entre_2degrees = 40;   //40 ms is the time between two movements of a 1 degree angle
const int temps_barriere_attente = 2000;        //When the gate is open, it waits 2 seconds (2000 ms) before going back down
int position_barriere = 89;                     //The value assigned to this variable changes when the gate is moving.

//The following variables are used in the gate movement program.
long t_actuel;
bool flag_barriere_montee = false;
bool flag_barriere_descente = false;
bool flag_barriere_attente = false;
bool flag_maintien_barriere = false;

//And these are used in the LED blink program.
bool etat_led = false;
long t_led_rouge = 0;
float tempo;


Servo servo_barriere;                           //The name of the servo motor is setting
LiquidCrystal lcd(7,8,9,10,11,12);              //Selection of the different pin number use for the lcd screen


void setup() {

  //Then we can assign each pin to input or output.
                                  //Reminder:
  pinMode(LEDPIN, OUTPUT);        //Pin 13
  pinMode(com_raspberry, INPUT);  //Pin 2
  pinMode(led_rouge, OUTPUT);     //Pin 6
  pinMode(potentiometre, INPUT);  //Pin A0

 
  servo_barriere.attach(3);       //The servomotor has to be attach to a pin. Here the pin 3
  servo_barriere.write(89);       //Because of the way the servomotor is put, we have to tilt the barrier to 89° so that it is horizontal
  lcd.begin(16,2);                //This is a configuration of the screen's size
  
  delay(2000);                    //When the setup is done, we wait 2 seconds. A lot of forums and website give this recommendation.
}


void barriere(void){
  
  lcd.setCursor(0,0);//The cursor position is initialized
  lcd.write("Bienvenue");//Then we write a word or a sentence on the screen
  
  if(!flag_barriere_montee && !flag_barriere_attente && !flag_barriere_descente){ //We check if the gate is always horizontal
    if (millis() - t_actuel >= temps_barriere_entre_2degrees) {                   //We check if the elapsed time is equal or higher then the value of the variable name (=40 ms)
      position_barriere -- ;                      //If it's confirmed, we decrease the angle of the gate. The direction of the servomotor is reversed
      servo_barriere.write(position_barriere);    //The angle is write on the servomotor
      t_actuel = millis();                        //The elapsed time is updated
      if (position_barriere == 20){
        flag_barriere_montee = true;              //When the gate is at the top, the variable name is setting. In this way, we won't go into the loop anymore
      }
    }
  }

  if(flag_barriere_montee && !flag_barriere_attente && !flag_barriere_descente){  //We check if the gate is at the top
    if(millis() - t_actuel >= temps_barriere_attente){                            //We check if the elapsed time is equal or higher then the value of the variable name (=2000 ms)
      flag_barriere_attente = true;               //The variable name is setting because we have waiting. In this way, we won't go into the loop anymore
    }
  }

  if(flag_barriere_montee && flag_barriere_attente && !flag_barriere_descente){   //We check if the gate is at the top and if we have waiting
    if (millis() - t_actuel >= temps_barriere_entre_2degrees) {                   //We check if the elapsed time is equal or higher then the value of the variable name (=40 ms)
      position_barriere ++ ;                      //We increase the angle of the gate
      servo_barriere.write(position_barriere);    //The angle is write on the servomotor
      t_actuel = millis();                        //The elapsed time is updated
      if (position_barriere == 89){
        flag_barriere_descente = true;            //When the gate is at the down, the variable name is setting
      }
    }
  }

  if(flag_barriere_montee && flag_barriere_attente && flag_barriere_descente){    //When the cycle is finished,
    flag_barriere_montee = false;                 //all of this variable name is reset
    flag_barriere_attente = false;
    flag_barriere_descente = false;
    flag_maintien_barriere = false;
    lcd.clear();                                  //The lcd screen is cleared
    digitalWrite(led_rouge, LOW);                 //and the red LED is turned off
  }
}

void clignotement(float temps_clignotement){          //The input value is the time for the flashing frequency which was calculated before
  
  //In this part of the program, we will test the elapsed time with the time required for the frequency
  
  if(millis() - t_led_rouge >= temps_clignotement){  //"millis()" is the elapsed time of the arduino card
    t_led_rouge = millis();
    etat_led = digitalRead(led_rouge);
    if(etat_led == LOW){                             //To create a blink, we check the current state of the LED to turn it on or off
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
  
  action_barriere = digitalRead(com_raspberry);     //The value of the input pin is read. This one comes from the raspberry card

  //Here, we check two condition. Firstly, if the raspberry detect a number plate. And secondly, if the gate is closed. If only one is HIGH, the condition is confirm.
  if (action_barriere == HIGH | flag_maintien_barriere == HIGH){
    flag_maintien_barriere = true;                //This flag authorise to pass if the gate is in movement. It will be reset at the end of the movement.
    barriere();                                   //This instruction call the part of the program which move the gate.
    clignotement(tempo);                          //And this one makes the red LED blink
  }
}
