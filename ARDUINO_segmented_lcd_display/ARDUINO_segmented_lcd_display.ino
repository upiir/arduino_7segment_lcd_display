// Driving bare LCD 7-segment display with Arduino UNO

// created by upir, 2025
// youtube channel: https://www.youtube.com/upir_upir

// YouTube video: https://youtu.be/BuPAdIORbwU
// Source files: https://github.com/upiir/arduino_7segment_lcd_display

// Links from the video:
// Do you like this video? You can buy me a coffee ☕: https://www.buymeacoffee.com/upir
// LCD Display: https://s.click.aliexpress.com/e/_oDje8eW
// Arduino UNO: https://s.click.aliexpress.com/e/_AXDw1h
// Arduino breadboard prototyping shield: https://s.click.aliexpress.com/e/_okqtRTY
// Breadboard Wires: https://s.click.aliexpress.com/e/_Dkbngin
// Jumper Wires: https://s.click.aliexpress.com/e/_oEisMHg
// 16segment LCD display (Arduino Compatible): https://s.click.aliexpress.com/e/_ophvG8W
// 7 segment LCD display (Arduino Compatible): https://s.click.aliexpress.com/e/_ok3t56Q
// LCD + Arduino blog post: https://bitsnbikes.blogspot.com/2009/03/7-segments-lcd-displays.html
// Someone else trying to make the display work: https://forum.arduino.cc/t/need-guidance-for-driving-single-digit-7-segment-lcds/376568
// Code from EEVblog: https://github.com/EEVblog/Arduino-Seven-Segment-LCD
// EEVblog LCD video series: https://youtube.com/playlist?list=PLvOlSehNtuHvmK-VGcZ33ZuATmcNB8tvH&si=hfDPYmMMTAElPtyB

// Related videos with segmented displays:
// Segmented eink display: https://youtu.be/Rf-E-7fFop0
// Posy’s designs ➔ REAL DISPLAYS: https://youtu.be/jz01j1TpM84
// Arduino + Ynvisible Displays: https://youtu.be/lxeH1ilL2T0
// Simple LED strip display: https://youtu.be/_pNuaVKABN4
// Transparent Edgelit Display: https://youtu.be/Cg9MDF1VE2g
// Arduino Breadboard Display: https://youtu.be/DD9FJr9zQ0U
// Another breadboard display: https://youtu.be/9JRgl6mOAfM
// Teenage Engineering display: https://youtu.be/CeOC7Bf_YKw
// Electroluminescent Display: https://youtu.be/MtiuXPFYjX8



// define segment names - this depends on how the LCD display is connected to Arduino UNO
// upon compiling the sketch, all the segment names will be replaced with the set numeric values
#define SEG_COM 2
#define SEG_A 8
#define SEG_B 9
#define SEG_C 5
#define SEG_D 4
#define SEG_E 3
#define SEG_F 7
#define SEG_G 10
#define SEG_DP 6

// 7 segment display pins and segments
// for this particular display, every set of two pins are connected to the same segment
// COM = common pin = common electrode
// DP = decimal point
//                                             
//           COM    F     A     B     G        
//         10 10  9  9  8  8  7  7  6  6       
//         ┌─┐┌─┐┌─┐┌─┐┌─┐┌─┐┌─┐┌─┐┌─┐┌─┐      
//       ┌─┴─┴┴─┴┴─┴┴─┴┴─┴┴─┴┴─┴┴─┴┴─┴┴─┴─┐    
//       │                                │    
//       │     ┌────────────────┐         │    
//       │     │       A        │         │    
//       │     └────────────────┘         │    
//       │   ┌───┐            ┌───┐       │    
//       │   │   │            │   │       │    
//       │   │ F │            │ B │       │    
//       │   │   │            │   │       │    
//       │   └───┘            └───┘       │    
//       │     ┌────────────────┐         │    
//       │     │       G        │         │    
//       │     └────────────────┘         │    
//       │   ┌───┐            ┌───┐       │    
//       │   │   │            │   │       │    
//       │   │ E │            │ C │       │    
//       │   │   │            │   │       │    
//       │   └───┘            └───┘       │    
//       │     ┌────────────────┐   ┌───┐ │    
//       │     │       D        │   │DP │ │    
//       │     └────────────────┘   └───┘ │    
//       │                                │    
//       └─┬─┬┬─┬┬─┬┬─┬┬─┬┬─┬┬─┬┬─┬┬─┬┬─┬─┘    
//         └─┘└─┘└─┘└─┘└─┘└─┘└─┘└─┘└─┘└─┘      
//          1  1  2  2  3  3  4  4  5  5       
//           COM    E     D     C    DP        
//                                                                                                                                        


// this array stores all the values for 
// individual segments for every single digit (0-9)
// values are stored as bits inside bytes
// decimal point is not used (= all zeroes)

byte segments_digits[10] { 
// ABCDEFG.
  B11111101, // digits 0
  B01100001, // digits 1
  B11011011, // digits 2
  B11110011, // digits 3
  B01100111, // digits 4
  B10110111, // digits 5
  B10111111, // digits 6
  B11100001, // digits 7
  B11111111, // digits 8
  B11110111  // digits 9
};


void setup() {
  
  // set all the needed Arduino pins as outputs (7 pins for 7-segment display, 1 pin for decimal point, 1 pin for common electrode)
  pinMode(SEG_COM, OUTPUT); // set pin 2 as output
  pinMode(SEG_A, OUTPUT); // set pin 3 as output
  pinMode(SEG_B, OUTPUT); // set pin 4 as output  
  pinMode(SEG_C, OUTPUT); // set pin 5 as output  
  pinMode(SEG_D, OUTPUT); // set pin 6 as output  
  pinMode(SEG_E, OUTPUT); // set pin 7 as output  
  pinMode(SEG_F, OUTPUT); // set pin 8 as output  
  pinMode(SEG_G, OUTPUT); // set pin 9 as output  
  pinMode(SEG_DP, OUTPUT); // set pin 10 as output              

}

int digit = 0; // which digit to display
int frame; // frame is a helper variable which is used to switch to a different digit after some time

void loop() { // put your main code here, to run repeatedly

  frame++; // increase the frame value by one
  if (frame > 25) { // if the frame value is bigger than...
    digit++; // increase the digit value
    if (digit > 9) {digit = 0;} // if the digit value is bigger than 9, jump back to 0
    frame = 0; // reset the frame value back to 0   
  }
  


  // LCD display need to be driven by AC (alternating current)
  // this could be simulated with Arduino by quickly switching 
  // ground (LOW state) and 5V (HIGH state)
  // if the segment should not be visible, the segment 
  // should be driven by the same voltage as the common pin
  //
  // the wait time of 10ms is randomly chosen, it might not be a perfectly valid value, but it works

  // common pin
  digitalWrite(SEG_COM, 0); // 0 = LOW = GROUND
  // segment pins
  digitalWrite(SEG_A, bitRead(segments_digits[digit], 7));  // segment A
  digitalWrite(SEG_B, bitRead(segments_digits[digit], 6));  // segment B
  digitalWrite(SEG_C, bitRead(segments_digits[digit], 5));  // segment C
  digitalWrite(SEG_D, bitRead(segments_digits[digit], 4));  // segment D
  digitalWrite(SEG_E, bitRead(segments_digits[digit], 3));  // segment E
  digitalWrite(SEG_F, bitRead(segments_digits[digit], 2));  // segment F
  digitalWrite(SEG_G, bitRead(segments_digits[digit], 1));  // segment G
  digitalWrite(SEG_DP, bitRead(segments_digits[digit], 0)); // segment decimal point

  delay(10); // wait 10ms

  // common pin
  digitalWrite(SEG_COM, 1); // 1 = HIGH = 5V
  // segment pins
  digitalWrite(SEG_A, !bitRead(segments_digits[digit], 7));  // segment A
  digitalWrite(SEG_B, !bitRead(segments_digits[digit], 6));  // segment B
  digitalWrite(SEG_C, !bitRead(segments_digits[digit], 5));  // segment C
  digitalWrite(SEG_D, !bitRead(segments_digits[digit], 4));  // segment D
  digitalWrite(SEG_E, !bitRead(segments_digits[digit], 3));  // segment E
  digitalWrite(SEG_F, !bitRead(segments_digits[digit], 2));  // segment F
  digitalWrite(SEG_G, !bitRead(segments_digits[digit], 1));  // segment G
  digitalWrite(SEG_DP, !bitRead(segments_digits[digit], 0)); // segment decimal point

  delay(10); // wait 10ms

}
 