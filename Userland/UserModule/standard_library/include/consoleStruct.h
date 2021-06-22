typedef enum Color {
	Black, Blue, Green, Cyan, Red, Purple, Brown, Gray, DarkGray, LightBlue, LightGreen, LightCyan, 
	LightRed, LightPurple, Yellow, White
} Color;

typedef struct ConsoleParameters {
    int cursorSymbol;
    int delimiterSymbol;
    Color leftDelimeterColor;
    Color rightDelimeterColor;
    Color leftBackgroundColor;
    Color rightBackgroundColor;
    Color leftActiveTextColor;
    Color rightActiveTextColor;
    Color leftInactiveTextColor;
    Color rightInactiveTextColor;
    Color leftActiveErrorColor;
    Color rightActiveErrorColor;
    Color leftInactiveErrorColor;
    Color rightInactiveErrorColor;
    int registryCaptureKey;
    int enterKey;
    int deleteKey;
} ConsoleParameters;