//-------------------------------------------------------------------------
// Programa Timbre
// pruebas RTCC DS3231  ok
// Puebas Display LCD ok
// Agregados buscar : 16/12/21
// -------------------------------------------------------------------------
#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

DS3231 rtc;

int segundo,minuto,hora,dia,mes;
long anio; //variable año
DateTime HoraFecha;
const int  pulsador_der = 2;    // pin 2 como entrada para el contador de pulsos ascendentes
const int  pulsador_izq = 3;    // pin 3 como entrada para el contador de pulsos descendentes
int contador            = 0;   // contador para el numero de veces presinados
int estado_pulsador_der = 0;         // estado actual del pulsador
int lastButtonState_der = 1;     // estado anterior del pulsador
int estado_pulsador_izq = 0;         // estado actual del pulsador
int lastButtonState_izq = 1;     // estado anterior del pulsador
 
//  Agregados ----------------------------- 
int millis_ant = 0; // 16/12/21   En esta variable guardamos el estado anterior de la varaible MILLS
int inc_seg =0;     // 16/12/21   Guarda incrementos de segundo de un contador interno
int muestra = 0;    // 16/12/21   Guarda cada cuantos segundos refrescamos el display
int t_ant = 0;
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27, 16,2);  //

void setup () {
  pinMode(pulsador_der, INPUT_PULLUP); //Resistencia de pullup interna
  pinMode(pulsador_izq, INPUT_PULLUP); //Resistencia de pullup interna
  
  Serial.begin(9600);
  Serial.println("Comienzo del Reloj");
  
  Wire.begin(); // muy importante esto 
  rtc.begin(); //Inicializamos el RTC
  
  Serial.println("Estableciendo Hora y fecha...");
  delay(200);

  
  rtc.adjust(DateTime(__DATE__, __TIME__)); // horario real
  Serial.println("DS3231 actualizado con la hora y fecha que se compilo este programa:");

  Serial.print("Fecha = ");
  delay(200);
  Serial.print(__DATE__);   // Día de la Computadora 
  delay(200);
  Serial.print("  Hora = ");
  delay(200);
  Serial.println(__TIME__);  // Hora de la Computadora 

  // **************************************************
  // * Imprime en Pantalla Presentación de la Escuela *
  // **************************************************
  lcd.init (); // Prepara el display para su uso
  lcd.backlight (); // Enciende backligth
  lcd.clear(); // Limpia display 
  lcd.begin(16, 2); // Indicar a la libreria que tenemos conectada un display de 16x2
  lcd.home (); // Mover el cursor a la fila (0) columna (0)
  lcd.print(" EET N 1"); // Imprime " EET N 1" en la primera linea
  lcd.setCursor ( 0, 1 ); // Mover el cursor a la fila (1) columna (0)
  lcd.print(" Dr. Etchebarne"); // Imprime " Dr. Etchebarne"
  delay(6000); // Espera 6 segundos
  lcd.clear(); // Limpia la pantalla
}

void loop () {

  long t_actual = millis();      // carga el valor actual de millis (Fijate que aqui se define la variable y no antes es una forma mas de hacerlo antes de usarla)
  if (t_actual - t_ant > 1000)   // compara si el valor actual con el anterior mas un tiempo en milisegundos
  { // 1000 mseg  equivale a 1 segundo 
    t_ant = t_actual; // aqui ingresa una vez por segundo 
    muestra ++; // incremento la variable muestra
    
  }

  HoraFecha = rtc.now(); //obtenemos la hora y fecha actual en una sola linea de comando 
  segundo=HoraFecha.second(); // guarda los segundos leidos en la variable "segundo"
  minuto=HoraFecha.minute(); // guarda los minutos leidos en la variable "minuto"
  hora=HoraFecha.hour(); // guarda la hora leida en la variable "hora"
  dia=HoraFecha.day(); // guarda el día leido en la variable "dia"
  mes=HoraFecha.month(); // guarda el mes leido en la variable "mes"
  anio=HoraFecha.year(); // guarda el año leido en la variable "anio"

  estado_pulsador_der = digitalRead(pulsador_der);
  estado_pulsador_izq = digitalRead(pulsador_izq);  


  // comparar el estado del botón a su estado anterior
  if (estado_pulsador_der != lastButtonState_der) {
    // si el estado fue cambiado, incremente el conteo
    if (estado_pulsador_der == HIGH) {
      // si el estado actual es alto, entonces 
      // que pase de off a on:
      contador++;
        if (contador > 2) {   // aqui es incorrecto si contador es > a 2 debes dejarlo en 2 no en cero 
        contador = 1;         // aqui queda en el maxomo valor 
      }
      Serial.println (" ");
      Serial.println(contador);
      delay(100);
    } 
  }
 lastButtonState_der  = estado_pulsador_der;
   // comparar el estado del botón a su estado anterior
  if (estado_pulsador_izq != lastButtonState_izq ) {
    // si el estado fue cambiado, decrementa el conteo
    if (estado_pulsador_izq == HIGH) {
      // si el estado actual es alto, entonces 
      // que pase de off a on:
      contador--;
        if (contador < 0) {
        contador = 0;        //vuelve a ser cero aqui queda en el minimo valor 
      }
      Serial.println(" ");
      Serial.println(contador);
      delay(100);
    } 
  }
  //guarda el último estado actual como el ultimo estado
  //para el proximo bucle
  lastButtonState_izq  = estado_pulsador_izq;
 

// Comienzo de exhibicion Display  =====================================================

if (muestra == 5) {   // si pasaron 5 segundos va a entrar en la rutina de mostrar y quito los retardos de 6 segundos
 muestra=0; 
 switch (contador)
 {
   // NOTA: vas a notar que saque muchos lcd.setCursor(X,Y)
   // cuando paro el cusor en la posicion 0 y escribo hola
   // el cursor ya queda parado un lugar despues del la (a) de (hola)
   // si lo que voy a escribir esta a continuacion no debo volver a poner el cursor en el luhgar que ya está
   // para optimizar un poco el codigo y los tiempos.

  case 0:
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("FECHA:"); // Imprime en Display "FECHA:"
      //lcd.setCursor(6,0); // Mover el cursor a la fila (0) columna (6)
      lcd.print(dia); // Imprime en Display la variable "dia"
      //lcd.setCursor(8,0); // Mover el cursor a la fila (0) columna (8)
      lcd.print("/"); // Imprime en Display "/"
      //lcd.setCursor(9,0); // Mover el cursor a la fila (0) columna (9)
      lcd.print(mes); // Imprime en Display la variable "mes"
      //lcd.setCursor(11,0); // Mover el cursor a la fila (0) columna (11)
      lcd.print("/"); // Imprime en Display "/"
      //lcd.setCursor(12,0); // Mover el cursor a la fila (0) columna (12)
      lcd.print(anio); // Imprime en Display la variable "anio"
      lcd.setCursor(0,1);  // Mover el cursor a la fila (1) columna (0)
      lcd.print("HORA:"); // Imprime en Display "HORA:"
      //lcd.setCursor(5,1); // Mover el cursor a la fila (1) columna (5)
      lcd.print(hora); // Imprime en Display la variable "hora"
      //lcd.setCursor(7,1); // Mover el cursor a la fila (1) columna (7)
      lcd.print(":"); // Imprime en Display ":"
      //lcd.setCursor(8,1); // Mover el cursor a la fila (1) columna (8)
      lcd.print(minuto); // Imprime en Display la variable "minuto"/* code */
      //delay (6000);
    break;
  
  case 1:
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("T1: 00:00"); // Imprime en Display "FECHA:"
      lcd.setCursor(0,1);  // Mover el cursor a la fila (1) columna (0)
      lcd.print("T2: 00:00"); // Imprime en Display "HORA:"
      //delay (6000);
    break;
  }
}
 

 // Fin Display ===========================================================

 // Aqui vamos a evaluar los Timbres programados ==========================

 




}



