#include <LiquidCrystal_I2C.h>

/* 
              
           ___________________________________________________________________

           *** {==[=======> (MANEJO DEL I2C CON PANTALLA LCD) <=======]==}  ***
           ___________________________________________________________________
*/
/*
 Board I2C / TWI pins
Uno       =   A4 (SDA), A5 (SCL)
Mega2560  =   20 (SDA), 21 (SCL)
Leonardo  =   2 (SDA), 3 (SCL)
Due       =   20 (SDA), 21 (SCL), SDA1, SCL1

esta configuracion de estos pines se encuentran dentro de la librería "wire" mas info: https://www.arduino.cc/en/Reference/Wire
 */

#include <LiquidCrystal_I2C.h> //DESCARGAR LIBRERÍA:https://github.com/ELECTROALL/Codigos-arduino/blob/master/LiquidCrystal_I2C.zip      esto cuando se utilice el codigo en  otro equipo  desde cero ya que la libreria no me salia en el buscador de id 

LiquidCrystal_I2C lcd(0x27,16,2); // si no te sale con esta direccion  puedes usar  dependiendo de la pantalla (0x3f,16,2) || (0x27,16,2)  ||(0x20,16,2) 
#define THERMISTORPIN A0         
// resistance at 25 degrees C  resistencia a 25 grados C   en este caso 24 por la variacion resistiva y por mas presicion 
#define THERMISTORNOMINAL 10000 //OHMIOS     
// temp. for nominal resistance (almost always 25 C)     temp. para la resistencia nominal (casi siempre 25 C)           en este caso la nominal es 24 por que el thermistor tiene lijeras variaciones resistivas
#define TEMPERATURENOMINAL 24   
// how many samples to take and average, more takes longer  cuántas muestras tomar y promediar, más tarda más

// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)      El coeficiente beta del termistor (generalmente 3000-4000)
#define BCOEFFICIENT 4000
// el valor de la resistencia  que se conecta en serie junto al thermistor ," ojo la resistencia debe ser del mismo valor que el thermistor" tambien ajustar las variaciones de esta misma
#define SERIESRESISTOR 10000   

int samples[NUMSAMPLES];
void setup() {
  lcd.init();
  Serial.begin(9600);
  lcd.backlight();
  lcd.clear();
 lcd.setCursor(0,0);
  
}

void loop() { 

   lcd.clear();
  uint8_t i;
  float average;

  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
  
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
  
  // convert the value to resistance
  //PUEDES imprimir la variable average si deseas ver en ohmios del sensor
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert absolute temp to C

  lcd.setCursor(0,0);
  lcd.print("Temperatura ");
    Serial.print("temperatura .\n");
  lcd.setCursor(0,1);         // esto indica  que 0 es la primera linea de imprecion de la pantalla   y 1 la SEGUNDA
  lcd.print(steinhart);
  lcd.print(" *C");
  Serial.print(steinhart);
  delay(1000);
}
