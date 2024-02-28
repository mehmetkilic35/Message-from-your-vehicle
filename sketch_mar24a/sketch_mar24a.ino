int esikDegeri = 270;
int red = 8;
int green = 10;
int deger;
void setup() {
}

void loop() {
  deger = analogRead (A0) ;
  if ( deger > esikDegeri ) {
    digitalWrite (red, HIGH);
    delay(50);
    dijitalWrite (red, LOW);
    delay(50);

    dijitalWrite (green, LOW);
  }
  else {
    dijitalWrite (red, LOW);

    dijitalWrite (green, HIGH);
    delay(700);
    dijitalWrite (green, HIGH);
    delay(700);
  }
}
