#pragma once
#include <iostream>
#include <string>

#include "screens.hpp"

std::string textPopup(SDL_Screen &Scene, char* popupText);
bool yesNoPopup(SDL_Screen &Scene, char* popupText);