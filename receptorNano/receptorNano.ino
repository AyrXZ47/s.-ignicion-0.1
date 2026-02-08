#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Pines del nRF24L01+ y 2N2222 
#define CE_PIN 9
#define CSN_PIN 10
#define transistor_PIN 2  // Pin conectado a la base del 2N2222

// Configuración del módulo RF24
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";  // Dirección para la comunicación

void setup() {
  // Configuración del pin del 2N2222
  pinMode(transistor_PIN, OUTPUT);
  digitalWrite(transistor_PIN, LOW);  // Apagado al inicio

  // Inicialización del radio
  if (!radio.begin()) {
    while (1); // Detener si el radio no se inicializa correctamente
  }

  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();
}

void loop() {
  // Verificar si hay datos disponibles
  if (radio.available()) {
    char text[32] = ""; // Buffer para el mensaje recibido
    radio.read(&text, sizeof(text));

    // Comprobar si el mensaje es "ON"
    if (strcmp(text, "ON") == 0) {
      digitalWrite(transistor_PIN, HIGH);  // Activa 2N2222
      delay(3000);                   // Mantener encendido por 3 segundos
      digitalWrite(transistor_PIN, LOW);  // Apaga 2N2222
    }
  }
}