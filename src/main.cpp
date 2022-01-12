//-------------------------------------------------------------------------
// Programa Timbre 
// pruebas RTCC DS3231  ok
// Puebas Display LCD ok
// Agregados buscar : 16/12/21
// Modificaciones variables para el uso del timbre : 17/12/21
// falta poner reloj en hora por programación (Investigar como)
// Falta grabar y recuperar valores programados de EEPORM  
// Falta evaluar los timbres programados
// Y actuar en consecuencia 
// -------------------------------------------------------------------------
#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h> // incluye la librería para el manejo de la memoria EEPROM

LiquidCrystal_I2C lcd (0x27, 16,2);  //

//DS3231 rtc;    // correcion descomentá esta linea  borrando // para vos y comentá la proxima agregando // delante de DS1307 rtc;
DS1307 rtc;       // asi sirve el programa para ambas versiones 

int segundo,minuto,hora,dia,mes;
long anio; //variable año
DateTime HoraFecha;
const int  pulsador_der = 2;    // pin 2 como entrada para el contador de pulsos ascendentes
const int  pulsador_izq = 3;    // pin 3 como entrada para el contador de pulsos descendentes

const int  puls_sube = 4;    // ver pin si es correcto 17/12/21
const int  puls_baja = 5;    // ver pin si es correcto 17/12/21
const int  jumper = 6;       // 17/12/21

int contador            = 0;   // contador para el numero de veces presionados
int estado_pulsador_der = 0;         // estado actual del pulsador
int lastButtonState_der = 1;     // estado anterior del pulsador
int estado_pulsador_izq = 0;         // estado actual del pulsador
int lastButtonState_izq = 1;     // estado anterior del pulsador

int estado_puls_sube = 1;         // estado actual del pulsador
int lastButtonState_sube = 1;     // estado anterior del pulsador
int estado_puls_baja = 1;         // estado actual del pulsador
int lastButtonState_baja = 1;     // estado anterior del pulsador

 
//  Agregados ----------------------------- 
int millis_ant = 0; // 16/12/21   En esta variable guardamos el estado anterior de la varaible MILLS
int inc_seg =0;     // 16/12/21   Guarda incrementos de segundo de un contador interno
int muestra = 0;    // 16/12/21   Guarda cada cuantos segundos refrescamos el display
long t_ant = 0;    // correccion int paso a long


// variables para cargar de eepron con los timbres programados 
// ahora valores provisorios para mostrar
int T1h = 8;
int T1m = 15;
int T2h = 9;
int T2m = 20;
int T3h = 10;
int T3m = 25;
int T4h = 11;
int T4m = 30; 
int T5h = 12;
int T5m = 45;
int T6h = 14;
int T6m = 50;


void setup () {
  pinMode(pulsador_der, INPUT_PULLUP); //Resistencia de pullup interna
  pinMode(pulsador_izq, INPUT_PULLUP); //Resistencia de pullup interna
  pinMode(puls_sube, INPUT_PULLUP); //Resistencia de pullup interna
  pinMode(puls_baja, INPUT_PULLUP); //Resistencia de pullup interna
  pinMode(jumper, INPUT_PULLUP); //Resistencia de pullup interna


  
  Serial.begin(9600);

  //EEPROM.write(0, 18); // direccion dato

  
  //EEPROM.read (0);

  // recupera valores de eepron si fue guardado alguna vez 
  int lectura = EEPROM.read (0);
  if (EEPROM.read (0) == 40){
    
    T1h = EEPROM.read (1);
    T1m = EEPROM.read (2);
    T2h = EEPROM.read (3);
    T2m = EEPROM.read (4);
    T3h = EEPROM.read (5);
    T3m = EEPROM.read (6);
    T4h = EEPROM.read (7);
    T4m = EEPROM.read (8);
    T5h = EEPROM.read (9);
    T5m = EEPROM.read (10);
    T6h = EEPROM.read (11);
    T6m = EEPROM.read (12);

  }
  else{
    T1h = 8;
    T1m = 15;
    T2h = 9;
    T2m = 20;
    T3h = 10;
    T3m = 25;
    T4h = 11;
    T4m = 30; 
    T5h = 12;
    T5m = 45;
    T6h = 14;
    T6m = 50;
  }

  Serial.println("Comienzo del Reloj");
  
  Wire.begin(); // muy importante esto 
  rtc.begin();  //Inicializamos el RTC
  
  Serial.println("Estableciendo Hora y fecha...");
  delay(200);

  // Esta línea establece el RTC con una fecha y hora explícitas, por ejemplo para establecer
  // 21 de enero de 2014 a las 3 a.m. llamarías:
   rtc.adjust (DateTime (2014, 1, 21,  3, 0, 0));
  //                       año, mes,dia, h, m, s
  
  
  //rtc.adjust(DateTime(__DATE__, __TIME__)); // horario real
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
  lcd.print(" Linea 1"); // " EET N 1" // Imprime " EET N 1" en la primera linea
  lcd.setCursor ( 0, 1 ); // Mover el cursor a la fila (1) columna (0)
  lcd.print(" Linea 2"); // " Dr. Etchebarne"// Imprime " Dr. Etchebarne"
  delay(1000); // Espera 6 segundos
  lcd.clear(); // Limpia la pantalla

  //Recupera valores EEPROM






}

void loop () {

  long t_actual = millis();      //  carga el valor actual de millis (Fijate que aqui se define la variable y no antes es una forma mas de hacerlo antes de usarla)
  if (t_actual - t_ant > 250)    // compara si el valor actual con el anterior mas un tiempo en milisegundos
  { // 1000 mseg  equivale a 1 segundo, 250 = 1/4 segundo
    t_ant = t_actual; // aqui ingresa una vez por segundo 
    muestra ++; // incremento la variable muestra
    //Serial.println(muestra);
    
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
  estado_puls_sube = digitalRead(puls_sube);
  estado_puls_baja = digitalRead(puls_baja);


  // comparar el estado del botón a su estado anterior
  if (estado_pulsador_der != lastButtonState_der) {
    // si el estado fue cambiado, incremente el conteo
    if (estado_pulsador_der == LOW) {                      //  correcion  HIGH paso a LOW
      // si el estado actual es alto, entonces 
      // que pase de off a on:
      contador++;
        if (digitalRead (jumper) == 0){   //¿ esta en programacion ? 
             if (contador>=16){
                 contador=15;  // segundo limite para programación
             }
        }
        else{
          if (contador >=3) {
            contador = 2;         // aqui queda en el maximo valor no estando en programacion  
          }

        }

/*
        if (contador >=3) {   // aqui es incorrecto si contador es > a 2 debes dejarlo en 2 no en cero  // correcion > paso a >= 
            if (jumper==0){   //¿ esta en programacion ? 
             if (contador>15){
                 contador=14;  // segundo limite para programación
             }
            }
 
            else{
             contador = 2;         // aqui queda en el maximo valor no estando en programacion  
            }
 */           
      
      Serial.println("CONT: " + String (contador) );
      
      delay(300);
    } 
  }
  
    lastButtonState_der  = estado_pulsador_der;
  
 


   // comparar el estado del botón a su estado anterior
  if (estado_pulsador_izq != lastButtonState_izq ) {
    
    // si el estado fue cambiado, decrementa el conteo
    if (estado_pulsador_izq == LOW) {                     //  correcion  HIGH paso a LOW  
      // si el estado actual es alto, entonces 
      // que pase de off a on:
      contador--;
        if (contador < 0) {
        contador = 0;        //vuelve a ser cero aqui queda en el minimo valor 
      }
      Serial.println("CONT: " + String (contador) );
      
      delay(300);
    } 
  }
  //guarda el último estado actual como el ultimo estado
  //para el proximo bucle
  
    lastButtonState_izq  = estado_pulsador_izq;
  
  

// --------------------------------------------------------- 
// Boton Subir 
// ---------------------------------------------------------
  // comparar el estado del botón a su estado anterior
  if (estado_puls_sube != lastButtonState_sube ) {
    if ( estado_puls_sube == LOW) {
      Serial.println("Sube");

        switch (contador)  // 
        {
            case(3):
                T1h++;
                if(T1h>=25){
                  T1h=24;
                }
            break;

            case(4):
                T1m++;
                if(T1m>=60){
                  T1m=59;
                }
            break;

            case(5):
                T2h++;
                if(T2h>=25){
                  T2h=24;
                }
            break;

            case(6):
                T2m++;
                if(T2m>=60){
                  T2m=59;
                }
            break;

            case(7):
                T3h++;
                if(T3h>=25){
                  T3h=24;
                }
            break;

            case(8):
                T3m++;
                if(T3m>=60){
                  T3m=59;
                }
            break;

            case(9):
                T4h++;
                if(T4h>=25){
                  T4h=24;
                }
            break;

            case(10):
                T4m++;
                if(T4m>=60){
                  T4m=59;
                }
            break;

            case(11):
                T5h++;
                if(T4h>=25){
                  T4h=24;
                }
            break;

            case(12):
                T5m++;
                if(T5m>=60){
                  T5m=59;
                }
            break;


            case(13):
                T6h++;
                if(T6h>=25){
                  T5h=24;
                }
            break;

            case(14):
                T6m++;
                if(T6m>=60){
                  T6m=59;
                }
            break;
            case(15):
                 EEPROM.write (0,40);
                 EEPROM.write (1,T1h);
                 EEPROM.write (2,T1m);
                 EEPROM.write (3,T2h);
                 EEPROM.write (4,T2m);
                 EEPROM.write (5,T3h);
                 EEPROM.write (6,T3m);
                 EEPROM.write (7,T4h);
                 EEPROM.write (8,T4m);
                 EEPROM.write (9,T5h);
                 EEPROM.write (10,T5m);
                 EEPROM.write (11,T6h);
                 EEPROM.write (12,T6m);
                 Serial.print("Memoria Grabada");



            break;

           
   
        }
   
      delay(100);
    } 
  }
  
  lastButtonState_sube  = estado_puls_sube;
  
  

   switch (contador)
 {

     case(3):
     break;
     
 }


// --------------------------------------------------------- 
// Boton Bajar
// ---------------------------------------------------------
  // comparar el estado del botón a su estado anterior
  if (estado_puls_baja != lastButtonState_baja ) {
    if (estado_puls_baja == LOW) {
      Serial.println("Baja");

        switch (contador)  // 
        {
            case(3):
                T1h--;
                if(T1h<0){
                  T1h=0;
                }
            break;

            case(4):
                T1m--;
                if(T1m<0){
                  T1m=0;
                }
            break;

            case(5):
                T2h--;
                if(T2h<0){
                  T2h=0;
                }
            break;

            case(6):
                T2m--;
                if(T2m<0){
                  T2m=0;
                }
            break;

            case(7):
                T3h--;
                if(T3h<0){
                  T3h=0;
                }
            break;

            case(8):
                T3m--;
                if(T3m<0){
                  T3m=0;
                }
            break;

            case(9):
                T4h--;
                if(T4h<0){
                  T4h=0;
                }
            break;

            case(10):
                T4m--;
                if(T4m<0){
                  T4m=0;
                }
            break;

            case(11):
                T5h--;
                if(T4h<0){
                  T4h=0;
                }
            break;

            case(12):
                T5m--;
                if(T5m<0){
                  T5m=0;
                }
            break;


            case(13):
                T6h--;
                if(T6h<0){
                  T6h=0;
                }
            break;

            case(14):
                T6m--;
                if(T6m<0){
                  T6m=0;
                }
            break;

            
   
        }
   
      delay(100);
    } 
  }
  
      lastButtonState_baja  = estado_puls_baja;
  
 
// Comienzo de exhibicion Display  =====================================================

if (muestra == 4) {   // si pasaron 5 segundos va a entrar en la rutina de mostrar y quito los retardos de 6 segundos
 muestra=0; 
 switch (contador)
 {
   // NOTA: vas a notar que saque muchos lcd.setCursor(X,Y)
   // cuando paro el cusor en la posicion 0 y escribo hola
   // el cursor ya queda parado un lugar despues del la (a) de (hola)
   // si lo que voy a escribir esta a continuacion no debo volver a poner el cursor en el luhgar que ya está
   // para optimizar un poco el codigo y los tiempos.

  case 0:
  //  Esto es para ir viendo como hacer que la hora no quede 12:9:6  y quede 12:09:06  hay que verlo 
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)

      lcd.setCursor(3,0);  // Mover el cursor a la columna () , fila()
      lcd.print(dia); // Imprime en Display la variable "dia"
      lcd.print("/"); // Imprime en Display "/"
      lcd.print(mes); // Imprime en Display la variable "mes"
      lcd.print("/"); // Imprime en Display "/"
      lcd.print(anio); // Imprime en Display la variable "anio"
      
      lcd.setCursor(4,1);  // Mover el cursor a la columna () , fila()
      if (hora<=9){
        lcd.print("0");
      }
      lcd.print(hora); // Imprime en Display la variable "hora"

      lcd.setCursor(6,1);  // Mover el cursor a la columna () , fila()
      lcd.print(":"); // Imprime en Display ":"
      if(minuto<=9){
        lcd.print("0");
      }
      lcd.print(minuto); // Imprime en Display la variable "minuto"/* code */

      lcd.setCursor(9,1);  // Mover el cursor a la columna () , fila()
      lcd.print(":"); // Imprime en Display ":"
      if(segundo<=9){
        lcd.print("0");
      }
      lcd.print(segundo); // Imprime en Display la variable "minuto"/* code */

/*   
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("FECHA:"); // Imprime en Display "FECHA:"
      lcd.print(dia); // Imprime en Display la variable "dia"
      lcd.print("/"); // Imprime en Display "/"
      lcd.print(mes); // Imprime en Display la variable "mes"
      lcd.print("/"); // Imprime en Display "/"
      lcd.print(anio); // Imprime en Display la variable "anio"
      lcd.setCursor(0,1);  // Mover el cursor a la fila (1) columna (0)
      lcd.print("HORA:"); // Imprime en Display "HORA:"
      lcd.print(hora); // Imprime en Display la variable "hora"
      lcd.print(":"); // Imprime en Display ":"
      lcd.print(minuto); // Imprime en Display la variable "minuto"
      lcd.print(":"); // Imprime en Display ":"
      lcd.print(segundo); // Imprime en Display la variable "segundo"
*/      
    break;
  
  case 1:
      lcd.clear();
      lcd.home (); // Mover el cursor a la columna (0) fila (0)
      lcd.print("T1:"); // Imprime en Display "Timbre1:"
      lcd.print(T1h);
      lcd.print("."); // 
      lcd.print(T1m);

      lcd.setCursor(8,0);  // Mover el cursor a la columna (8) fila (0)
      lcd.print("T2:"); // Imprime en Display "Timbre2:"
      lcd.print(T2h);
      lcd.print("."); // 
      lcd.print(T2m);

      lcd.setCursor (0,1); // Mover el cursor a la columna (0) fila (1)
      lcd.print("T3:"); // Imprime en Display "Timbre3:"
      lcd.print(T3h);
      lcd.print("."); // 
      lcd.print(T3m);

      lcd.setCursor(8,1);  // Mover el cursor a la columna (8) fila (1)
      lcd.print("T4:"); // Imprime en Display "Timbre4:"
      lcd.print(T4h);
      lcd.print("."); // 
      lcd.print(T4m);

      //delay (6000);
    break;

    case 2:
      lcd.clear();
      lcd.home (); // Mover el cursor a la columna (0) fila (0)
      lcd.print("T5:"); // Imprime en Display "Timbre5:"
      lcd.print(T5h);
      lcd.print("."); // 
      lcd.print(T5m);

      lcd.setCursor(0,8);  // Mover el cursor a la columna (8) fila (0)
      lcd.print("T6:"); // Imprime en Display "Timbre6:"
      lcd.print(T6h);
      lcd.print("."); // 
      lcd.print(T6m);
            
    break;
    // -----------------------------------------------------------------------
    // pantallas de programacion 
    case 3:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("Hora T1:"); // Imprime en Display "Timbre1:"
      lcd.print(T1h);
      
            
    break;

    case 4:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("Min. T1:"); // Imprime en Display "Timbre1:"
      lcd.print(T1m);
            
    break;
    case 5:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("Hora T2:"); // Imprime en Display "Timbre1:"
      lcd.print(T2h);
      
            
    break;
    case 6:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("Min. T2:"); // Imprime en Display "Timbre1:"
      lcd.print(T2m);
            
    break;
    case 7:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("Hora T3:"); // Imprime en Display "Timbre1:"
      lcd.print(T3h);
      
            
    break;
    case 8:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("Min. T3:"); // Imprime en Display "Timbre1:"
      lcd.print(T3m);
            
    break;
    case 9:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("Hora T4:"); // Imprime en Display "Timbre1:"
      lcd.print(T4h);
      
            
    break;
    case 10:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("Min. T4:"); // Imprime en Display "Timbre1:"
      lcd.print(T4m);
            
    break;
    case 11:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("Hora T5:"); // Imprime en Display "Timbre1:"
      lcd.print(T5h);
      
            
    break;
    case 12:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("Min. T5:"); // Imprime en Display "Timbre1:"
      lcd.print(T5m);
            
    break;
    case 13:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("Hora T6:"); // Imprime en Display "Timbre1:"
      lcd.print(T6h);
      
            
    break;
    case 14:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("Min. T6:"); // Imprime en Display "Timbre1:"
      lcd.print(T6m);
            
    break;
    case 15:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("Graba cambios?"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);  // Mover el cursor a la columna () , fila()
      lcd.print("Presione Subir  "); // Imprime en Display "Timbre1:"
      lcd.print(T6m);
    break;
   

  }








}
 
 // Fin Display ===========================================================

 // Aqui vamos a evaluar los Timbres programados ==========================

 




}



