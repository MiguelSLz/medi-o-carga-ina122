#include <Arduino.h>
#include <driver/adc.h>
#include <iostream>
#define RELE 12
#define amp_out 32

void setup() {
    pinMode(RELE, OUTPUT);
    adc1_config_width(ADC_WIDTH_BIT_12);
    Serial.begin(9600);

}

void loop() {

  digitalWrite(RELE, HIGH); //high relé fechado, low relé aberto
  //digitalWrite(RELE, LOW); //high relé fechado, low relé aberto 

  int raw_amp_out[512], raw_amp_out_sum=0;
  float v_amp, Isample[512], I=0;
  const float Gain=4.28, analogReadRange=4096, Vcc=3.3; // ganho do ina, ADC de 12 bits, Vcc do esp32

  for(uint16_t i=0; i<512; i++) { // média de 512 amostras
    raw_amp_out[i] = analogRead(amp_out);  // [sem escala]
    v_amp = (Vcc * raw_amp_out[i]) / analogReadRange; // [V]
    Isample[i] = (1000 * v_amp / Gain) / 0.1;        // I [mA], 0.1 = resistencia do shunt

    I=I + Isample[i];
    raw_amp_out_sum += raw_amp_out[i];
  }
  I = I / 512; // valor médio

  Serial.print("Corrente = ");
  Serial.print(I);
  Serial.println(" mA");

  Serial.print("Valor bruto do ADC = ");
  Serial.println(raw_amp_out_sum/512);

  Serial.println("-----------------------");

  I=0;
  delay(1000);
  
}
