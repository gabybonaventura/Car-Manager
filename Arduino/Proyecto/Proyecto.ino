#include <Servo.h>
#include <NewPing.h>
#include <Wire.h> 
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#define TRIGGER_PIN 13 // Pin del trigger del sensor de ultrasonido   
#define ECHO_PIN 12    // Pin del Echo del sensor de ultrasonido
#define MAX_DISTANCE 200 // Defino una maxima distancia a sondear
LiquidCrystal_I2C lcd(0x27,16,2); 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Inicializo el ultrasonido
SoftwareSerial BT1(4,2); // RX, TX recorder que se cruzan
Servo servoDelantero;
Servo servoTrasero;
const int sensorLluvia = A0;
const int actuadorLed = 7;
const int sensorLuz = 8;
const int actuadorServoDelantero = 9;
const int actuadorServoTrasero = 10;
const int actuadorBoton = 11;
int estadoBoton=0;   
int distanciaCM = 0;
int valueLluvia = 0;
long tiempoEnSegundos = 0;
long UltimoSegundo = 0;
int flagManual = 0; //se inicializa en 0, así está en automatico al iniciar el sistema.
int flagMoverServoTrasero = 1000;
int flagMoverServoDelantero = 1000;
int estadoSensar = 0;
int delaySensor = 0;
String cadena = ""; 
boolean isRain = false;
int distanceForShow = 0;


void setup() {
  pinMode(sensorLuz, INPUT);  //definir Sensor Luz como entrada
  pinMode(actuadorLed, OUTPUT); //definir Actuador Led como Salida
  pinMode(sensorLluvia, INPUT);  //definir Sensor lluvia como entrada
  pinMode(actuadorBoton, INPUT); //definir Sensor boton como entrada
  digitalWrite(actuadorLed, LOW); //Inicializar led apagado
  servoDelantero.attach(actuadorServoDelantero);  // Iniciar servo, con pin correspondiente
  servoDelantero.write(0); //Inicializar Servo en angulo 0
  servoTrasero.attach(actuadorServoTrasero);  // Iniciar servo, con pin correspondiente
  servoTrasero.write(0); //Inicializar Servo en angulo 0
  lcd.init(); // Inicializar el lcd 
  Serial.begin(9600);   //iniciar puerto serie
  BT1.begin(9600);//iniciar el serial de sw.
}

void loop(){
  //si está disponible el bluetooth y además el flag manual está en cero (sino no tiene sentido activar el modo manual)
  valueLluvia = analogRead(sensorLluvia);  //lectura analogica de pin
  boton();
  if(estadoBoton == 1){
    //lcd.backlight(); //Enciendo el LCD 
    sensar(); //comienza a sensar la distancia al objeto mas cercano
    moverServoTrasero(valueLluvia); //Este servo se mueve si llueve y ademas se puso march atras
  }
  else {
    lcd.clear(); //Limpio la pantalla del LCD
    //lcd.noBacklight(); //Apago el LCD
    distanceForShow = -1; 
  }
  if(flagManual == 0 and BT1.available()){
    String BTString = BT1.readString();
    if(BTString.indexOf("manual")!= -1){
      Serial.println("Modo Manual Activado");
      flagManual = 1;
    }
    Serial.println(BTString);
  }
  if(flagManual==0){
    modoAutomatico();
    //Serial.println("En Modo Automatico");   
  }
  else {
    modoManual();
    //Serial.println("En Modo Manual");   
  }
  if((estadoSensar == 500)) {
    cadena = "";          
    estadoSensar = 0;
    if(valueLluvia < 700 )
      isRain = true;
    else
      isRain = false;
    cadena = (String) "$" + isRain + "|" + digitalRead(sensorLuz) + "|" + distanceForShow + "#";
    //Protocolo de envido de mensajes hacia android
    //El caracter $ representa el inicio de la cadena, luego los datos se mandan en un orden determinado
    //el caracter # representa el fin de la cadena
    Serial.println(cadena);
    BT1.print(cadena);
  }
  estadoSensar++;
}

void boton() {
  if(digitalRead(actuadorBoton) == HIGH) { // Si se presion el boton
    estadoBoton=1-estadoBoton; // Se cambia de estadoBoton    
    delay(100); //Esperamos 100 ms por el tiempo que permanece el dedo sobre el boton
    //No es correcto utilizar un delay ya que se bloquean las demas actividades
    //Sin embargo en esta caso no es tan grave porque es un periodo muy corto y el boton se lo presiona esporadicamente
    Serial.println("Boton Precionado");
  }
}

void modoManual() {
  tiempoEnSegundos=millis()/1000; //La funcion millis() devuelve el tiempo en milisegundos desde que el arduino comenzo a ejecutar el sketch
  if(BT1.available()){
    String BTString = BT1.readString();
    Serial.println("String Envidado desde BT");
    Serial.println(BTString);   
    //no se si puedo usar switch con strings. Hago IFs anidados para asemejar a un switch.
    if(BTString.indexOf("servoDelanteroOn") != -1){
      Serial.println("servoDelanteroOn");
      flagMoverServoDelantero = 500;
    }
    else {
      if(BTString.indexOf("servoDelanteroOff") != -1){
        Serial.println("servoDelanteroOff"); 
        flagMoverServoDelantero = 1000;        
      }
      else {
        if(BTString.indexOf("servoTraseroOn") != -1){
          Serial.println("servoTraseroOn");
          flagMoverServoTrasero = 500;
        }
        else {
          if(BTString.indexOf("servoTraseroOff") != -1){
            Serial.println("servoTraseroOff");
            flagMoverServoTrasero = 1000;
          }
          else{
            if(BTString.indexOf("sensarOff") != -1){
              Serial.println("sensarOff"); 
              estadoBoton = 1 - estadoBoton;
              lcd.clear(); //Limpio la pantalla del LCD
              //lcd.noBacklight(); //Apago el LCD

            }
            else {
              if(BTString.indexOf("lucesDelanteroOn") != -1){
                Serial.println("lucesDelanteroOn"); 
                digitalWrite(actuadorLed, HIGH);
              }
              else { 
                if(BTString.indexOf("lucesDelanteroOff") != -1){
                  Serial.println("lucesDelanteroOff"); 
                  digitalWrite(actuadorLed, LOW);
                }
                else{
                  if(BTString.indexOf("sensarOn") != -1){
                    Serial.println("sensarOn");
                    //lcd.backlight(); //Enciendo el LCD 
                    estadoBoton = 1 - estadoBoton;
                  }
                  else{
                    if(BTString.indexOf("automatico") != -1){
                      Serial.println("Modo Automatica Activado"); 
                      flagManual = 0;
                    }
                    else {
                      if(BTString.indexOf("manual") != -1){
                        Serial.println("Modo Manual Activado"); 
                        flagManual = 1;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  moverServoTrasero(flagMoverServoTrasero);
  moverServoDelantero(flagMoverServoDelantero);
  UltimoSegundo = tiempoEnSegundos; // Se guarda los segundos que lleva ejecutando, para compararlo con el siguiente
}

void modoAutomatico() {
  tiempoEnSegundos=millis()/1000; // Se toma el los segundos que lleva ejecutando
  digitalWrite(actuadorLed, digitalRead(sensorLuz)); //Enciendo el led si detecta oscuridad, lo apaga si detecta luz
  moverServoDelantero(valueLluvia); //Este servo siempre se mueve si esta lloviendo
  UltimoSegundo = tiempoEnSegundos; // Se guarda los segundos que lleva ejecutando, para compararlo con el siguiente
}

void moverServoDelantero(int velocidad) {  
  if(velocidad < 700 && tiempoEnSegundos != UltimoSegundo ) {// Si se detecto lluvia y ademas el tiempo actual es distinto que el ultimo sensado, se mueve el servo
    if(servoDelantero.read() == 180) { //Si el servo esta en el angulo 180º
      servoDelantero.write(0); //Moverlo al 0º
    }
    else{
      if(servoDelantero.read() == 0) { //Si el servo esta en el angulo 0º
        servoDelantero.write(180); //Moverlo al 180º
      }
    }
  }
}

void moverServoTrasero(int velocidad) { //Aca replico codigo, pero me parece que de esta forma queda mucho mas claro  
  //Serial.println(velocidad); 
  if(velocidad < 700 && tiempoEnSegundos != UltimoSegundo ){ // Si se detecto lluvia y ademas el tiempo actual es distinto que el ultimo sensado, se mueve el servo
    if(servoTrasero.read() == 180) { //Si el servo esta en el angulo 180º
      servoTrasero.write(0); //Moverlo al 0º
    }
    else {
      if(servoTrasero.read() == 0) { //Si el servo esta en el angulo 0º
        servoTrasero.write(180); //Moverlo al 180º
      }
    }
  }  
}


void sensar() {
  if(delaySensor == 100) {
    delaySensor = 0;
    distanciaCM = (sonar.ping() / US_ROUNDTRIP_CM); // con la funcion ping obtengo la distancia y al dividirlo por US_ROUNDTRIP_CM lo paso a CM
    if(distanciaCM != 0){ // El LCD a veces tiene problemas para sensar y devuelve 0, aca filtro esos resultados
      distanceForShow = distanciaCM;
      printLCD(lcd, distanceForShow);

    }
  }
  delaySensor++;
}

void printLCD(LiquidCrystal_I2C LCD, int valor) {
  int i = 0;
  String distancia = "Distancia";  
  String cadena = (String)valor;
  LCD.clear();
  LCD.setCursor(0,0); 
  while(distancia[i]  != NULL){
    LCD.print(distancia[i]);
    i++;
  } 
  i = 0;
  LCD.setCursor(0,1); 
  while(cadena[i]  != NULL){
    LCD.print(cadena[i]);
    i++;
  }
  LCD.print(" ");
  LCD.print("C");
  LCD.print("M");
}

