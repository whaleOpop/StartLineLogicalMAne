#define in1 25
#define in2 24
#define in3 26
#define in4 27 
#define ena 9
#define enb 8
int m1Speed;
int m2Speed;
float D;
float P;
float perr;
float err;
float PD;
float kp=0;
float kd=0;
#include <Wire.h>//библеотеки датчика линии
#include <Octoliner.h>//библеотеки датчика линии
Octoliner octoliner(42); //реализация класса датчиков
int dataSensors[8];//массмв датчик

  
void pid(){
err=octoliner.mapLine(dataSensors);//переприсваем ошибку для прощего использования
PD=(kp*err)+((err-perr)*kd);//расчитываем P и D регулятор 
m1Speed=255-(int)PD;
m2Speed=255-(int)PD;
}


void dvig(){
  digitalWrite(in1, LOW);
 digitalWrite(in2,HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4,LOW);
  analogWrite(ena, m1Speed); //Управление скоростью моторов с помощью широтно импульсной модуляции
  analogWrite(enb, m2Speed);
}


void vivod(){
 analogWrite(50, 255);
  // массив для хранения показателей с датчиков линии
  int dataSensors[8];
  // считываем значение с датчиков линии
  for (int i = 0; i < 8; i++) {
    // выводим значения в Serial-порт
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
  octoliner.setSensitivity(255);
}


void loop() {
pid();//вызов ф-ции пид реглятора
dvig();//вызов движков (передаём значение пида)
}
