#define ena 2
#define enb 3
#define in1 40
#define in2 41
#define in3 42
#define in4 43

#define L4 A0
#define L3 A1
#define L2 A2
#define L1 A3
#define R1 A4
#define R2 A5
#define R3 A6
#define R4 A7
#define minim 31

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
  Serial.begin(9600);
  pinMode(L4, INPUT);
  pinMode(L3, INPUT);
  pinMode(L2, INPUT);
  pinMode(L1, INPUT);
  pinMode(R1, INPUT);
  pinMode(R2, INPUT);
  pinMode(R3, INPUT);
  pinMode(R4, INPUT);
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
  // объедениям линии в массив
  int line[5] = {analogRead(A0), analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4)};
  //передаём линию и размер
  pos.setLine(*line, 5);
  //выводим ошибку
  pos.getPos();

  //правое колесо пид
  pidr.setLine(35, 0, 0, pos.robotFlag()); // 35
  //левое  колесо пид
  pidl.setLine(35, 0, 0, pos.robotFlag()); // 35

  //Уловие проезда волнистой и квадратной линии
  // 11000
  if ((line[0] >= 3 && line[0] < 600) && (line[1] >= 3 && line[1] < 600))
  {
    Serial.println("square right");

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(ena, 225);
    analogWrite(enb, 225);
  }
  // 00011
  else if ((line[4] >= 3 && line[4] < 600) && (line[3] >= 3 && line[3] < 600))
  {
    Serial.println("square left");

    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(ena, 225);
    analogWrite(enb, 225);
  }
  // 00111
  else if ((line[4] >= 3 && line[4] < 600) && (line[3] >= 3 && line[3] < 600) && (line[2] >= 3 && line[2] < 600))
  {
    Serial.println("square left");

    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(ena, 225);
    analogWrite(enb, 225);
  }
  // 11100

  else if ((line[0] >= 3 && line[0] < 600) && (line[1] >= 3 && line[1] < 600) && (line[2] >= 3 && line[2] < 600))
  {
    Serial.println("square right");

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(ena, 225);
    analogWrite(enb, 225);
  }
  // 01111
  else if ((line[4] >= 3 && line[4] < 600) && (line[3] >= 3 && line[3] < 600) && (line[2] >= 3 && line[2] < 600) && (line[1] >= 3 && line[1] < 600))
  {
    Serial.println("square left");

    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(ena, 225);
    analogWrite(enb, 225);
  }
  // 11110
  else if ((line[0] >= 3 && line[0] < 600) && (line[1] >= 3 && line[1] < 600) && (line[2] >= 3 && line[2] < 600) && (line[3] >= 3 && line[3] < 600))
  {
    Serial.println("square right");

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(ena, 225);
    analogWrite(enb, 225);
  }

  else
  {
    //Включаем моторы и передаём значение пид-регулятора
    motorSeT(120, 120, pidl.PIDoras(), pidr.PIDoras());
    Serial.println("Sanya lox");
  }
}