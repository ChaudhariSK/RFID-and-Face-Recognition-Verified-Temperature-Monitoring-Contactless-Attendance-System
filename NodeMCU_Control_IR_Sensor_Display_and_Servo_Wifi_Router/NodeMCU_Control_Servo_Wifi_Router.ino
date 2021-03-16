
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>

#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <BlynkSimpleEsp8266.h>
//----------------------------------------Include the Servo Library
#include <Servo.h>

#include "PageIndex.h"; //--> Include the contents of the User Interface Web page, stored in the same folder as the .ino file

#define LEDonBoard 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router
#define ServoPort D0   //--> Defining Servo Port
#define ServoPort2 D3 
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
 
double temp_amb;
double temp_obj;
double calibration = 9.36;
 
char auth[] = "cCy8eiYj_4Y4ok41rao10MdHGOyf55rq";    // You should get Auth Token in the Blynk App.

//----------------------------------------SSID and Password of your WiFi router
const char *ssid = "SKC";
const char *password = "SK123456";
//----------------------------------------

Servo myservo;  //--> create servo object to control a servo

ESP8266WebServer server(80);  //--> Server on port 80

//----------------------------------------This routine is executed when you open NodeMCU ESP8266 IP Address in browser
void handleRoot() {
  server.send(200, "text/html", MAIN_page); //--> Send web page
}
//----------------------------------------

//----------------------------------------Procedure for handling servo control
void handleServo(){
  String POS = server.arg("servoPOS");
  int pos = POS.toInt();
  myservo.write(pos);   //--> Move the servo motor according to the POS value
  delay(8);
  Serial.print("Servo Angle:");
  Serial.println(pos);
  server.send(200, "text/plane","");
}
//----------------------------------------
//----------------------------------------Setup----------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
    
  pinMode(LEDonBoard,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(LEDonBoard, HIGH); //--> Turn off Led On Board

  myservo.attach(ServoPort); //--> attaches the servo on D1 to the servo object
  myservo.attach(ServoPort2);
  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(LEDonBoard, LOW);
    delay(250);
    digitalWrite(LEDonBoard, HIGH);
    delay(250);
    //----------------------------------------
    
  }
  //----------------------------------------
  digitalWrite(LEDonBoard, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //----------------------------------------

  //----------------------------------------Initialize Webserver
  server.on("/",handleRoot);  //--> Routine to handle at root location. This is to display web page.
  server.on("/setPOS",handleServo); //--> Sets servo position from Web request
  server.begin();  
  Serial.println("HTTP server started");
  mlx.begin();         //Initialize MLX90614
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  Blynk.begin(auth, ssid, password);
  
  Serial.println("Temperature Sensor MLX90614");
 
  display.clearDisplay();
  display.setCursor(25,15);  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(" Thermometer");
  display.setCursor(25,35);
  display.setTextSize(1);
  display.print("Initializing");
  display.display();
  delay(2500);
}
//------------------------------------------------------------------------------------
//----------------------------------------Loop----------------------------------------
void loop() {
 server.handleClient();
 //Reading room temperature and object temp
  //for reading Fahrenheit values, use
  //mlx.readAmbientTempF() , mlx.readObjectTempF() )
  Blynk.run();
  temp_amb = mlx.readAmbientTempC();
  temp_obj = mlx.readObjectTempC();
 
  //Serial Monitor
  Serial.print("Room Temp = ");
  Serial.println(temp_amb);
  Serial.print("Object temp = ");
  Serial.println(temp_obj + calibration);
 
  display.clearDisplay();
  display.setCursor(25,0);  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(" Temperature");
  
  display.setCursor(10,20);
  display.setTextSize(1);
  display.print("Ambient: ");
  display.print(temp_amb);
  display.print((char)247);
  display.print("C");
 
  display.setCursor(10,40);
  display.setTextSize(1);
  display.print("Object: ");
  display.print(temp_obj + calibration);
  display.print((char)247);
  display.print("C");
  
  display.display();
  
  Blynk.virtualWrite(V1, temp_amb);
  Blynk.virtualWrite(V2, (temp_obj + calibration));
  
  delay(500);
}
//------------------------------------------------------------------------------------
