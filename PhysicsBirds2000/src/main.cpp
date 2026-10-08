/***********************************************************************
 Bachelor of Software Engineering
 Media Design School
 Auckland
 New Zealand
 (c) [Year] Media Design School
 File Name   :   main.cpp
 Description :   The main thread that initialises and starts the SFML application.
 Author      :   Angelo Joseph Arawiran Bohol
 Mail        :   angelo.bohol@mds.ac.nz
 ***********************************************************************/

#include <iostream>
#include "myproject/core/Window.h"
#include "myproject/core/Settings.h"

int main()
{
    Settings& settings = Settings::getInstance();
    Window window(sf::VideoMode({ settings.windowWidth, settings.windowHeight }), "PhysicsBird2000");
    window.process();
    return 0;
}