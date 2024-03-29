//Programa : RFID - Controle de Acesso
//Autor : Projeto Ciclosafe. 
 
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  
 
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); 
 
char st[20];
 
void setup() 
{
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime seu cartao ou chaveiro para que a leitura seja efetuada!...");
  Serial.println();
  //Define o número de colunas e linhas do LCD:  
  lcd.begin(16, 2);  
  mensageminicial();
}

 void mensageminicial()
 {
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print (" APROXIME SEU ");
  lcd.setCursor(4,1);
  lcd.print ("CARTAO");
 }
void loop() 
{
  // Procura por novos cartões
  if ( ! mfrc522.PICC_IsNewCardPresent()){
    return;
  }
  // Seleciona um dos cartões
  if ( ! mfrc522.PICC_ReadCardSerial()){
    return;
  }
  //Mostra UID na serial
  Serial.print("52 57 3A 1A :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  if (conteudo.substring(1) == "52 57 3A 1A") //UID 1 - Chaveiro
  {
    Serial.println("SEJA BEM VINDO AO CICLOSAFE, SR USUARIO!");
    Serial.println();
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("BEM VINDO !");
    lcd.setCursor(4,1);
    lcd.print("USUARIO!");
    delay(3000);
    mensageminicial();
  }
 
  if (conteudo.substring(1) == "CB 99 CD 15") //UID 2 - Cartao
  {
    Serial.println("ACESSO NEGADO, TENTE NOVAMENTE!");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SEU CARTAO ");
    lcd.setCursor(0,1);
    lcd.print("FOI NEGADO!");
    delay(3000);
    mensageminicial();
  }
} 
