//
// Created by gehel on 23.04.16.
//

#include "Statsd.h"

EthernetUDP udp;

Statsd::Statsd(IPAddress ip, int port){
    _ip = ip;
    _port = port;
}
void Statsd::begin(int localPort) {
    udp.begin(localPort);
}

void Statsd::increment(const char *metric) {
    Statsd::_send(metric,1,"c");
}

void Statsd::decrement(const char *metric) {
    Statsd::_send(metric,-1,"c");
}

void Statsd::gauge(const char *metric, int gaugeValue){
    Statsd::_send(metric,gaugeValue,"g");
}

void Statsd::_send(const char *metric, int value, const char *cmd) {
    //Convert the integer to a string (an int's maximum string length is 12 characters [sign(1) + digits(10) + null(1)]) 2,147,483,648 to 2,147,483,647
    char valueString[12];
    itoa(value,valueString,10);

    //Concatenate the parts of the final string
    char buffer[strlen(metric) + strlen(valueString) + strlen(cmd) + 2 + 1]; //+2 for : and | and add +1 for null
    strcpy(buffer, metric);
    strcat(buffer,":");
    strcat(buffer,valueString);
    strcat(buffer,"|");
    strcat(buffer, cmd);

    const uint8_t* msg = (uint8_t*)buffer;

    udp.beginPacket(_ip, _port);
    udp.write(msg, strlen(buffer));
    udp.endPacket();
}
