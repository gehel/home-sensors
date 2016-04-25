//
// Created by gehel on 25.04.16.
//

#include "Sensor.h"

Sensor::Sensor(RF24Mesh& _mesh): mesh(_mesh) {}

void Sensor::collect() {
    mesh.update();
    mesh.DHCP();
}
