#pragma once

//we maintain a char buffer for each line of the display
static char display_line_buffer[NOLED_LINES][LINE_BUFFER+1] = {{0}, {0}};
static uint8_t display_line_position[NOLED_LINES] = {0, 0};
//and a variable to track whether we are editing a line - default to NOLED_LINES means not editing (rather than 0 which is a valid line no)
static uint8_t display_line_edit = NOLED_LINES;

//borrowed these from process_terminal, we'll use them below
const char keycode_to_ascii_lut[58] =         {0, 0, 0, 0, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 0, 0, 0, '\t', ' ', '-', '=', '[', ']', '\\', 0, ';', '\'', '`', ',', '.', '/'};
const char shifted_keycode_to_ascii_lut[58] = {0, 0, 0, 0, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', 0, 0, 0, '\t', ' ', '_', '+', '{', '}', '|', 0, ':', '\'', '~', '<', '>', '?'};

//update line display
void displayUpdateLine(uint8_t line){
    char display_line[NOLED_COLS+1];

    //if its shorter than the display, pad it
    if(strlen(display_line_buffer[line])<NOLED_COLS){
        strcpy(display_line, display_line_buffer[line]);
        for(uint8_t i=strlen(display_line_buffer[line]); i<NOLED_COLS; i++){
            display_line[i] = (char)' ';
        }
        display_line[NOLED_COLS] = 0;

    //same just copy string
    }else if(strlen(display_line_buffer[line]) == NOLED_COLS){
        strcpy(display_line, display_line_buffer[line]);

    //longer string, copy the relevant part into the display area
    }else{
        uint8_t offset = display_line_position[line];
        if(offset>(strlen(display_line_buffer[line])-NOLED_COLS)){
            offset = (strlen(display_line_buffer[line])-NOLED_COLS);
        }
        strncpy(display_line, display_line_buffer[line] + offset, NOLED_COLS);
    }

    noledWriteChars(line, 0, display_line);
}

//set line contents
void displaySetLine(uint8_t line, char* text){
    display_line_position[line] = 0;
    strcpy(display_line_buffer[line], text);
    displayUpdateLine(line);
}

//scroll line, true to go forward false for back
void displayScrollLine(uint8_t line, bool forward){
    if(forward && display_line_position[line]<(strlen(display_line_buffer[line])-NOLED_COLS)){
        display_line_position[line] ++;
        displayUpdateLine(line);
    }else if(!forward && (display_line_position[line]>0)){
        display_line_position[line] --;
        displayUpdateLine(line);
    }
}

//handle a keypress to toggle line edit mode
void displaySetLineEdit(uint8_t line) {
    //exiting line edit of current line

    //currently in edit mode
    if(display_line_edit<NOLED_LINES){
        //display edited line
        display_line_position[display_line_edit] = 0;
        displayUpdateLine(display_line_edit);
    }

    //if current line, exit edit mode
    if(display_line_edit == line){
        // noledCommand(0b00001100); //display on cursor off
        display_line_edit = NOLED_LINES;
    //else switching
    }else if(line<NOLED_LINES){
        display_line_edit = line;
    }

    if(display_line_edit<NOLED_LINES){
        // noledCommand(0b00001110); //display on cursor on
        display_line_edit = line;
        display_line_buffer[line][0] = 0;
        display_line_position[line] = 0;
        displayUpdateLine(line);
    }

    // if(display_line_edit == line){
    //     display_line_position[display_line_edit] = 0;
    //     displayUpdateLine(display_line_edit);
    //       noledCommand(0b00001100); //display on cursor off

    //     display_line_edit = NOLED_LINES;

    // //switching to different line, update edited one and switch to new
    // }else if(display_line_edit<NOLED_LINES){
    //     display_line_position[display_line_edit] = 0;
    //     displayUpdateLine(display_line_edit);

    //     noledCommand(0b00001110); //display on cursor on
    //     display_line_edit = line;
    //     display_line_buffer[line][0] = 0;
    //     display_line_position[line] = 0;
    //     displayUpdateLine(line);

    // //switching into edit
    // }else{
    //     noledCommand(0b00001110); //display on cursor on
    //     display_line_edit = line;
    //     display_line_buffer[line][0] = 0;
    //     display_line_position[line] = 0;
    //     displayUpdateLine(line);
    // }
}

//again borrowed this from process_terminal
bool displayCaptureChar(uint16_t keycode){
    bool result = false;
    char char_to_add;

    keycode = keycode%256;
    if (keycode <= 58) {
        char_to_add = 0;
        if (get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) {
            char_to_add = shifted_keycode_to_ascii_lut[keycode];
        } else if (get_mods() == 0) {
            char_to_add = keycode_to_ascii_lut[keycode];
        }

        if ((char_to_add != 0) || (keycode == KC_BSPC)) {
            result = true;
            uint8_t len = strlen(display_line_buffer[display_line_edit]);

            if(keycode == KC_BSPC){
                len--;
            }else if(len<LINE_BUFFER){
                display_line_buffer[display_line_edit][len] = char_to_add;
                len++;
            }

            display_line_buffer[display_line_edit][len] = 0;
            if(len>NOLED_COLS){
                display_line_position[display_line_edit] = len-NOLED_COLS;
            }else{
                display_line_position[display_line_edit] = 0;
            }
            displayUpdateLine(display_line_edit);
        }
    }

    return result;
}