#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <Wire.h>
#include "thuvien.h"
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;

ESP8266WebServer webServer(80);
//==========AP info=======================//
char* ssid_ap = "E-SMART HOME";
char* pass_ap = "12345678";
IPAddress ip_ap(192,168,1,1);
IPAddress gateway_ap(192,168,1,1);
IPAddress subnet_ap(255,255,255,0);
String statusD1="1",statusD2="1",statusD3="1",statusD4="1";
String ssid;
String pass;


//=========================================//
void setup() {
  Serial.begin(9600);
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
 
  digitalWrite(D1,HIGH);
  digitalWrite(D2,HIGH);
  
  setupLCD();
  EEPROM.begin(512);       //Khởi tạo bộ nhớ EEPROM
  delay(10);
  
  if(read_EEPROM()){
    checkConnection();
  }else{
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(ip_ap, gateway_ap, subnet_ap);
    WiFi.softAP(ssid_ap,pass_ap,1,false);
    Serial.println("Soft Access Point mode!");
    Serial.print("Please connect to");
    Serial.println(ssid_ap);
    Serial.print("Web Server IP Address: ");
    Serial.println(ip_ap);
  }
  webServer.on("/",mainpage);
  webServer.on("/getSTATUSD",get_STATUSD);
  webServer.on("/D1on",D1_on);
  webServer.on("/D1off",D1_off);
  webServer.on("/D2on",D2_on);
  webServer.on("/D2off",D2_off);
  webServer.on("/D3on",D3_on);
  webServer.on("/D3off",D3_off);
  webServer.on("/D4on",D4_on);
  webServer.on("/D4off",D4_off);
  webServer.on("/Allon",All_on);
  webServer.on("/Alloff",All_off);
  webServer.on("/getIP",get_IP);
  webServer.on("/writeEEPROM",write_EEPROM);
  webServer.on("/restartESP",restart_ESP);
  webServer.on("/clearEEPROM",clear_EEPROM);
  webServer.begin();
}
void loop() {
  webServer.handleClient();
  lcd_rtc();
}
//==========Chương trình con=================//
void mainpage(){
  String s = FPSTR(MainPage);
  webServer.send(200,"text/html",s);
}
void get_STATUSD(){
  if(digitalRead(D1)==0){
    statusD1 = "0";
  }else{
    statusD1 = "1";
  }
  if(digitalRead(D2)==0){
    statusD2 = "0";
  }else{
    statusD2 = "1";
  }
  if(digitalRead(D3)==0){
    statusD3 = "0";
  }else{
    statusD3 = "1";
  }
  if(digitalRead(D4)==0){
    statusD4 = "0";
  }else{
    statusD4 = "1";
  }
  String s = "{\"D1\": \""+ statusD1 +"\"," +
              "\"D2\": \""+ statusD2 + "\"," +
              "\"D3\": \""+ statusD3 + "\"," +
              "\"D4\": \""+ statusD4 +"\"}";
  webServer.send(200,"application/json",s);
}
//--------------Điều khiển chân D1----------------
void D1_on(){
  digitalWrite(D1,LOW);
  get_STATUSD();
}
void D1_off(){
  digitalWrite(D1,HIGH);
  get_STATUSD();
}
//--------------Điều khiển chân D2----------------
void D2_on(){
  digitalWrite(D2,LOW);
  get_STATUSD();
}
void D2_off(){
  digitalWrite(D2,HIGH);
  get_STATUSD();
}
//--------------Điều khiển chân D3----------------
void D3_on(){
  digitalWrite(D3,LOW);
  get_STATUSD();
}
void D3_off(){
  digitalWrite(D3,HIGH);
  get_STATUSD();
}
//--------------Điều khiển chân D4----------------
void D4_on(){
  digitalWrite(D4,LOW);
  get_STATUSD();
}
void D4_off(){
  digitalWrite(D4,HIGH);
  get_STATUSD();
}
//--------------Điều khiển chân D----------------
void All_on(){
  //digitalWrite(D1,LOW);
  //digitalWrite(D2,LOW);
  //digitalWrite(D3,LOW);
  //digitalWrite(D4,LOW);
  get_STATUSD();
}
void All_off(){
  digitalWrite(D1,HIGH);
  digitalWrite(D2,HIGH);
  digitalWrite(D3,HIGH);
  digitalWrite(D4,HIGH);
  get_STATUSD();
}

void get_IP(){
  String s = WiFi.localIP().toString();
  webServer.send(200,"text/html", s);
}
boolean read_EEPROM(){
  Serial.println("Reading EEPROM...");
  if(EEPROM.read(0)!=0){
    ssid = "";
    pass = "";
    for (int i=0; i<32; ++i){
      ssid += char(EEPROM.read(i));
    }
    Serial.print("SSID: ");
    Serial.println(ssid);
    for (int i=32; i<96; ++i){
      pass += char(EEPROM.read(i));
    }
    Serial.print("PASSWORD: ");
    Serial.println(pass);
    ssid = ssid.c_str();
    pass = pass.c_str();
    return true;
  }else{
    Serial.println("Data wifi not found!");
    return false;
  }
}
//---------------SETUP WIFI------------------------------
void checkConnection() {
  Serial.println();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  Serial.print("Check connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid,pass);
  int count=0;
  while(count < 50){
    if(WiFi.status() == WL_CONNECTED){
      Serial.println();
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("Web Server IP Address: ");
      Serial.println(WiFi.localIP());
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print(WiFi.localIP());
      return;
    }
    delay(200);
    Serial.print(".");
    count++;
  }
  Serial.println("Timed out.");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip_ap, gateway_ap, subnet_ap);
  WiFi.softAP(ssid_ap,pass_ap,1,false);
  Serial.println("Soft Access Point mode!");
  Serial.print("Please connect to");
  Serial.println(ssid_ap);
  Serial.print("Web Server IP Address: ");
  Serial.println(ip_ap);
}
void write_EEPROM(){
  ssid = webServer.arg("ssid");
  pass = webServer.arg("pass");
  Serial.println("Clear EEPROM!");
  for (int i = 0; i < 96; ++i) {
    EEPROM.write(i, 0);           
    delay(10);
  }
  for (int i = 0; i < ssid.length(); ++i) {
    EEPROM.write(i, ssid[i]);
  }
  for (int i = 0; i < pass.length(); ++i) {
    EEPROM.write(32 + i, pass[i]);
  }
  EEPROM.commit();
  Serial.println("Writed to EEPROM!");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("PASS: ");
  Serial.println(pass);
  String s = "Wifi configuration saved!";
  webServer.send(200, "text/html", s);
}
void restart_ESP(){
  ESP.restart();
}
void clear_EEPROM(){
  Serial.println("Clear EEPROM!");
  for (int i = 0; i < 512; ++i) {
    EEPROM.write(i, 0);           
    delay(10);
  }
  EEPROM.commit();
  String s = "Device has been reset!";
  webServer.send(200,"text/html", s);
}
void setupLCD() {
  Wire.begin(2,0);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
  lcd.print(" Hello My Boss "); // Start Printing
  if(!rtc.begin()){
  lcd.print("Couldn't find RTC :(");
  while(1);
  }
  if (! rtc.isrunning())
  {
    lcd.print("RTC is NOT running!"); 
  }
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__))); //auto update time from computer time
}
void lcd_rtc() {
  DateTime now = rtc.now();
  lcd.setCursor(4,1);
  if(now.hour() <=9)
  {
    lcd.print("0");
    lcd.print(now.hour());
  }
  else{
    lcd.print(now.hour());
    }
  lcd.print(':');
  if(now.minute() <=9) {
    lcd.print("0");
    lcd.print(now.minute());
    } else {
      lcd.print(now.minute());  
    }
   lcd.print(':');
   if(now.second() <=9) {
    lcd.print("0");
    lcd.print(now.second());
    } else {
      lcd.print(now.second());  
    }
  }
