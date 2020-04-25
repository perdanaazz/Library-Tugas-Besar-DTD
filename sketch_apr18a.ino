#include "CTBot.h"

CTBot tb;
int pinLDR = A0;
int pinLM35 = A0;
float suhu;
float tegangan_hasil;

void setup(){
  Serial.begin(115200);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  digitalWrite(D7, HIGH);
  digitalWrite(D8, HIGH);
  tb.wifiConnect("Will Hijacked", "perdana2020"); //Sesuaikan dengan SSID dan kata sandi WiFi Anda.
  tb.setTelegramToken("1242440467:AAHs6n0g1WqPQqMdcjrK_kKsLFSA2Yn1V1M"); //Sesuaikan dengan token bot Telegram Anda.
  if (tb.testConnection()){
    Serial.println("TERHUBUNG");
  }
  else {
    Serial.println("ERROR");
  }
}

void loop(){
  TBMessage pesan;
  if(tb.getNewMessage(pesan)){
    Serial.print("Ada pesan masuk ");
    Serial.print(pesan.text);
    //automation lampu
    if(pesan.text.equalsIgnoreCase("LAMPU ON")){
      digitalWrite(D7, LOW);
      tb.sendMessage(pesan.sender.id, "LAMPU SEKARANG MENYALA");
    }
    if(pesan.text.equalsIgnoreCase("LAMPU OFF")){
      digitalWrite(D7, HIGH);
      tb.sendMessage(pesan.sender.id, "LAMPU SEKARANG MATI");
    }
    //automation kipas angin
    if(pesan.text.equalsIgnoreCase("KIPAS ON")){
      digitalWrite(D8, LOW);
      tb.sendMessage(pesan.sender.id, "KIPAS ANGIN SEKARANG MENYALA");
    }
    if(pesan.text.equalsIgnoreCase("KIPAS OFF")){
      digitalWrite(D8, HIGH);
      tb.sendMessage(pesan.sender.id, "KIPAS ANGIN SEKARANG MATI");
    }
    if(pesan.text.equalsIgnoreCase("AUTOMATION")){
      //LM35;
      int suhu1 = analogRead(pinLM35);
      float milliV = (suhu1/1024.0)*3300;
      float suhu = milliV/100;
      Serial.print("\nSuhu sekarang = ");
      Serial.println(suhu);
      Serial.println(" Celcius");
      //LDR;
      int nilai = analogRead (pinLDR);
      float tegangan_hasil = 5.0 * nilai / 1024;
      Serial.print ("\nVoltase out = ");
      Serial.println (tegangan_hasil);
      Serial.println (" V");
      if (suhu > 30.00){
        digitalWrite(D8, LOW);
        tb.sendMessage(pesan.sender.id, "KIPAS ANGIN SEKARANG MENYALA. SUHU > 30 CELCIUS");
      }
      if (suhu < 30.00) {
        digitalWrite(D8, HIGH);
        tb.sendMessage(pesan.sender.id, "KIPAS ANGIN SEKARANG MATI. SUHU < 30 CELCIUS");
      }
      if (tegangan_hasil < 5.00){
       digitalWrite(D7, LOW);
       tb.sendMessage(pesan.sender.id, "LAMPU SEKARANG MENYALA. TEGANGAN OUT < 5.00");
      }
      if (tegangan_hasil == 5.00){
        digitalWrite(D7, HIGH);
        tb.sendMessage(pesan.sender.id, "LAMPU SEKARANG MATI. TEGANGAN OUT = 5.00");
      }
      if (suhu > 30.00 && tegangan_hasil < 5.00){
        digitalWrite(D7, LOW);
        digitalWrite(D8, LOW);  
        tb.sendMessage(pesan.sender.id, "LAMPU DAN KIPAS ANGIN SEKARANG MENYALA. SUHU > 30 CELCIUS DAN VOLTASE OUT < 5.00");
      }
    }
  }
}
