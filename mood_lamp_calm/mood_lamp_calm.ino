/* Creation Crate Mood Lamp
    This lamp smoothly cycless through a colour spectrum
    It only turns on when its sourroundings are dark
    Color equations : we need to know dis ltr!
      Red = sin(x)
      Green = sin(x + PI/3)
      Blue = sin(x + 2*PI/3)
      These equations are how the program will calculate the brightness of the LEDs.

      Question: what is X? Time? Sensor??
*/

/* Step 1: Input User Defined Variables
    Think of vars like containers---the store values!
    You can replace this with any whole number
*/
int pulseSpeed = 5; // orig val 5

// These are the pins we are using with the UNO R3 (Arduino compatible)
// Yutes can see these nums on the board itself

int ldrPin = 0; //LDR in Analog Input 0 to read the surrounding light
int redLed = 11; // red LED in Digital pin 11
int greenLed = 10; // green LED in Digital pin 10
int blueLed = 9; // blue LED in Digital pin 9

int ambientLight; // This variable stores the value of the light in the room
int power = 255; // This variable controls the brightness of the lamp (2-255)

float RGB[3]; // This is an 'array'. It can hold 3 values: RGB[0], RGB[1], RGB[2]. We'll use this to store the values of Red, Blue and Green LEDs.

/* We will be using the value of 180/PI a lot in the main loop so to save time we will calculate
    it once here in the setup and store it in CommonMathVariable. Note: it is PI(i), not P1(one)
*/
float CommonMathVariable = 180 / PI;

/* Step 2: Create Setup Loop
    This 'loop' is not really a loop. It runs once in the beginning to create the default
    values for our LEDs.
*/

void setup() {
  // This tells the UNO R3 to send data out to the LEDs
  pinMode (redLed, OUTPUT);
  pinMode (greenLed, OUTPUT);
  pinMode (blueLed, OUTPUT);

  //This sets all the output (LEDs) to low (as in off), so that they do not turn on during startup
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(blueLed, LOW);
} // Opening brackets must be accompanied by closing brackets

/* Step 3: Create Main Loop
    The previous sections are where we set up the variables. This section is where we put them to work!!
    This part of the program is a real 'loop'. It repeats itself over and over again,
    making small changes in the brightness of the LEDs each time - this creates a smooth transition
    in the coloooouuurrr.
*/

void loop () {
  for (float x = 0; x < PI; x = x + 0.000088) {
    RGB[0] = (power/2.14149) * abs(sin(x * (CommonMathVariable)));
    RGB[1] = power * abs(sin( (x + PI / 3) * (CommonMathVariable)));
    RGB[2] = (power/2.14149) * abs(sin( (x + (2 * PI) / 3) * (CommonMathVariable)));

    // This reads the light in the room and stores it as a number
    ambientLight = analogRead(ldrPin);

    // This 'if statement' will make the lamp turn on only if it is dark.
    // The darker it is the higher the number
    if (ambientLight > 900) {
      // These 'analog write' statements will send the values calculated above to the LEDs
      analogWrite(redLed, RGB[0]);
      analogWrite(greenLed, RGB[1]);
      analogWrite(blueLed, RGB[2]);
    } // don't forget to close the 'if statement' with bracket, you punk!
    else  // This 'else statement' will only activate if the 'if statement' above does not (i.e. it is too bright
      // in the room). The LEDs will turn off. (or else!!!)
    {
      analogWrite(redLed, LOW);
      analogWrite(greenLed, LOW);
      analogWrite(blueLed, LOW);
    }
    // This loop calculates the delay for each color depending on its current brightness. Brighter LEDs will change color slower
    // and vice versa.
    for (int i = 0; i < 3; i = i + 1) {
      if (RGB[i] < 1) {
        delay (20 * pulseSpeed); // delay by 20 pulses by default
      }
      // 'else if' means only one of these conditions can activate at a time
      else if (RGB[i] < 5) {
        delay (10 * pulseSpeed);
      }

      else if (RGB[i] < 10) {
        delay (2 * pulseSpeed);
        
      }     
      else if (RGB[i] < 100) {
        delay (1 * pulseSpeed); 
      }
      // This blank 'else statement' is a fail-safe mechanism. It instructs the program to do nuffin
      // if the conditions above don't activate. This prevents the programme from generating errors when calculating delays.
      else {}
    }
    // This delay gives the light-dependent resistoid time to settle and give accurate readings.
   
    delay(1);
  }
}// Remember to close yer brackets!!!
