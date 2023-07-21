#include <HTTPClient.h>
#include <UrlEncode.h>
#include <ESP32Servo.h>
#include <analogWrite.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <ArduinoJson.h>
#include <AsyncEventSource.h>
#include <AsyncJson.h>
#include <AsyncWebSocket.h>
#include <AsyncWebSynchronization.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include <StringArray.h>
#include <WebAuthentication.h>
#include <WebHandlerImpl.h>
#include <WebResponseImpl.h>
#include <WiFi.h>
#include <AsyncTCP.h>
Adafruit_MPU6050 mpu;
#define DHTTYPE DHT11  
#define SOUND_SPEED 0.034
#define RX2 16
#define TX2 17
DynamicJsonDocument doc(1024);
Servo servo;
const char* ssid = "******";
const char* password = "********";
bool ledState = 0;
const int ledPin = 2;
int echo=35,trig=32,ENA=13,IN1=12,IN2=14,IN3=27,IN4=26,ser = 25,state=5,sc=70,ok=0,dc=150;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Security Video Car</title>
</head>
<body>
<script src = 'https://cdnjs.cloudflare.com/ajax/libs/p5.js/1.6.0/p5.js'></script>
  <center>
  <img src='http://172.20.10.4:81/stream'> 
<script>
let json = {x:0,y:0,h:0,t1:0,hic:0};
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; 
  }
 window.addEventListener("keydown", event => {
  if(event.keyCode == 87 || event.keyCode == 38) websocket.send('_');
  else if(event.keyCode == 83 || event.keyCode == 40) websocket.send('__');
  else if(event.keyCode == 65 || event.keyCode == 37) websocket.send('___');
  else if(event.keyCode == 68 || event.keyCode == 39) websocket.send('____');
  else if(event.keyCode == 69) websocket.send('______');
  else if(event.keyCode == 81) websocket.send('_______');
  else if(event.keyCode == 50) websocket.send('________');
  else if(event.keyCode == 49) websocket.send('_________');
});
window.addEventListener("keyup", event => {
  websocket.send('stopp');
});
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    json = JSON.parse(event.data);
  }
  function onLoad(event) {
    initWebSocket();
  }
  let f;
function preload(){
  f = loadFont('https://cdn.glitch.global/b21b1333-51e8-4909-8b43-21bd51920910/Inconsolata-Black.ttf?v=1679779177987');
}
var cnv;
let button,button1,button2,lfc,rmc;
  function setup(){
  cnv = createCanvas(600, 400,WEBGL);
  cnv.position(0,0);
  button = createButton('Setari Camera');
  button.position(493,0);
  button.mousePressed(setare);
  button1 = createButton('Urmarire Linie : OPRIT');
  button1.style("background-color",color(255,0,0,75)); 
  button2 = createButton('Modul Freeroaming : OPRIT');
  button2.style("background-color",color(255,0,0,75));
  lfc = 0;
  rmc = 0;
  button1.position(448,height-22);
  button1.mousePressed(lf);
  button2.position(418,height-44);
  button2.mousePressed(rm);
    textFont(f,20);
  textAlign(LEFT);
  }
  function rm(){
    if(rmc == 0){
          websocket.send("___________");
    button2.html("Modul Freeroaming : PORNIT");
    button2.position(410,height-44);
    button2.style("background-color",color(0,255,0,75)); 
    rmc = 1;
    }else{
      websocket.send("___________");
        button2.html("Modul Freeroaming : OPRIT");
    button2.position(418,height-44);
    button2.style("background-color",color(255,0,0,75)); 
    rmc = 0;
    }
  }
  function lf(){
    if(lfc == 0){
    websocket.send("__________");
    button1.html("Urmarire Linie : PORNIT");
    button1.position(440,height-22);
    button1.style("background-color",color(0,255,0,75)); 
    lfc = 1;
    }else{
      websocket.send("__________");
    button1.html("Urmarire Linie : OPRIT");
    button1.position(448,height-22);
    button1.style("background-color",color(255,0,0,75)); 
    lfc = 0;
    }
  }
  let iframe;
  function setare(){
      if(iframe == undefined){
    iframe = document.createElement("iframe");
  iframe.id = "i1";
  iframe.src = 'http://172.20.10.4';
  iframe.style="position: absolute;top: 0px;right: 0px;";
  iframe.width="600"; iframe.height = "400"; iframe.frameborder = "0";
  document.body.appendChild(iframe);
  }else{
    document.getElementById("i1").remove();
    iframe = undefined;
  }
  }
let ry,rx;
json.x=-10,json.y=-10;
function draw(){
  background(220);
  fill(0);
  text('Rotatie x:' + Number.parseFloat(json.rx).toFixed(4) + 'm/s^2', -width/2,-180);
  text('Rotatie y:' + Number.parseFloat(json.ry).toFixed(4) + 'm/s^2', -width/2,-160);
  text('Rotatie z:'  + Number.parseFloat(json.rz).toFixed(4) + 'm/s^2', -width/2,-140);
  text('Temperatura:'+Number.parseFloat(json.t).toFixed(4)+'C', -width/2,-120);
  text('Temperatura DHT11:'+Number.parseFloat(json.t1).toFixed(2)+'C', -width/2,-100);
  text('Umiditate:'+Number.parseFloat(json.h).toFixed(2)+'%', -width/2,-80);
  text('Index caldura:'+Number.parseFloat(json.hic).toFixed(2)+'C', -width/2,-60);
  text('Distanta 1:'+Number.parseFloat(json.us1).toFixed(2)+'cm', -width/2,-40); 
  text('Tensiune Acumulatori:'+Number.parseFloat(json.v).toFixed(2)+'V', -width/2,-20); 
  push();
  if(json.co > 2250){
  push();
  fill(255,0,0);
  text('CO DETECTAT!!:'+json.co+'ppm', -width/2,0);   
  pop();
  }else{
    text('Senzor CO:'+json.co+'ppm', -width/2,0);   
  }
  if(json.foc == 0){
    push();
    fill(255,0,0);
    text('PERICOL FOC DETECTAT!!!',-width/2,60); 
    pop();
  }
  if(json.pir == 1){
    push();
    fill(255,0,0);
    text('Miscare detectata!', -width/2,80);  
    pop();
  }
  if(json.g == 0){
    fill(255,0,0);
    textSize(28);
    text('PERICOL SCURGERE DE GAZ!!!',-width/2,100); 
  }
  rotateX((json.x/10));
  rotateZ((json.y/10)*-1);
  noFill();
  box(130,80,190);
  pop();
}
let ok;
setInterval(() => {
  if(json.foc == 0){
    ok = 1;
    if(ok == 1){
       fetch("https://api.callmebot.com/whatsapp.php?phone=40770684334&text=PERICOL+FOC+DETECTAT!!!&apikey=4769196")
       ok = 0;
    }
  }
    if(json.g == 0){
      ok=1;
          if(ok == 1){
       fetch("https://api.callmebot.com/whatsapp.php?phone=40770684334&text=PERICOL+GAZ+DETECTAT!!!&apikey=4769196")
       ok = 0;
    }
  }
},250);
</script>
<center>
</body>
</html>
)rawliteral";
bool lf = false,rm = false;
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if(len == 1) state = 1;
  else if(len == 2) state = 2;
  else if(len == 3) state = 3;
  else if(len == 4) state = 4;
  else if(len == 5) state = 5;
  else if(len == 7) {if(sc <=180) {sc++; ok = 1;}
  }else if(len == 6) {if(sc >=0){sc--; ok = 1;}
  }else if(len == 8) {if(dc <= 225)dc++; 
  }else if(len == 9){if(dc >= 150) dc--;
 }else if(len == 10) lf = !lf;
 else if(len == 11) rm = !rm;
}
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
}
void fata(){
  ledcWrite(0,dc);
    digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}
void spate(){
  ledcWrite(0,dc);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
    digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}
void stanga(){
  ledcWrite(0,250);
   digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}
void dreapta(){
  ledcWrite(0,250);
   digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
    digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}
void stopp(){
  ledcWrite(0,0);
   digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
}
void linefollower(int left,int center,int right){
  if(left) stanga();
  else if(center) fata();
  else if(right) dreapta();
  else(stopp);
}
#define DHTPIN 5 
DHT dht(DHTPIN, DHTTYPE);
float h,t;
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial2.begin(9600,SERIAL_8N1,RX2,TX2);
  dht.begin();
ledcSetup(0,30000,8);
ledcAttachPin(ENA,0);
 pinMode(IN1,OUTPUT);
 pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
 pinMode(IN4,OUTPUT);
  pinMode(39,INPUT);
  pinMode(34,INPUT);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
    pinMode(33,INPUT);
    if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
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
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  initWebSocket();
    
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
server.begin();
  
  servo.attach(25);
}
float hic,distanceCM;
int gazco;
void freeRoam(){
  float ds,dd;
   if(distanceCM > 17){
    fata();
   }else{
    spate();
    delay(750);
    stanga();
    delay(1000);
    ds = distanceCM;
    dreapta();
    delay(2000);
    if(ds > dd){
      stanga();
    }else{
      dreapta();
    }
   }
}
void loop() {
  //Serial.println(dc);
if(ok == 1){
  servo.write(sc);
  delay(10);
  ok = 0;
}
digitalWrite(trig,LOW);
delayMicroseconds(2);
digitalWrite(trig,HIGH);
delayMicroseconds(10);
digitalWrite(trig,LOW);
  long duration = pulseIn(echo,HIGH);
   distanceCM = duration * SOUND_SPEED/2;
  ws.cleanupClients();
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  gazco = analogRead(36);
   h = dht.readHumidity();
   t = dht.readTemperature();
   hic = dht.computeHeatIndex(t, h,false);
  doc["x"] = a.acceleration.x;
  doc["y"] = a.acceleration.y;
  doc["z"] = a.acceleration.z;
  doc["rx"] = g.gyro.x;
  doc["ry"] = g.gyro.y;
  doc["rz"] = g.gyro.z;
  doc["t"] = temp.temperature;
  doc["h"] = h;
  doc["t1"] = t;
  doc["hic"] = hic;
  doc["g"] = digitalRead(39);
  doc["co"] = map(gazco,0,4095,20,2000) * 2.5;
  doc["pir"] = digitalRead(34);
  doc["us1"] = distanceCM;
  doc["foc"] = digitalRead(33);
  char data[255];
  size_t len = serializeJson(doc, data);
  ws.textAll(data,len);
  Serial.println(t);
  if(!lf && !rm){
      if(state == 1) fata();
  else if(state == 2)spate();
  else if(state == 3)stanga();
  else if(state == 4) dreapta();
  else stopp();
  }else if(rm) freeRoam();
  
  delay(10);
}
