#define ena 2
#define enb 3
#define in1 22
#define in2 23
#define in3 24
#define in4 25
int m1Speed;
int m2Speed;
float D;
float P;
float perr;
float err;
float PD;
float kp = 10;
float kd = 5;

Octoliner octoliner(42); //реализация класса датчиков
int dataSensors[8];//массмв датчик


void pid() {
  for (int i = 0; i < 8; i++) {
    dataSensors[i] = octoliner.analogRead(i);
  }
  err = octoliner.mapLine(dataSensors);
  PD = (kp * err) + ((err - perr) * kd); //расчитываем P и D регулятор
  m1Speed = 85 + (int)PD;
  m2Speed = 170 - (int)PD;
  Serial.println(m1Speed);
  ;
  Serial.println(m2Speed);

  perr = err;
}


void dvig() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(ena, m1Speed); //Управление скоростью моторов с помощью широтно импульсной модуляции
  analogWrite(enb, m2Speed);

}

void testdvig() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(ena, 255);
  analogWrite(enb, 255);
}

void vivod() {
  analogWrite(50, 255);
  int dataSensors[8];
  for (int i = 0; i < 8; i++) {
    dataSensors[i] = octoliner.analogRead(i);
    Serial.print(octoliner.analogRead(i));
    Serial.print("\t");
  }
  Serial.println(octoliner.mapLine(dataSensors));
  Serial.println();
}


void setup() {
  // открываем Serial-порт
  Serial.begin(115200);
  // начало работы с модулями I²C
  Wire.begin();
  // начало работы с датчиками линии
  octoliner.begin();
  // выставляем чувствительность фотоприёмников в диапазоне от 0 до 255
  octoliner.setSensitivity(300);
}


void loop() {
  //Serial.println(analogRead(d));
  //Serial.println(analogRead(c));
  //  Serial.println("kkekw");

  //pid();//вызов ф-ции пид реглятора
  //dvig();//вызов движков (передаём значение пида)
  //  vivod();
  //  testdvig();

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(ena, 255);
  analogWrite(enb, 255);

}
