// PlayGame.h
// Runs on LM4F120/TM4C123
// Play RallyX
// Karena Yu
//`11/24/2016

//-----------------------playgame-----------------------
// Inputs: an int that indicates difficulty level (1 == easy,2 == medium,3 == hard)
// Outputs: an int (0 == lose, 1 == win, -1 == neither)
// Outputs map and radar with gameplay by calling Display functions
// Does not output win/lose or menu screens (in main)
int playgame(int difficulty);
