#include <Arduino.h>

static float deg_radian(float degree)
{
  return degree * M_PI / 180.0;
}

class Parts
{
public:
  int _pin = -1;
  bool _number_parts = 0;
};
Parts b1; // 0ピン
Parts b2; // 1ピン
Parts b3; // 2ピン
Parts b4; // 3ピン

const int last_pin = 16;

class Line
{
public:
  int _pin;
  int _val;
  int _deg;
  bool _caught;
};
Line angelLine[last_pin];

// i = 3;
// angelLine[3]
// angelLine[3]._pin angelLine[3].
// angelLine[i]._pin angelLine[i]._val

int line_sensorpin[last_pin] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

const int recieve_pin = 5;
const int line_border = 750;

void setup()
{
  Serial.begin(115200);

  b1._pin = 1;
  b2._pin = 2;
  b3._pin = 3;
  b4._pin = 4;

  pinMode(b1._pin, OUTPUT);
  pinMode(b2._pin, OUTPUT);
  pinMode(b3._pin, OUTPUT);
  pinMode(b4._pin, OUTPUT);
  pinMode(recieve_pin, INPUT);
}

void loop()
{
  for (int i = 0; i < last_pin; i++)
  {
    angelLine[i]._pin = line_sensorpin[i];
    angelLine[i]._deg = i * 30;
  }

  // マルチプレクサ行き
  for (int i = 0; i < last_pin; i++)
  {
    b1._number_parts = (angelLine[i]._pin >> 0) & 0b0001; // (i >> x):iを右に一つずらし、0以下のものを消去 &:かつ。一桁目が一致していたら0b0001ｓ(正)になる
    b2._number_parts = (angelLine[i]._pin >> 1) & 0b0001;
    b3._number_parts = (angelLine[i]._pin >> 2) & 0b0001;
    b4._number_parts = (angelLine[i]._pin >> 3) & 0b0001;

    digitalWrite(b1._pin, b1._number_parts);
    digitalWrite(b2._pin, b2._number_parts);
    digitalWrite(b3._pin, b3._number_parts);
    digitalWrite(b4._pin, b4._number_parts);

    delayMicroseconds(20);

    angelLine[angelLine[i]._pin]._val = analogRead(recieve_pin);

    Serial.print(angelLine[i]._val);
    Serial.print(", ");

    // // 送信側
    // uint16_t data = 346;
    // uint8_t d1 = uint8_t(data & 0b11111111);
    // uint8_t d2 = uint8_t((data >> 8) & 0b11111111);
    // Serial.write(d1);
    // Serial.write(d2);

    // Serial.println(String(data));

    // if (Serial.available())
    // {
    //   String a = Serial.readStringUntil('/n');
    //   uint16_t b = a.toInt();
    // }

    // // 受信側
    // if (Serial.available() >= 2)
    // {
    //   uint8_t a = Serial.read();
    //   uint8_t b = Serial.read();

    //   uint16_t fixed_data = (uint16_t(b) << 8) | uint16_t(a);
    // }
  }

  Serial.println("");

  // "abc"

  // 反応しているか否かを正負で表記
  for (int i = 0; i < last_pin; i++)
  {
    angelLine[i]._caught = 0;
    if (angelLine[angelLine[i]._pin]._val > line_border)
      angelLine[angelLine[i]._pin]._caught = 1;

    Serial.print(angelLine[i]._caught);
    Serial.print(", ");
  }
  Serial.println("");

  // ベクトル合成
  float vx = 0.0, vy = 0.0;
  for (int i = 0; i < last_pin; i++)
  {
    if (angelLine[angelLine[i]._pin]._caught == 1)
    {
      float w = angelLine[angelLine[i]._pin]._val;
      float ang = deg_radian(angelLine[angelLine[i]._pin]._deg);
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

  Serial.print("vx:");
  Serial.print(vx);
  Serial.print(" vy:");
  Serial.print(vy);
  Serial.print(" vr:");
  Serial.print(vr);
  // Serial.print(" unitx:"); Serial.print(unitx);
  // Serial.print(" unity:"); Serial.print(unity);

  float normalx = unity;
  float normaly = -unitx;

  Serial.print(" normalx:");
  Serial.print(normalx);
  Serial.print(" normaly:");
  Serial.print(normaly);

  Serial.println("");

  delay(500);
}