#include "PID.h"
#include "Line.h"
// PIN L198n
#define ena 2
#define enb 3
#define in1 40
#define in2 41
#define in3 42
#define in4 43

void setup()
{
  Serial.begin(9600);
}
//инитилизируем обработку линии
Line pos;

//инитилизируем пид для правого колеса
PID pidr;

//инитилизируем пид для левого колеса
PID pidl;

//Ф-ция для установки значений пид ругулятора на моторы, уст Правого мотора, уст Левого мотора, pidl, pidr
void motorSeT(int rmotor, int lmotor, int pidl, int pidr)
{
  int speedl = lmotor + pidl;
  int speedr = rmotor - pidr;
  if (pos.robotFlag() == 0)
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(ena, (speedl));
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enb, (speedr));
  }
  if (pos.robotFlag() <= -1)
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(ena, (speedl));
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enb, (speedr));
  }

  if (pos.robotFlag() >= 1)
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    speedl = speedl;
    analogWrite(ena, (speedl));
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enb, (speedr));
  }
}

//ф-ция остановки моторов
void motorStop()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(ena, 0);
  analogWrite(enb, 0);
}

//Маркеры для задержках на millis();
unsigned long time_counterstop;
unsigned long time_counterleft;
unsigned long time_counterright;

void loop()
{

  int size;

  int line[5] = {analogRead(A0), analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4)};

  pos.setLine(*line, 5);

  //Передом значения в класс пид регулятор для правого колеса
  pidr.setLine(40, 0, 0, pos.robotFlag()); // 40

  //Передом значения в класс пид регулятор для левого колеса
  pidl.setLine(40, 0, 0, pos.robotFlag()); // 40

  //алогоритм прохождении линии "Квадрат"
  if ((line[4] >= 3 && line[4] < 600) && (line[3] >= 3 && line[3] < 600) || (line[2] >= 3 && line[2] < 600) || (line[1] >= 3 && line[1] < 600))
  {
    //Езда по условию квадрат
    Serial.println("square left");
    if (millis() - time_counterleft > 200)
    {
      time_counterleft = millis();
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      analogWrite(ena, 255);
      analogWrite(enb, 255);
    }
  }

  else if ((line[0] >= 3 && line[0] < 600) && (line[1] >= 3 && line[1] < 600) || (line[2] >= 3 && line[2] < 600) || (line[3] >= 3 && line[3] < 600))
  {
    //Езда по условию квадрат
    Serial.println("square right");
    if (millis() - time_counterright > 200)
    {
      time_counterright = millis();
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      analogWrite(ena, 255);
      analogWrite(enb, 255);
    }
  }
  else
  {
    //Езда по PIDу
    if (millis() - time_counterstop > 50)
    {
      time_counterstop = millis();
      motorSeT(125, 125, pidl.PIDr(), pidr.PIDr());
      Serial.println("Sanya lox");
    }
  }
}