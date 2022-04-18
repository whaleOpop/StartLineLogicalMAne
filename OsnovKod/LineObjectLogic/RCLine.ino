
// PIN L198n
#define ena 2
#define enb 3
#define in1 40
#define in2 41
#define in3 42
#define in4 43


class Line
{
private:
  int size;
  int *line;

public:
  //[Возвращает ошибку робота]
  int robotFlag()
  {
    int pos = 0;

    if (line[0] >= 3 && line[0] < 600)
      pos = 2;
    if (line[1] >= 3 && line[1] < 600)
      pos = 1;
    if (line[2] >= 3 && line[2] < 600)
      pos = 0;
    if (line[3] >= 3 && line[3] < 600)
      pos = -1;
    if (line[4] >= 3 && line[4] < 600)
      pos = -2;
    Serial.println("flagPosition " + pos);
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
  Line pos;
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

  int PIDr()
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
      analogWrite(ena, 225);
      analogWrite(enb, 225);
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
      analogWrite(ena, 225);
      analogWrite(enb, 225);
    }
  }
  else
  {
    //Езда по PIDу
    if (millis() - time_counterstop > 50)
    {
      time_counterstop = millis();
      motorSeT(150, 150, pidl.PIDr(), pidr.PIDr());
      Serial.println("Sanya lox");
    }
  }
}