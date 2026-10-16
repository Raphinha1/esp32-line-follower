#include <Arduino.h>

// Sensores
const int S0 = 26;
const int S1 = 27;
const int S2 = 14;
const int S3 = 12;
const int lectura = 13;
int valorSensores[16];

// Motores
const int PWMotorDer = 4;
const int DIRMotorDer = 17;
const int PWMotorIzq = 16;
const int DIRMotorIzq = 2;

// PID
float Kp = 1.2;     // Valor más agresivo
float Ki = 0.001;   // Pequeño valor integral
float Kd = 0.8;     // Añadir derivativo
int velBase = 128;  // Mayor velocidad base
float error = 0, errorP = 0, integral = 0;

// Calibración
int sensoresMin[16];
int sensoresMax[16];
bool calibrado = false;

TaskHandle_t TaskSensores;

void setup() {
  Serial.begin(115200);

  // Pines sensores
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(lectura, INPUT);

  // Pines motores
  pinMode(PWMotorDer, OUTPUT);
  pinMode(DIRMotorDer, OUTPUT);
  pinMode(PWMotorIzq, OUTPUT);
  pinMode(DIRMotorIzq, OUTPUT);

  // Crear tarea en núcleo 1 para lectura de sensores
  xTaskCreatePinnedToCore(
    tareaLeerSensores,
    "LeerSensores",        
    10000,                 
    NULL,                  
    1,                    
    &TaskSensores,         
    1                      
  );

  // Calibrar al inicio
  calibrarSensores();
}

void loop() {
  calcularError();
  PID();
  delay(5);
}

// Tarea para lectura de sensores en núcleo 1
void tareaLeerSensores(void *parameter) {
  const int delayEstabilizacion = 200; // microsegundos para estabilización
  while (true) {
    for (int i = 0; i < 16; i++) {
      seleccionarCanal(i);
      delayMicroseconds(delayEstabilizacion);
      valorSensores[i] = analogRead(lectura);
    }
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

void seleccionarCanal(int canal) {
  digitalWrite(S0, canal & 0x01);
  digitalWrite(S1, (canal >> 1) & 0x01);
  digitalWrite(S2, (canal >> 2) & 0x01);
  digitalWrite(S3, (canal >> 3) & 0x01);
}

void calibrarSensores() {
  for (int i = 0; i < 16; i++) {
    sensoresMin[i] = 4095;
    sensoresMax[i] = 0;
  }
  
  for (int n = 0; n < 100; n++) {
    for (int i = 0; i < 16; i++) {
      seleccionarCanal(i);
      delayMicroseconds(200);
      int val = analogRead(lectura);
      if (val < sensoresMin[i]) sensoresMin[i] = val;
      if (val > sensoresMax[i]) sensoresMax[i] = val;
    }
  }
  calibrado = true;
}

void calcularError() {
  if (!calibrado) calibrarSensores();
  
  long sumaPon = 0;
  int suma = 0;
  int sensoresActivos = 0;

  for (int i = 0; i < 16; i++) {
    // Normalizar valores usando calibración
    int valor = map(valorSensores[i], sensoresMin[i], sensoresMax[i], 0, 1000);
    valor = constrain(valor, 0, 1000);
    
    if (valor > 50) { // Umbral para considerar sensor activo
      sumaPon += (long)valor * (i * 1000 - 7500);
      suma += valor;
      sensoresActivos++;
    }
  }

  if (suma != 0 && sensoresActivos > 0) {
    error = sumaPon / suma;
  } else {
    // Si no se detecta línea, mantener última corrección o buscar línea
    error = errorP > 0 ? 10000 : -10000;
  }
}

void PID() {
  static unsigned long lastTime = 0;
  unsigned long now = millis();
  float deltaTime = (now - lastTime) / 1000.0; // En segundos
  lastTime = now;
  
  // Cálculo de términos PID
  integral += error * deltaTime;
  integral = constrain(integral, -100, 100); // Anti-windup
  float derivada = (error - errorP) / deltaTime;
  
  float corregir = Kp * error + Ki * integral + Kd * derivada;
  
  // Asignación de velocidades
  int velocidadDer = constrain(velBase + corregir, 0, 255);
  int velocidadIzq = constrain(velBase - corregir, 0, 255);
  
  // Dirección de los motores
  digitalWrite(DIRMotorDer, velocidadDer >= 0 ? HIGH : LOW);
  digitalWrite(DIRMotorIzq, velocidadIzq >= 0 ? HIGH : LOW);
  
  // Escritura PWM con analogWrite tradicional
  analogWrite(PWMotorDer, abs(velocidadDer));
  analogWrite(PWMotorIzq, abs(velocidadIzq));
  
  errorP = error;
}