//
// Created by gehel on 23.04.16.
//

#ifndef COLLECTOR_STATSD_H
#define COLLECTOR_STATSD_H

#include <UIPEthernet.h>

class Statsd  {
public:
    Statsd(IPAddress ip, int port);
    void begin(int localPort);
    void increment(const char *metric);
    void decrement(const char *metric);
    void gauge(const char *metric, int gaugeValue);
private:
    void _send(const char *metric, int value, const char *cmd);
    IPAddress _ip;
    int _port;
};

#endif //COLLECTOR_STATSD_H
