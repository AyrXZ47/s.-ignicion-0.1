#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <avr/wdt.h>  // Librería para el Watchdog Timer

// Pines del nRF24L01+ y del relé
#define CE_PIN 9
#define CSN_PIN 10
#define RELAY_PIN 2  // Pin conectado al relé

// Configuración del módulo RF24
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";  // Dirección para la comunicación

void setup() {
  // Configuración del Watchdog Timer para 2 segundos
  wdt_enable(WDTO_2S);

  // Configuración de pines
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Relé apagado al inicio
  Serial.begin(9600);

  Serial.println("Iniciando configuración...");

  // Inicialización del radio
  if (!radio.begin()) {
    Serial.println("Error: No se pudo inicializar el radio. Reiniciando...");
    delay(100);  // Espera breve antes de reiniciar
    wdt_enable(WDTO_2S);  // Configurar el Watchdog para reiniciar inmediatamente
    while (1) {}  // Esperar a que el Watchdog provoque el reinicio
  }

  // Configuración del radio
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();

  // Confirmación de que el sistema está listo
  Serial.println("Radio inicializado correctamente");
  Serial.print("Dirección de lectura configurada: ");
  Serial.println((char*)address);
  Serial.println("Sistema listo para recibir datos.");
}

void loop() {
  // Reiniciar el Watchdog Timer al inicio del loop
  wdt_reset();

  // Verificar si hay datos disponibles
  if (radio.available()) {
    char text[32] = ""; // Buffer para el mensaje recibido
    radio.read(&text, sizeof(text));
    Serial.print("Mensaje recibido: ");
    Serial.println(text);

    // Activar el relé si el mensaje es "ON"
    if (strcmp(text, "ON") == 0) {
      Serial.println("Activando relé");
      digitalWrite(RELAY_PIN, HIGH);  // Activar relé
      delay(1500);                   // Mantener encendido por 1.5 segundos
      digitalWrite(RELAY_PIN, LOW);  // Apagar relé
      Serial.println("Relé desactivado");
    } else {
      Serial.println("Mensaje no reconocido");
    }
  } else {
    Serial.println("Esperando datos...");
    delay(1500);  // Reducir el ciclo si no hay datos disponibles
  }
}
