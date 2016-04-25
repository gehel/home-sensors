//
// Created by gehel on 25.04.16.
//

#ifndef COLLECTOR_SENSOR_H
#define COLLECTOR_SENSOR_H

#include <RF24Mesh.h>

struct payload_t {                 // Structure of our payload
    unsigned long ms;
    unsigned long counter;
};

class Sensor {
public:
    Sensor(RF24Mesh& mesh);
    void collect();

private:
    RF24Mesh& mesh;
};


#endif //COLLECTOR_SENSOR_H
