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
  digitalWrite(D7, HIGH);
  digitalWrite(D8, HIGH);
  TBMessage pesan;
  if(tb.getNewMessage(pesan)){
    Serial.print("Ada pesan masuk ");
    Serial.print(pesan.text);
    //automation lampu
    if(pesan.text.equalsIgnoreCase("LAMPU ON")){
      digitalWrite(D7, LOW);
      tb.sendMessage(pesan.sender.id, "LAMPU SEKARANG MENYALA");
    }
    else if(pesan.text.equalsIgnoreCase("LAMPU OFF")){
      digitalWrite(D7, HIGH);
      tb.sendMessage(pesan.sender.id, "LAMPU SEKARANG MATI");
    }
    //automation kipas angin
    else if(pesan.text.equalsIgnoreCase("KIPAS ON")){
      digitalWrite(D8, LOW);
      tb.sendMessage(pesan.sender.id, "KIPAS ANGIN SEKARANG MENYALA");
    }
    else if(pesan.text.equalsIgnoreCase("KIPAS OFF")){
      digitalWrite(D8, HIGH);
      tb.sendMessage(pesan.sender.id, "KIPAS ANGIN SEKARANG MATI");
    }
    else if(pesan.text.equalsIgnoreCase("AUTOMATION")){
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
        delay(20000);
      }
      if (suhu < 30.00) {
        digitalWrite(D8, HIGH);
        delay(0);
      }
      if (tegangan_hasil < 5.00){
       digitalWrite(D7, LOW);
       delay(20000);
      }
      if (tegangan_hasil == 5.00){
        digitalWrite(D7, HIGH);
        delay(0);
      }
    }
    else {
        tb.sendMessage(pesan.sender.id, "PERINTAH YANG ANDA KIRIM SALAH");
        tb.sendMessage(pesan.sender.id, "KETIK 'LAMPU ON' UNTUK MENYALAKAN LAMPU");
        tb.sendMessage(pesan.sender.id, "KETIK 'LAMPU OFF' UNTUK MEMATIKAN LAMPU");
        tb.sendMessage(pesan.sender.id, "KETIK 'KIPAS ON' UNTUK MENYALAKAN KIPAS ANGIN");
        tb.sendMessage(pesan.sender.id, "KETIK 'KIPAS OFF' UNTUK MEMATIKAN KIPAS ANGIN");
    }
  }
  delay(500000);
}
