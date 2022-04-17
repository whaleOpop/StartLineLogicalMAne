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
