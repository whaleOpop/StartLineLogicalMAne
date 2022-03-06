
#define L4 A0
#define L3 A1
#define L2 A2
#define L1 A3
#define R1 A4
#define R2 A5
#define R3 A6
#define R4 A7

int linemax[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int linemin[8];
int linelast[8];

class line {
  private: int linemax[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  
    int linemin[8];
    int linelast[8];
    
  public:
  
    void vivod() {

    }
    void getLine() {

    }
    void getMinLine() {

    }
    void getMaxLine() {

    }
};


void setup() {
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


void loop() {

  int line[8] = {map(analogRead(A0), 0, 1024, 0, 255)
                 , map(analogRead(A1), 0, 1024, 0, 255),
                 map(analogRead(A2), 0, 1024, 0, 255),
                 map(analogRead(A3), 0, 1024, 0, 255),
                 map(analogRead(A4), 0, 1024, 0, 255),
                 map(analogRead(A5), 0, 1024, 0, 255),
                 map(analogRead(A6), 0, 1024, 0, 255),
                 map(analogRead(A7), 0, 1024, 0, 255)
                };

  for (int i = 0; i < 8; i++) {

    Serial.print(" ");
    Serial.print(line[i]);

  }
  Serial.println();
  Serial.println("Вывод");


  for (int i = 0; i < 8; i++) {
    if (line[i] > linemax[i]) {
      linemax[i] = line[i];
    }
    if (line[i] < linelast[i]) {
      linemin[i] = line[i];

    }
  }
  for (int i = 0; i < 8; i++) {
    linelast[i] = line[i];
  }

  for (int i = 0; i < 8; i++) {
    Serial.print(" ");
    Serial.print(linemax[i]);

  }
  Serial.println();
  Serial.println("макс");


  for (int i = 0; i < 8; i++) {
    Serial.print(" ");
    Serial.print(linemin[i]);
  }
  Serial.println();
  Serial.println("мин");
}