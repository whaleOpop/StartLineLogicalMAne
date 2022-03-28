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

    if (line[0] >= 3 && line[0] < 600)
      pos = -2;
    if (line[1] >= 3 && line[1] < 600)
      pos = -1;
    if (line[2] >= 3 && line[2] < 600)
      pos = 0;
    if (line[3] >= 3 && line[3] < 600)
      pos = 1;
    if (line[4] >= 3 && line[4] < 600)
      pos = 2;

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

  float PDs()
  {
    error = pos.robotFlag();
    PD = kp * error + kd * (error - prevErr);
    prevErr = error;
    return PD;
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
//Левый мотор Ф-ция пида для левого колеса
void motorSeTL(int lmotor, int pid)
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(ena, (lmotor - pid));
  
}
//Правый мотор Ф-ция пида для левого колеса
void motorSeTR(int rmotor, int pid)
{
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enb, (rmotor + pid));
}
//Левый мотор, Правый мотор
void motorTest(int lmotor, int rmotor)
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(ena, lmotor);
  analogWrite(enb, rmotor);
}
void motorStop()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(ena, 0);
  analogWrite(enb, 0);
}

void loop()
{
  int size;

  int line[5] = {analogRead(A0), analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4)};

  pos.setLine(*line, 5);
  pos.getPos();
  //правое колесо пид
  pidr.setLine(50, 0, 0, pos.robotFlag());
  //левое  колесо пид
  pidl.setLine(50, 0, 0, pos.robotFlag());
  Serial.println(pos.robotFlag());
  // Serial.print("PID ");
  // Serial.println(pid.PIDoras());
    motorSeTL(150, pidr.PIDoras());

    motorSeTR(150, pidr.PIDoras());
  // motorTest(1500,155);
}