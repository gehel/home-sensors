//
// Created by gehel on 25.04.16.
//

#ifndef COLLECTOR_SENSOR_H
#define COLLECTOR_SENSOR_H

#include <RF24Mesh.h>

struct payload_t {                 // Structure of our payload
    unsigned long temperature;
    unsigned long humidity;
};

class Sensor {
public:
    Sensor(RF24Mesh& mesh);

private:
    RF24Mesh& mesh;
};


#endif //COLLECTOR_SENSOR_H
