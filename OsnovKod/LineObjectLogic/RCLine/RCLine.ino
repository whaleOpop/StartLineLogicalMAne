#include "TrackingCamDxlUart.h"
#include "PID.h"
#include "Line.h"
TrackingCamDxlUart trackingCam;
unsigned long previousMillis = 0;
#define switcher 4
#define ena 2
#define enb 3
#define in1 40
#define in2 41
#define in3 42
#define in4 43
//флаг вперед
#define STATE_FORWARD 0
//флаг направо
#define STATE_RIGHT 1
//флаг лево
#define STATE_LEFT 2
//флаг назад
#define STATE_2BACKGR 3
//флаг лево
#define STATE_1LEFTGR 4
//флаг право
#define STATE_1RIGHTGR 5
//ставим флаг по умолчанию прямо
int state = STATE_FORWARD;

unsigned long time_counterunick;
int locker;
void delay_two(int n, int key)
{
  if (millis() - time_counterunick > n && key != 0)
  {
    locker = 1;
    time_counterunick = millis();
  }
  if (key == 2)
  {

    locker = 0;
  }
}
void setup()
{
  pinMode(switcher, INPUT);
  trackingCam.init(52, 1, 115200, 30);
  Serial.begin(115200);
}

int minline[8];
int maxline[8];
Line pos;
PID pidr;
PID pidl;
//Установливай значение пид-регулятора на моторы
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
//Левый мотор, Правый мотор

void motorStop()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(ena, 0);
  analogWrite(enb, 0);
}

void motorLeft()
{
}
//Флаги
unsigned long time_counterstop;
unsigned long time_counterleft;
unsigned long time_counterright;

void loop()
{
  uint8_t n = trackingCam.readBlobs(2);

  // объедениям линии в массив
  int line[5] = {analogRead(A0), analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4)};
  //передаём линию и размер
  pos.setLine(*line, 5);
  //выводим ошибку
  // pos.getPos();

  //правое колесо пид
  pidr.setLine(35, 0, 0, pos.robotFlag()); // 35
  //левое  колесо пид
  pidl.setLine(35, 0, 0, pos.robotFlag()); // 35

  //Уловие проезда волнистой и квадратной линии
  // 11000

  //Включаем моторы и передаём значение пид-регулятора
  unsigned long timestart;
  boolean left = !digitalRead(A0);
  boolean right = !digitalRead(A4);
  Serial.println(pos.robotFlag());
  int targetState;
  //обработка камеры
  if (n != 0)
  {
    for (int i = 0; i < n; i++)
    {
      if (n == 2)
      {
        targetState = STATE_2BACKGR;
      }
      else if (n = 1)
      {
        if (trackingCam.blob[i].cx >= 80 && trackingCam.blob[i].cx <= 160)
        {
          targetState = STATE_1RIGHTGR;
        }

        else
        {
          targetState = STATE_1LEFTGR;
        }
      }
    }
  }
  else if (left == right)
  {
    // под сенсорами всё белое или всё чёрное
    // едем вперёд
    targetState = STATE_FORWARD;
  }
  else if (left)
  {
    // левый сенсор упёрся в трек
    // поворачиваем налево
    targetState = STATE_LEFT;
  }
  else
  {
    targetState = STATE_RIGHT;
  }

  // if (state == STATE_FORWARD && targetState != STATE_FORWARD)
  // {
  //   int brakeTime = (currentSpeed > SLOW_SPEED) ? currentSpeed : 0;
  //   stepBack(brakeTime, targetState);
  // }

  uint32_t now = millis();
  switch (targetState)
  {
  case STATE_1RIGHTGR:
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(ena, (250));
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enb, (250));
    if (millis() - time_counterright > 1000)
    {
      time_counterright = millis();
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      analogWrite(ena, (250));
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      analogWrite(enb, (250));
      Serial.println("STATE_1RIGHTGR");
    }

    break;
    //поворот на зелёном правом
  case STATE_1LEFTGR:
    //
    if (millis() - time_counterleft > 500)
    {
      time_counterleft = millis();
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      analogWrite(ena, (250));
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      analogWrite(enb, (250));
    }

    Serial.println("STATE_1LEFTGR");
    break;

  case STATE_2BACKGR:
    while (millis() - now < 1400)
    {

      Serial.println("Sanya lox");
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      analogWrite(ena, (255));
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      analogWrite(enb, (255));
    }
    Serial.println("STATE_2BACKGR");
    break;
    //Включение пид регулятора
  case STATE_FORWARD:

    motorSeT(140, 140, pidl.PIDoras(), pidr.PIDoras());
    Serial.println("PID");
    break;
    //поворот на правоo
  case STATE_RIGHT:
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(ena, (255));
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enb, (255));
    Serial.println("STATE_RIGHT");
    break;
    //поворот на лево
  case STATE_LEFT:
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(ena, 255);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enb, (255));
    Serial.println("STATE_LEFT");
    break;
    //поворот на зелёном левом
  }
  while (millis() - previousMillis < 33)
  {
  };
  previousMillis = millis();
  now = 0;
}