#include <SPI.h>
#include <WizFi250.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h> // I2C통신을 위한 라이브러리
#include "RTClib.h"
#include <Arduino.h>
#include <TM1637Display.h>

#define PIN 5 // led 연결 핀 설정

Adafruit_NeoPixel stick = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);
RTC_DS1307 RTC; // 시간 모듈

int getInt(String input); 

#define VARID      "9e12851a39ab262f41bc922a27073232" //openweathermap에서 받아온 api key

char ssid[] = "정소영의 iPhone";       // 접속할 와이파이 아이디
char pass[] = "01096039817";        // 접속할 와이파이 비밀번호
int status = WL_IDLE_STATUS;       // the Wifi radio's status

char server[] = "api.openweathermap.org"; //날씨정보를 받아 올 서버

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
 const unsigned long postingInterval = 1000L; // delay between updates, in milliseconds

boolean getIsConnected = false;

String curtime; // 현재 시간 비교용 변수
String wt_temp; // 온도 및 강수확률 정보 저장 변수
String wt_pop;

String today; // 오늘인지 여부 확인
String total_pty = "none"; // 오늘의 날씨 최종 여부 확인, 기본값 none
float popArr[8] = {0}; // 강수확률 저장할 배열

const int powerButton = 10; // 조명 on/off 10번

//on, off 버튼 토글 동작을 위한 변수
int oneTimeFlag;
 boolean onOffStatus;

// Initialize the Ethernet client object
 WiFiClient client; 

void httpRequest();
void printWifiStatus();

TM1637Display popD(8, 9); // 강수확률 출력 7세그먼트
uint8_t data[] = {0x0, 0x0, 0x0, 0x0}; // 세그먼트 초기화(off)를 위한 배열

boolean check = false;

void setup()
 {
   // initialize serial for debugging
   Serial.begin(115200);
   Serial.println(F("\r\nSerial Init"));

  stick.begin(); //네오픽셀을 초기화하기 위해 모든LED를 off시킨다
  stick.show();
  
   Wire.begin(); // rtc모듈 설정
  RTC.begin();
   // RTC.adjust(DateTime("Jun 06 2017", "13:47:10")); // RTC현재 시간 설정
 
   tempD.setBrightness(0x0f); // 7세그먼트 밝기 설정
  popD.setBrightness(0x0f);
   tempD.setSegments(data); // 초기 세그먼트 off
   popD.setSegments(data);

 

  pinMode(powerButton, INPUT_PULLUP); // 파워버튼 핀
  //pinMode(PIN, OUTPUT); // LED 핀

 

  WiFi.init();

 

  // check for the presence of the shield
   if (WiFi.status() == WL_NO_SHIELD) {
     Serial.println("WiFi shield not present");
     // don't continue
     while (true);
   }

 

  // attempt to connect to WiFi network
   while ( status != WL_CONNECTED) {
     Serial.print("Attempting to connect to WPA SSID: ");
     Serial.println(ssid);
     // Connect to WPA/WPA2 network
     status = WiFi.begin(ssid, pass);
   }

 

  Serial.println("You're connected to the network");
  
   printWifiStatus();
   httpRequest();
 }

void loop() {
   // if there's incoming data from the net connection send it out the serial port
   // this is for debugging purposes only
  
   /* 기상청 파싱 시작 */
   //if (getIsConnected) { // 연결이 되었다면 다음 코드 진행
    while (client.available()) {
       //라인을 기준으로 문자열을 저장한다.
       String line = client.readStringUntil('\n');
       //Serial.println(line); // xml문서 내용 출력
    
       // 시간 확인
      int hour = line.indexOf("</hour>");
       if (hour > 0) {
         String tmp_str = "<hour>";
         curtime = line.substring(line.indexOf(tmp_str) + tmp_str.length(), hour); // <day>부터 </day>사이의 값 읽어와 today에 저장
        //Serial.println("hour is " + curtime);
       }

      //날짜 확인
      int day = line.indexOf("</day>");
       if (day > 0) {
         String tmp_str = "<day>";
         today = line.substring(line.indexOf(tmp_str) + tmp_str.length(), day); // <day>부터 </day>사이의 값 읽어와 today에 저장
        //Serial.println("\nday is " + today);
       }

      if (today.equals("0")) { // 오늘 날짜라면(xml 오늘 : 0, 내일 : 1, 모레 : 2) 이후 코드 진행

        //날씨 정보
        int pty = line.indexOf("</pty>");
         if (pty > 0) {
           String tmp_str = "<pty>";
           String wt_pty = line.substring(line.indexOf(tmp_str) + tmp_str.length(), pty);

          if (!(wt_pty.equals("0"))) { // 코드가 전체적으로 진행이 끝났을 때, 강수 형태가 존재한다면 (즉 강수형태 파싱 결과가 0이 아니라면)
             total_pty = "something"; // 그날의 전체적인 날씨는 무언가가 내리는 날씨

 

          Serial.print("weather is ");
           Serial.println(wt_pty);
         }

        // 강수확률
        int pop = line.indexOf("</pop>");
         if (pop > 0) {
           String tmp_str = "<pop>";
           wt_pop = line.substring(line.indexOf(tmp_str) + tmp_str.length(), pop);
           Serial.print("Rainfall probability is ");
           Serial.println(wt_pop);
         }
        
         if (curtime.equals("3")) {
             tempArr[0] = wt_temp.toFloat();
             popArr[0] = wt_pop.toFloat();
         }
         else if (curtime.equals("6")) {
             tempArr[1] = wt_temp.toFloat();
             popArr[1] = wt_pop.toFloat();
         }
         // 이하 배열에 계속 각 시간별 데이터 저장

      }
       check = true; // 오늘치 값에 대한 파싱이 끝났음을 체크
    }
   }
   /* 파싱 끝 */
  

  /* 스위치 led 제어 */
   // 불이 켜져 있는 경우에는 불이 꺼지고, 불이 꺼져있던 경우에는 불이 켜짐
  if (digitalRead(powerButton) == LOW) { //power 버튼이 눌러지면 onOffstatus의 상태값 토글
    if (oneTimeFlag == 0) {
       oneTimeFlag = 1;
       onOffStatus = !onOffStatus;
     }
   }
   else {
     oneTimeFlag = 0;
   }

 

  //total_pty = "something";

  if (onOffStatus == 0) {                                //onOffStatus의 상태가 0이면
    for (int i = 0; i < stick.numPixels(); i++) {
       stick.setPixelColor(i, 0, 0, 0);
       stick.show();
     }
   }
   else {
     if (total_pty.equals("something")) { // 전체적으로 파싱이 끝난 결과 무언가가 내리는 날씨라면
        theaterChase(stick.Color(67, 116, 217), 50); //파란색 출력
      }
       else { // 아무것도 없는 상태라면
        theaterChase(stick.Color(255, 228, 0), 50); //노란색 출력
      }
   }
   /* led 제어 끝 */

 

  if(check) { // 오늘 날짜에 대한 파싱이 끝난 상태라면
    compareTime(); // 세그먼트에 정보를 출력해주기 위한 함수
  }
  
 }

 

// this method makes a HTTP connection to the server
 void httpRequest() {
   Serial.println();

  // close any connection before send a new request
   // this will free the socket on the WiFi shield
   client.stop();

  // if there's a successful connection
   if (client.connect(server, 80)) {
     Serial.println("Connecting...");

    // send the HTTP PUT request
     client.print("GET /wid/queryDFSRSS.jsp?zone=4211061100");
     client.println(" HTTP/1.1");
     client.println("HOST: www.kma.go.kr");
     client.println("User-Agent: launchpad-wifi");
     client.println("Connection: close");
     client.println();

    // note the time that the connection was made
     lastConnectionTime = millis();
     getIsConnected = true;
   }
   else {
     // if you couldn't make a connection
     Serial.println("Connection failed");
     getIsConnected = false;
     return;
   }
   client.println();
   Serial.println("Weather information for ");
 }


 void printWifiStatus() {
   // print the SSID of the network you're attached to
   Serial.print("SSID: ");
   Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
   IPAddress ip = WiFi.localIP();
   Serial.print("IP Address: ");
   Serial.println(ip);

  // print the received signal strength
   long rssi = WiFi.RSSI();
   Serial.print("Signal strength (RSSI):");
   Serial.print(rssi);
   Serial.println(" dBm");
 }

int getInt(String input) {
   int i = 2;

  while (input[i] != '"') {
     i++;
   }
   input = input.substring(2, i);
   char carray[20];
   //Serial.println(input);
   input.toCharArray(carray, sizeof(carray));
   //Serial.println(carray);
   temp = atoi(carray);
   //Serial.println(temp);
   return temp;
 }

 

//입력한 색으로 LED를 깜빡거리며 표현한다
void theaterChase(uint32_t c, uint8_t wait) {
   for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
     for (int q = 0; q < 3; q++) {
       for (int i = 0; i < stick.numPixels(); i = i + 3) {
         stick.setPixelColor(i + q, c);  //turn every third pixel on
       }
       stick.show();

      delay(wait);

      for (int i = 0; i < stick.numPixels(); i = i + 3) {
         stick.setPixelColor(i + q, 0);      //turn every third pixel off
       }
     }
   }
 }

 

void compareTime() {
     DateTime now = RTC.now(); // rtc모듈에 셋팅된 값 불러오기 위함   
     uint8_t tempData[] = { 0xff, 0xff, 0xff, 0x00 }; // 온도와 강수확률을 표시해줄 데이터
    uint8_t popData[] = { 0xff, 0xff, 0xff, 0x00 };
     int value = 1244; // colon on/off를 위한 변수들
    uint8_t segto = 0x80 | tempD.encodeDigit((value / 100) % 10);   
     int index; // 현재 시각에 해당하는 날씨 정보를 불러오기 위한 인덱스 설정
    //[0 : 3], [1 : 6], [2 : 9], [3 : 12], [4 : 15], [5 : 18], [6 : 21], [7 : 24]

    // 기상청에서 이전 시간 데이터는 xml에서 지워버리기 때문에, 3시간씩 뒤로 미룬 값을 출력해야
    // 0이 아닌 값이 출력 된다
    // (ex : 14시에 파싱을 시작했을 때 의도대로라면 12시인 index3 값이 들어가야 하지만, 12시 데이터는 지워진 상태이므로
    // 15시인 index4값을 출력해줘야 값이 출력됨)
     if(now.hour() <= 3) { // am 3시까지
      index = 0;
     }
     else if(now.hour() <= 6) { // am 4~6시
      index = 1;
     }
     else if(now.hour() <= 9) { // am 7~9시
      index = 2;
     }
     else if(now.hour() <= 12) { // am 10~12시
      index = 3;
     }
     else if(now.hour() <= 15) { // pm 1~3시
      index = 4;
     }
     else if(now.hour() <= 18) { // pm 4~6시
      index = 5;
     }
     else if(now.hour() <= 21) { // pm 7~9시
      index = 6;
     }
     else if(now.hour() <= 24) { // pm 10시~12시
      index = 7;
     }
  

    /* 기온 정보 저장 */
     tempData[0] = tempD.encodeDigit(tempArr[index] / 10); // 0에는 10의 자리
    tempData[1] = tempD.encodeDigit((int)(tempArr[index]) % 10); // 1에는 1의 자리
    tempData[2] = tempD.encodeDigit((int)(tempArr[index] * 10) % 10); // 2에는 소수점 첫째자리
    /* 기온 정보 끝 */
    
     /* 강수 확률 정보 저장 */
     popData[0] = popD.encodeDigit(popArr[index] / 10); // 0에는 10의 자리
    popData[1] = popD.encodeDigit((int)(popArr[index]) % 10); // 1에는 1의 자리
    popData[2] = popD.encodeDigit((int)(popArr[index] * 10) % 10); // 2에는 소수점 첫째자리
    /* 강수 확률 정보 끝 */

    /* 각 세그먼트에 정보 출력 */
     tempD.setSegments(tempData);
     //tempD.setSegments(&segto, 1, 1); // colon on
     popD.setSegments(popData);
     //popD.setSegments(&segto, 1, 1); // colon on
 }

