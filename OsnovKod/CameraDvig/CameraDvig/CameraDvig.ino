String chiselko
 
int skorost() {
 
  if (knopka1 == true) {  //если нажата эта кнопка,то число прибавляется к скорости
 
    if (pokaz == 2385618) {
      chiselro = chiselko + "1";
    }
    if (pokaz == 2386574) {
      chiselro = chiselko + "2";
    }
    if (pokaz == 8485618) {
      chiselro = chiselko + "3";
    }
    if (pokaz == 2847253) {
      chiselro = chiselko + "4";
    }
    if (pokaz == 8296056) {
      chiselro = chiselko + "5";
    }
    if (pokaz == 2643758) {
      chiselro = chiselko + "6";
    }
    if (pokaz == 7236358) {
      chiselro = chiselko + "7";
    }
    if (pokaz == 9465277) {
      chiselro = chiselko + "8";
    }
    if (pokaz == 6751726) {
      chiselro = chiselko + "9";
    }
    if (pokaz == 2389884) {
      chiselro = chiselko + "0";
    }
 
    if (knopkaOK == true) {
      int chislo = chiselko;
      skor = skor + chislo;
    }
  }
  if (knopka2 == true) { //если нажата эта кнопка,то число отнимается от скорости
    if (pokaz == 2385618) {
      chiselro = chiselko + "1";
    }
    if (pokaz == 2386574) {
      chiselro = chiselko + "2";
    }
    if (pokaz == 8485618) {
      chiselro = chiselko + "3";
    }
    if (pokaz == 2847253) {
      chiselro = chiselko + "4";
    }
    if (pokaz == 8296056) {
      chiselro = chiselko + "5";
    }
    if (pokaz == 2643758) {
      chiselro = chiselko + "6";
    }
    if (pokaz == 7236358) {
      chiselro = chiselko + "7";
    }
    if (pokaz == 9465277) {
      chiselro = chiselko + "8";
    }
    if (pokaz == 6751726) {
      chiselro = chiselko + "9";
    }
    if (pokaz == 2389884) {
      chiselro = chiselko + "0";
    }
 
    if (knopkaOK == true) {
      int chislo = chiselko;
      skor = skor + chislo;
    }
  }
  return skor;
 chiselko = ""; 
}
