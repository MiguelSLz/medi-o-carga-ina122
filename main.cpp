#include <Arduino.h>
#include <driver/adc.h>

#define RELE 12
#define carga1 32
#define carga2 33
#define carga3 25
#define carga4 26
#define RX2_PIN 16
#define TX2_PIN 17

// Configuração da Média Móvel
#define N_AMOSTRAS 500

// Buffers e variáveis globais para persistência dos dados
uint16_t buff_c1[N_AMOSTRAS] = {0};
uint16_t buff_c2[N_AMOSTRAS] = {0};
uint16_t buff_c3[N_AMOSTRAS] = {0};
uint16_t buff_c4[N_AMOSTRAS] = {0};

uint32_t soma_c1 = 0, soma_c2 = 0, soma_c3 = 0, soma_c4 = 0;
uint16_t idx = 0; // Índice atual do buffer circular
unsigned long lastPrint = 0; // Timer para controlar o envio Serial

void setup() {

    pinMode(RELE, OUTPUT);
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);

    //Serial.begin(9600);
    Serial2.begin(115200, SERIAL_8N1, RX2_PIN, TX2_PIN);
}

void loop() {

    digitalWrite(RELE, HIGH); //high relé fechado, low relé aberto
    //digitalWrite(RELE, LOW); //high relé fechado, low relé aberto 

    // --- LÓGICA DA MÉDIA MÓVEL (SMA) ---
    
    // 1. Subtrai a leitura mais antiga da soma total
    soma_c1 -= buff_c1[idx];
    soma_c2 -= buff_c2[idx];
    soma_c3 -= buff_c3[idx];
    soma_c4 -= buff_c4[idx];

    // 2. Realiza a nova leitura e atualiza o buffer na posição atual
    buff_c1[idx] = analogRead(carga1);
    buff_c2[idx] = analogRead(carga2);
    buff_c3[idx] = analogRead(carga3);
    buff_c4[idx] = analogRead(carga4);

    // 3. Adiciona a nova leitura à soma total
    soma_c1 += buff_c1[idx];
    soma_c2 += buff_c2[idx];
    soma_c3 += buff_c3[idx];
    soma_c4 += buff_c4[idx];

    // 4. Avança o índice (buffer circular)
    idx++;
    if (idx >= N_AMOSTRAS) {
        idx = 0;
    }

    // Mantém o delay de amostragem original (taxa de atualização do buffer)
    delayMicroseconds(50); 


    // --- ENVIO SERIAL (Substitui o delay(50) bloqueante) ---
    // Envia os dados a cada 50ms (aproximadamente o mesmo timing original)
    if (millis() - lastPrint >= 50) {
        lastPrint = millis();

        // Calcula as médias atuais
        uint32_t media1 = soma_c1 / N_AMOSTRAS;
        uint32_t media2 = soma_c2 / N_AMOSTRAS;
        uint32_t media3 = soma_c3 / N_AMOSTRAS;
        uint32_t media4 = soma_c4 / N_AMOSTRAS;

        // pacote com formato: adc1,adc2,adc3
        Serial2.print(media1);
        Serial2.print(",");      
        Serial2.print(media2);
        Serial2.print(",");
        Serial2.print(media3);
        Serial2.print(",");        
        Serial2.println(media4); 
    }
}
