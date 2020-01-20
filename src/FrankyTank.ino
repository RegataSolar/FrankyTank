#include <Servo.h>


///////////////////
// CONFIGURACIÓN //
///////////////////
#define FILTRO_SERVO_PWM 994
#define OFFSET_SERVO 78
#define AMPLITUD_SERVO 40

///////////////////
// PINES MOTORES //
///////////////////
#define PIN_BRUSHLESS 11
#define PIN_SERVO 9
Servo motorBrushless;
Servo motorServo;

///////////////////
// PINES SEÑALES //
///////////////////
#define SENAL_VELOCIDAD 6
#define SENAL_GIRO 5

// VARIABLES MOVIMIENTO
int velocidad = 0;
int giro = 0;

void setup() {
  // Iniciar Serial
  Serial.begin(9600);

  // Iniciar Motores
  motorBrushless.attach(PIN_BRUSHLESS);
  motorServo.attach(PIN_SERVO);
  motorServo.write(OFFSET_SERVO);

  // Configurar ESC
  //motorBrushless.writeMicroseconds(2000);
  //delay(5000);
  motorBrushless.writeMicroseconds(1000);
  delay(5000);

  // Iniciar señales de entrada del mando
  pinMode(SENAL_VELOCIDAD, INPUT);
  pinMode(SENAL_GIRO, INPUT);


}

void loop() {

  leer_entradas();

  motorBrushless.writeMicroseconds(velocidad);
  motorServo.write(giro);
  /*
    Serial.print(velocidad);
    Serial.print("\t");
    Serial.print(giro);
    Serial.println();
  */

}

void leer_entradas() {
  int pulseServo = pulseIn(SENAL_GIRO, HIGH);
  int pulseBrushless = pulseIn(SENAL_VELOCIDAD, HIGH);
  velocidad = servo_a_pwm(pulseBrushless, 0, 972, 2000, 1000);
  velocidad = constrain(velocidad, 1000, 2000);
  if (abs(velocidad - 1500) < 50) {
    velocidad = 1500;
  }
  /*Serial.print(pulseBrushless);
  Serial.print("\t");
  Serial.print(velocidad);
  Serial.println();*/
  
  giro = OFFSET_SERVO - servo_a_pwm(pulseServo, 10, 1000, AMPLITUD_SERVO, -AMPLITUD_SERVO);
  giro = constrain(giro, OFFSET_SERVO - AMPLITUD_SERVO, OFFSET_SERVO + AMPLITUD_SERVO);
  if (abs(giro - OFFSET_SERVO) < 5) {
    giro = OFFSET_SERVO;
  }
  /*Serial.print(pulseServo);
  Serial.print("\t");
  Serial.print(pulseBrushless);
  Serial.println();*/

}

int servo_a_pwm(int pulso, int minI, int maxI, int minO, int maxO) {
  return map((pulso - FILTRO_SERVO_PWM), minI, maxI, minO, maxO);
}
