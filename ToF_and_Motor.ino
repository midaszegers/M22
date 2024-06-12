#define PIN_D18  12   // GPIO pin 18    LED 11
#define PIN_D22  15   // GPIO pin 21     

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

unsigned long currentMillis;
unsigned long previousMillisAPI = 0;
const long interval = 300000000; // Fetch data every 5 minutes
int weatherId = 801;
int temp;


#include <SparkFun_RFD77402_Arduino_Library.h> //Use Library Manager or download here: https://github.com/sparkfun/SparkFun_RFD77402_Arduino_Library
RFD77402 myDistance; //Hook object to the library

void setup(){
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  
  pinMode(PIN_D22, OUTPUT);
  pinMode(PIN_D18, OUTPUT);


  Serial.begin(9600);
  while (!Serial);
  Serial.println("RFD77402 Read Example");

  if (myDistance.begin() == false)
  {
    Serial.println("Sensor failed to initialize. Check wiring.");
    while (1); //Freeze!
  }
  Serial.println("Sensor online!");
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
    temp = obj["main"]["temp"].as<int>();
    weatherId = obj["weather"][0]["id"].as<int>();
//    itoa(temp, tempStr, 10);

    Serial.println(weatherId);

    http.end();
  } else {
    Serial.println("Error!");
  }
}



void loop(){

  currentMillis = micros();
  
  if (currentMillis - previousMillisAPI >= interval) {
    previousMillisAPI = micros();
    fetchWeatherData();
  }
  myDistance.takeMeasurement(); //Tell sensor to take measurement

  unsigned int distance = myDistance.getDistance(); //Retrieve the distance value
  if(distance<0){
    distance = 2000;
  }

  if (distance < 1000 || weatherId == 800 || weatherId < 700 || temp > 19){
    digitalWrite(PIN_D22,HIGH);
    Serial.println("MOTOR ON"); 
  } else{
    digitalWrite(PIN_D22,LOW);
}
}
