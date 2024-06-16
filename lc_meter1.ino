//LCD config
#include   <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int inp = 10, outp = 13;
const int lcd_L = 6, lcd_Back = 7,   flag_inp = 8;

int deger = 0;
int btn_dly = 0;
int btn_sta = 0;
int   btn_Flag = 0; //Buton durum kaydedicisi.
int btn_Light = 0;
double pulse,   frequency, capacitance, inductance;

bool islem = false;

void setup()   {

  lcd.begin(16, 2);
  Serial.begin(115200);

  pinMode(lcd_Back,   OUTPUT);
  pinMode(flag_inp, INPUT_PULLUP);
  pinMode(lcd_L, INPUT);
  pinMode(inp,   INPUT);
  pinMode(outp, OUTPUT);

  Serial.println("Why hello!");
//  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("OZOX Elc. 2020");
   lcd.setCursor(0, 1);
  lcd.print("Mode:Inductance");
  delay(1000);
}

void   loop() {
  btn_sta = digitalRead(flag_inp);
  while (btn_sta == 0)
  {
     delay(100);
    btn_dly++;
    btn_sta = digitalRead(flag_inp);
    islem   = true;
  }

  if (islem == true)
  {
    if (btn_dly > 0 && btn_dly   <= 10)
    {
      btn_Flag = 0;
      btn_dly = 0;
    }
    else   if (btn_dly > 10 && btn_dly <= 20)
    {
      btn_Flag = 1;
      btn_dly   = 0;
    }
    else if (btn_dly > 20 && btn_dly <= 30)
    {
      btn_Flag   = 2;
      btn_dly = 0;
    }
    else
    {
      btn_dly = 0;
       islem = false;
    }
  }
  if (btn_Flag == 0)
  {
    induc();
   }
  else if (btn_Flag == 1)
  {
    capasit();
  }
  else if (btn_Flag   == 2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("-Osman   SEN-");
    lcd.setCursor(0, 1);
    lcd.print("Tel:05338250164");
     delay(100);
  }

  if (digitalRead(lcd_L) == HIGH)
  {
    digitalWrite(lcd_Back,   HIGH);
  }
  else
  {
    digitalWrite(lcd_Back, LOW);
  }
}

void   induc() {
  digitalWrite(outp, HIGH);
//  delayMicroseconds(100);
  delay(5);//give some time to charge   inductor.
  digitalWrite(outp, LOW);
  delayMicroseconds(100);
  pulse = pulseIn(inp, HIGH, 5000); //returns 0 if timeout
   if (pulse > 0.1) { //if a timeout did not occur and it took a reading:
    capacitance = 1.E-6; // - insert value here
    frequency = 1.E6 / (2 * pulse);
    inductance = 1. / (capacitance * frequency * frequency * 4.*3.14159 * 3.14159); //one of my profs told me just do squares like this
    inductance *= 1E6; //note that this is the same as saying inductance = inductance*1E6
     //Serial print
    Serial.print("High for uS:");
    Serial.print( pulse );
    Serial.print("\	frequency Hz:");
    Serial.print( frequency );
    Serial.print("\	inductance uH:");
    Serial.println( inductance );
     delay(20);
    //LCD print
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mode:Inductance");

    lcd.setCursor(0, 1);
    lcd.print("Ind   :");
    if (inductance <= 999) {
      lcd.print(inductance);
      lcd.setCursor(14,   1);
      lcd.print("uH");
    }
    else {
      lcd.print(1.E-3 * inductance);
      lcd.setCursor(14, 1);

      lcd.print("mH");
     }
    delay(20);
  }
}

void capasit() {
  digitalWrite(outp,   HIGH);
  delay(5);//give some time to charge inductor.
  digitalWrite(outp,   LOW);
  delayMicroseconds(100); //make sure resination is measured
  pulse   = pulseIn(inp, HIGH, 5000); //returns 0 if timeout
  if (pulse > 0.1) { //if   a timeout did not occur and it took a reading:

    inductance = 100.; //   - insert value here

    frequency = 1.E6 / (2 * pulse);
    capacitance   = 2. / (inductance * frequency * frequency * 4.*3.14159 * 3.14159); //one of my   profs told me just do squares like this
    capacitance  *= 1E15; //note that   this is the same as saying inductance = inductance*1E6
    //Serial print
     Serial.print("High for uS:");
    Serial.print( pulse );
    Serial.print("\	frequency   Hz:");
    Serial.print( frequency );
    Serial.print("\	Capacitance nF:");
     Serial.println( capacitance );
    delay(20);
    //LCD print
    lcd.clear();
     lcd.setCursor(0, 0);
    lcd.print("Mode:Capacitance");
    lcd.setCursor(0,   1);
    lcd.print("Cap :");
    if (capacitance <= 980) {
      lcd.print(capacitance);
       lcd.setCursor(14, 1);
      lcd.print("nF");
    }
    else {
       lcd.print(capacitance * 1.E-3);
      lcd.setCursor(14, 1);

      lcd.print("uF");
     }
    delay(20);
  }
}
