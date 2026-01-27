#include <Arduino.h>
#include <driver/adc.h>

#define RELE 12
#define carga1 32
#define carga2 33
#define carga3 25
#define RX2_PIN 16
#define TX2_PIN 17

void setup() {

    pinMode(RELE, OUTPUT);
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);

    //Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, RX2_PIN, TX2_PIN);
}

void loop() {

  digitalWrite(RELE, HIGH); //high relé fechado, low relé aberto
  //digitalWrite(RELE, LOW); //high relé fechado, low relé aberto 

  uint32_t adc_carga1=0, adc_carga2=0, adc_carga3=0;

  for(uint16_t i=0; i<500; i++) { // média de 500 amostras

      adc_carga1 += analogRead(carga1);
      adc_carga2 += analogRead(carga2);
      adc_carga3 += analogRead(carga3);

      delayMicroseconds(50);
  }

    adc_carga1 = adc_carga1 / 500;
    adc_carga2 = adc_carga2 / 500;
    adc_carga3 = adc_carga3 / 500;

    // pacote com formato: adc1,adc2,adc3
    Serial2.print(adc_carga1);
    Serial2.print(",");      
    Serial2.print(adc_carga2);
    Serial2.print(",");      
    Serial2.println(adc_carga3); 

  delay(50);
  
}
