See bin/windows and bin/linux for windows and linux executables respectively
The executables should be run from the directies in which they are found

For some reason images and buttons dont work in the windows exectable (issues with SDL2 dependancies when linking), so there are keybinds to get to different parts of the app:
	-'s' goes to singleplayer mode
	-'t' goes to twoplayer mode
	-'esc' will return to main menu when in game
	-press enter to confirm name when prompted in game

Text also seems to be rendered weird

The actual tictactoe program files are found in src/game and include/game, the rest of the files concern graphics, menuscreens etc
