#include <stdio.h>
#include <stdlib.h>

/* Defines constants representing each state in the DFA */
enum Statetype {
    REGULAR,            /* Regular code state */
    SAW_SLASH,          /* Saw a '/' character */
    INSIDE_COMMENT,     /* Inside a comment block */
    ASTERISK_IN_COMMENT,/* Found '*' inside a comment */
    STRING_START,       /* Potentially inside a string literal */
    ESCAPE_STRING,      /* Handling escape sequence in a string literal */
    CHAR_START,         /* Potentially inside a character literal */
    ESCAPE_CHAR         /* Handling escape sequence in a character literal */
};

/* State transition functions */
enum Statetype regularState(int c);
enum Statetype sawSlashState(int c);
enum Statetype insideCommentState(int c);
enum Statetype asteriskInCommentState(int c);
enum Statetype stringStartState(int c);
enum Statetype escapeStringState(int c);
enum Statetype charStartState(int c);
enum Statetype escapeCharState(int c);

/*
 * main: Reads input from stdin, processes it by character using 
 * DFA, and removes comments while preserving string and character literals.
 *
 * Data Flow:
 *  - Reads characters from stdin (standard input) using getchar().
 *  - Writes characters to stdout (standard output) using putchar().
 * 
 * Returns:
 *  - Returns EXIT_FAILURE if an unterminated comment is detected.
 *  - Otherwise, returns 0 (EXIT_SUCCESS).
 */
int main(void) {
    int c; /* Current character from the input */
    
    /* Initialize the DFA in the REGULAR state */
    enum Statetype state = REGULAR;

    /* Process input character by character */
    while ((c = getchar()) != EOF) {
        switch (state) {
            case REGULAR:
                state = regularState(c);
                break;
            case SAW_SLASH:
                state = sawSlashState(c);
                break;
            case INSIDE_COMMENT:
                state = insideCommentState(c);
                break;
            case ASTERISK_IN_COMMENT:
                state = asteriskInCommentState(c);
                break;
            case STRING_START:
                state = stringStartState(c);
                break;
            case ESCAPE_STRING:
                state = scapeStringState(c);
                break;
            case CHAR_START:
                state = charStartState(c);
                break;
            case ESCAPE_CHAR:
                state = escapeCharState(c);
                break;
        }
    }

    /* If still inside a comment at the end of input, return failure */
    if (state == INSIDE_COMMENT || state == ASTERISK_IN_COMMENT) {
        return EXIT_FAILURE;
    }

    /* Otherwise, return success */
    return EXIT_SUCCESS;
}

/*
 * regularState: Processes regular characters and detects the start of
 * comments, string literals, or character literals. Outputs regular characters
 * to stdout.
 * 
 * Parameters:
 *  - c: The current character from stdin being processed.
 * 
 * Returns:
 *  - The next state for the DFA based on the input character.
 * 
 * Global Impact:
 *  - Reads from stdin. 
 *  - Writes characters to stdout when processing regular text.
 */
enum Statetype regularState(int c) {
    if (c == '/') {
        return SAW_SLASH;
    } else if (c == '"') {
        putchar(c);  /* Output the opening double quote */
        return STRING_START;
    } else if (c == '\'') {
        putchar(c);  /* Output the opening single quote */
        return CHAR_START;
    } else {
        putchar(c);  /* Output the regular character */
        return REGULAR;
    }
}

/*
 * sawSlashState: Checks if the '/' character starts a comment or is part
 * of a string or character literal. Outputs characters if not in a comment.
 * 
 * Parameters:
 *  - c: The current character from stdin being processed.
 * 
 * Returns:
 *  - The next state for the DFA (INSIDE_COMMENT, STRING_START, CHAR_START, etc.)
 *    based on the input character.
 * 
 * Global Impact:
 *  - Reads from stdin. 
 *  - Writes to stdout when not in a comment.
 */
enum Statetype sawSlashState(int c) {
    if (c == '*') {
        return INSIDE_COMMENT;  /* Start of a comment */
    } else if (c == '/') {
        return SAW_SLASH;       /* Handle consecutive slashes */
    } else if (c == '"') {
        putchar('/');           /* Output the initial '/' */
        putchar(c);             /* Output the quote */
        return STRING_START;
    } else if (c == '\'') {
        putchar('/');           /* Output the initial '/' */
        putchar(c);             /* Output the single quote */
        return CHAR_START;
    } else {
        putchar('/');           /* Output the initial '/' */
        putchar(c);             /* Output the current character */
        return REGULAR;
    }
}

/*
 * insideCommentState: Ignores all characters within a comment. Transitions
 * to checking for the end of the comment when '*' is encountered.
 * 
 * Parameters:
 *  - c: The current character from stdin being processed.
 * 
 * Returns:
 *  - The next state for the DFA (INSIDE_COMMENT or ASTERISK_IN_COMMENT).
 * 
 * Global Impact:
 *  - Reads from stdin. 
 *  - No output to stdout since comments are ignored.
 */
enum Statetype insideCommentState(int c) {
    if (c == '*') {
        return ASTERISK_IN_COMMENT;  /* Found an asterisk, check for comment end */
    } else {
        return INSIDE_COMMENT;       /* Stay inside comment */
    }
}

/**
 * asteriskInCommentState: Determines if a comment is ending by checking
 * for a closing '/' after the asterisk '*'.
 * 
 * Parameters:
 *  - c: The current character from stdin being processed.
 * 
 * Returns:
 *  - The next state for the DFA (REGULAR or INSIDE_COMMENT).
 * 
 * Global Impact:
 *  - Reads from stdin. 
 *  - No output to stdout while inside a comment.
 */
enum Statetype asteriskInCommentState(int c) {
    if (c == '/') {
        return REGULAR;  /* End of comment */
    } else if (c == '*') {
        return ASTERISK_IN_COMMENT;  /* Stay in the asterisk state */
    } else {
        return INSIDE_COMMENT;       /* Return to the comment content */
    }
}

/*
 * stringStartState: Processes characters inside a string literal. Outputs
 * all characters to stdout, and transitions to escape handling if necessary.
 * 
 * Parameters:
 *  - c: The current character from stdin being processed.
 * 
 * Returns:
 *  - The next state for the DFA (STRING_START, ESCAPE_STRING, or REGULAR).
 * 
 * Global Impact:
 *  - Reads from stdin. 
 *  - Writes to stdout to print string literal characters.
 */
enum Statetype stringStartState(int c) {
    if (c == '"') {
        putchar(c);  /* Output the closing double quote */
        return REGULAR;
    } else if (c == '\\') {
        putchar(c);  /* Output the backslash */
        return ESCAPE_STRING;
    } else {
        putchar(c);  /* Output string content */
        return STRING_START;
    }
}

/*
 * escapeStringState: Handles escape sequences within a string literal.
 * Outputs the escaped character to stdout.
 * 
 * Parameters:
 *  - c: The current character from stdin being processed.
 * 
 * Returns:
 *  - The next state for the DFA (STRING_START).
 * 
 * Global Impact:
 *  - Reads from stdin. 
 *  - Writes to stdout to print the escaped character.
 */
enum Statetype escapeStringState(int c) {
    putchar(c);  /* Output the escaped character */
    return STRING_START;
}

/*
 * charStartState: Processes characters inside a character literal. Outputs
 * all characters to stdout, and transitions to escape handling if necessary.
 * 
 * Parameters:
 *  - c: The current character from stdin being processed.
 * 
 * Returns:
 *  - The next state for the DFA (CHAR_START, ESCAPE_CHAR, or REGULAR).
 * 
 * Global Impact:
 *  - Reads from stdin. 
 *  - Writes to stdout to print character literal characters.
 */
enum Statetype charStartState(int c) {
    if (c == '\'') {
        putchar(c);  /* Output the closing single quote */
        return REGULAR;
    } else if (c == '\\') {
        putchar(c);  /* Output the backslash */
        return ESCAPE_CHAR;
    } else {
        putchar(c);  /* Output character literal content */
        return CHAR_START;
    }
}

/*
 * escapeCharState: Handles escape sequences within a string literal.
 * Outputs the escaped character to stdout.
 * 
 * Parameters:
 *  - c: The current character from stdin being processed.
 * 
 * Returns:
 *  - The next state for the DFA (STRING_START).
 * 
 * Global Impact:
 *  - Reads from stdin. 
 *  - Writes to stdout to print the escaped character.
 */
enum Statetype escapeCharState(int c) {
    putchar(c);  /* Output the escaped character */
    return CHAR_START;
}