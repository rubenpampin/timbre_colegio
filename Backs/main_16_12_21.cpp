//-------------------------------------------------------------------------
// Programa Timbre
// pruebas RTCC DS1307  ok
// Puebas Display LCD ok
// -------------------------------------------------------------------------
#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

DS1307 rtc;

int segundo,minuto,hora,dia,mes;
long anio; //variable año
DateTime HoraFecha;

#include  <LiquidCrystal_I2C.h>
 
// Constructor de la librería de LCD 16x2
// Aqui se configuran los pines asignados a la pantalla del PCF8574
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C lcd (0x27, 16,2);  //
 

void setup () {
  Serial.begin(9600);
  Serial.println("Comienzo del Reloj");
  
  Wire.begin(); // muy importante esto 
  rtc.begin(); //Inicializamos el RTC
  
  Serial.println("Estableciendo Hora y fecha...");

  delay(200);

  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); esta no funciona se cuelga 
  //rtc.adjust(DateTime(2016,6,10,18,29,0));//2016,Junio,10,18 h,29 min,0 seg   // Horario a mano
  rtc.adjust(DateTime(__DATE__, __TIME__)); // horario real
  Serial.println("DS1307 actualizado con la hora y fecha que se compilo este programa:");

  //delay(200);

  Serial.print("Fecha = ");
  delay(200);
  Serial.print(__DATE__);   // hora de la computadora 
  delay(200);
  Serial.print("  Hora = ");
  delay(200);
  Serial.println(__TIME__);  // dia de la computadora 

  lcd. init ();
  
  // Enciende backligth
  lcd. backlight ();
  // Limpia display 
  lcd.clear();
  
  // print the Message on the LCD. 
  // lcd. print ( " Secadero V1.1" );

   // Indicar a la libreria que tenemos conectada una pantalla de 16x2
  lcd.begin(16, 2);
  // Mover el cursor a la primera posición de la pantalla (0, 0)
  lcd.home ();
  // Imprimir "Hola Mundo" en la primera linea
  lcd.print("Prueba Display");
  // Mover el cursor a la segunda linea (1) primera columna
  lcd.setCursor ( 0, 1 );
  // Imprimir otra cadena en esta posicion
  lcd.print("Timbre Colegio");
  // Esperar un segundo
  delay(1000);

  
}

void loop () {

  HoraFecha = rtc.now(); //obtenemos la hora y fecha actual en una sola linea de comando 



  // *****************************************************************
  // Cargamos las variables con las lecturas realizadas 
  // *****************************************************************
    segundo=HoraFecha.second();
    minuto=HoraFecha.minute();
    hora=HoraFecha.hour();
    dia=HoraFecha.day();
    mes=HoraFecha.month();
    anio=HoraFecha.year();

  // *****************************************************************
  // Enviamos por el puerto serie la hora y fecha.
  //******************************************************************
    Serial.print("hora = ");  
    Serial.print(hora);  

    Serial.print(":");
    Serial.print(minuto);

    Serial.print(":");
    Serial.print(segundo);

    Serial.print("  Fecha = ");
    Serial.print(dia);

    Serial.print("/");

    Serial.print(mes);

    Serial.print("/");

    Serial.print(anio);
    Serial.println();

    delay(2000);    // retardo de tiempo para poder ver con tranquilidad 

  // EN EL CICLO PRINCIPAL SOLAMENTE RECORREMOS EL MENSAJE DE UN LADO A OTRO
  // Variable para conteo de ciclos
  //int i;
 
  // Desplazar la pantalla a la derecha 2 veces
  for ( int i = 0; i < 5; i++ ) {
    lcd.scrollDisplayRight();
    delay (500);
  }
 
  // Desplazar la pantalla a la izquierda 2 veces
  for ( int i = 0; i < 5; i++ ) {
    lcd.scrollDisplayLeft();
    delay (5000);
  }




}



// ************************************************** //
/*  https://www.luisllamas.es/reloj-y-calendario-en-arduino-con-los-rtc-ds1307-y-ds3231/
#include <Wire.h>
#include "RTClib.h"

const int outputPin = LED_BUILTIN;
bool state = false;

// RTC_DS1307 rtc;
RTC_DS3231 rtc;

void setup() {
   Serial.begin(9600);
   delay(1000);

   if (!rtc.begin()) {
      Serial.println(F("Couldn't find RTC"));
      while (1);
   }

   if (rtc.lostPower()) {
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   }
}

// Comprobar si esta programado el encendido
bool isScheduledON(DateTime date)
{
   int weekDay = date.dayOfTheWeek();
   float hours = date.hour() + date.minute() / 60.0;

   // De 09:30 a 11:30 y de 21:00 a 23:00
   bool hourCondition = (hours > 9.50 && hours < 11.50) || (hours > 21.00 && hours < 23.00);

   // Miercoles, Sabado o Domingo
   bool dayCondition = (weekDay == 3 || weekDay == 6 || weekDay == 0); 
   if (hourCondition && dayCondition)
   {
      return true;
   }
   return false;
}

void loop() {
   DateTime now = rtc.now();

   if (state == false && isScheduledON(now))      // Apagado y debería estar encendido
   {
      digitalWrite(outputPin, HIGH);
      state = true;
      Serial.print("Activado");
   }
   else if (state == true && !isScheduledON(now))  // Encendido y deberia estar apagado
   {
      digitalWrite(outputPin, LOW);
      state = false;
      Serial.print("Desactivar");
   }

   delay(3000);
}
*/