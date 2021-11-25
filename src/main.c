//TEST UPLOAD

// Sample code for ECE 198

// Written by Bernie Roehl, August 2021

// This file contains code for a number of different examples.
// Each one is surrounded by an #ifdef ... #endif block inside of main().

// To run a particular example, you should remove the comment (//) in
// front of exactly ONE of the following lines:

#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions
#include <time.h> // time function

#include "ece198.h"
#include "LiquidCrystal.h"

bool LED_Counter();
bool RGB_Reaction();
bool Game_24();
int What_Operator(int i, int a, int b); //24_Game Helper Function
char num_value(int i); //24_Game Helper Function

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
    
    // setCursor(0,0);
    // print("Welcome to our");
    // setCursor(0,1);
    // print("GRAND PUZZLE");
    // HAL_Delay(3000);



    bool win1 = 0;
    bool win2 = 0;
    bool win3 = 0;

    while (1){
        //We'll be using the onboard button.

        
        
        clear();
        setCursor(0,0);
        print("Press button to play");

        while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))  //when button pressed
        {
        

            //Puzzle 1: 
            if (win1 == false){
                win1 = LED_Counter();
                break;
            }

            //Puzzle 2:
            
            if (win2 == false){
                win2 = RGB_Reaction();
                break;
            }
            

            //Puzzle 3:
            if (win3 == false){
                win3 = Game_24();
                break;
            }

        }

        //Whole game is complete when all three puzzles are won
        if( (win1 == true) && (win2 == true) && (win3 == true))
        {
            //Either add LCD screen win code here or out of the infinite while loop

            char win[100];
            sprintf(win, "All 3 puzzles have been completed.\nYOU WIN!\n");

            clear();
            setCursor(0,0);
            print("PUZZLES COMPLETE");
            setCursor(0,1);
            print("YOU WIN!");
            break;
        }

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

}


// This function is called by the HAL once every millisecond
void SysTick_Handler(void)
{
    HAL_IncTick(); // tell HAL that a new tick has happened
    // we can do other things in here too if we need to, but be careful
}

bool LED_Counter(){
    char *keypad_symbols = "123A456B789C*0#D";
    HAL_Delay(2000);

    char p1[100];
    sprintf(p1, "PUZZLE 1: LED COUNTER\n");
    SerialPuts(p1);
    sprintf(p1, "RULE: Count number of blinks and input with keypad\n");
    SerialPuts(p1);

    //Instruction for Puzzle 1:
    clear();
    setCursor(0,0);
    print("Count the ");
    setCursor(0,1);
    print("number of blinks");
    
    HAL_Delay(4000);

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

    bool win = 0;
    char buff[100];
    if (key == answer)
    {
        sprintf(buff, "Correct Key Press: %d\r\nPUZZLE 1 COMPLETE\n", total);
        win = true;
    } else {
        sprintf(buff, "Incorrect Key Press: %c     Correct Key Press: %d\r\nPush button to retry\n", key, total);
        win = false;
    }
    SerialPuts(buff);

    while (ReadKeypad() >= 0);  // wait until key is released

    return win;
}

//Outputs: RGB LED, LCD display 
//Inputs: External button
bool RGB_Reaction()
{   

    // Modified game rules:
    // Users will be prompted to react to color WHITE. LED will cycle through different colours 10 times.
    // There are 8 possible colors. color value for WHITE is 7. (1 1 1) 
    // rc for random colour

    char *keypad_symbols = "123A456B789C*0#D";

    // Serial output for testing
    char p2[100];
    sprintf(p2, "PUZZLE 2: RGB Reaction\n");
    SerialPuts(p2);
    sprintf(p2, "RULE: Press any key on keypad when LED is WHITE \n");
    SerialPuts(p2);


    clear();
    setCursor(0,0);
    print("Click any key ");
    setCursor(0,1);
    print("when WHITE ");
    HAL_Delay(4000);


    srand(HAL_GetTick());   // Ensure randomized seeds
    int correct = rand() % 9 + 1;  // The order in which the CORRECT colour will show up
    int rc = 0;
    int i = 0;
    bool gameWon = 0;
    srand(HAL_GetTick());   // Ensure randomized seeds
    while (i < 10 && gameWon == 0)
        {
            if (i == correct)
            {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 7 & 0x01);  
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 7 & 0x02);  
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 7 & 0x04); 
                HAL_Delay(1200);
                // This is where user has window to REACT
                for (int i = 0; i < 1200; i++)
                {
                    //User can use any key on keypad to react
                    if (ReadKeypad() >= 0)  
                    {
                        HAL_Delay(250);
                        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0 & 0x01);  //Turn off LED
                        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0 & 0x02);  
                        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0 & 0x04);
                        gameWon = 1;

                        // Serial output for testing
                        char win[100];
                        sprintf(win, "PUZZLE 2 COMPLETE\n");
                        SerialPuts(win);

                        clear();
                        setCursor(0,0);
                        print("PUZZLE 2 COMPLETE\n");
                        HAL_Delay(2000);
                        return true;
                    }
                }
            } else
            {
                rc = rand() % 6 + 1;    // randomizes between 1 to 6, so OTHER colours cannot be WHITE
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, rc & 0x01);
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, rc & 0x02); 
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, rc & 0x04);
                HAL_Delay(1200);
            }
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0 & 0x01);  
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0 & 0x02);  
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0 & 0x04);
            HAL_Delay(1200);
            i++;

        }
        char lost[100];
        sprintf(lost, "DIDN'T REACT IN TIME\nPush button to retry\n");
        SerialPuts(lost);
        return false;

}

bool Game_24()
{   
    HAL_Delay(1000);

    char p3[100];
    sprintf(p3, "PUZZLE 3: 24 Game\n");
    SerialPuts(p3);
    sprintf(p3, "RULE: Arrange four given numbers to compute 24\n");
    SerialPuts(p3);

    clear();
    setCursor(0,0);
    print("Use given nums");
    setCursor(0,1);
    print("to make 24 ");
    HAL_Delay(2000);

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
                        sprintf(buff, "24 found (((%d%c%d)%c%d)%c%d)\r\n", 
                                        num[0], num_value(c), num[1], num_value(b), num[2], num_value(a), num[3]);
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

    bool win = 0;
    if (total == 24)
    {
        sprintf(buff, "Correct Value: %d\r\nPUZZLE 3 COMPLETE\n", total);
        win = true;

    } else {
        sprintf(buff, "Incorrect Value: %d     Correct Value: 24\r\nPush button to retry\n", total);
        win = false;

    }

    SerialPuts(buff);
    return win;

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

char num_value(int i){
    if (i == 0)
    {
        return '+';

    } else if (i == 1)
    {
        return '-';

    } else if (i == 2)
    {
        return '*';

    } else
    {
        return '/';
    }
}