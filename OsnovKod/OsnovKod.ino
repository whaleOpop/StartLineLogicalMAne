#define in1 28
#define in2 29
#define in3 44
#define in4 45 
#define ena 2
#define enb 3
#define d A15
#define c A14 //скорость движка
#define a A13 //скорость движка  
int m1Speed;
int m2Speed;
float D;
float P;
float perr;
float err;
float PD;
float kp=10;
float kd=5;
#include <Wire.h>//библеотеки датчика линии
#include <Octoliner.h>//библеотеки датчика линии
Octoliner octoliner(42); //реализация класса датчиков
int dataSensors[8];//массмв датчик

  
void pid(){
   // массив для хранения показателей с датчиков линии
  int dataSensors[8];
  // считываем значение с датчиков линии
  for (int i = 0; i < 8; i++) {
    // выводим значения в Serial-порт
    dataSensors[i] = octoliner.analogRead(i);
    //Serial.print(octoliner.analogRead(i));
    //Serial.print("\t");
  }
  //Serial.println(octoliner.mapLine(dataSensors));
  //Serial.println();
  //переприсваем ошибку для прощего использования  возврашает ошибку с датчиков с помощью библеотеки для этих же датчиковoctoliner.mapLine(dataSensors);
err=octoliner.mapLine(dataSensors);
PD=(kp*err)+((err-perr)*kd);//расчитываем P и D регулятор 
m1Speed=85+(int)PD;
m2Speed=170-(int)PD;
Serial.print(m1Speed);
Serial.print("\t");
   Serial.print(m2Speed);
   Serial.print("\n");
   perr=err;
}


void dvig(){
  digitalWrite(in1, HIGH);
 digitalWrite(in2,LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4,LOW);
  analogWrite(ena, m1Speed); //Управление скоростью моторов с помощью широтно импульсной модуляции
  analogWrite(enb, m2Speed);
   
}

void testdvig(){
  digitalWrite(in1,LOW );
 digitalWrite(in2,HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4,LOW);

  //analogWrite(ena, (int)); //Управление скоростью моторов с помощью широтно импульсной модуляции
 // analogWrite(enb, (int)analogRead(a));
   
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
  octoliner.setSensitivity(300);
}


void loop() {
  //Serial.println(analogRead(d));
  //Serial.println(analogRead(c));
  //Serial.println(analogRead(a));
  
//pid();//вызов ф-ции пид реглятора
//dvig();//вызов движков (передаём значение пида)
//vivod();
testdvig();

}
