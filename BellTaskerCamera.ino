/* !!!! NoneMCU 1.0 (ESP-12E Module), 80 MHz, 115200, 4M(3M SPIFFS) !!!!!
Tasker Network Event Server



*/
#include <ESP8266WiFi.h>
#include <Streaming.h> // консоль
//#include <ESP8266HTTPClient.h>
#include <Metro.h> // Подключаем библиотеку timer



#define ConsoleComm 1
const byte interruptPin = D1;

const char* ssid = "xxx"; // SSID wifi точки
const char* password = "xxx"; //пароль на wifi точку
const char* host = "xxx.xxx.xxx.xxx"; // ip планшета


bool bell_switch = false; // храним состояние кнопки звонка
bool switch_ignore = false; //флаг начала игнорирования повторного нажатия на кнопку звонка, чтобы не запускать прогу всякий раз

//HTTPClient http;
WiFiClient client;


const int httpPort = xxxx; //порт куда отсылаем POST запрос


Metro timer_bell_ignore = Metro(120000); // Создаем ссылку на таймер не реагирования на однократное нажатие на звонок, чтобы кучу раз не запускать прогу;


void setup() {
    Serial.begin(115200);
    delay(10);



    // Connect to WiFi network
#if ConsoleComm

    Serial << "Connecting to "<< ssid << endl;
#endif

    WiFi.mode(WIFI_STA); // отключаем AP
    WiFi.begin(ssid, password);

 
#if ConsoleComm

    Serial <<"" << "WiFi Connected" << endl;



    // Print the IP address
    Serial << "IP Address: "<< WiFi.localIP() << endl;
#endif



    attachInterrupt(digitalPinToInterrupt(interruptPin), detect_switch, CHANGE);




}


void loop() {


    // если кто-то нажал кнопку мы вызываем прогу на планшете, но не чаще 1 раз в timer_bell_ignore мин.
    if (bell_switch && !switch_ignore) {
		switch_ignore = true;
		#if ConsoleComm
        Serial << "Bell switch:"<< bell_switch << " Switch_ignore:"<< switch_ignore << endl;
#endif
        sendPOST();
		
		bell_switch = false;
    }

    if (timer_bell_ignore.check() && switch_ignore) {
		

 		#if ConsoleComm
        Serial << "timer_bell_ignore switch! bell_switch:"<< bell_switch << "switch_ignore:"<< switch_ignore << endl;
#endif
       bell_switch = false;
        switch_ignore = false; //сбрасываем ключ
    }


}


//формируем и отправляем POST запрос

void sendPOST()

{

#if ConsoleComm

    Serial << "Prepare POST request... "<< endl;

#endif



    if (!client.connect(host, httpPort)) {

#if ConsoleComm

        Serial.println("connection failed");

#endif

        return;

    }



    //строка с данными POST запроса

    String data = "say"; // слово, что посылаем в POST запросе





    // Send request to the server:

    client.println("POST / HTTP/1.1");

    client.println("Host: server_name");

    client.println("Accept: */*");

    client.println("Content-Type: application/x-www-form-urlencoded");

    client.print("Content-Length: ");

    client.println(data.length());

    client.println();

    client.print(data);



    delay(500); // Can be changed

    if (client.connected()) {

        client.stop();  // DISCONNECT FROM THE SERVER

    }



#if ConsoleComm

    Serial.println();

    Serial.println("closing connection");

#endif


}


//********************обработчики прерываний*******************************
void  detect_switch() { // обработка внешнего прерывания. Сработает по переднему фронту

    bell_switch = true; // кто-то нажал на кнопку звонка
}

