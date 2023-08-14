#include "popups.hpp"

std::string textPopup(SDL_Screen &Scene, char* popupText) {
	std::string inputText = "";
	SDL_FlushEvent(SDL_TEXTINPUT);
	SDL_StartTextInput();
	while (true) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					//return "";
				break;
				case SDL_TEXTINPUT: {
					inputText += event.text.text;
				}
				break;
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0) {
						inputText.pop_back();
					} else if (event.key.keysym.sym == SDLK_RETURN) {
						SDL_StopTextInput();
						return inputText;
					}
				break;
			}
		}
		Scene.clearScreen();
		Scene.drawText(popupText, 0, 0, 30);
		if (inputText.length() > 0) {
			Scene.drawText(inputText.data(), 0, 30, 30);
		} else {
			Scene.drawText((char*)"___", 5, 30, 30);
		}
		Scene.finishDrawing();
	}
	SDL_StopTextInput();
	return "";
}

bool yesNoPopup(SDL_Screen &Scene, char* popupText) {
	bool optionYesNo = false;
	while (true) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					return false;
				break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode) {
						default:
						break;
						case SDL_SCANCODE_RIGHT:
							optionYesNo = !optionYesNo;
						break;
						case SDL_SCANCODE_LEFT:
							optionYesNo = !optionYesNo;
						break;
					}
					if (event.key.keysym.sym == SDLK_RETURN)
						return optionYesNo;
				break;
			}
		}
		Scene.clearScreen();
		Scene.drawText(popupText, 0, 0, 30);
		if (optionYesNo) {
			Scene.drawRectangle(90, 95, 100, 40, CLR_GRY);
		} else {
			Scene.drawRectangle(200, 95, 100, 40, CLR_GRY);
		}
		Scene.drawText((char*)"Yes", 90, 100, 30);
		Scene.drawText((char*)"No", 200, 100, 30);
		
		Scene.finishDrawing();
	}
	return false;
}