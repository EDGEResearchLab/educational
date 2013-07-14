/*
    File:   TiltOMeter.ino
    Description:
        Read input from a 3axis accel and illuminate leds to reflect 
        the direction the board is tilted.  Maintain a center LED on 
        a PWM pin to reflect the z-axis.  Fade from 255 to 0 as the 
        board is tilted away from vertical z.
    Date:
        July 13, 2013 - Engineering Bootcamp
*/

// Declare constants for the pins we will be using
// for inputs (A0-A3) and outputs, digital 2 - 10.
// A shortcut to not having to type "const int" for
// each value is to use a "," instead of a ";" to 
// list subsequent const ints and use a ";" on the
// last one
const int   redNorth = 6,
            redWest = 9,
            redSouth = 7,
            redEast = 8,
            yellowNorth = 2,
            yellowWest = 5,
            yellowSouth = 3,
            yellowEast = 4,
            greenCenter = 10,

            accelX = A3,
            accelY = A2,
            accelZ = A1,
            refVolt = A0;

// This function is ran once per power-cycle at power on
// or reset.
void setup()
{
    // Set up the serial output baud rate (9600 = default)
    // Note: White space does not matter in C type languages
    // so you could put Serial.begin(9600); or
    // Serial.begin(       9600); and it will work.  It's
    // mostly a stylistic preference.
    Serial.begin( 9600 );
  
    // A quick way to initialize
    // all the pins to output, defaulted to off.
    // We are using pins 2 - 10 (digital).
    for( int i = 2; i < 11; i++ )
    {
        pinMode( i, OUTPUT );
        digitalWrite( i, LOW ); 
    }
}

// This function runs on repeat, so keep it as clean as possible
// to make your code easily maintainable.  Always assume that 
// you may find use in your code for a future project, even
// if it is just pieces - documentation speeds up remembering
// algorithms and design.
void loop()
{
  // Get a reference reading, used to sanitize the accel reads.
  int refRead = analogRead( refVolt );
  // Read the x/y/z and adjust for the read.
  // Note: we aren't converting to G-force, all we need to
  // do is have consistent results for us to work with when
  // determining which axis we are tilted on.
  int axisXVal = analogRead( accelX ) - refRead / 2;
  int axisYVal = analogRead( accelY ) - refRead / 2;
  int axisZVal = analogRead( accelZ ) - refRead / 2;

  // Set the green value based on the z axis.
  // We put the calculation for the value into the
  // GetCenterPWMVal() function to demonstrate function use.
  analogWrite( greenCenter, GetCenterPWMVal( axisZVal ) );

  // We also broke out the handling of turning on and off LEDS
  // and the conditional processing.
  IlluminateLeds( axisXVal, axisYVal );

  // Print the reads to the serial monitor, helps with debugging reads!
  Serial.println( axisXVal );
  Serial.println( axisYVal );
  Serial.println( axisZVal );
  Serial.println();

  // A short delay is necessary when dealing with analog sensors.
  delay(2);
}

/*
    Determine the PWM value for the
    center led (green).  Return the
    calculated value (integer).

    Remember that a function is set up with:
    returnDataType FunctionName( parameters )
    {
        // Function content...

        return Variable; // variable should be of type returnDataType
    }
*/
int GetCenterPWMVal( int zvalue )
{
    zvalue = zvalue * 2;

    if (zvalue < 0)
    {
        zvalue = 0;
    }
    else if (zvalue > 255)
    {
        zvalue = 255;
    }

    return zvalue;
}

/*
    Illuminate the LEDs based on the passed in
    values for axis X and Y.  No return type
    because the function is "void", and we really
    have no reason to return anything.
*/
void IlluminateLeds(int axisXVal, int axisYVal)
{
    // Manually trace through your conditions to make sure you aren't
    // supersceding yourself on accident.
    // What would happen if you had this first if and the next
    // else if conditions flopped? (i.e. if(axisYVal >= 60)...
    // else if (axisYVal >= 120) ...)  Would you get the same result?
    if ( axisYVal >= 120 )
    {
        digitalWrite(redNorth, HIGH);
        digitalWrite(yellowNorth, HIGH);
        digitalWrite(redSouth, LOW);
        digitalWrite(yellowSouth, LOW);
    }
    else if ( axisYVal >= 60 )
    {
        digitalWrite(redNorth, LOW);
        digitalWrite(yellowNorth, HIGH);
        digitalWrite(redSouth, LOW);
        digitalWrite(yellowSouth, LOW);
    }
    else if ( axisYVal <= -120 )
    {
        digitalWrite(redNorth, LOW);
        digitalWrite(yellowNorth, LOW);
        digitalWrite(redSouth, HIGH);
        digitalWrite(yellowSouth, HIGH);
    }
    else if ( axisYVal <= -60 )
    {
        digitalWrite(redNorth, LOW);
        digitalWrite(yellowNorth, LOW);
        digitalWrite(redSouth, LOW);
        digitalWrite(yellowSouth, HIGH);
    }
    // If none of the above conditions are met, 
    // this is what you do.  In our case, the
    // logic here is that if it's not
    // above 120/60 or below -60/-120, then it is
    // flat (i.e. between 60 and -60)
    else
    {
        digitalWrite(redNorth, LOW);
        digitalWrite(yellowNorth, LOW);
        digitalWrite(redSouth, LOW);
        digitalWrite(yellowSouth, LOW);
    }

    if ( axisXVal >= 120 )
    {
        digitalWrite(redEast, HIGH);
        digitalWrite(yellowEast, HIGH);
        digitalWrite(redWest, LOW);
        digitalWrite(yellowWest, LOW);
    }
    else if ( axisXVal >= 60 )
    {
        digitalWrite(redEast, LOW);
        digitalWrite(yellowEast, HIGH);
        digitalWrite(redWest, LOW);
        digitalWrite(yellowWest, LOW);
    }
    else if ( axisXVal <= -120 )
    {
        digitalWrite(redEast, LOW);
        digitalWrite(yellowEast, LOW);
        digitalWrite(redWest, HIGH);
        digitalWrite(yellowWest, HIGH);
    }
    else if ( axisXVal <= -60 )
    {
        digitalWrite(redEast, LOW);
        digitalWrite(yellowEast, LOW);
        digitalWrite(redWest, LOW);
        digitalWrite(yellowWest, HIGH);
    }
    else
    {
        digitalWrite(redEast, LOW);
        digitalWrite(yellowEast, LOW);
        digitalWrite(redWest, LOW);
        digitalWrite(yellowWest, LOW);
    }
}
