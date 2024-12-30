#include <iostream>
#include <string>
#include <cctype>
#include "grid.h"
#include <cassert>
using namespace std;

const int HORIZ = 0;
const int VERT = 1;

const int FG = 0;
const int BG = 1;

void plotHorizontalLine(int r, int c, int distance, char ch);
void plotVerticalLine(int r, int c, int distance, char ch);
void plotBackgroundHorizontalLine(int r, int c, int distance, char ch);
void plotBackgroundVerticalLine(int r, int c, int distance, char ch);
void plotRectangle(int r, int c, int height, int width, char ch);
bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg);
int performCommands(string commandString, char& plotChar, int& mode, int& badPos);
int isValidCommandString(string commandString, int& badPos);

int main()
{
    for (;;)
    {
        cout << "Enter the number of grid rows and columns (max 30 each): ";
        int nRows;
        int nCols;
        cin >> nRows >> nCols;
        cin.ignore(10000, '\n');
        if (nRows >= 1 && nRows <= MAXROWS && nCols >= 1 && nCols <= MAXCOLS)
        {
            setSize(nRows, nCols);
            break;
        }
        cout << "The numbers must be between 1 and 30." << endl;
    }
    char currentChar = '*';
    int currentMode = FG;
    for (;;)
    {
        cout << "Enter a command string (empty line to quit): ";
        string cmd;
        getline(cin, cmd);
        if (cmd == "")
            break;
        int position;
        int status = performCommands(cmd, currentChar, currentMode, position);
        switch (status)
        {
        case 0:
            draw();
            break;
        case 1:
            cout << "Syntax error at position " << position << endl;
            break;
        case 2:
            if (!isprint(currentChar))
                cout << "Current character is not printable" << endl;
            if (currentMode != FG && currentMode != BG)
                cout << "Current mode is " << currentMode << ", not FG or BG" << endl;
            break;
        case 3:
            cout << "Cannot perform command at position " << position << endl;
            break;
        default:
            // It should be impossible to get here.
            cout << "performCommands returned " << status << "!" << endl;
        }
    }
}

//plots a horizontal line starting at (r,c) extending (distance) columns to the right of (r,c)
void plotHorizontalLine(int r, int c, int distance, char ch) {
    if (r > getRows() || r < 1) {
        return;
    }
    for (int i = 0; i <= distance; i++) {
        if (!(c + i > getCols()) && ! (c + i < 1)) 
            setChar(r, c + i, ch);
    }
}

//plots a vertical line starting at (r,c) and extending (distance) rows down from (r,c)
void plotVerticalLine(int r, int c, int distance, char ch) {
    if (c > getCols() || c < 1) {
        return;
    }
    for (int i = 0; i <= distance; i++) {
        if(!(r + i > getRows()) && !(r + i < 1))
            setChar(r + i, c, ch);
    }
}

//same as plotHorizontalLine but does not print the character if a non ' ' character is present
void plotBackgroundHorizontalLine(int r, int c, int distance, char ch) {
    if (r > getRows() || r < 1) {
        return;
    }
    for (int i = 0; i <= distance; i++) {
        if (!(c + i > getCols()) && !(c + i < 1) && getChar(r, c + i) == ' ')
            setChar(r, c + i, ch);
    }
}

//same as plotVerticalLine but does not print the character if a non ' ' character is present
void plotBackgroundVerticalLine(int r, int c, int distance, char ch) {
    if (c > getCols() || c < 1) {
        return;
    }
    for (int i = 0; i <= distance; i++) {
        if (!(r + i > getRows()) && !(r + i < 1) && getChar(r + i, c) == ' ')
            setChar(r + i, c, ch);
    }
}

//plots horizontal and vertical lines to create a rectangle
//irrelevant to plotLine and performCommands
void plotRectangle(int r, int c, int height, int width, char ch) {
    plotHorizontalLine(r, c, width, ch);
    plotVerticalLine(r, c, height, ch);
    plotVerticalLine(r, c + width, height, ch);
    plotHorizontalLine(r + height, c, width, ch);
}

//the plot line function plots a straight line either up, left, down, or right
bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg) {
    //checks values of dir and fgbg, ensuring they are 0 or 1
    //checks that plotChar is a valid character(printable letter/number/symbol)
    if ((!(dir == 0) && !(dir == 1)) || (!(fgbg == 0) && !(fgbg == 1))) {
        return false;
    }
    if (!(isprint(plotChar)))
        return false;

    //checks if any added elements would lay outside of grid. Return false if they would.
    if (dir == HORIZ) {
        if (c + distance > getCols() || c + distance < 1 || c > getCols() || c < 1) {
            return false;
        }
        if (r > getRows() || r < 0) {
            return false;
        }
    }
    else {
        if (r + distance > getRows() || r + distance < 1 || r > getRows() || r < 1) {
            return false;
        }
        if (c > getCols() || c < 0) {
            return false;
        }
    }

    //places characters on the grid using parameters to determine direction/location
    if (dir == HORIZ) {
        if (distance < 0) {
            c = c + distance;
        }
        if (fgbg == FG) {
            plotHorizontalLine(r, c, abs(distance), plotChar);
        }
        else {
            plotBackgroundHorizontalLine(r, c, abs(distance), plotChar);
        }
    }
    else {
        if (distance < 0) {
            r = r + distance;
        }
        if (fgbg == FG) {
            plotVerticalLine(r, c, abs(distance), plotChar);
        }
        else {
            plotBackgroundVerticalLine(r, c, abs(distance), plotChar);
        }
    }
    return true;
}

//determines whether the command string follows the rules outlined in the spec
//returns 1 and updates badPos if incorrect syntax is found
int isValidCommandString(string commandString, int& badPos) {
        for (int i = 0; i < commandString.size(); i++) {
            if (commandString[i] == 'c' || commandString[i] == 'C') {
            }
            else if (commandString[i] == 'b' || commandString[i] == 'B') {
                if ((i + 1) >= commandString.size() || !(isprint(commandString[i + 1])))
                {
                    badPos = i + 1;
                    return 1;
                }
                i++;
            }
            else if (commandString[i] == 'f' || commandString[i] == 'F') {
                if ((i + 1) >= commandString.size() || !(isprint(commandString[i + 1])))
                {
                    badPos = i + 1;
                    return 1;
                }
                i++;
            }
            else if (commandString[i] == 'v' || commandString[i] == 'V' || commandString[i] == 'h' || commandString[i] == 'H') {
                //position after V must exist to make the command valid
                if ((i + 1) >= commandString.size()) {
                    badPos = i + 1;
                    return 1;
                //if there is a negative sign,
                //there must be a digit after it and there can be a digit after that digit
                }
                else if (commandString[i + 1] == '-') {
                    i++;
                    if ((i + 1) >= commandString.size() || !(isdigit(commandString[i + 1]))) {
                        badPos = i + 1;
                        return 1;
                    }
                    i++;
                    if ((i + 1) < commandString.size() && isdigit(commandString[i + 1])) {
                        i++;
                    }
                // if there is no negative sign,
                // there must be a digit after the command letter and there can be a digit after that digit
                }else if (isdigit(commandString[i + 1])) {
                    i++;
                    if ((i + 1) < commandString.size() && isdigit(commandString[i + 1])) {
                        i++;
                    }
                }
                else {
                    badPos = i + 1;
                    return 1;
                }
            }
            else {
                badPos = i;
                return 1;
            }
        }
        return 0;
}

int performCommands(string commandString, char& plotChar, int& mode, int& badPos) {
    //checks for a valid, printable character, a valid mode vaule(FG or BG), and command string
    if (!(isprint(plotChar)))
        return 2;
    if (mode != FG && mode != BG)
        return 2;
    if (isValidCommandString(commandString, badPos) == 1) {
        return 1;
    }


    //Parses through the verified command string executing commands
    int row = 1;
    int col = 1;
    for (int i = 0; i < commandString.size(); i++) {
        if (commandString[i] == 'c' || commandString[i] == 'C') {
            plotChar = '*';
            mode = FG;
            row = 1;
            col = 1;
            for (int i = 0; i <= getRows(); i++) {
                plotLine(i, 1, getCols()-1, HORIZ, ' ', FG);
            }
        }
        else if (commandString[i] == 'b' || commandString[i] == 'B') {
            mode = BG;
            plotChar = commandString[i + 1];
            i++;
        }
        else if (commandString[i] == 'f' || commandString[i] == 'F') {
            mode = FG;
            plotChar = commandString[i + 1];
            i++;
        }
        else if (commandString[i] == 'v' || commandString[i] == 'V') {
            //determines whether there is a negative sign and whether there are two digits after the command or just one
            //plots line using the command string's sign, direction, and distance
            //checks for out of grid error
            if (commandString[i + 1] == '-') {
                i++;
                if (isdigit(commandString[i + 2])) {
                    int dist = -1 * (((commandString[i + 1] - '0') * 10) + (commandString[i + 2] - '0'));
                    if (dist + row > getRows() || dist + row < 1) {
                        badPos = i-1;
                        return 3;
                    }
                    plotLine(row, col, dist, VERT, plotChar, mode);
                    row += dist;
                    i++;
                    i++;
                }
                else {
                    int dist = -1 * (commandString[i + 1] - '0');
                    if (dist + row > getRows() || dist + row < 1) {
                        badPos = i - 1;
                        return 3;
                    }
                    plotLine(row, col, dist, VERT, plotChar, mode);
                    row += dist;
                    i++;
                }

            }
            else {
                if (isdigit(commandString[i + 2])) {
                    int dist = ((commandString[i + 1] - '0') * 10) + (commandString[i + 2] - '0');
                    if (dist + row > getRows() || dist + row < 1) {
                        badPos = i;
                        return 3;
                    }
                    plotLine(row, col, dist, VERT, plotChar, mode);
                    row += dist;
                    i++;
                    i++;
                }
                else {
                    int dist = commandString[i + 1] - '0';
                    if (dist + row > getRows() || dist + row < 1) {
                        badPos = i;
                        return 3;
                    }
                    plotLine(row, col, dist, VERT, plotChar, mode);
                    row += dist;
                    i++;
                }
            }
        }
        else if (commandString[i] == 'h' || commandString[i] == 'H') {
            if (commandString[i + 1] == '-') {
                i++;
                if (isdigit(commandString[i + 2])) {
                    int dist = -1 * (((commandString[i + 1] - '0') * 10) + (commandString[i + 2] - '0'));
                    if (dist + col > getCols() || dist + col < 1) {
                        badPos = i - 1;
                        return 3;
                    }
                    plotLine(row, col, dist, HORIZ, plotChar, mode);
                    col += dist;
                    i++;
                    i++;
                }
                else {
                    int dist = -1 * (commandString[i + 1] - '0');
                    if (dist + col > getCols() || dist + col < 1) {
                        badPos = i - 1;
                        return 3;
                    }
                    plotLine(row, col, dist, HORIZ, plotChar, mode);
                    col += dist;
                    i++;
                }
            }
            else {
                if (isdigit(commandString[i + 2])) {
                    int dist = ((commandString[i + 1] - '0') * 10) + (commandString[i + 2] - '0');
                    if (dist + col > getCols() || dist + col < 1) {
                        badPos = i;
                        return 3;
                    }
                    plotLine(row, col, dist, HORIZ, plotChar, mode);
                    col += dist;
                    i++;
                    i++;
                }
                else {
                    int dist = commandString[i + 1] - '0';
                    if (dist + col > getCols() || dist + col < 1) {
                        badPos = i;
                        return 3;
                    }
                    plotLine(row, col, dist, HORIZ, plotChar, mode);
                    col += dist;
                    i++;
                }
            }
        }
        else {
            badPos = i;
            return 1;
        }
    }
    return 0;
}