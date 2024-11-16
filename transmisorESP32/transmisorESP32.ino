#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define BUTTON_PIN 15
#define CE_PIN 4
#define CSN_PIN 5

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

void setup() {
  // Inicialización del botón y la comunicación serial
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("Iniciando setup...");

  // Inicialización del radio
  if (!radio.begin()) {
    Serial.println("Error inicializando el radio");
    while (1); // Quedarse aquí si hay error
  }
  Serial.println("Radio inicializado correctamente");

  radio.openWritingPipe(address);
  Serial.print("Abriendo el canal de escritura en la dirección: ");
  Serial.println((char*)address);

  radio.setPALevel(RF24_PA_HIGH);
  Serial.println("Nivel de potencia del radio establecido a alto");

  radio.stopListening();
  Serial.println("Radio configurado para modo de escritura");

  Serial.println("Setup finalizado");
}

void loop() {
  // Verificar el estado del botón
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Botón presionado");

    // Enviar mensaje a través del radio
    const char text[] = "ON";
    bool success = radio.write(&text, sizeof(text));

    // Verificar si el envío fue exitoso
    if (success) {
      Serial.println("Mensaje enviado: ON");
    } else {
      Serial.println("Fallo al enviar el mensaje");
    }

    // Esperar medio segundo antes de verificar el botón nuevamente
    delay(500);
  }
}
