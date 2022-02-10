#include <SPI.h>
#include <Ethernet.h>
//#include <SD.h>
#include <DHT.h>
//#include <ArduinoJson.h>
#include <MQ2.h>

#define DHTPIN A1 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT  
#define MQ2PIN A2
#define pin2 3
#define pin1 2
//
//int pin2 = 3;
//int pin1 = 2;
unsigned long sampletime_ms = 3000;//sampe 1s ;
unsigned long duration1;
unsigned long duration2;
unsigned long lowpulseoccupancy1 = 0;
unsigned long lowpulseoccupancy2 = 0;
float ratio1 = 0;
float ratio2 = 0;
float concentration1 = 0;
float concentration2 = 0;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 100, 40); // Endereço IP que a Ethernet Shield terá. Deve ser alterado para um endereço livre da sua rede.
EthernetServer server(80);     // Cria um servidor WEB
DHT dht(DHTPIN, DHTTYPE);
MQ2 mq2(MQ2PIN);

void setup() {
    Serial.begin(9600);
    Ethernet.begin(mac, ip);  // Inicializa a Ethernet Shield
    server.begin();           // Inicia esperando por requisições dos clientes (Browsers)
    dht.begin();
    mq2.begin();
} // fim do setup

void loop() {
    EthernetClient client = server.available();  // Tenta pegar uma conexão com o cliente (Browser)
    if (client) {  // Existe um cliente em conexão ?        
        boolean currentLineIsBlank = true;
        while (client.connected()) {          
            if (client.available()) {
                char c = client.read(); // lê 1 byte (character) do cliente
                                
                if (c == '\n' && currentLineIsBlank) {  
                    // ENVIA A PÁGINA WEB
                    client.println("HTTP/1.1 200 OK");
//                    client.println("Content-Type: text/html");
                    client.println("Content-Type: aplication/json");
//                    client.println("Connection: keep-alive");
                    client.println("Connection: close");
                    client.println();

                    duration1 = pulseIn(pin1, LOW);
                    duration2 = pulseIn(pin2, LOW);
                    lowpulseoccupancy1 = lowpulseoccupancy1+duration1;
                    lowpulseoccupancy2 = lowpulseoccupancy2+duration2;

                    ratio1 = lowpulseoccupancy1/(sampletime_ms*10.0);  // Integer percentage 0=>100
                    concentration1 = 1.1*pow(ratio1,3)-3.8*pow(ratio1,2)+520*ratio1+0.62; // using spec sheet curve
                  
                    ratio2 = lowpulseoccupancy2/(sampletime_ms*10.0);  // Integer percentage 0=>100
                    concentration2 = 1.1*pow(ratio2,3)-3.8*pow(ratio2,2)+520*ratio2+0.62; // 

                    client.print("{\n\t\"temperatura\": ");
                    client.print(dht.readTemperature());
                    client.print(",\n\t\"umidade\": ");
                    client.print(dht.readHumidity());
                    client.print(",\n\t\"gas inflamavel\": ");
                    client.print(mq2.readLPG());
                    client.print(",\n\t\"CO2\": ");
                    client.print(mq2.readCO());
                    client.print(",\n\t\"fumaca\": ");
                    client.print(mq2.readSmoke());
                    client.print(",\n\t\"ar\": {\n\t\t\"concentracao1\": ");
                    client.print(concentration1);
                    client.print(" pcs/0.01cf,\n\t\t\"concentracao2\": ");
                    client.print(concentration2);
                    client.print(" pcs/0.01cf,\n\t\t\"consideracao de ar\": ");
                    if (concentration1 < 1000) {
                     client.print("CLEAN");
                    } else if (concentration1 > 1000 && concentration1 < 10000) {
                     client.print("GOOD");
                    } else if (concentration1 > 10000 && concentration1 < 20000) {      
                     client.print("ACCEPTABLE");
                    } else if (concentration1 > 20000 && concentration1 < 50000) {
                     client.print("HEAVY");
                    } else {   // (concentration1 > 50000 )
                     client.print("HAZARD");  
                    }
                    client.print("\n\t}\n}");

                    lowpulseoccupancy1 = 0;
                    lowpulseoccupancy2 = 0;

                    break;                
                }
                
                // toda linha de texto recebida do cliente termina com os caracteres \r\n
                if (c == '\n') {
                    // ultimo caractere da linha do texto recebido
                    // iniciando nova linha com o novo caractere lido
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // um caractere de texto foi recebido do cliente
                    currentLineIsBlank = false;
                }
            } // fim do if (client.available())
        } // fim do while (client.connected())
        
        delay(1);      // da um tempo para o WEB Browser receber o texto
        client.stop(); // termina a conexão
    } // fim do if (client)
} // fim do loop
