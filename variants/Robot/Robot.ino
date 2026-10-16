//Sensores
const int S0 = 26;
const int S1 = 27;
const int S2 = 14;
const int S3 = 12;
const int lectura = 13; 
int valorSensores[16];

//Motores
const int PWMotorDer = 17;
const int DIRMotorDer = 5;
const int PWMotorIzq = 16;
const int DIRMotorIzq = 4;

//Variables PID
float Kp = 2.5;
float Ki = 0.0;
float Kd = 5.0;
float error = 0, errorP = 0, integral = 0;

int velBase = 60;

void setup() {
  Serial.begin(115200);

  //Sensores
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  //Motores
  pinMode(DIRMotorDer, OUTPUT);
  pinMode(DIRMotorIzq, OUTPUT);

  //PWM
  pinMode(PWMotorDer, OUTPUT);
  pinMode(PWMotorIzq, OUTPUT);
}

void loop() {
  leerSensores();
  calcularError();
  PID();
  delay(3);
}

void leerSensores() {
  for (int i = 0; i < 16; i++) {
   seleccionarCanal(i);
   delayMicroseconds(5);
   int valor = analogRead(lectura);

   valorSensores[i] = (valor > 3500) ? 1 : 0;
 }
 Serial.print("Lecturas: ");
for (int i = 0; i < 16; i++) {
 Serial.print(valorSensores[i]);
 Serial.print(" ");
}
Serial.println();
delay(200);
}

void seleccionarCanal(int canal) {
  digitalWrite(S0, canal & 0x01);
  digitalWrite(S1, (canal >> 1) & 0x01);
  digitalWrite(S2, (canal >> 2) & 0x01);
  digitalWrite(S3, (canal >> 3) & 0x01);
}

void calcularError() {
  long sumaPon = 0;
  int suma = 0;

  for (int i = 0; i < 16; i++) {
    int valor = valorSensores[i];
    sumaPon += (long)valor * i * 1000;
    suma += valor;
  }

  if (suma > 0)
    error = (sumaPon / suma) - 6800;
  else
    error = errorP;
}

void PID() {
  float derivativa = error - errorP;
  integral += error;

  float corregir = Kp * error + Ki * integral + Kd * derivativa;

  int velocidadDer = constrain(velBase + corregir, 0, 70);
  int velocidadIzq = constrain(velBase - corregir, 0, 70);

  digitalWrite(DIRMotorDer, HIGH);
  digitalWrite(DIRMotorIzq, LOW);

  analogWrite(PWMotorDer, velocidadDer);
  analogWrite(PWMotorIzq, velocidadIzq);

  errorP = error;

}