///////////////////   TOF    ///////////////////
#include "Adafruit_VL53L1X.h"

#define IRQ_PIN 4
#define XSHUT_PIN 0

Adafruit_VL53L1X vl53 = Adafruit_VL53L1X(XSHUT_PIN, IRQ_PIN);

///////////////////   WIFI   ///////////////////

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

char ssid[] = "iotroam";       // your network SSID (name)
char password[] = "midas123";  // your network key

//URL Endpoint for the API
String URL = "http://api.openweathermap.org/data/2.5/weather?";
String ApiKey = "736b45d041f9ca797b0ce29a2f219f64";

//String origin = "51.4392064,5.4951936";
String lat = "51.4392064";
String lon = "5.4951936";

unsigned long previousMillisAPI = 0;
unsigned long previousMillisTOF = 0;
//const long interval = 30000000; // Fetch data every 5 minutes
const long interval = 300000000; 

  int16_t distance;


//////////////////////////////////////

#define PIN_D5   19    // GPIO pin 1    LED 1     (all of these 11 are the LEDs)
#define PIN_D8   23    // GPIO pin 3    LED 2  
#define PIN_D12  32   // GPIO pin 12    LED 3
#define PIN_D13  33   // GPIO pin 13    LED 4  
//#define PIN_D10  22   // GPIO pin 10    LED 5
//#define PIN_D11  21   // GPIO pin 11    LED 6
#define PIN_D10  18   // GPIO pin 10    LED 5  
#define PIN_D11  5   // GPIO pin 11    LED 6  
#define PIN_D14  25   // GPIO pin 14    LED 7 
#define PIN_D15  26   // GPIO pin 15    LED 8
#define PIN_D16  27   // GPIO pin 16    LED 9
#define PIN_D17  14   // GPIO pin 17    LED 10
#define PIN_D18  12   // GPIO pin 18    LED 11

#define PIN_D22  15   // GPIO pin 21               (hall effect sensor)

int text_ok = 0;

int a[] = {32, 546, 260, 112, 136, 1675, 136, 112, 260, 546, 32};                                         //zonnetje open
//int a[] = {1024, 512, 256, 128, 64, 32, 16, 8, 4, 31, 2047};
int b[] = {96, 145, 146, 404, 529, 1042, 1044, 1169, 786, 276, 272, 144, 240};                            //Regenbui
int c[] = {32, 546, 260, 112, 254, 1785, 249, 113, 289, 577, 65, 65, 33, 49, 17, 25, 15};                 //Halfbewolkt
int d[] = {96, 149, 146, 405, 530, 1045, 1042, 1173, 786, 277, 274, 149, 240};                            //Mist
int e[] = {96, 145, 146, 404, 529, 1042, 1044, 1168, 789, 274, 277, 144, 240};                            //Natte sneeuw
int f[] = {64, 80, 84, 84, 84, 84, 84, 20, 468, 596, 724, 532, 48};                                       //Wind
int g[] = {48, 72, 72, 200, 264, 520, 520, 524, 274, 146, 178, 66, 130, 130, 146, 242, 98, 34, 34, 18};   //Bewolkt
int h[] = {96, 144, 144, 384, 520, 1050, 1068, 1032, 768, 272, 272, 144, 240};                            //Bliksem
int i[] = {32, 546, 260, 112, 248, 1787, 248, 112, 260, 546, 32};                                         //Dicht zonnetje

//int a[] = {126, 144, 144, 144, 126};
//int b[] = {254, 146, 146, 146, 108};
//int c[] = {254, 130, 130, 130, 130};
//int d[] = {254, 130, 130, 130, 124};
//int e[] = {254, 146, 146, 146, 146};
//int f[] = {254, 144, 144, 144, 128};
//int g[] = {124, 130, 138, 138, 76};
//int h[] = {254, 16, 16, 16, 254};
//int i[] = {130, 250, 130};

//int j[] = {12, 2, 2, 2, 252};
//int k[] = {254, 16, 40, 68, 130};
//int l[] = {254, 2, 2, 2, 2};
//int m[] = {254, 64, 32, 64, 254};
//int n[] = {254, 32, 16, 8, 254};
//int o[] = {124, 130, 130, 130, 124};
//int p[] = {254, 136, 136, 136, 112};
//int q[] = {124, 130, 138, 134, 126};
//int r[] = {254, 144, 152, 148, 98};
//int s[] = {100, 146, 146, 146, 76};
//int t[] = {128, 128, 254, 128, 128};
//int u[] = {252, 2, 2, 2, 252};
//int v[] = {248, 4, 2, 4, 248};
//int w[] = {254, 4, 8, 4, 254};
//int x[] = {198, 40, 16, 40, 198};
//int y[] = {224, 16, 14, 16, 224};
//int z[] = {134, 138, 146, 162, 194};

int one[]     = {0, 65, 255, 1, 0};
int two[]     = {67, 133, 137, 145, 97};
int three[]   = {66, 129, 145, 145, 110};
int four[]    = {24, 40, 72, 255, 8};
int five[]    = {242, 145, 145, 145, 142};
int six[]     = {62, 81, 145, 145, 14};
int seven[]   = {128, 131, 140, 176, 192};
int eight[]   = {110, 145, 145, 145, 110};
int nine[]    = {112, 137, 137, 138, 124};
int zero[]    = {126, 129, 129, 129, 126};
int celcius[] = {126, 129, 129, 129, 129};
int deg[]     = {0, 96, 144, 144, 96};

int weatherId = 801;
const char* currenticon = "a";
char tempStr[11]; // Array to store the temperature as a string
//String tempStr;
float delayTime = 1;
//int16_t distance;



//POV clock cariables
unsigned long currentMillis, elapsed_loop_counter, previousMillis;
unsigned long counter_1, current_count;

//Interruption varaibles to count rotation speed
//We create 4 variables to store the previous value of the input signal (if LOW or HIGH)
byte last_IN_state;               //Here we store the previous state on digital pin 13
float one_rot_time = 0;           //Here we store the full rotation time
float time_per_deg = 0;           //Here we store the time it takes to make one degree rotation

void IRAM_ATTR interruptHandler();

void setup() {

 
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

   HTTPClient http;

  // Set HTTP Request Final URL with Location and API key information
  http.begin(URL + "lat=" + lat + "&lon=" + lon + "&units=metric&appid=" + ApiKey);

  // Start connection and send HTTP Request
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {

    // Read Data as a JSON string
    String JSON_Data = http.getString();
    Serial.println(JSON_Data);

    // Retrieve some information about the weather from the JSON format
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, JSON_Data);
    JsonObject obj = doc.as<JsonObject>();
    
    // Display the Current Weather Info
    const int temp = obj["main"]["temp"].as<int>();
    const int weatherId = obj["weather"][0]["id"].as<int>();
    itoa(temp, tempStr, 10);
    Serial.println(tempStr);

    Serial.println(weatherId);

    if (weatherId > 199 && weatherId < 231) { //bliksem
      currenticon = "h";
      Serial.println("Bliksem");
    }

    if (weatherId > 299 && weatherId < 322) { //regen
      currenticon = "b";
      Serial.println("Regen");
    }

    if (weatherId > 499 && weatherId < 505) { //regen
      currenticon = "b";
      Serial.println("Regen");
    }

    if (weatherId == 511) { //sneeuw
      currenticon = "e";
      Serial.println("Sneeuw");
    }

    if (weatherId > 515 && weatherId < 532) { //regen
      currenticon = "b";
      Serial.println("Regen");
    }

    if (weatherId > 599 && weatherId < 623) { //sneeuw
      currenticon = "e";
      Serial.println("Sneeuw");
    }

    if (weatherId > 700 && weatherId < 782) { //mist
      currenticon = "f";
      Serial.println("Mist");
    }

    if (weatherId == 800) { //zon
      currenticon = "a";
      Serial.println("Zon");
    }

    if (weatherId > 800 && weatherId < 803) { //halfbewolkt
      currenticon = "c";
      Serial.println("Halfbewolkt");
    }

    if (weatherId > 802 && weatherId < 805) { //bewolkt
      currenticon = "g";
      Serial.println("Bewolkt");
    }
  doc.clear(); 

    Serial.println(currenticon);
  } else {
    Serial.println("Error!");
  }
  http.end();

  // Set pin D22 as input
  pinMode(PIN_D22, INPUT);

  // Attach an interrupt to pin D22
  attachInterrupt(digitalPinToInterrupt(PIN_D22), interruptHandler, CHANGE);

  // Set GPIO LED pins as outputs
  pinMode(PIN_D5, OUTPUT);
  pinMode(PIN_D8, OUTPUT);
  pinMode(PIN_D12, OUTPUT);
  pinMode(PIN_D13, OUTPUT);
  pinMode(PIN_D10, OUTPUT);
  pinMode(PIN_D11, OUTPUT);
  pinMode(PIN_D14, OUTPUT);
  pinMode(PIN_D15, OUTPUT);
  pinMode(PIN_D16, OUTPUT);
  pinMode(PIN_D17, OUTPUT);
  pinMode(PIN_D18, OUTPUT);

//  digitalWrite(PIN_D5, HIGH);
//  delay(150);
//  digitalWrite(PIN_D5, LOW);
//  
//  digitalWrite(PIN_D8, HIGH);
//  delay(150);
//  digitalWrite(PIN_D8, LOW);
//
//  digitalWrite(PIN_D12, HIGH);
//  delay(150);
//  digitalWrite(PIN_D12, LOW);
//
//  digitalWrite(PIN_D13, HIGH);
//  delay(150);
//  digitalWrite(PIN_D13, LOW);
//
//  digitalWrite(PIN_D10, HIGH);
//  delay(150);
//  digitalWrite(PIN_D10, LOW);
//
//  digitalWrite(PIN_D11, HIGH);
//  delay(150);
//  digitalWrite(PIN_D11, LOW);
//
//  digitalWrite(PIN_D14, HIGH);
//  delay(150);
//  digitalWrite(PIN_D14, LOW);
//
//  digitalWrite(PIN_D15, HIGH);
//  delay(150);
//  digitalWrite(PIN_D15, LOW);
//
//  digitalWrite(PIN_D16, HIGH);
//  delay(150);
//  digitalWrite(PIN_D16, LOW);
//
//  digitalWrite(PIN_D17, HIGH);
//  delay(150);
//  digitalWrite(PIN_D17, LOW);
//
//  digitalWrite(PIN_D18, HIGH);
//  delay(150);
//  digitalWrite(PIN_D18, LOW);



//  ///////////////////   TOF    ///////////////////
//  Serial.println(F("Adafruit VL53L1X sensor demo"));
//
//  Wire.begin();
//  if (! vl53.begin(0x29, &Wire)) {
//    Serial.print(F("Error on init of VL sensor: "));
//    Serial.println(vl53.vl_status);
//    while (1)       delay(10);
//  }
//  Serial.println(F("VL53L1X sensor OK!"));
//
//  Serial.print(F("Sensor ID: 0x"));
//  Serial.println(vl53.sensorID(), HEX);
//
//  if (! vl53.startRanging()) {
//    Serial.print(F("Couldn't start ranging: "));
//    Serial.println(vl53.vl_status);
//    while (1)       delay(10);
//  }
//  Serial.println(F("Ranging started"));
//
//  // Valid timing budgets: 15, 20, 33, 50, 100, 200 and 500ms!
//  vl53.setTimingBudget(50);
//  Serial.print(F("Timing budget (ms): "));
//  Serial.println(vl53.getTimingBudget());
//
}


void draw_a_line(int this_line) {
  int now_line;
  now_line = this_line;
  if (now_line >= 1024) {
    digitalWrite(PIN_D18, HIGH);
    now_line -= 1024;
  } else {
    digitalWrite(PIN_D18, LOW);
  }
  if (now_line >= 512)  {
    digitalWrite(PIN_D17, HIGH);
    now_line -= 512;
  } else {
    digitalWrite(PIN_D17, LOW);
  }
  if (now_line >= 256)  {
    digitalWrite(PIN_D16, HIGH);
    now_line -= 256;
  } else {
    digitalWrite(PIN_D16, LOW);
  }
  if (now_line >= 128)  {
    digitalWrite(PIN_D15, HIGH);
    now_line -= 128;
  } else {
    digitalWrite(PIN_D15, LOW);
  }
  if (now_line >= 64)   {
    digitalWrite(PIN_D14, HIGH);
    now_line -= 64;
  } else {
    digitalWrite(PIN_D14, LOW);
  }
  if (now_line >= 32)   {
    digitalWrite(PIN_D11, HIGH);
    now_line -= 32;
  } else {
    digitalWrite(PIN_D11, LOW);
  }
  if (now_line >= 16)   {
    digitalWrite(PIN_D10, HIGH);
    now_line -= 16;
  } else {
    digitalWrite(PIN_D10, LOW);
  }
  if (now_line >= 8)    {
    digitalWrite(PIN_D13, HIGH);
    now_line -= 8;
  } else {
    digitalWrite(PIN_D13, LOW);
  }
  if (now_line >= 4)    {
    digitalWrite(PIN_D12, HIGH);
    now_line -= 4;
  } else {
    digitalWrite(PIN_D12, LOW);
  }
  if (now_line >= 2)    {
    digitalWrite(PIN_D8, HIGH);
    now_line -= 2;
  } else {
    digitalWrite(PIN_D8, LOW);
  }
  if (now_line >= 1)    {
    digitalWrite(PIN_D5, HIGH);
    now_line -= 1;
  } else {
    digitalWrite(PIN_D5, LOW);
  }
}






void displayChar(char cr, float line_delay) {
  if (cr == 'a') {
    for (int i = 0; i < 11; i++) {
      draw_a_line(a[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == 'b') {
    for (int i = 0; i < 13; i++) {
      draw_a_line(b[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == 'c') {
    for (int i = 0; i < 17; i++) {
      draw_a_line(c[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == 'd') {
    for (int i = 0; i < 13; i++) {
      draw_a_line(d[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == 'e') {
    for (int i = 0; i < 13; i++) {
      draw_a_line(e[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == 'f') {
    for (int i = 0; i < 13; i++) {
      draw_a_line(f[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == 'g') {
    for (int i = 0; i < 20; i++) {
      draw_a_line(g[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == 'h') {
    for (int i = 0; i < 13; i++) {
      draw_a_line(h[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == 'i') {
    for (int itr = 0; itr < 11; itr++) {
      draw_a_line(i[itr]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
//  if (cr == 'j') {
//    for (int i = 0; i < 5; i++) {
//      draw_a_line(j[i]);
//      delayMicroseconds(line_delay);
//    } draw_a_line(0);
//  }
//  if (cr == 'k') {
//    for (int i = 0; i < 5; i++) {
//      draw_a_line(k[i]);
//      delayMicroseconds(line_delay);
//    } draw_a_line(0);
//  }
//  if (cr == 'l') {
//    for (int i = 0; i < 5; i++) {
//      draw_a_line(l[i]);
//      delayMicroseconds(line_delay);
//    } draw_a_line(0);
//  }
//  if (cr == 'm') {
//    for (int i = 0; i < 5; i++) {
//      draw_a_line(m[i]);
//      delayMicroseconds(line_delay);
//    } draw_a_line(0);
//  }
//  if (cr == 'n') {
//    for (int i = 0; i < 5; i++) {
//      draw_a_line(n[i]);
//      delayMicroseconds(line_delay);
//    } draw_a_line(0);
//  }
//  if (cr == 'o') {
//    for (int i = 0; i < 5; i++) {
//      draw_a_line(o[i]);
//      delayMicroseconds(line_delay);
//    } draw_a_line(0);
//  }
//  if (cr == 'p') {
//    for (int i = 0; i < 5; i++) {
//      draw_a_line(p[i]);
//      delayMicroseconds(line_delay);
//    } draw_a_line(0);
//  }
//  if (cr == 'q') {
//    for (int i = 0; i < 5; i++) {
//      draw_a_line(q[i]);
//      delayMicroseconds(line_delay);
//    } draw_a_line(0);
//  }
//  if (cr == 'r') {
//    for (int i = 0; i < 5; i++) {
//      draw_a_line(r[i]);
//      delayMicroseconds(line_delay);
//    } draw_a_line(0);
//  }
//  if (cr == 's') {
//    for (int i = 0; i < 5; i++) {
//      draw_a_line(s[i]);
//      delayMicroseconds(line_delay);
//    } draw_a_line(0);
//  }
//  if (cr == 't') {
//    for (int i = 0; i < 5; i++) {
//      draw_a_line(t[i]);
//      delayMicroseconds(line_delay);
//    } draw_a_line(0);
//  }
//  if (cr == 'u') {
//    for (int i = 0; i < 5; i++) {
//      draw_a_line(u[i]);
//      delayMicroseconds(line_delay);
//    } draw_a_line(0);
//  }
//  if (cr == 'v') {
//    for (int i = 0; i < 5; i++) {
//      draw_a_line(v[i]);
//      delayMicroseconds(line_delay);
//    } draw_a_line(0);
//  }
//  if (cr == 'w') {
//    for (int i = 0; i < 5; i++) {
//      draw_a_line(w[i]);
//      delayMicroseconds(line_delay);
//    } draw_a_line(0);
//  }
//  if (cr == 'x') {
//    for (int i = 0; i < 5; i++) {
//      draw_a_line(x[i]);
//      delayMicroseconds(line_delay);
//    } draw_a_line(0);
//  }
//  if (cr == 'y') {
//    for (int i = 0; i < 5; i++) {
//      draw_a_line(y[i]);
//      delayMicroseconds(line_delay);
//    } draw_a_line(0);
//  }
//  if (cr == 'z') {
//    for (int i = 0; i < 5; i++) {
//      draw_a_line(z[i]);
//      delayMicroseconds(line_delay);
//    } draw_a_line(0);
//  }
  if (cr == '1') {
    for (int i = 0; i < 5; i++) {
      draw_a_line(one[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == '2') {
    for (int i = 0; i < 5; i++) {
      draw_a_line(two[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == '3') {
    for (int i = 0; i < 5; i++) {
      draw_a_line(three[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == '4') {
    for (int i = 0; i < 5; i++) {
      draw_a_line(four[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == '5') {
    for (int i = 0; i < 5; i++) {
      draw_a_line(five[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == '6') {
    for (int i = 0; i < 5; i++) {
      draw_a_line(six[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == '7') {
    for (int i = 0; i < 5; i++) {
      draw_a_line(seven[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == '8') {
    for (int i = 0; i < 5; i++) {
      draw_a_line(eight[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == '9') {
    for (int i = 0; i < 5; i++) {
      draw_a_line(nine[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == '0') {
    for (int i = 0; i < 5; i++) {
      draw_a_line(zero[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == '%') {
    for (int i = 0; i < 5; i++) {
      draw_a_line(celcius[i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  if (cr == '*') {
    for (int i = 0; i < 5; i++) {
      draw_a_line(deg [i]);
      delayMicroseconds(line_delay);
    } draw_a_line(0);
  }
  delayMicroseconds(line_delay * 2);
}




void displayString(const char* s, float line_delay) {
  for (int i = 0; i <= strlen(s); i++) {
    displayChar(s[i], line_delay);
  }
}



void fetchWeatherData() {
   HTTPClient http;

  // Set HTTP Request Final URL with Location and API key information
  http.begin(URL + "lat=" + lat + "&lon=" + lon + "&units=metric&appid=" + ApiKey);

  // Start connection and send HTTP Request
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {

    // Read Data as a JSON string
    String JSON_Data = http.getString();
    Serial.println(JSON_Data);

    // Retrieve some information about the weather from the JSON format
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, JSON_Data);
    JsonObject obj = doc.as<JsonObject>();
    
    // Display the Current Weather Info
    const int temp = obj["main"]["temp"].as<int>();
    const int weatherId = obj["weather"][0]["id"].as<int>();
    itoa(temp, tempStr, 10);
    Serial.println(tempStr);

    Serial.println(weatherId);

    if (weatherId > 199 && weatherId < 231) { //bliksem
      currenticon = "h";
      Serial.println("Bliksem");
    }

    if (weatherId > 299 && weatherId < 322) { //regen
      currenticon = "b";
      Serial.println("Regen");
    }

    if (weatherId > 499 && weatherId < 505) { //regen
      currenticon = "b";
      Serial.println("Regen");
    }

    if (weatherId == 511) { //sneeuw
      currenticon = "e";
      Serial.println("Sneeuw");
    }

    if (weatherId > 515 && weatherId < 532) { //regen
      currenticon = "b";
      Serial.println("Regen");
    }

    if (weatherId > 599 && weatherId < 623) { //sneeuw
      currenticon = "e";
      Serial.println("Sneeuw");
    }

    if (weatherId > 700 && weatherId < 782) { //mist
      currenticon = "f";
      Serial.println("Mist");
    }

    if (weatherId == 800) { //zon
      currenticon = "a";
      Serial.println("Zon");
    }

    if (weatherId > 800 && weatherId < 803) { //halfbewolkt
      currenticon = "c";
      Serial.println("Halfbewolkt");
    }

    if (weatherId > 802 && weatherId < 805) { //bewolkt
      currenticon = "g";
      Serial.println("Bewolkt");
    }

    Serial.println(currenticon);
      doc.clear(); 

  } else {
    Serial.println("Error!");
  }
  http.end();
}



void POVdisplay() {
  elapsed_loop_counter = currentMillis - previousMillis;
  delayTime = time_per_deg * 3.5; //we want 2 degrees for each line of the letters

  //This if here is to make sure I'll start printing at 216 deg so the text will be centered.
  if ((elapsed_loop_counter >=  time_per_deg * (180)) && (elapsed_loop_counter <  time_per_deg * (217)) &&  text_ok) {
    char displayText[10]; // Define a character array to hold the formatted string
    sprintf(displayText, "%s%s*%%", currenticon, tempStr);
    displayString(displayText, delayTime);
    text_ok = 0;
  }
}

//void TOF() {
//  
//  if (vl53.dataReady()) {
//    // new measurement for the taking!
//    distance = vl53.distance();
//    
//    if (distance == -1) {
//      distance =2000;
//    }
//    Serial.print(F("Distance: "));
//    Serial.print(distance);
//    Serial.println(" mm");
//
//    // data is read out, time for another reading!
//    vl53.clearInterrupt();
//  }
//}

void loop() {
  currentMillis = micros();
    // Execute TOF() function for 1 second every 3 seconds
//    if ((currentMillis - previousMillisTOF >= 3000000) && (currentMillis - previousMillisTOF < 3500000)) {
//        // Start TOF sensor readings
//        TOF();
//    }
//    
//    // If the TOF reading time is up, reset the timer
//    if (currentMillis - previousMillisTOF >= 4000000) {
//        previousMillisTOF = currentMillis;
//    }
//

//  if (distance < 1000) {
    POVdisplay();
//  }

  if (currentMillis - previousMillisAPI >= interval) {
    previousMillisAPI = micros();
    fetchWeatherData();
  }
} 









// Change ISR declaration to use GPIO pin interrupts
void IRAM_ATTR interruptHandler() {
  // First, we take the current count value in microseconds using the micros() function
  current_count = micros();

  ///////////////////////////////////////Channel 1
  // We check if pin D22 (GPIO 13) is HIGH
  if(digitalRead(PIN_D22)){                              
    // If the last state was 0, then we have a state change
    if(last_IN_state == 0){                         
      // Store the current state into the last state for the next loop
      last_IN_state = 1;
      // Set counter_1 to current value
      counter_1 = current_count;
    }
  } else if(last_IN_state == 1){  
//    Serial.println("LOOP!");                     
    // If pin D22 (GPIO 13) is LOW and the last state was HIGH, then we have a state change
    // Store the current state into the last state for the next loop
    last_IN_state = 0;                              
    // Calculate the time difference. Channel 1 is current_time - timer_1.
    one_rot_time = current_count - counter_1;   
    // Calculate the time it takes to make one degree rotation
    time_per_deg = one_rot_time / 360.0;
    // Record the current micros for timing purposes
    previousMillis = micros();
    // Set text_ok flag to 1 to indicate that text can be displayed
    text_ok = 1;
  }
}
