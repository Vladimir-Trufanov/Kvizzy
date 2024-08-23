#define G_PIN 3
#define R_PIN 5
#define B_PIN 7

void setup() 
{
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  // включит красно-жёлтый 
  // с насыщенностью 200 из 255
  // и максимальной яркостью
  setHSV(20, 200, 255);
}

void loop() 
{
}

// включить цвет в HSV, принимает 0-255 по всем параметрам
void setHSV(uint8_t h, uint8_t s, uint8_t v) 
{
  float r, g, b;
  byte _r, _g, _b;
  float H = (float)h / 255;
  float S = (float)s / 255;
  float V = (float)v / 255;
  int i = int(H * 6);
  float f = H * 6 - i;
  float p = V * (1 - S);
  float q = V * (1 - f * S);
  float t = V * (1 - (1 - f) * S);
  
  switch (i % 6) 
  {
    case 0: r = V, g = t, b = p; break;
    case 1: r = q, g = V, b = p; break;
    case 2: r = p, g = V, b = t; break;
    case 3: r = p, g = q, b = V; break;
    case 4: r = t, g = p, b = V; break;
    case 5: r = V, g = p, b = q; break;
  }
  
  _r = r * 255;
  _g = g * 255;
  _b = b * 255;
  
  // инверсия для общего анода
  analogWrite(R_PIN, 255 - _r);
  analogWrite(G_PIN, 255 - _g);
  analogWrite(B_PIN, 255 - _b);
}
