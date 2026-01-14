#include <Arduino.h>
#include <driver/adc.h>
#define RELE 12
#define amp_out 32

void setup() {
    pinMode(RELE, OUTPUT);
    adc1_config_width(ADC_WIDTH_BIT_12);

}

void loop() {

  digitalWrite(RELE, HIGH); //high relé fechado, low relé aberto
  //digitalWrite(RELE, LOW); //high relé fechado, low relé aberto 

  uint16_t v_amp, I, raw_amp_out;
  const float Gain=4.28, analogReadRange=4096, Vcc=3.3; // ganho do ina, ADC de 12 bits, Vcc do esp32

  raw_amp_out = analogRead(amp_out);  // [sem escala]
  v_amp = (Vcc * raw_amp_out) / analogReadRange; // [V]
  I = (1000 * v_amp / Gain) / 0.1;        // I [mA], 0.1 = resistencia do shunt

  delay(1000);
  
}
