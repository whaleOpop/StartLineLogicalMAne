#include "TrackingCamDxlUart.h"

TrackingCamDxlUart trackingCam;
unsigned long previousMillis = 0;

#define ena 2
#define enb 3
#define in1 40
#define in2 41
#define in3 42
#define in4 43
#define STATE_FORWARD 0
#define STATE_RIGHT 1
#define STATE_LEFT 2
#define STATE_2BACKGR 3
#define STATE_1LEFTGR 4
#define STATE_1RIGHTGR 5
int state = STATE_FORWARD;
class Line_pos
{
private:
  int size;
  int *line;
  int *minline;
  int *maxline;
  bool stop;

public:
  //[Возвращает ошибку робота]
  int robotFlag()
  {
    int pos = 0;
    // 10000
    if (line[0] >= 3 && line[0] < 600)
      pos = 2;
    // 01000
    if (line[1] >= 3 && line[1] < 600)
      pos = 1;
    // 00100
    if (line[2] >= 3 && line[2] < 600)
      pos = 0;
    // 00010
    if (line[3] >= 3 && line[3] < 600)
      pos = -1;
    // 00001
    if (line[4] >= 3 && line[4] < 600)
      pos = -2;

    return pos;
  }

  //Выводит линию на которой находится робот
  void getLine()
  {
    for (int i = 0; i < 5; i++)
    {
      Serial.print(line[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
  //Линия робота,размер массива линии
  void setLine(int &line_data, int data_size)
  {
    size = data_size;
    line = &line_data;
  }
  void getPos()
  {
    Serial.println(robotFlag());
  }
};

class PID
{
private:
  int P;
  int I;
  int D;
  int kp;
  int kd;
  int ki;
  int PD;
  int speed;
  int error;
  Line_pos pos;
  int err_p = -1;
  int prevErr = pos.robotFlag();
  ;
  int err_arr[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

public:
  //Коефицент Kp , Ki , Kd , ошибка робота pos.robotFlag()
  void setLine(int kp_data, int ki_data, int kd_data, int errod_data)
  {
    kp = kp_data; // 10
    kd = kd_data; // 5
    ki = ki_data; // 5
    error = errod_data;
  }

  int PIDoras()
  {
    err_p = (err_p + 1) % 10;
    err_arr[err_p] = error;
    P = error * kp;
    D = (err_arr[err_p] - err_arr[(err_p + 11) % 10]) * kd;
    int err_sum = 0;
    for (int i = 0; i < 10; i++)
      err_sum += err_arr[i];
    I = err_sum / 10 * ki;
    prevErr = error;
    return P + I + D;
  }
};

void setup()
{
  trackingCam.init(52, 1, 115200, 30);
  Serial.begin(115200);
}

int minline[8];
int maxline[8];
Line_pos pos;
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

  boolean left = !digitalRead(A0);
  boolean right = !digitalRead(A4);

  int targetState;
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
        if (trackingCam.blob[i].cx >= 100 && trackingCam.blob[i].cx <= 140)
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

  switch (targetState)
  {
    //Включение пид регулятора
  case STATE_FORWARD:
    motorSeT(140, 140, pidl.PIDoras(), pidr.PIDoras());
     Serial.println("STATE_FORWARD");
    break;
    //поворот на право
  case STATE_RIGHT:

    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(ena, (255));
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enb, (200));
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
  case STATE_1LEFTGR:
    if (millis() - time_counterleft > 300)
    {
      time_counterleft = millis();
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      analogWrite(ena, 255);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      analogWrite(enb, (255));
      Serial.println("STATE_1LEFTGR");
    }
    break;
    //поворот на зелёном правом
  case STATE_1RIGHTGR:
    if (millis() - time_counterright > 300)
    {
      time_counterright = millis();
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      analogWrite(ena, (255));
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      analogWrite(enb, (200));
    }
    Serial.println("STATE_1RIGHTGR");
    break;
  case STATE_2BACKGR:
    if (millis() - time_counterstop > 600)
    {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      analogWrite(ena, (255));
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      analogWrite(enb, (255));
    }
    Serial.println("STATE_2BACKGR");
    break;
  }
  while (millis() - previousMillis < 33)
  {
  };
  previousMillis = millis();
}