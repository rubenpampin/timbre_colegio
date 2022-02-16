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

byte flecha [] = {
  B01000,
  B00100,
  B00010,
  B11111,
  B11111,
  B00010,
  B00100,
  B01000
};

byte campana [] = {
  B00000,
  B00100,
  B01110,
  B01110,
  B11111,
  B00000,
  B00100,
  B00000
};

byte n2 [] = {
  B11111,
  B00000,
  B10001,
  B11001,
  B10101,
  B10011,
  B10001,
  B00000
};

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
int T7h = 16;
int T7m = 20;
int T8h = 11;
int T8m = 22;
int T9h = 22;
int T9m = 22;
int T10h = 22;
int T10m = 22;
int T11h = 22;
int T11m = 22;
int T12h = 22;
int T12m = 22;
int T1s = 0;
int T2s = 0;
int T3s = 0;
int T4s = 0;
int T5s = 0;
int T6s = 0;
int T7s = 0;
int T8s = 0;
int T9s = 0;
int T10s = 0;
int T11s = 0;
int T12s = 0;
int Fd = 1;
int Fm = 1;
int Fa = 2022;
int Rh = 10;
int Rm = 10;



void setup () {

  

  pinMode(pulsador_der, INPUT_PULLUP); //Resistencia de pullup interna
  pinMode(pulsador_izq, INPUT_PULLUP); //Resistencia de pullup interna
  pinMode(puls_sube, INPUT_PULLUP); //Resistencia de pullup interna
  pinMode(puls_baja, INPUT_PULLUP); //Resistencia de pullup interna
  pinMode(jumper, INPUT_PULLUP); //Resistencia de pullup interna


  
  Serial.begin(9600);
  delay(2000);

  //EEPROM.write(0, 18); // direccion dato

  
  //EEPROM.read (0);

  // recupera valores de eepron si fue guardado alguna vez 

  Serial.println("Lectura de EEPROM");

  int lectura = EEPROM.read (0);

  if (lectura == 40){

      Serial.println("Recupera valores de guardados");
    
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
    T7h = EEPROM.read (13);
    T7m = EEPROM.read (14);
    T8h = EEPROM.read (15);
    T8m = EEPROM.read (16);
    T9h = EEPROM.read (17);
    T9m = EEPROM.read (18);
    T10h = EEPROM.read (19);
    T10m = EEPROM.read (20);
    T11h = EEPROM.read (21);
    T11m = EEPROM.read (22);
    T12h = EEPROM.read (23);
    T12m = EEPROM.read (24);
    T1s = EEPROM.read (25); 
    T2s = EEPROM.read (26);
    T3s = EEPROM.read (27);    
    T4s = EEPROM.read (28);    
    T5s = EEPROM.read (29);    
    T6s = EEPROM.read (30);    
    T7s = EEPROM.read (31);    
    T8s = EEPROM.read (32);    
    T9s = EEPROM.read (33);    
    T10s = EEPROM.read (34);    
    T11s = EEPROM.read (35);    
    T12s = EEPROM.read (36);  
    /*      
    Fd = EEPROM.read (37);       
    Fm = EEPROM.read (38);   
    Fa = EEPROM.read (39);
    Rh = EEPROM.read (40);
    Rm = EEPROM.read (41);  
    */                            

  }

  else{
     Serial.println("Pone valores de inicio");
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
    T7h = 16;
    T7m = 20;
    T8h = 21;
    T8m = 22;
    T9h = 22;
    T9m = 22;
    T10h = 22;
    T10m = 22;
    T11h = 22;
    T11m = 22;
    T12h = 22;
    T12m = 22;
    T1s = 0;
    T2s = 0;    
    T3s = 0;    
    T4s = 0;    
    T5s = 0;    
    T6s = 0;    
    T7s = 0;    
    T8s = 0;    
    T9s = 0;    
    T10s = 0;    
    T11s = 0;    
    T12s = 0;
    Fd = 1;
    Fm = 1;
    Fa = 2022;
    Rh = 10;
    Rm = 10;        
        

  }

  Serial.println("Comienzo del Reloj");
  
  Wire.begin(); // muy importante esto 
  rtc.begin();  //Inicializamos el RTC
  
  Serial.println("Estableciendo Hora y fecha...");
  delay(200);

  // Esta línea establece el RTC con una fecha y hora explícitas, por ejemplo para establecer
  // 21 de enero de 2014 a las 3 a.m. llamarías:
  // rtc.adjust (DateTime (Fa, Fm, Fd,  Rh, Rm, 0));
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
  lcd.createChar(0, flecha);
  lcd.createChar(1, campana);
  lcd.createChar(2, n2);
  lcd.home (); // Mover el cursor a la fila (0) columna (0)
  lcd.print(" Linea 1"); // " EET N 1" // Imprime " EET N 1" en la primera linea
  lcd.setCursor ( 0, 1 ); // Mover el cursor a la fila (1) columna (0)
  lcd.print(" Linea 2"); // " Dr. Etchebarne"// Imprime " Dr. Etchebarne"
  delay(1000); // Espera 6 segundos
  lcd.clear(); // Limpia la pantalla

  //Recupera valores EEPROM

  HoraFecha = rtc.now(); //obtenemos la hora y fecha actual en una sola linea de comando 
  segundo=HoraFecha.second(); // guarda los segundos leidos en la variable "segundo"
  Rm=minuto=HoraFecha.minute(); // guarda los minutos leidos en la variable "minuto"
  Rh=hora=HoraFecha.hour(); // guarda la hora leida en la variable "hora"
  Fd=dia=HoraFecha.day(); // guarda el día leido en la variable "dia"
  Fm=mes=HoraFecha.month(); // guarda el mes leido en la variable "mes"
  Fa=anio=HoraFecha.year(); // guarda el año leido en la variable "anio"


  // Esta línea establece el RTC con una fecha y hora explícitas, por ejemplo para establecer
  // 21 de enero de 2014 a las 3 a.m. llamarías:
  // rtc.adjust (DateTime (Fa, Fm, Fd,  Rh, Rm, 0));
  //                    año, mes,dia, h,  m,  s



}

void loop () {

   
  long t_actual = millis();      //  carga el valor actual de millis (Fijate que aqui se define la variable y no antes es una forma mas de hacerlo antes de usarla)
  if (t_actual - t_ant > 250)    // compara si el valor actual con el anterior mas un tiempo en milisegundos
  { // 1000 mseg  equivale a 1 segundo, 250 = 1/4 segundo
    t_ant = t_actual; // aqui ingresa una vez por segundo 
    muestra ++; // incremento la variable muestra
    //Serial.println(muestra);
    
  }

  if (T1s==1 && T1h==hora && T1m==minuto && segundo==0)
    {
      if (t_actual - t_ant > 15000)
        {
        Serial.println("Enciende Alarma");
        }
      
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
             if (contador>=49){
                 contador=48;  // segundo limite para programación
             }
        }
        else{
          if (contador >=7) {
            contador = 6;         // aqui queda en el maximo valor no estando en programacion  
          }

        }

     
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
            case(7):
                T1h++;
                if(T1h>=25){
                  T1h=24;
                }
            break;

            case(8):
                T1m++;
                if(T1m>=60){
                  T1m=59;
                }
            break;

            case(9):
                T1s++;
                if(T1s>=2){
                  T1s=1;
                }
            break;            

            case(10):
                T2h++;
                if(T2h>=25){
                  T2h=24;
                }
            break;

            case(11):
                T2m++;
                if(T2m>=60){
                  T2m=59;
                }
            break;

            case(12):
                T2s++;
                if(T2s>=2){
                  T2s=1;
                }
            break;            

            case(13):
                T3h++;
                if(T3h>=25){
                  T3h=24;
                }
            break;

            case(14):
                T3m++;
                if(T3m>=60){
                  T3m=59;
                }
            break;

            case(15):
                T3s++;
                if(T3s>=2){
                  T3s=1;
                }
            break;            

            case(16):
                T4h++;
                if(T4h>=25){
                  T4h=24;
                }
            break;

            case(17):
                T4m++;
                if(T4m>=60){
                  T4m=59;
                }
            break;

            case(18):
                T4s++;
                if(T4s>=2){
                  T4s=1;
                }
            break;            

            case(19):
                T5h++;
                if(T5h>=25){  //MODIFICACION
                  T5h=24; // MODIFICACION
                }
            break;

            case(20):
                T5m++;
                if(T5m>=60){
                  T5m=59;
                }
            break;

            case(21):
                T5s++;
                if(T5s>=2){
                  T5s=1;
                }
            break;            


            case(22):
                T6h++;
                if(T6h>=25){
                  T5h=24;
                }
            break;

            case(23):
                T6m++;
                if(T6m>=60){
                  T6m=59;
                }
            break;

            case(24):
                T6s++;
                if(T6s>=2){
                  T6s=1;
                }
            break;            

            case(25): // AGREGADO
                T7h++;
                if(T7h>=25){
                  T7h=24;
                }
            break;

            case(26):  // AGREGADO
                T7m++;
                if(T7m>=60){
                  T7m=59;
                }
            break;

            case(27):
                T7s++;
                if(T7s>=2){
                  T7s=1;
                }
            break;            

            case(28):  // AGREGADO
                T8h++;
                if(T8h>=25){
                  T8h=24;
                }
            break;

            case(29):  // AGREGADO
                T8m++;
                if(T8m>=60){
                  T8m=59;
                }
            break;

            case(30):
                T8s++;
                if(T8s>=2){
                  T8s=1;
                }
            break;            

            case(31):  // AGREGADO
                T9h++;
                if(T9h>=25){
                  T9h=24;
                }
            break;

            case(32):  // AGREGADO
                T9m++;
                if(T9m>=60){
                  T9m=59;
                }
            break;

            case(33):
                T9s++;
                if(T9s>=2){
                  T9s=1;
                }
            break;            

            case(34):  // AGREGADO
                T10h++;
                if(T10h>=25){
                  T10h=24;
                }
            break;

            case(35):  // AGREGADO
                T10m++;
                if(T10m>=60){
                  T10m=59;
                }
            break;

            case(36):
                T10s++;
                if(T10s>=2){
                  T10s=1;
                }
            break;            

            case(37):  // AGREGADO
                T11h++;
                if(T11h>=25){
                  T11h=24;
                }
            break;

            case(38):  // AGREGADO
                T11m++;
                if(T11m>=60){
                  T11m=59;
                }
            break;

            case(39):
                T11s++;
                if(T11s>=2){
                  T11s=1;
                }
            break;            

            case(40):  // AGREGADO
                T12h++;
                if(T12h>=25){
                  T12h=24;
                }
            break;

            case(41):  // AGREGADO
                T12m++;
                if(T12m>=60){
                  T12m=59;
                }
            break;

            case(42):
                T12s++;
                if(T12s>=2){
                  T12s=1;
                }
            break;            

            case(43):
                Fd++;
                if(Fd>=32){
                  Fd=31;
                }
            break;            

            case(44):
                Fm++;
                if(Fd>=13){
                  Fd=12;
                }
            break;     

            case(45):
                Fa++;
                if(Fa>=4000){
                  Fa=3999;
                }
            break;                

            case(46):
                Rh++;
                if(Rh>=25){
                  Rh=24;
                }
            break;  

            case(47):
                Rm++;
                if(Rm>=60){
                  Rm=59;
                }
            break;                         

            case(48):
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
                 EEPROM.write (13,T7h);  // AGREGADO
                 EEPROM.write (14,T7m);  // AGREGADO
                 EEPROM.write (15,T8h);  // AGREGADO
                 EEPROM.write (16,T8m);  // AGREGADO
                 EEPROM.write (17,T9h);  // AGREGADO
                 EEPROM.write (18,T9m);  // AGREGADO
                 EEPROM.write (19,T10h);  // AGREGADO
                 EEPROM.write (20,T10m);  // AGREGADO
                 EEPROM.write (21,T11h);  // AGREGADO
                 EEPROM.write (22,T11m);  // AGREGADO
                 EEPROM.write (23,T12h);  // AGREGADO
                 EEPROM.write (24,T12m);  // AGREGADO
                 EEPROM.write (25,T1s);  // AGREGADO                                  
                 EEPROM.write (26,T2s);  // AGREGADO                                  
                 EEPROM.write (27,T3s);  // AGREGADO                                  
                 EEPROM.write (28,T4s);  // AGREGADO                                  
                 EEPROM.write (29,T5s);  // AGREGADO                                  
                 EEPROM.write (30,T6s);  // AGREGADO                                  
                 EEPROM.write (31,T7s);  // AGREGADO                                  
                 EEPROM.write (32,T8s);  // AGREGADO                                  
                 EEPROM.write (33,T9s);  // AGREGADO                                  
                 EEPROM.write (34,T10s);  // AGREGADO                                  
                 EEPROM.write (35,T11s);  // AGREGADO                                  
                 EEPROM.write (36,T12s);  // AGREGADO
                 /*
                 EEPROM.write (37,Fd);  // AGREGADO
                 EEPROM.write (38,Fm);  // AGREGADO
                 EEPROM.write (39,Fa);  // AGREGADO
                 EEPROM.write (40,Rh);  // AGREGADO
                 EEPROM.write (41,Rm);  // AGREGADO
                 */
                 Serial.print("Memoria Grabada");

                 rtc.adjust (DateTime (Fa, Fm, Fd,  Rh, Rm, 0)); // ajusta el reloj 

                 contador=0;  ///Vuelve todo al comienzo 



            break;

           
   
        }
   
      delay(100);
    } 
  }
  
  lastButtonState_sube  = estado_puls_sube;
  
  

   switch (contador)
 {

     case(6):
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
            case(7):
                T1h--;
                if(T1h<0){
                  T1h=0;
                }
            break;

            case(8):
                T1m--;
                if(T1m<0){
                  T1m=0;
                }
            break;

            case(9):
                T1s--;
                if(T1s<0){
                  T1s=0;
                }
            break;

            case(10):
                T2h--;
                if(T2h<0){
                  T2h=0;
                }
            break;

            case(11):
                T2m--;
                if(T2m<0){
                  T2m=0;
                }
            break;

            case(12):
                T2s--;
                if(T2s<0){
                  T2s=0;
                }
            break;

            case(13):
                T3h--;
                if(T3h<0){
                  T3h=0;
                }
            break;

            case(14):
                T3m--;
                if(T3m<0){
                  T3m=0;
                }
            break;

            case(15):
                T3s--;
                if(T3s<0){
                  T3s=0;
                }
            break;

            case(16):
                T4h--;
                if(T4h<0){
                  T4h=0;
                }
            break;

            case(17):
                T4m--;
                if(T4m<0){
                  T4m=0;
                }
            break;

            case(18):
                T4s--;
                if(T4s<0){
                  T4s=0;
                }
            break;

            case(19):
                T5h--;
                if(T5h<0){
                  T5h=0;
                }
            break;

            case(20):
                T5m--;
                if(T5m<0){
                  T5m=0;
                }
            break;

            case(21):
                T5s--;
                if(T5s<0){
                  T5s=0;
                }
            break;

            case(22):
                T6h--;
                if(T6h<0){
                  T6h=0;
                }
            break;

            case(23):
                T6m--;
                if(T6m<0){
                  T6m=0;
                }
            break;

            case(24):
                T6s--;
                if(T6s<0){
                  T6s=0;
                }
            break;

            case(25):  // AGREGADO
                T7h--;
                if(T7h<0){
                  T7h=0;
                }
            break;

            case(26):  // AGREGADO
                T7m--;
                if(T7m<0){
                  T7m=0;
                }
            break;

            case(27):
                T7s--;
                if(T7s<0){
                  T7s=0;
                }
            break;

            case(28):  // AGREGADO
                T8h--;
                if(T8h<0){
                  T8h=0;
                }
            break;

            case(29):  // AGREGADO
                T8m--;
                if(T8m<0){
                  T8m=0;
                }
            break;

            case(30):
                T8s--;
                if(T8s<0){
                  T8s=0;
                }
            break;

            case(31):  // AGREGADO
                T9h--;
                if(T9h<0){
                  T9h=0;
                }
            break;

            case(32):  // AGREGADO
                T9m--;
                if(T9m<0){
                  T9m=0;
                }
            break;

            case(33):
                T9s--;
                if(T9s<0){
                  T9s=0;
                }
            break;

            case(34):  // AGREGADO
                T10h--;
                if(T10h<0){
                  T10h=0;
                }
            break;

            case(35):  // AGREGADO
                T10m--;
                if(T10m<0){
                  T10m=0;
                }
            break;

            case(36):
                T10s--;
                if(T10s<0){
                  T10s=0;
                }
            break;

            case(37):  // AGREGADO
                T11h--;
                if(T11h<0){
                  T11h=0;
                }
            break;

            case(38):  // AGREGADO
                T11m--;
                if(T11m<0){
                  T11m=0;
                }
            break;

            case(39):
                T11s--;
                if(T11s<0){
                  T11s=0;
                }
            break;

            case(40):  // AGREGADO
                T12h--;
                if(T12h<0){
                  T12h=0;
                }
            break;

            case(41):  // AGREGADO
                T12m--;
                if(T12m<0){
                  T12m=0;
                }
            break;

            case(42):
                T12s--;
                if(T12s<0){
                  T12s=0;
                }
            break;

            case(43):
                Fd--;
                if(Fd<1){
                  Fd=1;
                }
            break;

            case(44):
                Fm--;
                if(Fm<1){
                  Fm=1;
                }
            break;

            case(45):
                Fa--;
                if(Fa<2022){
                  Fa=2022;
                }
            break;

            case(46):
                Rh--;
                if(Rh<0){
                  Rh=0;
                }
            break;

            case(47):
                Rm--;
                if(Rm<0){
                  Rm=0;
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
      if (dia<9){
        lcd.print("0");
        lcd.print(dia);
      }
      else{
      lcd.print(dia); // Imprime en Display la variable "dia"
      }
      lcd.print("/"); // Imprime en Display "/"
      if (mes<9){
        lcd.print("0");
        lcd.print(mes);
      }
      else {
      lcd.print(mes); // Imprime en Display la variable "mes"
      }
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

    break;
  
  case 1:
      lcd.clear();
      lcd.home (); // Mover el cursor a la columna (0) fila (0)
      lcd.print("TIMBRE  1 "); // Imprime en Display "Timbre5:"
      if (T1h<=9){
        lcd.setCursor(10,0);
        lcd.print("0");
        lcd.print(T1h);
      }
      else{
      lcd.print(T1h);
      }
      if (T1m<=9){
        lcd.print(":0"); // 
        lcd.print(T1m);}
      else{
        lcd.print(":");
        lcd.print(T1m);  
      }
      if (T1s == 1) {
        lcd.write(byte(1));
      }

      lcd.setCursor(0,1);  // Mover el cursor a la columna (8) fila (0)
      lcd.print("TIMBRE  2 "); // Imprime en Display "Timbre6:"
      if (T2h<=9){
        lcd.setCursor(10,1);
        lcd.print("0");
        lcd.print(T2h);
      }
      else{
      lcd.setCursor(10,1);  
      lcd.print(T2h);
      }
      if (T2m<=9){
        lcd.print(":0"); // 
        lcd.print(T2m);}
      else{
        lcd.print(":");
        lcd.print(T2m);  
      }
      if (T2s == 1) {
        lcd.write(byte(1));
      }  
    break;


    case 2:
      lcd.clear();
      lcd.home (); // Mover el cursor a la columna (0) fila (0)
      lcd.print("TIMBRE  3 "); // Imprime en Display "Timbre5:"
      if (T3h<=9){
        lcd.setCursor(10,0);
        lcd.print("0");
        lcd.print(T3h);
      }
      else{
      lcd.print(T3h);
      }
      if (T3m<=9){
        lcd.print(":0"); // 
        lcd.print(T3m);}
      else{
        lcd.print(":");
        lcd.print(T3m);  
      }
      if (T3s == 1) {
        lcd.write(byte(1));
      }

      lcd.setCursor(0,1);  // Mover el cursor a la columna (8) fila (0)
      lcd.print("TIMBRE  4 "); // Imprime en Display "Timbre6:"
      if (T4h<=9){
        lcd.setCursor(10,1);
        lcd.print("0");
        lcd.print(T4h);
      }
      else{
      lcd.setCursor(10,1);  
      lcd.print(T4h);
      }
      if (T4m<=9){
        lcd.print(":0"); // 
        lcd.print(T4m);}
      else{
        lcd.print(":");
        lcd.print(T4m);  
      }
      if (T4s == 1) {
        lcd.write(byte(1));
      }
  
    break;


    case 3:
      lcd.clear();
      lcd.home (); // Mover el cursor a la columna (0) fila (0)
      lcd.print("TIMBRE  5 "); // Imprime en Display "Timbre5:"
      if (T5h<=9){
        lcd.setCursor(10,0);
        lcd.print("0");
        lcd.print(T5h);
      }
      else{
      lcd.print(T5h);
      }
      if (T5m<=9){
        lcd.print(":0"); // 
        lcd.print(T5m);}
      else{
        lcd.print(":");
        lcd.print(T5m);  
      }
      if (T5s == 1) {
        lcd.write(byte(1));
      }

      lcd.setCursor(0,1);  // Mover el cursor a la columna (8) fila (0)
      lcd.print("TIMBRE  6 "); // Imprime en Display "Timbre6:"
      if (T6h<=9){
        lcd.setCursor(10,1);
        lcd.print("0");
        lcd.print(T6h);
      }
      else{
      lcd.setCursor(10,1);  
      lcd.print(T6h);
      }
      if (T6m<=9){
        lcd.print(":0"); // 
        lcd.print(T6m);}
      else{
        lcd.print(":");
        lcd.print(T6m);  
      }
      if (T6s == 1) {
        lcd.write(byte(1));
      }
  
    break;

    case 4:
      lcd.clear();
      lcd.home (); // Mover el cursor a la columna (0) fila (0)
      lcd.print("TIMBRE  7 "); // Imprime en Display "Timbre5:"
      if (T7h<=9){
        lcd.setCursor(10,0);
        lcd.print("0");
        lcd.print(T7h);
      }
      else{
      lcd.print(T7h);
      }
      if (T7m<=9){
        lcd.print(":0"); // 
        lcd.print(T7m);}
      else{
        lcd.print(":");
        lcd.print(T7m);  
      }
      if (T7s == 1) {
        lcd.write(byte(1));
      }

      lcd.setCursor(0,1);  // Mover el cursor a la columna (8) fila (0)
      lcd.print("TIMBRE  8 "); // Imprime en Display "Timbre6:"
      if (T8h<=9){
        lcd.setCursor(10,1);
        lcd.print("0");
        lcd.print(T8h);
      }
      else{
      lcd.setCursor(10,1);  
      lcd.print(T8h);
      }
      if (T8m<=9){
        lcd.print(":0"); // 
        lcd.print(T8m);}
      else{
        lcd.print(":");
        lcd.print(T8m);  
      }
        if (T8s == 1) {
        lcd.write(byte(1));
      }
  
    break;

    case 5:
      lcd.clear();
      lcd.home (); // Mover el cursor a la columna (0) fila (0)
      lcd.print("TIMBRE  9 "); // Imprime en Display "Timbre5:"
      if (T9h<=9){
        lcd.setCursor(10,0);
        lcd.print("0");
        lcd.print(T9h);
      }
      else{
      lcd.print(T9h);
      }
      if (T9m<=9){
        lcd.print(":0"); // 
        lcd.print(T9m);}
      else{
        lcd.print(":");
        lcd.print(T9m);  
      }
      if (T9s == 1) {
        lcd.write(byte(1));
      }

      lcd.setCursor(0,1);  // Mover el cursor a la columna (8) fila (0)
      lcd.print("TIMBRE 10 "); // Imprime en Display "Timbre6:"
      if (T10h<=9){
        lcd.setCursor(10,1);
        lcd.print("0");
        lcd.print(T10h);
      }
      else{
      lcd.setCursor(10,1);  
      lcd.print(T10h);
      }
      if (T10m<=9){
        lcd.print(":0"); // 
        lcd.print(T10m);}
      else{
        lcd.print(":");
        lcd.print(T10m);  
      }
      if (T10s == 1) {
        lcd.write(byte(1));
      }
  
    break;

    case 6:
      lcd.clear();
      lcd.home (); // Mover el cursor a la columna (0) fila (0)
      lcd.print("TIMBRE 11 "); // Imprime en Display "Timbre5:"
      if (T11h<=9){
        lcd.setCursor(10,0);
        lcd.print("0");
        lcd.print(T11h);
      }
      else{
      lcd.print(T11h);
      }
      if (T11m<=9){
        lcd.print(":0"); // 
        lcd.print(T11m);}
      else{
        lcd.print(":");
        lcd.print(T11m);  
      }
      if (T11s == 1) {
        lcd.write(byte(1));
      }

      lcd.setCursor(0,1);  // Mover el cursor a la columna (8) fila (0)
      lcd.print("TIMBRE 12 "); // Imprime en Display "Timbre6:"
      if (T12h<=9){
        lcd.setCursor(12,1);
        lcd.print("0");
        lcd.print(T12h);
      }
      else{
      lcd.setCursor(10,1);  
      lcd.print(T12h);
      }
      if (T12m<=9){
        lcd.print(":0"); // 
        lcd.print(T12m);}
      else{
        lcd.print(":");
        lcd.print(T12m);  
      }
      if (T12s == 1) {
        lcd.write(byte(1));
      }
  
    break;


    // -----------------------------------------------------------------------
    // pantallas de programacion 
    case 7:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T1"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);
      lcd.write(byte(0));
      lcd.print("H:");
      lcd.print(T1h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T1m);
      lcd.setCursor (13,1);
      if (T1s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 8:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T1"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T1h);
      lcd.setCursor (6,1);
      lcd.write(byte(0));
      lcd.print("M:");
      lcd.print(T1m);
      lcd.setCursor (13,1);
      if (T1s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 9:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T1"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T1h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T1m);
      lcd.setCursor (12,1);
      lcd.write(byte(0));
      if (T1s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 10:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T2"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);
      lcd.write(byte(0));
      lcd.print("H:");
      lcd.print(T2h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T2m);
      lcd.setCursor (13,1);
      if (T2s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 11:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T2"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T2h);
      lcd.setCursor (6,1);
      lcd.write(byte(0));
      lcd.print("M:");
      lcd.print(T2m);
      lcd.setCursor (13,1);
      if (T2s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }      
    break;

    case 12:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T2"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T2h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T2m);
      lcd.setCursor (12,1);
      lcd.write(byte(0));
      if (T2s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 13:  // muestra los Timbres horas y minutos paso a paso
    lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T3"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);
      lcd.write(byte(0));
      lcd.print("H:");
      lcd.print(T3h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T3m);
      lcd.setCursor (13,1);
      if (T3s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 14:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T3"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T3h);
      lcd.setCursor (6,1);
      lcd.write(byte(0));
      lcd.print("M:");
      lcd.print(T3m);
      lcd.setCursor (13,1);
      if (T3s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 15:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T3"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T3h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T3m);
      lcd.setCursor (12,1);
      lcd.write(byte(0));
      if (T3s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 16:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T4"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);
      lcd.write(byte(0));
      lcd.print("H:");
      lcd.print(T4h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T4m);       
      lcd.setCursor (13,1);
      if (T4s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 17:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T4"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T4h);
      lcd.setCursor (6,1);
      lcd.write(byte(0));
      lcd.print("M:");
      lcd.print(T4m);        
      lcd.setCursor (13,1);
      if (T4s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 18:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T4"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T4h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T4m);
      lcd.setCursor (12,1);
      lcd.write(byte(0));
      if (T4s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 19:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T5"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);
      lcd.write(byte(0));
      lcd.print("H:");
      lcd.print(T5h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T5m);       
      lcd.setCursor (13,1);
      if (T5s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 20:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T5"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T5h);
      lcd.setCursor (6,1);
      lcd.write(byte(0));
      lcd.print("M:");
      lcd.print(T5m);        
      lcd.setCursor (13,1);
      if (T5s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 21:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T5"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T5h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T5m);
      lcd.setCursor (12,1);
      lcd.write(byte(0));
      if (T5s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 22:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T6"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);
      lcd.write(byte(0));
      lcd.print("H:");
      lcd.print(T6h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T6m);       
      lcd.setCursor (13,1);
      if (T6s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 23:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T6"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T6h);
      lcd.setCursor (6,1);
      lcd.write(byte(0));
      lcd.print("M:");
      lcd.print(T6m);        
      lcd.setCursor (13,1);
      if (T6s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 24:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T6"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T6h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T6m);
      lcd.setCursor (12,1);
      lcd.write(byte(0));
      if (T6s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 25:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T7"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);
      lcd.write(byte(0));
      lcd.print("H:");
      lcd.print(T7h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T7m);       
      lcd.setCursor (13,1);
      if (T7s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 26:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T7"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T7h);
      lcd.setCursor (6,1);
      lcd.write(byte(0));
      lcd.print("M:");
      lcd.print(T7m);        
      lcd.setCursor (13,1);
      if (T7s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 27:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T7"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T7h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T7m);
      lcd.setCursor (12,1);
      lcd.write(byte(0));
      if (T7s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 28:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T8"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);
      lcd.write(byte(0));
      lcd.print("H:");
      lcd.print(T8h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T8m);       
      lcd.setCursor (13,1);
      if (T8s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 29:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T8"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T8h);
      lcd.setCursor (6,1);
      lcd.write(byte(0));
      lcd.print("M:");
      lcd.print(T8m);        
      lcd.setCursor (13,1);
      if (T8s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 30:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T8"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T8h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T8m);
      lcd.setCursor (12,1);
      lcd.write(byte(0));
      if (T8s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 31:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T9"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);
      lcd.write(byte(0));
      lcd.print("H:");
      lcd.print(T9h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T9m);       
      lcd.setCursor (13,1);
      if (T9s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 32:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T9"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T9h);
      lcd.setCursor (6,1);
      lcd.write(byte(0));
      lcd.print("M:");
      lcd.print(T9m);        
      lcd.setCursor (13,1);
      if (T9s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 33:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T9"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T9h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T9m);
      lcd.setCursor (12,1);
      lcd.write(byte(0));
      if (T9s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 34:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T10"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);
      lcd.write(byte(0));
      lcd.print("H:");
      lcd.print(T10h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T10m);       
      lcd.setCursor (13,1);
      if (T10s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 35:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T10"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T10h);
      lcd.setCursor (6,1);
      lcd.write(byte(0));
      lcd.print("M:");
      lcd.print(T10m);        
      lcd.setCursor (13,1);
      if (T10s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 36:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T10"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T10h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T10m);
      lcd.setCursor (12,1);
      lcd.write(byte(0));
      if (T10s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 37:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T11"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);
      lcd.write(byte(0));
      lcd.print("H:");
      lcd.print(T11h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T11m);       
      lcd.setCursor (13,1);
      if (T11s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 38:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T11"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T11h);
      lcd.setCursor (6,1);
      lcd.write(byte(0));
      lcd.print("M:");
      lcd.print(T11m);        
      lcd.setCursor (13,1);
      if (T11s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 39:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T11"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T11h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T11m);
      lcd.setCursor (12,1);
      lcd.write(byte(0));
      if (T11s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 40:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T12"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);
      lcd.write(byte(0));
      lcd.print("H:");
      lcd.print(T12h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T12m);       
      lcd.setCursor (13,1);
      if (T12s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 41:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T12"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T12h);
      lcd.setCursor (6,1);
      lcd.write(byte(0));
      lcd.print("M:");
      lcd.print(T12m);        
      lcd.setCursor (13,1);
      if (T12s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 42:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("PROGRAMACION T12"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(T12h);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(T12m);
      lcd.setCursor (12,1);
      lcd.write(byte(0));
      if (T12s == 1){
        lcd.print("ON");
      }
      else {
        lcd.print("OFF");

      }
    break;

    case 43:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("  FIJAR FECHA"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);
      lcd.write(byte(0));
      lcd.print("DIA:");
      lcd.print(Fd);
      
    break;

    case 44:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("  FIJAR FECHA"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);
      lcd.write(byte(0));
      lcd.print("MES:");
      lcd.print(Fm);
      
    break;

    case 45:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("  FIJAR FECHA"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);
      lcd.write(byte(0));
      lcd.print("A");
      lcd.write(byte(2));
      lcd.print("O:");
      lcd.print(Fa);
      
    break;

    case 46:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("   FIJAR HORA"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);
      lcd.write(byte(0));
      lcd.print("H:");
      lcd.print(Rh);
      lcd.setCursor (7,1);
      lcd.print("M:");
      lcd.print(Rm);       
      
    break;

    case 47:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("   FIJAR HORA"); // Imprime en Display "Timbre1:"
      lcd.setCursor(1,1);
      lcd.print("H:");
      lcd.print(Rh);
      lcd.setCursor (6,1);
      lcd.write(byte(0));
      lcd.print("M:");
      lcd.print(Rm);

    break;        

    case 48:  // muestra los Timbres horas y minutos paso a paso
      lcd.clear();
      lcd.home (); // Mover el cursor a la fila (0) columna (0)
      lcd.print("Graba cambios?"); // Imprime en Display "Timbre1:"
      lcd.setCursor(0,1);  // Mover el cursor a la columna () , fila()
      lcd.print("Presione Subir  "); // Imprime en Display "Timbre1:"
      
      
      
    break;
   

  }








}
 
 // Fin Display ===========================================================

 // Aqui vamos a evaluar los Timbres programados ==========================

 




}