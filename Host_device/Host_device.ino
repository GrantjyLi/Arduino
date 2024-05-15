#include<ESP8266WiFi.h>

const char* ssid = "notESP8266";
const char* pw = "pw4ESP8266";

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

void setup(){
    Serial.begin(115200);
    delay(100);
    Serial.println();

    Serial.println(
        WiFi.softAP(ssid, pw, 1, false, 4) ?
        "AP established." : "AP setup Failed."
    );

    Serial.printf("IP Address: ");
    Serial.println(WiFi.softAPIP());
}

void loop(){
    Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
    delay(1000);

}