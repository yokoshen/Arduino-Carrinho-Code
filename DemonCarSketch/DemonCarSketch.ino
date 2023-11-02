#include <Dabble.h>

//Sensor Sonico
#include <Ultrasonic.h>
#define pino_trigger 11
#define pino_echo 7 
Ultrasonic ultrasonic(pino_trigger, pino_echo);

//LCD
//#include <Wire.h> 
//#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//#define INCLUDE_TERMINAL_MODULE
//String Serialdata = "";
//bool dataflag = 0;

#define pinIN1 10    
#define pinIN2 9    
#define pinIN3 6    
#define pinIN4 5  
#define Buzina 8 
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
//#define Led 7

int StatusLed = 0;

int pDireita  = 100;
int pEsquerda = 100;

boolean botaoAnt = HIGH;
boolean botao    = HIGH;
boolean botao1 = false;

void setup() {
  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);
  pinMode(pinIN3, OUTPUT);
  pinMode(pinIN4, OUTPUT);
  pinMode(Buzina, OUTPUT);
  //pinMode(Led, OUTPUT);
  Dabble.begin(9600);
  //Serial.begin(9600);


  //LCD
  //lcd.init();          
  //lcd.backlight();
  //lcd.setCursor(3,0);
  //lcd.print("Demon Car");
  //Serial.begin(9600);
}

 
void loop() 
{
    Dabble.processInput(); 

    if (GamePad.isStartPressed()) 
    {
      botao = 1;     
      digitalWrite(Buzina, HIGH);  
      delay(100);
      digitalWrite(Buzina, LOW);  
    }
    else
    {
      botao = 0;
    }
    
    if (GamePad.isSquarePressed()) 
    {
        //StatusLed = !StatusLed;
        //digitalWrite(Led, StatusLed);          
    }

    if (botao && (botao != botaoAnt)) 
    {
      botao1 = !botao1;
    }
    botaoAnt = botao;

    if (botao1) 
    {
        digitalWrite(pinIN1, HIGH);
        digitalWrite(pinIN2, HIGH);
        digitalWrite(pinIN3, HIGH);
        digitalWrite(pinIN4, HIGH);
    }
    else
    {
      int CALCULO = GamePad.getAngle();
      CALCULO = CALCULO - 270;

      if (CALCULO < 0)
      {
        CALCULO = CALCULO + 360;
      }

      int pot2 = CALCULO;
      int pot1 = GamePad.getRadius();

      if (GamePad.isTrianglePressed()) 
      {
        digitalWrite(Buzina, HIGH);          
      }
      else
      {
        digitalWrite(Buzina, LOW);
      }
        
      //LCD
      /*if (GamePad.isCirclePressed()) 
      {
        lcd.setCursor(3,0);
        lcd.print("IPHONE LIXO");         
      }*/

      if (pot2 <= 180) {
          //Esquerda 
          pDireita  = 100;
          pEsquerda = map(pot2, 180, 0, 100, 0); 
          //Serial.println(pot2);
          //Serial.println();
      } 
      else {
          //Direita
          pDireita  = map(pot2, 180, 360, 100, 0);
          pEsquerda = 100; 
          //Serial.print(pot2);  
          //Serial.println();
      }

      if (GamePad.isCrossPressed()) 
      {
          //inverte os motores  
          int velocidade = map(pot1, 0, 7, 0, 255);

          analogWrite(pinIN1, LOW);
          analogWrite(pinIN2, velocidade * pDireita / 100);
      
          analogWrite(pinIN3, LOW);
          analogWrite(pinIN4, velocidade * pEsquerda / 100); 
      } 
      else 
      {
        //Sensor Sonico
         float cmMsec;
         long microsec = ultrasonic.timing();
         cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
         //Serial.print("Distancia em cm: ");
         //Serial.println(cmMsec);

          //Para frente
          if(cmMsec >= 15)
          {
            int velocidade = map(pot1, 0, 7, 0, 255);

            analogWrite(pinIN1, velocidade * pDireita / 100);
            analogWrite(pinIN2, LOW);
        
            analogWrite(pinIN3, velocidade * pEsquerda / 100);
            analogWrite(pinIN4, LOW);  
          }   
          else{
            digitalWrite(pinIN1, HIGH);
            digitalWrite(pinIN2, HIGH);
            digitalWrite(pinIN3, HIGH);
            digitalWrite(pinIN4, HIGH);
          }                 
        }
    }
}
