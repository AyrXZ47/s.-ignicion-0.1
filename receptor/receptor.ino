#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <avr/wdt.h>  // Librería para el Watchdog Timer

// Pines del nRF24L01+ y el relé
#define CE_PIN 9
#define CSN_PIN 10
#define RELAY_PIN 2  // Pin conectado a la base del transistor que controla el relé

// Configuración del módulo RF24
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";  // Dirección para la comunicación

void setup() {
  // Configuración del Watchdog Timer para 2 segundos
  wdt_enable(WDTO_2S);

  // Configuración del pin del relé y comunicación serial
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Mantener el relé apagado al inicio
  Serial.begin(9600);
  Serial.println("Iniciando setup...");

  // Intentar inicializar el radio
  if (!radio.begin()) {
    Serial.println("Error inicializando el radio. Reiniciando...");
    delay(100);  // Pequeña espera antes de reiniciar
    wdt_enable(WDTO_2S);  // Configurar el watchdog para reiniciar inmediatamente
    while (1) {}  // Esperar a que el watchdog provoque el reinicio
  }

  Serial.println("Radio inicializado correctamente");
  radio.openReadingPipe(0, address);
  Serial.print("Abriendo el canal de lectura en la dirección: ");
  Serial.println((char*)address);

  radio.setPALevel(RF24_PA_HIGH);
  Serial.println("Nivel de potencia del radio establecido a alto");

  radio.startListening();
  Serial.println("Radio configurado para modo de lectura");

  Serial.println("Setup finalizado");
}

void loop() {
  // Reiniciar el Watchdog Timer al inicio del loop
  wdt_reset();

  // Verificar si hay datos disponibles
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.print("Mensaje recibido: ");
    Serial.println(text);

    // Comprobar si el mensaje es "ON"
    if (strcmp(text, "ON") == 0) {
      Serial.println("Activando relé");
      digitalWrite(RELAY_PIN, HIGH);  // Activa el transistor para encender el relé

      // Mantener el relé encendido por un breve tiempo (ajustable)
      delay(1500);  // Mantener el relé activado por 1.5 segundos

      Serial.println("Desactivando relé");
      digitalWrite(RELAY_PIN, LOW);  // Apaga el transistor para desactivar el relé
    }
  } else {
    Serial.println("No hay datos disponibles del radio");
    delay(1500);  // Pequeña espera para evitar bucles rápidos en caso de fallo continuo
  }
}
