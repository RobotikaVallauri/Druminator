//librerie aggiuntive
#include <Servo.h>

//Pulsanti
#define btnStop 13 //pulsante Stop
#define btnHiHat 2 //pulsante HiHat 
#define btnSnare 4 //pulsante Snare 
#define btnFloorTom 6 //pulsante FloorTom 

//Led
#define ledHiHat 3 //led HiHat 
#define ledSnare 5 //led Snare 
#define ledFloorTom 7 //led FloorTom 

//Buzzer
#define buzzer 12 

//Servo
Servo FloorTom;
Servo Snare;
Servo HiHat;

//Variabili globali
int beat = 120;//battiti per minuto
int d = 250; //delay tra un battito e un altro relazionato direttamente a beat
int cont = 0; //contatore
char msg; //messaggio BlueTooth
int rnd = -1; //Random 
int contTeach =0; //contatore per la modalità teach
bool state = false;//utilizzata per il controllo nella modalità teach

/*
 *  Significato Messaggi BlueTooth
 *
 *	0-->Intervallo
 *	1-->Battito Singolo HiHat
 *	2-->Battito Singolo Snare
 *	3-->Battito Singolo Kick
 *	4-->Ritmo Rock
 *	5-->Ritmo Valzer
 *	6-->Ritmo Blues
 *	7-->Battito Singolo HiHat & Snare
 *	8-->Battito Singolo HiHat & Kick
 *	9-->Battito Singolo Kick & Snare
 *	A-->+ Beat
 *	B-->- Beat
 *	C-->Modalità Teach    
 *	D-->Ritmo Dance
 *	E-->Battito Singolo HiHat & Kick & Snare
 */

void setup() {//impostazione dei componenti INPUT e OUTPUT
  //drum set
  FloorTom.attach(11);
  Snare.attach(10);
  HiHat.attach(9);
  //Le bacchette si posizionano in automatico sullo strumento
  FloorTom.write(0);
  Snare.write(0);
  HiHat.write(0);
  //HC-06
  Serial.begin(9600);
  //Button 
  pinMode(btnStop, INPUT);
  pinMode(btnHiHat, INPUT);
  pinMode(btnSnare, INPUT);
  pinMode(btnFloorTom, INPUT);
  //Led
  pinMode(ledHiHat, OUTPUT);
  pinMode(ledSnare, OUTPUT);
  pinMode(ledFloorTom, OUTPUT);
  //Buzzer
  pinMode(buzzer, OUTPUT);
}

void loop() {
  randomSeed(millis());//impostazione del random
  noTone(buzzer);//disabilito buzzer
  cont=0; //resetto la variabile contatore
  
  if (Serial.available() > 0) {//Se arriva il messaggio
    msg = Serial.read(); //Legge il dato in arrivo e lo mette in msg
    Serial.print(msg); //Stampa il valore del dato nel Serial monitor (per vederlo ricordate di impostare il baund a 38400)    
    //controllo del messaggio e di conseguenza si compie l'azione corrispondente
	switch (msg) {
	  case '0':
	    delay(250);
		break;
      case '1':
        ColpoH();
        break;
      case '2':
        ColpoS();
        break;
      case '3':
        ColpoF();
        break;
      case '4':
	  //viene ripetuta la sequeza per 10 volte o si ferma prima se se viene fermata con il tasto stop
        while(digitalRead(btnStop)==LOW && cont<10){
          Rock();
          cont++;
        }
        //ritmo di chiusura
		Chiusura();
        break;
      case '5':
	  //viene ripetuta la sequeza per 10 volte o si ferma prima se se viene fermata con il tasto stop
        while(digitalRead(btnStop)==LOW && cont<10){
          Valzer();
          cont++;
        }
        //ritmo di chiusura
		Chiusura();
        break;
      case '6':
	  //viene ripetuta la sequeza per 10 volte o si ferma prima se se viene fermata con il tasto stop
        while(digitalRead(btnStop)==LOW && cont<10){
          Blues();
          cont++;
        }
		//ritmo di chiusura
        Chiusura();
        break;
      case '7':
        ColpoHS();
        break;
      case '8':
        ColpoHF();
        break;
      case '9':
        ColpoSF();
        break;
      case 'A':
        beat+=10;
        d = (60000/beat)-250; //1 minuto / numero di Beat - (Il tempo di battito)
        break;
      case 'B':
        beat-=10;
        d = (60000/beat)-250; //1 minuto / numero di Beat - (Il tempo di battito)
        break;
      case 'C': //Modalità insegnante a 60 bpm
        while(digitalRead(btnStop)==LOW){
          //random tra i 3 led possibili
		  rnd = random(3);
          contTeach=0;
          state=false;
		  //3 casi
          switch(rnd){
			
			//caso HiHat
            case 0:
			  //accensione
              digitalWrite(ledHiHat, HIGH);            
			  //si ha tempo un secondo per effettuare il battito
              while(contTeach<750 && state==false){
                if(digitalRead(btnHiHat)==HIGH){
                  state = true;
                }
                delay(1);
                contTeach++;
              }
              digitalWrite(ledHiHat, LOW);
              if(state == true)//effettua il battito
                ColpoH();
              else{//segnala l'errore
                tone(buzzer, 500, 200);
                delay(250);
                noTone(buzzer);
              }              
              delay(250);
              break;
			
			
			//caso Snare
            case 1:
			//accensione
            digitalWrite(ledSnare, HIGH);
			//si ha tempo un secondo per effettuare il battito
            while(contTeach<750 && state==false){
                if(digitalRead(btnSnare)==HIGH){
                  state = true;
                }
                delay(1);
                contTeach++;
              }
            digitalWrite(ledSnare, LOW);
            if(state == true)//effettua il battito
              ColpoS();
            else{//segnala l'errore
              tone(buzzer, 500, 200);
              delay(250);
              noTone(buzzer);
            }     
            delay(250);
              break
			//caso FloorTom  
            
			
			case 2:
            digitalWrite(ledFloorTom, HIGH);
			//si ha tempo un secondo per effettuare il battito
            while(contTeach<750 && state==false){
                if(digitalRead(btnFloorTom)==HIGH){
                  state = true;
                }
                delay(1);
                contTeach++;
              }
            digitalWrite(ledFloorTom, LOW);
            if(state == true)//effettua il battito
              ColpoF();
            else{//segnala l'errore
              tone(buzzer, 500, 200);
              delay(250);
              noTone(buzzer);
            }    
            delay(250);
              break;
          }
        }
        break;
      case 'D':
	  //viene ripetuta la sequeza per 10 volte o si ferma prima se se viene fermata con il tasto stop
        while(digitalRead(btnStop)==LOW && cont<10){
          Dance();
          cont++;
        }
        Chiusura();
        break;
      case 'E':
        ColpoHSF();
        break;
    }
  }
  //Controllo Manuale
  else if(digitalRead(btnHiHat)==HIGH){
     ColpoH();
     digitalWrite(ledHiHat, HIGH);
     delay(250);
  }  
  else if(digitalRead(btnSnare)==HIGH){
     ColpoS();
     digitalWrite(ledSnare, HIGH);
     delay(250);
  }
  else if(digitalRead(btnFloorTom)==HIGH){
     ColpoF();
     digitalWrite(ledFloorTom, HIGH);
     delay(250);
  }
  digitalWrite(ledHiHat, LOW);
  digitalWrite(ledSnare, LOW);
  digitalWrite(ledFloorTom, LOW);
}

//ritmo Rock
void Rock() {
  //H  |o-o-o-o|
  //S  |----o--|  
  //F  |o------|
  ColpoHF();
  delay(d);
  ColpoH();
  delay(d);
  ColpoHS();
  delay(d);
  ColpoH();
  delay(d);
}

//ritmo Dance
void Dance(){
  //H  |--o---o|
  //S  |----o--|  
  //F  |o---o--|
  ColpoF();
  delay(d);
  ColpoH();
  delay(d);
  ColpoSF();
  delay(d);
  ColpoH();
  delay(d);
}

//ritmo Chiusura
void Chiusura(){
  //H  |------o|
  //S  |o-o-o-o|  
  //F  |o-o-o--|
  ColpoSF();
  delay(d);
  ColpoSF();
  delay(d);
  ColpoSF();
  delay(d);
  ColpoHS();
  delay(d);
}

//ritmo Valzer
void Valzer() {
  //H  |o-o-o|
  //S  |--o-o|  
  //F  |o----|
  ColpoHF();
  delay(d);
  ColpoHS();
  delay(d);
  ColpoHS();
  delay(d);
}

//ritmo Blues
void Blues() {
  //H  |o-o-o-o-o-o|
  //S  |------o----|  
  //F  |o---------o|
  ColpoHF();
  delay(d);
  ColpoH();
  delay(d);
  ColpoH();
  delay(d);
  ColpoHS();
  delay(d);
  ColpoH();
  delay(d);
  ColpoHF();
  delay(d);
}

//Procedura Battito Hihat
void ColpoH() {
  HiHat.write(20);
  delay(150);
  HiHat.write(0);
  delay(100);
  HiHat.write(20);
}

//Procedura Battito Floor Tom
void ColpoF() {
  FloorTom.write(20);
  delay(150);
  FloorTom.write(0);
  delay(100);
  FloorTom.write(20);
}

//Procedura Battito Snare
void ColpoS() {
  Snare.write(20);
  delay(150);
  Snare.write(0);
  delay(100);
  Snare.write(20);
}

//Procedura Battito Snare & Floor Tom
void ColpoSF() {
  Snare.write(20);
  FloorTom.write(20);
  delay(150);
  Snare.write(0);
  FloorTom.write(0);
  delay(100);
  Snare.write(20);
  FloorTom.write(20);
}

//Procedura Battito HiHat & Floor Tom
void ColpoHF() {
  HiHat.write(20);
  FloorTom.write(20);
  delay(150);
  HiHat.write(0);
  FloorTom.write(0);
  delay(100);
  HiHat.write(20);
  FloorTom.write(20);
}

//Procedura Battito HiHat & Snare
void ColpoHS() {
  HiHat.write(20);
  Snare.write(20);
  delay(150);
  HiHat.write(0);
  Snare.write(0);
  delay(100);
  HiHat.write(20);
  Snare.write(20);
}

//Procedura Battito HiHat & Snare & Floor Tom
void ColpoHSF(){
  HiHat.write(20);
  Snare.write(20);
  FloorTom.write(20);
  delay(150);
  HiHat.write(0);
  Snare.write(0);
  FloorTom.write(0);
  delay(100);
  HiHat.write(20);
  Snare.write(20);
  FloorTom.write(20);
}