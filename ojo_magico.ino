
//   Emulador de ojo mágico
//
// Escrito por Amanda Azañón Teruel 6/7/2023
// https://www.youtube.com/@RadioProyectos
//


#include <SPI.h>

#include <TFT_eSPI.h>       // Hardware-specific library
#include "ojo_p.h"

#define PIN_CAG 34
#define EM34 //Comentar para emulación de 6E5C (Una sombra)

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library
TFT_eSprite img(&tft); //Pantalla en memoria, aquí trabajamos el dibujo
TFT_eSprite mascara(&tft); //Máscara
TFT_eSprite Aizda(&tft); //Aguja izquierda
TFT_eSprite Adcha(&tft); //Aguja derecha

void setup(void) {
  Serial.begin(115200);
  tft.init();

  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  img.createSprite(240, 240);
  mascara.createSprite(238, 238);
  
  Adcha.createSprite(12, 207);
  Aizda.createSprite(12, 207);
  
  
  tft.setSwapBytes(true);
  tft.setPivot(120,120); 
  img.setSwapBytes(true);
  Adcha.setSwapBytes(true);
  Aizda.setSwapBytes(true);
  
  mascara.pushImage(0,0,240,240,ojo_verde_2);
  Adcha.pushImage(0,0,12,207,dcha);
  Aizda.pushImage(0,0,12,207,izda2);
}

#define DIVCAG (4096/45)
void sombra(int vcag)
{
  int i;
  vcag /= DIVCAG;
  Serial.print("Vcag: ");
  Serial.println(vcag);
  if (vcag < 2) vcag=2;

#ifdef EM34
  if ((vcag-10) > 2) {
    for (i=0; i<=(vcag-10); i++) {
      //Serial.print("--> ");
      //Serial.println(vcag);
      Adcha.pushRotated(&img,-(i+180), TFT_BLACK);
      Aizda.pushRotated(&img, i+180, TFT_BLACK);
    }  
    //Serial.println("::2 ");
  } else {
    Adcha.pushRotated(&img,-(4+180), TFT_BLACK);
    Aizda.pushRotated(&img, 4+180, TFT_BLACK);
  }
#endif

  for (i=0; i<=vcag; i++) {
    Adcha.pushRotated(&img,-i, TFT_BLACK);
    Aizda.pushRotated(&img, i, TFT_BLACK);
  }  
}

void loop() {

int cag=0;
cag=analogRead(PIN_CAG);
Serial.println(cag);
  
img.pushImage(0,0,240,240,ojo_verde);
sombra(cag);
mascara.pushToSprite(&img,0,0,TFT_BLACK);
img.pushSprite(0,0);

}
