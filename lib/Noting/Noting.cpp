#include <Arduino.h>

int getNoteFrequency(char note, int octave = 0) {
    int toReturn = 0;

    switch (note) {
        case 'c':
            toReturn = 32;
            break;

        case 'C':
            toReturn = 34;
            break;

        case 'd':
            toReturn = 36;
            break;

        case 'D':
            toReturn = 38;
            break;

        case 'e':
            toReturn = 41;
            break;

        case 'f':
            toReturn = 43;
            break;

        case 'F':
            toReturn = 46;
            break;

        case 'g':
            toReturn = 49;
            break;

        case 'G':
            toReturn = 52;
            break;

        case 'a':
            toReturn = 55;
            break;

        case 'A':
            toReturn = 58;
            break;

        case 'b':
            toReturn = 61;
            break;

        default:
            break;
    }

    return toReturn * pow(2, octave);
}

int getNoteLength(int first, int second, char bpm = 120) {
    //int time = 1000 * ((float)first / (float)second); // ms
    int time = 1152 / second;
    time *= first;

    return time;
}
