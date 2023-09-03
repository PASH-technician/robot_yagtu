#include <LiquidCrystal_I2C.h>
#include <ArduinoSTL.h>
#include <list>

#define WIDTH_LINE 16
#define LINES 2

LiquidCrystal_I2C lcd(0x27, WIDTH_LINE, LINES);

String s1 = "Robotcop 2.0";

uint8_t indexCursor = 0;
uint8_t lineCursor = 0;

using namespace std;

void setup() {
  Serial.begin(9600);
  Serial.println("Начало выполнения");
  lcd.init();
  lcd.backlight();

  autoTransferPrinting(s1);
}

void loop() {
  blinkCursor(300);
}

void setCursor(uint8_t x, uint8_t y) {
  indexCursor = x;
  lineCursor = y;
  lcd.setCursor(indexCursor, lineCursor);
}

list<String> getSplit(String str) {
  String inputStr = removingExtraSpaces(str);
  list<String> resultStr;
  String newStr;

  for (char c : inputStr) {
    if (c == ' ') {
      if (newStr.length() != 0) {
        resultStr.push_back(newStr);
        newStr = "";
      }
      continue;
    }
    newStr += c;
  }
  resultStr.push_back(newStr);
  return resultStr;
}

String removingExtraSpaces(String str) {
  String result;
  for (char c : str) {
    if (c == ' ' && result.end() == ' ') {
      continue;
    }
    result += c;
  }
  return result;
}

list<String> getWordsFitLine(list<String> listStr) {
  list<String> resultList;
  String newStr;

  for (String str : listStr) {
    String space;
    if (newStr.length() != 0) {
      space = " ";
    } else {
      space = "";
    }
    if (newStr.length() + str.length() + space.length() <= WIDTH_LINE) {
      newStr += space;
      newStr += str;
      continue;
    }
    resultList.push_back(newStr);
    newStr = str;
  }
  resultList.push_back(newStr);

  for (String str : resultList) {
    Serial.println(str);
  }

  return resultList;
}

void autoTransferPrinting(String str) {
  list<String> listStr = getWordsFitLine(getSplit(str));
  int numberLine = 0;
  setCursor(0, 0);
  for (String str : listStr) {
    setCursor(0, numberLine);
    printBlinkCursor(str);
    numberLine += 1;
  }
}

void printBlinkCursor(String str) {
  for (int i = 0; i < str.length(); i++) {
    lcd.print(str[i]);
    blinkCursor();
  }
}

void blinkCursor(int timeBlink) {
  delay(timeBlink);
  lcd.noCursor();
  delay(timeBlink);
  lcd.cursor();
}

void blinkCursor() {
  blinkCursor(50);
}