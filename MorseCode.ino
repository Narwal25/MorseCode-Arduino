// Declaration of All Pins
const int yellowLEDPin = 5;
const int redLEDPin = 6;  // PWM compaitible for BC command.
const int greenLEDPin = 7;
const int orangeLEDPin = 8;
const int blueLEDPin = 9;
const int irReceiverPin = 10;
const int potentiometerPin = A0;

void setup() {
  Serial.begin(9600);
  // Specifing the Pin Mode
  pinMode(yellowLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(orangeLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
  pinMode(potentiometerPin, INPUT);
  pinMode(irReceiverPin, INPUT);
  
}

void loop() {
  if (Serial.available() > 0) {

    String input = Serial.readStringUntil('\n');
    String output;

    // Identifying Text
    if (input.startsWith("*") || input.startsWith("?") || input.startsWith("<")) {
      output = decodeFromBCCode(input);
      Serial.println(output);
      flashLED(greenLEDPin, input);
    } else {
      output = encodeToBCCode(input);
      Serial.println(output);
      flashLED(blueLEDPin, output);
    }
    if (input.startsWith("B") || input.startsWith("PB") || input.startsWith("BC") || input.startsWith("BB")) {
      handleCommand(input);
    }
  } 
}

// Encode Translation
String encodeCharacter(char c) {
  switch (c) {
    case 'A': return "*";
    case 'B': return "??*";
    case 'C': return "?*<";
    case 'D': return "*?";
    case 'E': return "?";
    case 'F': return "?*?";
    case 'G': return "?**"; 
    case 'H': return "**";
    case 'I': return "??";
    case 'J': return "*??"; 
    case 'K': return "*<";
    case 'L': return "??<";
    case 'M': return "?*";
    case 'N': return "<*";
    case 'O': return "<?";
    case 'P': return "?<";
    case 'Q': return "**<"; 
    case 'R': return "?<*";
    case 'S': return "?<?"; 
    case 'T': return "<";
    case 'U': return "<<";
    case 'V': return "***"; 
    case 'W': return "**?"; 
    case 'X': return "*<?";
    case 'Y': return "?<<"; 
    case 'Z': return "*?<";
    case '1': return "<??*";
    case '2': return "<??<";
    case '3': return "<?*?";
    case '4': return "<?**";
    case '5': return "<?*<";
    case '6': return "<?<?";
    case '7': return "<?<*";
    case '8': return "<?<<";
    case '9': return "<*??";
    case '0': return "<???";
    case '.': return "<??";
    case ',': return "<?*";
    case '\'': return "<*?";
    case '?': return "<**";
    case '!': return "<*<";
    case '+': return "<<?";
    case '-': return "<<*";
    case '*': return "<<<";
    case '/': return "<?<";
    case '=': return "<***<";
    
    default: return String(c);  // If character is not in Translation, keep it unchanged
  }
}


// Encode Function
String encodeToBCCode(String input) {
  String bcCode = "";

  char prevChar = ' ';

  for (char c : input) {
    c = toupper(c);
    String encoded = encodeCharacter(c);

    if (prevChar != ' ' && c != ' ') {
      bcCode += "~";
    } else if ( c == ' ') {
      bcCode += " ";
      prevChar = c;
      continue;
    }

    if (!encoded.equals("~") && !encoded.equals(" ")) {
      bcCode += encoded;
    }

    prevChar = c;
  }

  return bcCode;
}


// Decode Translation
String decodeToken(String token) {
  if (token == "*") return "A";
  else if (token == "??*") return "B";
  else if (token == "?*<") return "C";
  else if (token == "*?") return "D";
  else if (token == "?") return "E";
  else if (token == "?*?") return "F";
  else if (token == "?**") return "G";
  else if (token == "**") return "H";
  else if (token == "??") return "I";
  else if (token == "*??") return "J";
  else if (token == "*<") return "K";
  else if (token == "??<") return "L";
  else if (token == "?*") return "M";
  else if (token == "<*") return "N";
  else if (token == "<?") return "O";
  else if (token == "?<") return "P";
  else if (token == "**<") return "Q";
  else if (token == "?<*") return "R";
  else if (token == "?<?") return "S";
  else if (token == "<") return "T";
  else if (token == "<<") return "U";
  else if (token == "***") return "V";
  else if (token == "**?") return "W";
  else if (token == "*<?") return "X";
  else if (token == "?<<") return "Y"; 
  else if (token == "*?<") return "Z";
  else if (token == "<??*") return "1";
  else if (token == "<??<") return "2";
  else if (token == "<?*?") return "3";
  else if (token == "<?**") return "4";
  else if (token == "<?*<") return "5";
  else if (token == "<?<?") return "6";
  else if (token == "<?<*") return "7";
  else if (token == "<?<<") return "8";
  else if (token == "<*??") return "9";
  else if (token == "<???") return "0";
  else if (token == "<??") return ".";
  else if (token == "<?*") return ",";
  else if (token == "<*?") return "\'";
  else if (token == "<**") return "?";
  else if (token == "<*<") return "!";
  else if (token == "<<?") return "+";
  else if (token == "<<*") return "-";
  else if (token == "<<<") return "*";
  else if (token == "<?<") return "/";
  else if (token == "<***<") return "=";
  else return "";  // if can not translate, return empty.
}


// Decode Function
String decodeFromBCCode(String input) {
  String asciiText = "";
  String letter = "";
  
  for (char c : input){
    if (c == '~') {
      asciiText += decodeToken(letter);
      letter = "";
    }
    else if (c == ' ') {
      asciiText += decodeToken(letter);
      asciiText += " ";
      letter = "";
    }
    else {
      letter += c;
    }  
  }

  asciiText += decodeToken(letter);
  
  return asciiText;
}


// Flash LED Function
void flashLED(int ledPin, const String& code) {
  int potValue = analogRead(potentiometerPin);
  int timeUnit = map(potValue, 0, 1023, 20, 400);  // Map potentiometer value to time unit
  
  for (int i = 0; i < code.length(); ++i) {
    char signal = code.charAt(i);

    if (signal == '*') {
      digitalWrite(ledPin, HIGH);
      delay(2 * timeUnit);
    } else if (signal == '?') {
      digitalWrite(ledPin, HIGH);
      delay(timeUnit);
    } else if (signal == '<') {
      digitalWrite(ledPin, HIGH);
      delay(3 * timeUnit);
    }

    digitalWrite(ledPin, LOW);

    if (i < code.length() - 1) {
      char nextSignal = code.charAt(i + 1);

      if (nextSignal == ' ') {
        delay(5 * timeUnit);  // Delay between two words
      } else if (nextSignal == '~') {
        delay(2 * timeUnit);  // Delay between characters forming the same word
      } else {
        delay(timeUnit);  // Delay between signals forming the same letter
      }
    }
  }
}
  


// Handle Command Function
void handleCommand(const String& command) {

  // Command: BxB
  if (command.startsWith("B") and command.substring(2) == "B")  {
    int xValue = command.substring(1).toInt();

    if (xValue >= 1 && xValue <= 9 && xValue % 2 == 0) {
      int potValue = analogRead(potentiometerPin);
      String encodedPotValue = encodeToBCCode(String(potValue, DEC));
      Serial.println(encodedPotValue);
    } 
  } 
  
  // Command: PB followed by 3 decimal digits
  else if (command.startsWith("PB") and command.length() == 5) {
    String digits = command.substring(2);
    int sum = 0;

    for (int i = 0; i < digits.length(); ++i) {
      sum += digits.charAt(i) - '0';
    }

    if (sum < 10) {
      int irValue = digitalRead(irReceiverPin);
      Serial.println(irValue ? "HIGH" : "LOW");
    } else {
      Serial.println("UNKNOWN");
    }
  } 
  
  // Command: BC followed by 3 decimal digits
  else if (command.startsWith("BC") and command.length() == 5) {
    int brightness = command.substring(2).toInt();
    
    if (brightness <= 255) {
      analogWrite(redLEDPin, brightness);
      delay(250);
      analogWrite(redLEDPin, 0);
    }
  } 
    
  // Command: BB followed by 4 decimal digits
  else if (command.startsWith("BB") && command.length() == 6) {
    String digits = command.substring(2);
    int yellowTime = (digits.charAt(0) - '0') * 100;
    int redTime = (digits.charAt(1) - '0') * 100;
    int greenTime = (digits.charAt(2) - '0') * 100;
    int orangeTime = (digits.charAt(3) - '0') * 100;

    unsigned long startTime = millis();
    while (millis() - startTime < max(max(yellowTime, redTime), max(greenTime, orangeTime))) {
      digitalWrite(yellowLEDPin, millis() - startTime < yellowTime ? HIGH : LOW);
      digitalWrite(redLEDPin, millis() - startTime < redTime ? HIGH : LOW);
      digitalWrite(greenLEDPin, millis() - startTime < greenTime ? HIGH : LOW);
      digitalWrite(orangeLEDPin, millis() - startTime < orangeTime ? HIGH : LOW);
    }

    digitalWrite(yellowLEDPin, LOW);
    digitalWrite(redLEDPin, LOW);
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(orangeLEDPin, LOW);
  }
}
