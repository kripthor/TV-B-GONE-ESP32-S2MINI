#define IRLED 16
#define LED 15

#include "tvcodes.h"


void xmitCodeElement(uint16_t ontime, uint16_t offtime, uint8_t PWM_code)
{
  if(PWM_code)
    ledcWrite(IRLED,  50);
  else
    ledcWrite(IRLED,  100);
  delayMicroseconds(ontime*10);
  ledcWrite(IRLED,  0);
  delayMicroseconds(offtime*10);
}

void setup() {
  pinMode(IRLED, OUTPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  int region=0; // 0 for EU, 1 for US
  
  delayMicroseconds(250000);
  
  const int *codes;
  const unsigned short *codes2;
  int sz=0;
  int p=0;

  if(region==0) {
    codes=codes_eu;
    codes2=codes2_eu;
    sz=sizeof(codes_eu)/sizeof(codes_eu[0])/3;
  } else {
    codes=codes_us;
    codes2=codes2_us;
    sz=sizeof(codes_us)/sizeof(codes_us[0])/3;
  }

  for(int i=0 ;i<sz;i++) {
    const int *cc=codes+3*i;
    int freq=pgm_read_dword(cc+0);
    const unsigned short *code=codes2+pgm_read_dword(cc+2);
    ledcAttach(IRLED, freq, 8);
 
    for (int k=0;k<pgm_read_dword(cc+1);k+=2)
      xmitCodeElement(pgm_read_word(code+k), pgm_read_word(code+k+1), (freq!=0));  

    digitalWrite(LED, HIGH);
    delay(60);  
    digitalWrite(LED, LOW);
    delay(60);
    
  }
  digitalWrite(LED, HIGH);
  delay(1000);  
  digitalWrite(LED, LOW);
  delay(1000);
}
