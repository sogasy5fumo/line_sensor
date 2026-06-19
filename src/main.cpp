#include <Arduino.h>

// void size_3(int number){
//   if(number < 100)
//   {
//
//   }
// }

static float deg_radian(float degree)
{
  return degree * M_PI / 180.0;
}

class Parts
{
public:
  int pin = -1;
  bool number_parts = 0;
};
Parts b1; // 0ピン
Parts b2; // 1ピン
Parts b3; // 2ピン
Parts b4; // 3ピン

const int last_pin = 16;

class Line
{
public:
  int pin;
  int val;
  int deg;
  bool caught;
};
Line angelLine[last_pin];

int Line_sensorpin[last_pin] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

const int recieve_pin = 5;
const int Line_border = 400;

void setup()
{
  Serial.begin(115200);

  b1.pin = 1;
  b2.pin = 2;
  b3.pin = 3;
  b4.pin = 4;

  pinMode(b1.pin, OUTPUT);
  pinMode(b2.pin, OUTPUT);
  pinMode(b3.pin, OUTPUT);
  pinMode(b4.pin, OUTPUT);
  pinMode(recieve_pin, INPUT);
}

void loop()
{
  for (int i = 0; i < last_pin; i++)
  {
    angelLine[i].pin = Line_sensorpin[i];
    angelLine[i].deg = i * 30;
  }

  // マルチプレクサ行き
  for (int i = 0; i < last_pin; i++)
  {
    b1.number_parts = (angelLine[i].pin >> 0) & 0b0001; // (i >> x):iを右に一つずらし、0以下のものを消去 &:かつ。一桁目が一致していたら0b0001ｓ(正)になる
    b2.number_parts = (angelLine[i].pin >> 1) & 0b0001;
    b3.number_parts = (angelLine[i].pin >> 2) & 0b0001;
    b4.number_parts = (angelLine[i].pin >> 3) & 0b0001;

    digitalWrite(b1.pin, b1.number_parts);
    digitalWrite(b2.pin, b2.number_parts);
    digitalWrite(b3.pin, b3.number_parts);
    digitalWrite(b4.pin, b4.number_parts);

    angelLine[angelLine[i].pin].val = analogRead(recieve_pin);

    Serial.print(angelLine[i].val);
    Serial.print(", ");
  }

  Serial.println("");

  // 反応しているか否かを正負で表記
  for (int i = 0; i < last_pin; i++)
  {
    angelLine[i].caught = 0;
    if (angelLine[angelLine[i].pin].val > Line_border)
      angelLine[angelLine[i].pin].caught = 1;

    Serial.print(angelLine[i].caught);
    Serial.print(", ");
  }
  Serial.println("");

  // ベクトル合成
  float vx = 0.0, vy = 0.0;
  for (int i = 0; i < last_pin; i++)
  {
    if (angelLine[angelLine[i].pin].caught == 1)
    {
      float w = angelLine[angelLine[i].pin].val;
      float ang = deg_radian(angelLine[angelLine[i].pin].deg);
      vx += w * cos(ang);
      vy += w * sin(ang);
    }
  }
  float vr = sqrt(vx * vx + vy * vy); // r成分

  float unitx = 0.0, unity = 0.0;
  if (vr > 0)
  {
    unitx = vx / vr; // 単位ベクトル変換
    unity = vy / vr;
  }

  delay(10);
}