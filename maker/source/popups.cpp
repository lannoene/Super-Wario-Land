#include "popups.hpp"

std::string createPopup(SDL_Screen &Scene, char* popupText) {
	std::string inputText = "";
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
			Scene.drawText("___", 5, 30, 30);
		}
		Scene.finishDrawing();
	}
	return "e";
}

