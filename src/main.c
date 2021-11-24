//TEST UPLOAD

// Sample code for ECE 198

// Written by Bernie Roehl, August 2021

// This file contains code for a number of different examples.
// Each one is surrounded by an #ifdef ... #endif block inside of main().

// To run a particular example, you should remove the comment (//) in
// front of exactly ONE of the following lines:

// #define BUTTON_BLINK
// #define LIGHT_SCHEDULER
// #define TIME_RAND
// #define KEYPAD
// #define KEYPAD_CONTROL
// #define SEVEN_SEGMENT
// #define KEYPAD_SEVEN_SEGMENT
#define COLOR_LED
// #define ROTARY_ENCODER
// #define ANALOG
// #define PWM

#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions
#include <time.h> // time function

#include "ece198.h"
#include "LiquidCrystal.h"

void LED_Counter();
void RGB_Reaction();
void Game_24();
int What_Operator(int i, int a, int b); //24_Game Helper Function

int main(void){
    HAL_Init();

    __HAL_RCC_GPIOA_CLK_ENABLE(); //Enable Port A
    __HAL_RCC_GPIOB_CLK_ENABLE(); //Enable Port B
    __HAL_RCC_GPIOC_CLK_ENABLE(); //Enable Port C

    //Built-in-LED
    InitializePin(GPIOA, GPIO_PIN_5 , GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);
    //Keypad GPIOA Pins
    InitializePin(GPIOA, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_NOPULL, 0); 

    //Keypad GPIOB Pins
    InitializePin(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_NOPULL, 0);
    
    //Keypad GPIOC PIN_7 and Built in Button GPIO PIN_13
    InitializePin(GPIOC, GPIO_PIN_13 | GPIO_PIN_7, GPIO_MODE_INPUT, GPIO_NOPULL, 0);

    //Initialize RGB LED
    InitializePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);

    //Initialize LCD Display
    LiquidCrystal(GPIOB, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6);

    InitializeKeypad();
    SerialSetup(9600);

    srand(time(NULL)); //Generates A New Set of random numbers for every reset
    
    while (1){
        //We'll be using the onboard button.
        //Initially, if button is pressed, LED_Counter puzzle will run

        setCursor(0,0);
        print("Welcome to our");
        setCursor(0,1);
        print("GRAND PUZZLE");
        HAL_Delay(3000);
        clear();
        setCursor(0,0);
        print("Press button to play");


        while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))  //when button pressed
        {
            //Instruction for Puzzle 1:
            clear();
            setCursor(0,0);
            print("Rules: Count the ");
            setCursor(0,1);
            print("number of blinks");
            //Puzzle 1: 
            LED_Counter();

        }

        //If LED_Counter is won, button will now be used to start RGB_Reaction
        
        //Implement initiation of RGB_Reaction here

        //If RGB_Reaction is won, button will be used to start Game_24

        //Implement initiation of Game_24 here

        //Whole game is complete when all three puzzles are won
        


    }

    //RGB LED REACTION TIMER
    //USE STARTER CODE VIDEO 5:00 Calculate time interval
    //USE: HAL_Get_Tick() once -> another time to count interval
    //USE: sprintf(...) for debugging purposes

    //Example:
    //Char buff[100];
    //sprintf(buff, "Value is %d units", value);
    //SerialPuts(buff);

    
    /*
    
    //Store Current time
    uint32_t now = HAL_Get_Tick();

    //Reaction of 1 seccond
    while((HAL_Get_Tick() - now) < 1000) //1 Second Reaction timer
    {
    }
    */
    //Prevous Code Commented out except for last function after Main
    /*
    HAL_Init(); // initialize the Hardware Abstraction Layer

    // Peripherals (including GPIOs) are disabled by default to save power, so we
    // use the Reset and Clock Control registers to enable the GPIO peripherals that we're using.

    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable port A (for the on-board LED, for example)
    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable port B (for the rotary encoder inputs, for example)
    __HAL_RCC_GPIOC_CLK_ENABLE(); // enable port C (for the on-board blue pushbutton, for example)

    // initialize the pins to be input, output, alternate function, etc...

    InitializePin(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED

    // note: the on-board pushbutton is fine with the default values (no internal pull-up resistor
    // is required, since there's one on the board)

    // set up for serial communication to the host computer
    // (anything we write to the serial port will appear in the terminal (i.e. serial monitor) in VSCode)

    SerialSetup(9600);

    // as mentioned above, only one of the following code sections will be used
    // (depending on which of the #define statements at the top of this file has been uncommented)
    */
#ifdef BUTTON_BLINK
    // Wait for the user to push the blue button, then blink the LED.

    // wait for button press (active low)
    while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
    {
    }

    while (1) // loop forever, blinking the LED
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(250);  // 250 milliseconds == 1/4 second
    }
#endif

#ifdef LIGHT_SCHEDULER
    // Turn on the LED five seconds after reset, and turn it off again five seconds later.

    while (true) {
        uint32_t now = HAL_GetTick();
        if (now > 5000 && now < 10000)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, true);   // turn on LED
        else
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, false);  // turn off LED
    }
#endif

#ifdef TIME_RAND
    // This illustrates the use of HAL_GetTick() to get the current time,
    // plus the use of random() for random number generation.
    
    // Note that you must have "#include <stdlib.h>"" at the top of your main.c
    // in order to use the srand() and random() functions.

    // while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));  // wait for button press
    // srand(HAL_GetTick());  // set the random seed to be the time in ms that it took to press the button
    // if the line above is commented out, your program will get the same sequence of random numbers
    // every time you run it (which may be useful in some cases)

    while (true) // loop forever
    {
        while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));  // wait for button press

        // Display the time in milliseconds along with a random number.
        // We use the sprintf() function to put the formatted output into a buffer;
        // see https://www.tutorialspoint.com/c_standard_library/c_function_sprintf.htm for more
        // information about this function
        char buff[100];
        sprintf(buff, "Time: %lu ms   Random = %ld\r\n", HAL_GetTick(), random());
        // lu == "long unsigned", ld = "long decimal", where "long" is 32 bit and "decimal" implies signed
        SerialPuts(buff); // transmit the buffer to the host computer's serial monitor in VSCode/PlatformIO

        while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));  // wait for button to be released
    }
#endif

#ifdef KEYPAD
    // Read buttons on the keypad and display them on the console.

    // this string contains the symbols on the external keypad
    // (they may be different for different keypads)
    char *keypad_symbols = "123A456B789C*0#D";
    // note that they're numbered from left to right and top to bottom, like reading words on a page

    InitializeKeypad();
    while (true)
    {
        while (ReadKeypad() < 0);   // wait for a valid key
        SerialPutc(keypad_symbols[ReadKeypad()]);  // look up its ASCII symbol and send it to the hsot
        while (ReadKeypad() >= 0);  // wait until key is released
    }
#endif

#ifdef KEYPAD_CONTROL
    // Use top-right button on 4x4 keypad (typically 'A') to toggle LED.

    InitializeKeypad();
    while (true)
    {
        while (ReadKeypad() < 0);   // wait for a valid key
        int key = ReadKeypad();
        if (key == 3)  // top-right key in a 4x4 keypad, usually 'A'
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);   // toggle LED on or off
         while (ReadKeypad() >= 0);  // wait until key is released
    }
#endif

#ifdef SEVEN_SEGMENT
    // Display the numbers 0 to 9 inclusive on the 7-segment display, pausing for a second between each one.
    // (remember that the GND connection on the display must go through a 220 ohm current-limiting resistor!)
    
    Initialize7Segment();
    while (true)
        for (int i = 0; i < 10; ++i)
        {
            Display7Segment(i);
            HAL_Delay(1000);  // 1000 milliseconds == 1 second
        }
#endif

#ifdef KEYPAD_SEVEN_SEGMENT
    // Combines the previous two examples, displaying numbers from the keypad on the 7-segment display.

    // this string contains the symbols on the external keypad
    // (they may be different for different keypads)
    char *keypad_symbols = "123A456B789C*0#D";
    // note that they're numbered from left to right and top to bottom, like reading words on a page

    InitializeKeypad();
    Initialize7Segment();
    while (true)
    {
        int key = ReadKeypad();
        if (key >= 0)
            Display7Segment(keypad_symbols[key]-'0');  // tricky code to convert ASCII digit to a number
    }
#endif

#ifdef COLOR_LED
    // Cycle through all 8 possible colors (including off and white) as the on-board button is pressed.
    // This example assumes that the color LED is connected to pins D11, D12 and D13.

    // Remember that each of those three pins must go through a 220 ohm current-limiting resistor!
    // Also remember that the longest pin on the LED should be hooked up to GND.

    // InitializePin(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // initialize color LED output pins
    // while (true) {
    //     for (int color = 0; color < 8; ++color) {
    //         // bottom three bits indicate which of the three LEDs should be on (eight possible combinations)
    //         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, color & 0x01);  // blue  (hex 1 == 0001 binary)
    //         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, color & 0x02);  // green (hex 2 == 0010 binary)
    //         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, color & 0x04);  // red   (hex 4 == 0100 binary)

    //         while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));   // wait for button press 
    //         while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));  // wait for button release
    //     }
    // }
#endif

#ifdef ROTARY_ENCODER
    // Read values from the rotary encoder and update a count, which is displayed in the console.

    InitializePin(GPIOB, GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_PULLUP, 0);   // initialize CLK pin
    InitializePin(GPIOB, GPIO_PIN_4, GPIO_MODE_INPUT, GPIO_PULLUP, 0);   // initialize DT pin
    InitializePin(GPIOB, GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_PULLUP, 0);  // initialize SW pin
    
    bool previousClk = false;  // needed by ReadEncoder() to store the previous state of the CLK pin
    int count = 0;             // this gets incremented or decremented as we rotate the encoder

    while (true)
    {
        int delta = ReadEncoder(GPIOB, GPIO_PIN_5, GPIOB, GPIO_PIN_4, &previousClk);  // update the count by -1, 0 or +1
        if (delta != 0) {
            count += delta;
            char buff[100];
            sprintf(buff, "%d  \r", count);
            SerialPuts(buff);
        }
        bool sw = !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10);  // read the push-switch on the encoder (active low, so we invert it using !)
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, sw);  // turn on LED when encoder switch is pushed in
    }
#endif

#ifdef ANALOG
    // Use the ADC (Analog to Digital Converter) to read voltage values from two pins.

    __HAL_RCC_ADC1_CLK_ENABLE();        // enable ADC 1
    ADC_HandleTypeDef adcInstance;      // this variable stores an instance of the ADC
    InitializeADC(&adcInstance, ADC1);  // initialize the ADC instance
    // Enables the input pins
    // (on this board, pin A0 is connected to channel 0 of ADC1, and A1 is connected to channel 1 of ADC1)
    InitializePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1, GPIO_MODE_ANALOG, GPIO_NOPULL, 0);   
    while (true)
    {
        // read the ADC values (0 -> 0V, 2^12 -> 3.3V)
        uint16_t raw0 = ReadADC(&adcInstance, ADC_CHANNEL_0);
        uint16_t raw1 = ReadADC(&adcInstance, ADC_CHANNEL_1);

        // print the ADC values
        char buff[100];
        sprintf(buff, "Channel0: %hu, Channel1: %hu\r\n", raw0, raw1);  // hu == "unsigned short" (16 bit)
        SerialPuts(buff);
    }
#endif

#ifdef PWM
    // Use Pulse Width Modulation to fade the LED in and out.
    uint16_t period = 100, prescale = 16;

    __TIM2_CLK_ENABLE();  // enable timer 2
    TIM_HandleTypeDef pwmTimerInstance;  // this variable stores an instance of the timer
    InitializePWMTimer(&pwmTimerInstance, TIM2, period, prescale);   // initialize the timer instance
    InitializePWMChannel(&pwmTimerInstance, TIM_CHANNEL_1);          // initialize one channel (can use others for motors, etc)

    InitializePin(GPIOA, GPIO_PIN_5, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2); // connect the LED to the timer output

    while (true)
    {
        // fade the LED in by slowly increasing the duty cycle
        for (uint32_t i = 0; i < period; ++i)
        {
            SetPWMDutyCycle(&pwmTimerInstance, TIM_CHANNEL_1, i);
            HAL_Delay(5);
        }
        // fade the LED out by slowly decreasing the duty cycle
        for (uint32_t i = period; i > 0; --i)
        {
            SetPWMDutyCycle(&pwmTimerInstance, TIM_CHANNEL_1, i);
            HAL_Delay(5);
        }
    }
#endif
}


// This function is called by the HAL once every millisecond
void SysTick_Handler(void)
{
    HAL_IncTick(); // tell HAL that a new tick has happened
    // we can do other things in here too if we need to, but be careful
}

void LED_Counter(){
    char *keypad_symbols = "123A456B789C*0#D";
    HAL_Delay(2000);
    
    int total = rand() % 9 + 1; // digits 1 to 9
    int i;

    for ( i = 1 ; i <= (2*total); ++i ) //Loop twice the amount of total
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(250);  // 250 milliseconds == 1/4 second
    }

    while (ReadKeypad() < 0);   // wait for a valid key

    char key = keypad_symbols[ReadKeypad()];

    char answer = '0' + total;

    char buff[100];
    if (key == answer)
    {
        sprintf(buff, "Correct Key Press: %d\r\n", total);
    } else {
        sprintf(buff, "Incorrect Key Press: %c     Correct Key Press: %d\r\n", key, total);
    }
    SerialPuts(buff);

    while (ReadKeypad() >= 0);  // wait until key is released
}

//Outputs: RGB LED, LCD display 
//Inputs: External button
void RGB_Reaction()
{   
    HAL_Delay(2000);
    // Print "RGB REACTION GAME"
    // Print "Click button when LED turns WHITE"

    // Modified game rules:
    // Users will be prompted to react to color WHITE. LED will cycle through different colours 10 times.
    // There are 8 possible colors. color value for WHITE is 7. (1 1 1) 
    // rc for random colour
    srand(HAL_GetTick());
    int correct = rand() % 9 + 1;  // The order in which the CORRECT colour will show up
    int rc = 0;
    int i = 0;
    bool gameWon = 0;
    srand(HAL_GetTick());
    while (i < 10 && gameWon == 0)
        {
            char buff[100];
            if (i == correct)
            {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 7 & 0x01);  
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 7 & 0x02);  
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 7 & 0x04); 
                HAL_Delay(1200);
                // This is where user has window to REACT
                for (int i = 0; i < 1200; i++)
                {
                    if (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
                    {
                        gameWon = 1;
                    }
                }
            } else
            {
                rc = rand() % 6 + 1;    // randomizes between 1 to 6, so OTHER colours cannot be WHITE
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, rc & 0x01);
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, rc & 0x02); 
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, rc & 0x04);
                sprintf(buff, "Colour value is: %d ", rc);
                SerialPuts(buff);
                HAL_Delay(1200);
            }
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0 & 0x01);  
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0 & 0x02);  
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0 & 0x04);
            HAL_Delay(1200);
            i++;

        }

}

void Game_24()
{
    //Prints 24 Game to console
    char game[100];
    sprintf(game, "24 Game\r\n");
    SerialPuts(game);

    //Key Values
    char *keypad_symbols = "123+456-789*000/";
    
    //Number of Digits
    int num[4] = {};
    srand(HAL_GetTick());

    bool value = false;
    while( value == false){
        //Random Numbers
        for (int i=0; i < 4; ++i){
            num[i] = rand() % 9 + 1;
        }

        //Algorithm to check if value works
        for(int a = 0; a < 4; ++a)
        {
            for(int b = 0; b < 4; ++b)
            {
                for(int c = 0; c < 4; ++c)
                {
                    //Calculates Value
                    int answer = What_Operator(a, What_Operator(b, What_Operator(c,num[0],num[1] ), num[2]), num[3]);
                    if (answer == 24){
                        value = true;
                        char buff[100];
                        sprintf(buff, "24 found (((%d%d%d)%d%d)%d%d)\r\n", num[0],c,num[1],b,num[2],a,num[3]);
                        SerialPuts(buff);
                    }
                }
            }
        }
    }

    for (int i=0; i < 4; ++i){
            sprintf(game, "Value:%d \r\n", num[i]);
            SerialPuts(game);
            //LCD Screen output numbers here
    }

    char buff[100];
    char key = '0';
    int total = num[0];

    int i = 1;
    while( i < 4){
        while (ReadKeypad() < 0);
        key = keypad_symbols[ReadKeypad()];
        int temp = total;

        if (key == '+')
        {
            total += num[i];

        } else if (key == '-')
        {
            total -= num[i];

        } else if (key == '*')
        {
            total *= num[i];

        } else if (key == '/')
        {
            total /= num[i];
        }

        sprintf(buff, "%d %c %d = %d\r\n", temp, key,num[i], total);
        SerialPuts(buff);
        HAL_Delay(100);
        while (ReadKeypad() > 0);
        ++i;
    }

    if (total == 24)
    {
        sprintf(buff, "Correct Value: %d\r\n", total);
        //LCD Screen
    } else {
        sprintf(buff, "Incorrect Value: %d     Correct Value: 24\r\n", total);
        //LCD Screen
    }
    
    SerialPuts(buff);

}

int What_Operator(int i, int a, int b)
{
    if (i == 0)
    {
        return a + b;

    } else if (i == 1)
    {
        return a - b;

    } else if (i == 2)
    {
        return a*b;

    } else
    {
        return a/b;
    }
}