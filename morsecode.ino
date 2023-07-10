
    const int buzzerPin = 12;          // set buzzer pin
    const int ledPin = 6;              // set LED pin
    
    String strInput = ""; 
      
    // Morse Code Specific delays
    int mcUnit = 75;                    // length of time in milliseconds
                                        //   mcUnit is the only value that
                                        //   needs adjusting
    int dit = mcUnit;                   // dit equals 1 unit of time
    int dah = 3 * mcUnit;               // dah equals 3 units of time
    int partsDelay = mcUnit;            // 1 unit between parts 
    int letterDelay = 3 * mcUnit;       // 3 units between letters
    int wordDelay = 7 * mcUnit;         // 7 units between words
   
    String letters[] = {
        ".-", "-...", "-.-.", "-..",      // A-D
        ".", "..-.", "--.", "....",       // E-H
        "..", ".---", "-.-", ".-..",      // I-L
        "--", "-.", "---", ".--.",        // M-P
        "--.-", ".-.", "...", "-",        // Q-T
        "..-", "...-", ".--", "-..-",     // U-X
        "-.--", "--.."                    // Y-Z
    };

    String numbers[] = {
        "-----", ".----", "..---",       // 0-2 
        "...--", "....-", ".....",       // 3-5
        "-....", "--...", "---..",       // 6-8
        "----."                          // 9
    };                        

    String Lpunctuation[] = {
        "-.-.--",   // ! -33
        ".-..-.",   // " -34
        "X",        // # -35
        "...-..-",  // $ -36
        "X",        // % -37  
        ".-...",    // & -38
        ".----.",   // ' -39
        "-.--.",    // ( -40
        "-.--.-",   // ) -41
        "X",        // * -42
        ".-.-.",    // + -43
        "X",        // , -44
        "-....-",   // - -45
        ".-.-.-",   // . -46
        "-..-.",    // / -47
    };

    String Upunctuation[] = {
        "---...",   // : -58
        "-.-.-.",   // ; -59
        "X",        // < -60
        "-...-",    // = -61
        "X",        // > -62
        "..--..",   // ? -63
        ".--.-."    // @ -64
    };

    String SpecialCase[] = {
        "_", "..--.-",   // _ -95
    };
    
    String ProSigns[] = {
        "...-.-",   // End Of Work
        "........", // Error
        "-.-",      // Invitation to Transmit - K
        "-.-.-",    // Starting Signal
        ".-.-.",    // New Page Signal - +
        "...-.",    // Understood  - also Ŝ
        ".-..."     // Wait - also &
    };
void setup() {
    Serial.begin(9600);                         // Used for Serial Monitor
    pinMode(buzzerPin, OUTPUT);                 // set buzzer pin, OUTPUT automatically sets it to LOW
    pinMode(ledPin, OUTPUT);                    // set LED pin, OUTPUT automatically sets it to LOW
}

void DitOrDah(int DitOrDahDelay){ //this turns on and off the buzzwe and led accirding to teh delay values
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(DitOrDahDelay);                       // dit or dah delay
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
}

void doSequence(String sequence){
    int strLen = 0;
    char chValue = "";

    strLen = sequence.length();                 // get length of string
    for (int x = 0; x < strLen; x++){           // loop through all parts
        chValue = sequence.charAt(x);           // get char value
        if(chValue == '.'){
            DitOrDah(dit);                      // do a Dit
            Serial.print(".");                  // print dit
        }
        else if (chValue == '-'){
            DitOrDah(dah);                      // do a Dah
            Serial.print("-");                  // print dah
        }
        else if (chValue == 'X'){               // No translation
            Serial.print("X - no translation"); // print no translation 
        }
        else if (int(chValue) == 0){            // Null char in string
            // Null found                       // May not be needed
            // Do Nothing                       // should be skipped
        } // end if statement

        if (x < strLen) delay(partsDelay);      // gap between parts
    } // end for
    Serial.println("");
} // end doSequence

void translate(char ch){
    /* Recall that letters have a numeric ASCII value
     * so you can add and substract them
     * a = 97
     * A = 65
     * 0 = 48
     * 
     * Modify this function for additonal character checks 
     * ASCII numerical order below (mostly)
     */
    if (ch >= '!' && ch <= '/'){
        Serial.print(ch);
        Serial.print(" ");
        doSequence(Lpunctuation[ch - '!']);    
    } else if (ch >= '0' && ch <= '9'){
        Serial.print(ch);
        Serial.print(" ");
        doSequence(numbers[ch - '0']);
    } else if (ch >= ':' && ch <= '@'){
        Serial.print(ch);
        Serial.print(" ");
        doSequence(Upunctuation[ch - ':']);    
    } else if (ch >= 'A' && ch <= 'Z'){
        Serial.print(ch);
        Serial.print(" ");
        doSequence(letters[ch - 'A']);
    } else if (ch >= 'a' && ch <= 'z'){
        Serial.print(ch);
        Serial.print(" ");
        doSequence(letters[ch - 'a']);
    } else if (ch == '_'){
        Serial.print(ch);
        Serial.print(" ");
        doSequence(SpecialCase[1]);    
    } else {
        Serial.println("invalid character");
        // Skip invalid character
    } // end if statement
} // end translate

void doString(String strValue){
    // Local Variables
    char chValue;                               
    int strLen = 0;

    strLen = strValue.length();                 // get the length of the string

    for(int x = 0; x <= strLen; x++){           // loop through string to get each value
        chValue = strInput.charAt(x);           // get char at position x

        if (chValue == " " || int(chValue) == 32){// Found Space between words
            Serial.println("(space)");
            delay(wordDelay);                   // delay between words
        } else if (int(chValue) == 13){         // Carrige Return found
            // Do Nothing
        } else if (int(chValue) == 10){         // Line feed found
            // Do Nothing
        } else if (int(chValue) == 0){          // Null found
            // Do Nothing
        } else {
            translate(chValue);                 // translate character to Morse Code
            if (x < strLen) delay(letterDelay); // delay between letters
        } // end if
    } // end for
} // end doString
void loop(){
    // main code here, runs repeatedly
    Serial.println("Enter a string you want to translate to Morse Code");

    // Use the Serial Monitor to input a message
    while (Serial.available() == 0){
        // Do Nothing                           // Wait until user input
    }
    strInput = Serial.readString();             // get the new String
    Serial.print(" Translating => ");
    Serial.println(strInput);                   // print string to serial monitor
    doString(strInput);                         // go to doString function

    delay(1000);                                // delay between loops
    Serial.println("");                         // Add a new line feed to serial monitor   
    strInput="";                                // clear the string
}
