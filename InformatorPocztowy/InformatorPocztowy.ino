#include <WiFi.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define uS_TO_S_FACTOR      1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP       1800       /* Time ESP32 will go to sleep (in seconds) */
#define LOWER_BOUND_VOLTAGE 1.4      /* Voltage value below which low battery info is sent */ 

// Set GPIOs for LED and PIR Motion Sensor
RTC_DATA_ATTR const int led = 2;
RTC_DATA_ATTR const gpio_num_t motionSensor = GPIO_NUM_27;
RTC_DATA_ATTR const int voltagePin = 36;

RTC_DATA_ATTR const char* ssid     = "***";
RTC_DATA_ATTR const char* password = "***";
RTC_DATA_ATTR const char* serverName = "***";
RTC_DATA_ATTR String apiKeyValue = "***";

void wifi_post(int lowBattery) {
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  WiFiClient client;
  HTTPClient http;
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP);
  
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String triggerTime;

  timeClient.begin();
  timeClient.setTimeOffset(3600);
  Serial.print("Trying to force a time client update");
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }

  triggerTime = timeClient.getFormattedTime();
  Serial.println(triggerTime);
  
  // Prepare your HTTP POST request data
  String httpRequestData = "api_key=" + apiKeyValue + "&trigger_time=" + triggerTime + "&low_battery=" + lowBattery + "";
  Serial.print("httpRequestData: ");
  Serial.println(httpRequestData);

  // Send HTTP POST request
  int httpResponseCode = http.POST(httpRequestData);
      
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
  WiFi.disconnect();
}

// Checks if motion was detected, sets LED HIGH and posts a message
void detects_movement() {
  digitalWrite(led, HIGH);
  wifi_post(0);
  delay(1000);
  digitalWrite(led, LOW);
}

// Checks the level of the voltage, sets the LED HIGH and posts a message, if needed
void check_voltage() {
  digitalWrite(led, HIGH);
   
  double nonConvertedVoltage = analogRead(voltagePin);
  double measuredVoltage = (nonConvertedVoltage / 4095.0 * 3.3);
  Serial.print("Analog read: ");
  Serial.println(nonConvertedVoltage);
  Serial.print("Measured voltage: ");
  Serial.println(measuredVoltage);

  if(measuredVoltage < LOWER_BOUND_VOLTAGE) {
   wifi_post(1);
  }
    
  delay(1000);
  digitalWrite(led, LOW);
}

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:
      Serial.println("Wakeup caused by external signal using RTC_IO.");
      detects_movement();
      check_voltage();
      break;
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("Wakeup caused by timer.");
      check_voltage();
      break;
    default:
      Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
      break;
  }
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  //It is necessary to declare the input and output pins
  pinMode(voltagePin, INPUT_PULLUP); 
  pinMode(motionSensor, INPUT);
  pinMode(led, OUTPUT);

  // Calling a function distributing the work of ESP32
  print_wakeup_reason();

  // Setting ESP32 to wake up for an external trigger.
  esp_sleep_enable_ext0_wakeup(motionSensor, 1); //1 = High, 0 = Low

  // Setting ESP32 to wake up for a timer trigger.
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " seconds.");

  Serial.println("Going to sleep now.");
  delay(1000);
  Serial.flush(); 
  esp_deep_sleep_start();
  Serial.println("This will never be printed.");
}

void loop() {
  
}