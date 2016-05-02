#include <Arduino.h>
#include <Ethernet.h>
#include <RF24.h>
#include <RF24Network.h>
#include <RF24Mesh.h>
#include <printf.h>
#include <DHT.h>
#include <SPI.h>
#include <Statsd.h>
#include <Sensor.h>

#define DEBUG
//#define collector
#define home_sensor

#ifdef DEBUG
    #define DEBUG_PRINT(x)  Serial.print (x)
    #define DEBUG_PRINTLN(x)  Serial.println (x)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
#endif

#ifdef collector
IPAddress statsd_ip(192, 168, 1, 40); // IP Address of StatsD Server
int statsd_port = 8125; // StatsD Port
int statsd_local_port = 54321;
IPAddress ip(192, 168, 1, 201); // IP Address of Arduino w/ Ethernet Sheild
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // Ethernet Shield MAC
Statsd statsd(statsd_ip, statsd_port);
#endif

RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

Sensor sensor(mesh);

#ifdef collector
const uint16_t this_node = 00;    // Address of our node in Octal format ( 04,031, etc)
const uint16_t other_node = 01;   // Address of the other node in Octal format
#endif
#ifdef home_sensor
const uint16_t this_node = 01;    // Address of our node in Octal format ( 04,031, etc)
#endif

#ifdef home_sensor
#define DHT_PIN 2
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);
uint32_t displayTimer = 0;
#endif

payload_t payload;

void printEthernetConfig();

void printRadioDetails();

void setup() {
    Serial.begin(57600);
    printf_begin();

#ifdef collector
    DEBUG_PRINTLN("Start ehternet");
    Ethernet.begin(mac, ip);
    statsd.begin(statsd_local_port);
    printEthernetConfig();
#endif

#ifdef collector
    DEBUG_PRINTLN("Start SPI");
    SPI.begin();
#endif

    DEBUG_PRINTLN("Start radio");
    radio.begin();
    printRadioDetails();

    DEBUG_PRINTLN("Start RF24 network");
    network.begin(/*channel*/ 90, /*node address*/ this_node);
    mesh.setNodeID(this_node);
    mesh.begin();

#ifdef home_sensor
    DEBUG_PRINTLN("Start DHT");
    dht.begin();
#endif
}

void printRadioDetails() {
    DEBUG_PRINTLN("Radio details");
#ifdef DEBUG
    radio.printDetails();
#endif
}

void printEthernetConfig() {
    DEBUG_PRINTLN("ethernet setup done");
    DEBUG_PRINT("dnsserver: ");
    DEBUG_PRINTLN(Ethernet.dnsServerIP());
    DEBUG_PRINT("gatewayIP: ");
    DEBUG_PRINTLN(Ethernet.gatewayIP());
    DEBUG_PRINT("localIP: ");
    DEBUG_PRINTLN(Ethernet.localIP());
    DEBUG_PRINT("subnetMask: ");
    DEBUG_PRINTLN(Ethernet.subnetMask());
}

void loop() {
    DEBUG_PRINT(".");
    mesh.update();

#ifdef collector
    mesh.DHCP();
    Ethernet.maintain();
    statsd.gauge("arduino.collector.uptimeInSeconds", millis() / 1000);
#endif

#ifdef collector
    while ( network.available() ) {     // Is there anything ready for us?
        RF24NetworkHeader header;        // If so, grab it and print it out
        payload_t payload;
        network.read(header, &payload, sizeof(payload));
        Serial.print("Received packet, temp: ");
        Serial.print(payload.temperature);
        Serial.print(", humidity: ");
        Serial.println(payload.humidity);
        statsd.gauge("arduino.collector.temperature", payload.temperature);
        statsd.gauge("arduino.collector.humidity", payload.humidity);
    }
#endif

#ifdef home_sensor
    if (millis() - displayTimer >= 1000) {
    displayTimer = millis();
    payload.humidity = 50;
    payload.temperature = 22;
    // Send an 'M' type message containing the current millis()
    if (!mesh.write(&payload, 'S', sizeof(payload))) {
      // If a write fails, check connectivity to the mesh network
      if ( ! mesh.checkConnection() ) {
        //refresh the network address
        DEBUG_PRINTLN("Renewing Address");
        mesh.renewAddress();
      } else {
        DEBUG_PRINTLN("Send fail, Test OK");
      }
    } else {
      DEBUG_PRINT("Send OK: "); DEBUG_PRINTLN(displayTimer);
    }
  }
#endif

}
