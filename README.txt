    The inspiration for this project is the Nimatron, an electromechanical machine exhibited at the 1939–1940 New York World's
    Overview
    This project is a hardware implementation of a Nim playing MCU. It uses an ATMega328P microcontroller (MCU) and an LCD display.

    Requirements
    Microsoft Visual Studio Code
    PIO Core 6.1.19
    Adafruit SPI and GFX libraries are downloaded by PIO when initializing the project

    Project description
    There are two main components of this project: the hardware and the software. I will briefly go through the hardware part
and more in-depth through the software part. The game consists of 4 heaps, each with 7 objects. In a turn, a player can take any
number of elements from a single heap. A player loses by taking the last remaining object, or if their time runs out.

    Hardware design
    List of used elements:
        - ATMega328P Mini Xplained MCU
        - breadboard
        - 4 + 1 input buttons
        - ST7735S 1.8 TFT SPI LCD
        - bidirectional level shifter 3V3 <-> 5V
        - microUSB cable for power
        - wires
    This project is software heavy, which makes the hardware pretty simplistic. The master button is onboard the MCU, while the
rest of them are easily connected to pins PD2-PD5 using wires. Since the MCU is powered up through the microUSB port, it is
working at 5V, but the LCD I used has 3V3 I/O logic, so simply connecting the display to SPI pins wouldn't work. This is where
the level shifter comes in. Input VCCs come from the MCU's 5V and 3V3 pins.
    pins_lcd.txt contains the pins used for the display, and the entire electrical diagram can be viewed inside the "assets"
folder.

    Software design
    This is where most of the work was done. There are several modules that make up the code:
        - game logic module: handles game states and players
        - graphics module: handles the screen's rendering
        - interruptions module: used for button debouncing
        - timer module: used for counting milliseconds since power up
        - USART module: used only for debugging
    The game logic module is pretty straightforward: it contains functions that handle the player and CPU's move, the game loop
a function that checks if the game ended (all objects taken). None of these functions use hardware registers directly, they call
other functions that handle the hardware part. The game is represented through a struct that holds the number of objects in each
heap and the total number of objects.
    The graphics module contains functions for drawing on different parts of the screen. Since the game does not have graphics
that need changing every frame, it is way more efficient to only update parts of the screen when something does actually change
instead of sending a whole framebuffer every frame. The game has three main screens, each with one or more subsections. These are:
    - title screen: only shows up when powering up the MCU
    - game screen: here are drawn the heaps, time left and a message that tells the player if it is their turn or not
    - win/lose screen: shows up when the game ends, also displays the current score in the top left corner
    Rendering is done using the Adafruit ST7735 and ST7789 and Adafruit GFX libraries. Sizes and positions of screen elements
are hardcoded for this specific 128x160 LCD. If other displays are used, the game may not render correctly.
    Interruptions are used for button debouncing. There are two categories of buttons: master button and heap buttons. Each of
these categories have their own interruption routines and state flags, but their logic is similar. A buttons has state 1 if it
is pressed, 0 otherwise. Routines check which buttons was pressed by checking if their current state differs from their previous
state, it checks if this new state is 0, then sets the flag for button pressed on 1. For the master button, this is done using
two boolean variables. The heaps buttons share two 8 bit variables, from which states and flags can be read/written using masks.
    The timer is used for counting milliseconds. This allows setting a time limit for the player's move, making the game a
little bit more interesting. I also use this timer to create a two seconds delay when the CPU makes its move, to give the
impression that it is thinking like a human. Since i am using a 4 byte variable to hold the number of milliseconds, getting
this number must be done with additional care, because the MCU has an 8 bit architecture. A correct copy to all 4 registers
is ensured by first disabling interruptions, copying and then enabling them again.
    The USART module is only used for debugging/testing new features. The code gets compiled without the debugging
functionalities by default, but they can be enabled if the "#define __DEBUG" line is not commented inside core.h.
    All these are used inside the main function, which first initializes the MCU and the game, then it enters in an infinite
loop that counts the number of won/lost rounds for every game.

    Future additions
    Right now, the CPU is almost impossible to beat, so i would like to add dificulty levels, by making it make "mistakes" with
varying probabilities.

    Libraries
    Adafruit GFX Library: https://github.com/adafruit/Adafruit-GFX-Library
    Adafruit ST7735 and ST7789 library: https://github.com/adafruit/Adafruit-ST7735-Library
