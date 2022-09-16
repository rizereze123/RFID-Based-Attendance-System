 #include <Arduino.h>
 #include <ESP8266WiFi.h>
 #include <ESP8266HTTPClient.h>
 #include <WiFiClient.h>
 #include <LiquidCrystal_I2C.h>

 LiquidCrystal_I2C lcd(0x27,16,2);

#define buzzer D0
#define led_merah D7
#define led_hijau D8

const char* ssid     = "123";
const char* password = "admin123";

String 
host = "http://192.168.0.100",
device_token = "7d0cbb45ce7748858eb74b6c3efadd03";
//d058a38558054a83a1fbb7baaf330fc9
//4477f222e5bc4b55a01884619c13da0a
//7d0cbb45ce7748858eb74b6c3efadd03
void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  pinMode(buzzer, OUTPUT);
  pinMode(led_merah, OUTPUT);
  pinMode(led_hijau, OUTPUT);
  lcd.init();
  lcd.backlight();
  
  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  connectToWiFi();
}

void loop() {
  displaylcd();
  if (!WiFi.isConnected()) {
    connectToWiFi();
  }
  
  // wait for serial data available
  if (Serial.available()) {
    String cardUID = String(Serial.readString());
    sendHTTPGetMethod(cardUID);
  }

  delay(5000);
}

void connectToWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected");
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

void sendHTTPGetMethod(String cardUID) {
    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, host + "/absen/get?card_uid=" + String(cardUID) + "&device_token=" + String(device_token))) {  

      String x = host + "/absen/get?card_uid=" + String(cardUID) + "&device_token=" + String(device_token);
      Serial.println(x);
      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET status code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
          digitalWrite(buzzer, HIGH);
          digitalWrite(led_hijau, HIGH);
          lcd.setCursor(0,0);
          lcd.print("Data Tlah Sukses");
          lcd.setCursor(0,1);
          lcd.print("Berhasil Dikirim");
          delay(5000);
          lcd.clear();
          digitalWrite(buzzer, LOW);
          digitalWrite(led_hijau, LOW);
          delay(5000);
        }
      } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
          digitalWrite(buzzer, HIGH);
          digitalWrite(led_merah, HIGH);
          lcd.setCursor(0,0);
          lcd.print("Data Tidak Valid");
          lcd.setCursor(0,1);
          lcd.print("Gagal Diproses!!");
          delay(5000);
          lcd.clear();
          digitalWrite(buzzer, LOW);
          digitalWrite(led_merah, LOW);
          delay(5000);
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
}

void displaylcd(){
    lcd.setCursor(0,0);
    lcd.print("SLAMAT DATANG DI");
    lcd.setCursor(0,1);
    lcd.print("Sistem Absensi!!");
}
