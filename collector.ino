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


IPAddress statsd_ip(192, 168, 1, 40); // IP Address of StatsD Server
int statsd_port = 8125; // StatsD Port
int statsd_local_port = 54321;
IPAddress ip(192, 168, 1, 201); // IP Address of Arduino w/ Ethernet Sheild
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // Ethernet Shield MAC
Statsd statsd(statsd_ip, statsd_port);

RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

Sensor sensor(mesh);

const uint16_t this_node = 00;    // Address of our node in Octal format ( 04,031, etc)
const uint16_t other_node = 01;   // Address of the other node in Octal format

#define DHT_PIN 2
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
    Serial.begin(57600);
    printf_begin();
    Serial.println("ethernet setup");
    Ethernet.begin(mac, ip);
    statsd.begin(statsd_local_port);
    Serial.println("ethernet setup done");
    Serial.print("dnsserver: ");
    Serial.println(Ethernet.dnsServerIP());
    Serial.print("gatewayIP: ");
    Serial.println(Ethernet.gatewayIP());
    Serial.print("localIP: ");
    Serial.println(Ethernet.localIP());
    Serial.print("subnetMask: ");
    Serial.println(Ethernet.subnetMask());
    Serial.println("Start SPI");
    SPI.begin();
    Serial.println("Start radio");
    radio.begin();
    Serial.println("Radio details");
    radio.printDetails();
    Serial.println("Nework start");
    network.begin(/*channel*/ 90, /*node address*/ this_node);
    mesh.setNodeID(0);
    mesh.begin();
    dht.begin();
}

void loop() {
    Serial.print(".");
    Ethernet.maintain();
    sensor.collect();
    statsd.gauge("arduino.collector.uptimeInSeconds", millis() / 1000);
    while ( network.available() ) {     // Is there anything ready for us?
        RF24NetworkHeader header;        // If so, grab it and print it out
        payload_t payload;
        network.read(header, &payload, sizeof(payload));
        Serial.print("Received packet #");
        Serial.print(payload.counter);
        Serial.print(" at ");
        Serial.println(payload.ms);
    }
}
