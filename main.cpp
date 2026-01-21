#include <Arduino.h>
#include <driver/adc.h>
#include <iostream>

#define RELE 12
#define amp_out 32

void setup() {
    pinMode(RELE, OUTPUT);
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);
    Serial.begin(9600);

}

void loop() {

  digitalWrite(RELE, HIGH); //high relé fechado, low relé aberto
  //digitalWrite(RELE, LOW); //high relé fechado, low relé aberto 

  uint32_t raw_amp_out_sum=0;
  float I=0;

  for(uint16_t i=0; i<512; i++) { // média de 512 amostras
      raw_amp_out_sum += analogRead(amp_out); 
      delayMicroseconds(100);
  }
  float raw_average = (float)raw_amp_out_sum / 512.0; // valor médio

  I = 0.001580*raw_average + 0.24878; // equação de calibração encontrada experimentalmente

  if (raw_average < 30) I = 0;

  Serial.print("Corrente = ");
  Serial.print(I, 3);
  Serial.println(" A");

  Serial.print("Valor bruto do ADC = ");
  Serial.println(raw_average);

  Serial.println("-----------------------");

  delay(1000);
  
}
