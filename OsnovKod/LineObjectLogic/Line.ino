#define L4 A0
#define L3 A1
#define L2 A2
#define L1 A3
#define R1 A4
#define R2 A5
#define R3 A6
#define R4 A7

class Line_pos
{
private:
  int line[8];
  int blackWhite;

public:
  float robot_10pos()
  {
    int posSum = 0;
    int posMedian = 0;
    int signal_corrected;
    for (int i = 0; i < 8; i++)
    {
      if (line[i] > blackWhite)
      {
        signal_corrected = 1;
      }
      else
      {
        signal_corrected = 0;
      }
      posSum += signal_corrected;
      posMedian += signal_corrected;
    }
    return posMedian / posSum - 7;
  }
  void setLine(int line_data[8], int blackWhite_data)
  {
    line[8] = line_data[8];
    blackWhite = blackWhite_data;
  }
  void getPos()
  {
    Serial.println(robot_10pos());
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
  int prevErr = pos.robot_10pos();
  ;
  int err_arr[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

public:
  void setLine(int kp_data, int ki_data, int kd_data, int speed_data)
  {
    kp = kp_data;//10
    kd = kd_data;//5
    ki = ki_data;//5
    speed = speed_data;
  }
  int PDs()
  {

    error = pos.robot_10pos();
    PD=kp*error+kd*(error-prevErr);
    prevErr = error;
    return PD;
  }
  int PIDoras()
  {
    error = pos.robot_10pos();
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
// int lastline[8];
// int maxline[8] = {8, 8, 8, 8, 8, 7, 7, 10};
// int minline[8];
void loop()
{
  Line_pos pos;

  int line[8] = {map(analogRead(A0), 0, 1023, 0, 255),
                 map(analogRead(A1), 0, 1023, 0, 255),
                 map(analogRead(A2), 0, 1023, 0, 255),
                 map(analogRead(A3), 0, 1023, 0, 255),
                 map(analogRead(A4), 0, 1023, 0, 255),
                 map(analogRead(A5), 0, 1023, 0, 255),
                 map(analogRead(A6), 0, 1023, 0, 255),
                 map(analogRead(A7), 0, 1023, 0, 255)};



  // for (int i = 0; i < 8; i++)
  // {
  //   Serial.print(" ");
  //   Serial.print(line[i]);
  // }
  // Serial.println("Vivod");
  // for (int i = 0; i < 8; i++)
  // {
  //   maxline[i] = max(maxline[i], line[i]);
  // }
  // for (int i = 0; i < 8; i++)
  // {
  //   Serial.print(" ");
  //   Serial.print(maxline[i]);
  // }
  // Serial.println("Msx");
  // lastline[8] = line[8];
}