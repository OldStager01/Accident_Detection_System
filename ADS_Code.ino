#include <SoftwareSerial.h>

SoftwareSerial Serial1(2, 3);  //make RX arduino line is pin 2, make TX arduino line is pin 3.

SoftwareSerial gps(10, 11);

#include <Adafruit_MPU6050.h>  //Accelerometer
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <LiquidCrystal.h>  //LCD Display

LiquidCrystal lcd(4, 5, 6, 7, 8, 9);  //Pins for LCD Display

Adafruit_ADXL345_Unified mpu = Adafruit_ADXL345_Unified();  // Accelerometer Object


/*int xsample = 0;

int ysample = 0;

int zsample = 0;

#define samples 10
#define minVal -7
#define MaxVal 7
*/

int xaxis = 0, yaxis = 0, zaxis = 0;
int deltx = 0, delty = 0, deltz = 0;
int magnitude = 0;
int sensitivity = 20;//Sensitivity for detection of Impact

String num = "1234567890"  //Enter Mobile Number WITHOUT Country Code



int i = 0, k = 0;

int gps_status = 0;

float latitude = 0;

float logitude = 0;

String Speed = "";

String gpsString = "";

char *test = "$GPRMC";


void initModule(String cmd, char *res, int t)

{

  while (1)

  {

    Serial.println(cmd);

    Serial1.println(cmd);

    delay(100);

    while (Serial1.available() > 0)

    {

      if (Serial1.find(res))

      {

        Serial.println(res);

        delay(t);

        return;

      }


      else

      {

        Serial.println("Error");
      }
    }

    delay(t);
  }
}

void setup()

{

  Serial1.begin(9600);

  Serial.begin(9600);

  lcd.begin(16, 2);

  lcd.print("Accident Alert  ");

  lcd.setCursor(0, 1);

  lcd.print("     System     ");

  delay(2000);

  lcd.clear();

  lcd.print("Initializing");

  lcd.setCursor(0, 1);

  lcd.print("Please Wait...");

  while (!mpu.begin()) {
    Serial.println("ADXL345 not connected!");
    delay(1000);
  }
  Serial.println("ADXL345 ready!");
  Serial.println("Initializing....");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }
  Serial.println("Accelerometer Initialized Successfully");

  delay(1000);

  lcd.clear();

  Serial.println("GSM Initializing....");

  initModule("AT", "OK", 1000);

  initModule("ATE1", "OK", 1000);

  initModule("AT+CPIN?", "READY", 1000);

  initModule("AT+CMGF=1", "OK", 1000);

  initModule("AT+CNMI=2,2,0,0,0", "OK", 1000);

  Serial.println("GSM Initialized Successfully");

  lcd.clear();

  lcd.print("Initialized");

  lcd.setCursor(0, 1);

  lcd.print("Successfully");

  delay(2000);

  lcd.clear();

  lcd.print("Callibrating ");

  lcd.setCursor(0, 1);

  lcd.print("Acceleromiter");

  sensors_event_t a;
  mpu.getEvent(&a);

  int xaxis = a.acceleration.x;

  int yaxis = a.acceleration.y;

  int zaxis = a.acceleration.z;


  Serial.println(xaxis);

  Serial.println(yaxis);

  Serial.println(zaxis);

  delay(1000);



  lcd.clear();

  lcd.print("Waiting For GPS");

  lcd.setCursor(0, 1);

  lcd.print("     Signal    ");

  delay(2000);

  gps.begin(9600);

  get_gps();

  show_coordinate();

  delay(2000);

  lcd.clear();

  lcd.print("GPS is Ready");

  delay(1000);

  lcd.clear();

  lcd.print("System Ready");

  Serial.println("System Ready..");
}


void loop()

{

  delay(200);
  sensors_event_t a;
  mpu.getEvent(&a);

  int oldx = xaxis; //store previous axis readings for comparison
  int oldy = yaxis;
  int oldz = zaxis;

  xaxis = a.acceleration.x;

  yaxis = a.acceleration.y;

  zaxis = a.acceleration.z;


  deltx = xaxis - oldx;                                           
  delty = yaxis - oldy;
  deltz = zaxis - oldz;

  magnitude = sqrt(sq(deltx) + sq(delty) + sq(deltz));

  Serial.print("x="); 

  Serial.println(deltx);

  Serial.print("y=");

  Serial.println(delty);

  Serial.print("z=");

  Serial.println(deltz);


  if (magnitude >= sensitivity)

  {

    get_gps();

    show_coordinate();

    lcd.clear();

    lcd.print("Sending SMS ");

    Serial.println("Sending SMS");

    Send();

    Serial.println("SMS Sent");

    delay(2000);

    lcd.clear();

    lcd.print("System Ready");
  }
  
}


void gpsEvent()

{

  gpsString = "";

  while (1)

  {

    while (gps.available() > 0)  //Serial incoming data from GPS

    {

      char inChar = (char)gps.read();

      gpsString += inChar;  //store incoming data from GPS to temparary string str[]

      i++;

      // Serial.print(inChar);

      if (i < 7)

      {

        if (gpsString[i - 1] != test[i - 1])  //check for right string

        {

          i = 0;

          gpsString = "";
        }
      }

      if (inChar == '\r')

      {

        if (i > 60)

        {

          gps_status = 1;

          break;

        }

        else

        {

          i = 0;
        }
      }
    }

    if (gps_status)

      break;
  }
}


void get_gps()

{

  lcd.clear();

  lcd.print("Getting GPS Data");

  lcd.setCursor(0, 1);

  lcd.print("Please Wait.....");

  gps_status = 0;

  int x = 0;

  while (gps_status == 0)

  {

    gpsEvent();

    int str_lenth = i;

    coordinate2dec();

    i = 0;
    x = 0;

    str_lenth = 0;
  }
}


void show_coordinate()

{

  lcd.clear();

  lcd.print("Lat:");

  lcd.print(latitude);

  lcd.setCursor(0, 1);

  lcd.print("Log:");

  lcd.print(logitude);

  Serial.print("Latitude:");

  Serial.println(latitude);

  Serial.print("Longitude:");

  Serial.println(logitude);

  Serial.print("Speed(in knots)=");

  Serial.println(Speed);

  delay(2000);

  lcd.clear();

  lcd.print("Speed(Knots):");

  lcd.setCursor(0, 1);

  lcd.print(Speed);
}


void coordinate2dec()

{

  String lat_degree = "";

  for (i = 20; i <= 21; i++)

    lat_degree += gpsString[i];



  String lat_minut = "";

  for (i = 22; i <= 28; i++)

    lat_minut += gpsString[i];


  String log_degree = "";

  for (i = 32; i <= 34; i++)

    log_degree += gpsString[i];


  String log_minut = "";

  for (i = 35; i <= 41; i++)

    log_minut += gpsString[i];



  Speed = "";

  for (i = 45; i < 48; i++)  //extract longitude from string

    Speed += gpsString[i];



  float minut = lat_minut.toFloat();

  minut = minut / 60;

  float degree = lat_degree.toFloat();

  latitude = degree + minut;



  minut = log_minut.toFloat();

  minut = minut / 60;

  degree = log_degree.toFloat();

  logitude = degree + minut;
}


void Send()

{

  Serial1.println("AT");

  delay(500);

  serialPrint();

  Serial1.println("AT+CMGF=1");

  delay(500);

  serialPrint();

  Serial1.print("AT+CMGS=");

  Serial1.print('"');

  Serial1.print(num);  //mobile no. for SMS alert

  Serial1.println('"');

  delay(500);

  serialPrint();

  Serial1.print("Latitude:");

  Serial1.println(latitude);

  delay(500);

  serialPrint();

  Serial1.print(" longitude:");

  Serial1.println(logitude);

  delay(500);

  serialPrint();

  Serial1.print(" Speed:");

  Serial1.print(Speed);

  Serial1.println("Knots");

  delay(500);

  serialPrint();

  Serial1.print("http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=");

  Serial1.print(latitude, 6);

  Serial1.print("+");  //28.612953, 77.231545   //28.612953,77.2293563

  Serial1.print(logitude, 6);

  Serial1.write(26);

  delay(2000);

  serialPrint();
}


void serialPrint()

{

  while (Serial1.available() > 0)

  {

    Serial.print(Serial1.read());
  }
}