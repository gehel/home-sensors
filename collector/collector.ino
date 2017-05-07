#include <UIPEthernet.h>
// The connection_data struct needs to be defined in an external file.
#include <UIPServer.h>
#include <UIPClient.h>
#include <Dhcp.h>

EthernetServer server = EthernetServer(1000);

void setup()
{
    Serial.begin(57600);

    uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
    IPAddress myIP(192,168,1,6);

    Ethernet.begin(mac,myIP);

    server.begin();
}

void loop()
{
    size_t size;

    if (EthernetClient client = server.available())
    {
        if (client)
        {
            while((size = client.available()) > 0)
            {
                uint8_t* msg = (uint8_t*)malloc(size);
                size = client.read(msg,size);
                Serial.write(msg,size);
                client.write(msg,size);
                free(msg);
            }
        }
    }
}
