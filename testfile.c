// Single-line comment at the start of the file
int main() {
    /* Multi-line comment at the start of the function
       with several lines of text to ensure proper handling */
       
    int x = 10; /* Inline comment after code */
    
    // Another single-line comment

    /* Comment with a line break inside
       continues here */

    /* Comment followed by code */
    int y = 20;

    /* String literal with fake comment inside */
    char* str = "This is a string /* not a comment */ literal";

    /* Character literal with fake comment inside */
    char ch = 'a';  // Normal character
    char ch2 = '\''; // Character literal with escaped quote
    char ch3 = '/*'; // Fake comment inside a char literal

    /* Multi-line comment ending on the same line */ int z = 30;

    /* Comment inside a string literal with an escape sequence */
    char* escapedStr = "This is a string with an escape sequence: \"quote\" ";

    /* Character literal with an escaped backslash */
    char ch4 = '\\';

    /* String literal that ends without a closing quote
       This will test unterminated string handling */
    char* unterminatedStr = "This is an unterminated string literal;

    /* Comment that isn't terminated properly to test unterminated comment handling
       This should trigger a failure in your decomment program */

    return 0;
    // Single-line comment at the end of the file
}
