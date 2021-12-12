const int button1 = 2;
const int button2 = 3;
const int button3 = 4;
const int button4 = 5;
const int led1 = 7; //yellow LED
const int led2 = 8; //green LED
const int led3 = 9; //red LED
const int led4 = 10; //blue LED
const int buzzer = 12;
const int tones[] = {1900, 1600, 1300, 1000, 3200};
int buttonState [] = {0,0,0,0};

int lastButtonstate[] = {0,0,0,0};
int buttonCounter[] = {0,0,0,0};
int gameOn = 0;
int wait = 0;
int currentLevel = 1;
int dlay = 500;
int ledTime = 500;
int n_levels = 4; // current number of levels; 
int n_levels_orig = n_levels; // fall back to orig on fail
int pinAndTone = 0;
int correct = 0;
int speedFactor = 4;
int speedFactorOrig = speedFactor;
int ledDelay = 200;

void playTone (int tone, int duration){
  for (long i = 0; i < duration * 1000L; i += tone*2)
  {
    digitalWrite(buzzer, HIGH);
    delayMicroseconds(tone);
    digitalWrite(buzzer, LOW);
    delayMicroseconds(tone);
  }
}

void winnerTune(){
  playTone(700, 100);
  playTone(500, 100);
  playTone(200, 100);
  playTone(100, 100);
}

void looserTune(){
  playTone(100, 200);
  playTone(200, 200);
  playTone(500, 200);
  playTone(700, 200);

  //buzz
  delay(100);
  playTone(8000, 100);
  delay(100);
  playTone(8000, 100);
}

void looserTuneOrig(){
    int i;
    for (i=0; i < 4; i = i + 1){
     digitalWrite(i+7, HIGH);
    }
    playTone(tones[4], ledTime);
    for (i=0; i < 4; i = i + 1){
     digitalWrite(i+7, LOW);
    }
    delay(200);
    for (i=0; i < 4; i = i + 1){
     digitalWrite(i+7, HIGH);
    }
    playTone(tones[4], ledTime);
    for (i=0; i < 4; i = i + 1){
     digitalWrite(i+7, LOW);
    }   
}
void setup() {
  randomSeed(analogRead(0));

  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(buzzer, OUTPUT);
}
void loop() {
  int n_array[n_levels];
  int u_array[n_levels];
  int i;
  if (gameOn == 0 ){
    for (i=0; i < n_levels; i=i+1){
      n_array[i]=0;
      u_array[i]=0;
      n_array[i]=random(0,4);
    }
    gameOn=1;
  }
  if (wait == 0) {
    delay (200);
    i = 0;
    for (i = 0; i < currentLevel; i=i+1){
      ledDelay = ledTime/(1+(speedFactor/n_levels) * (currentLevel - 1));
      pinAndTone = n_array[i];
      digitalWrite(pinAndTone +7, HIGH);
      playTone(tones[pinAndTone ], ledDelay);
      digitalWrite(pinAndTone +7, LOW);
      delay(100/speedFactor);
    }
    wait = 1;
  }
  i = 0;
  int buttonChange = 0;
  int j = 0;
  while (j < currentLevel){
    while (buttonChange == 0){
      for (i = 0; i < 4; i=i+1){
        buttonState[i] = digitalRead(i+2);
        buttonChange+= buttonState[i];
      }
    }
    for (i = 0; i < 4; i=i+1){
      if (buttonState[i] == HIGH) {
        digitalWrite(i+7, HIGH);
        playTone(tones[i], ledTime);
        digitalWrite(i+7, LOW);
        wait = 0;
        u_array[j]=i;
        buttonState[i] = LOW;
        buttonChange=0;
      }
    }
    if (u_array[j] == n_array[j]){
      j++;
      correct = 1;
    }
    else{
      correct = 0;
      i = 4;
      j = currentLevel;
      wait = 0;
    }
  }
  if (correct == 0){
    delay(300);
    i = 0;
    gameOn=0;
    currentLevel=1;
    n_levels = n_levels_orig;
    speedFactor = speedFactorOrig;
    
    for (i=0; i < 4; i = i + 1){
      digitalWrite(i+7, HIGH);
    }
    looserTune();
    for (i=0; i < 4; i = i + 1){
      digitalWrite(i+7, LOW);
    }
    
    delay(500);
    gameOn = 0;
  }
  if (correct == 1){
    currentLevel++;
    delay (100);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    winnerTune();
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
  }
  if (currentLevel == n_levels){
    delay(500);
    int notes[] = {2, 2, 2,2, 0, 1, 2, 1, 2};
    int note = 0;
    int tempo[] {200, 200, 200, 400, 400, 400, 200, 200, 600};
    int brakes[] = {100, 100, 100, 200, 200, 200, 300, 100, 200};
    for (i=0; i<9; i=i+1){
      note = notes[i];
      digitalWrite(note+7, HIGH);
      playTone(tones[note], tempo[i]/4);
      digitalWrite(note+7, LOW);
      delay(brakes[i]);
    }
    gameOn = 0;
    currentLevel = 1;
    n_levels = n_levels + 2;
    speedFactor += 1;
  }
}
