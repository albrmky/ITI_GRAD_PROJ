/*
 * Parser.h
 *
 *  Created on: Mar 26, 2022
 *      Author: noureldeen albrmky
 */

#ifndef PARSER_H_
#define PARSER_H_

u8 ASCII_To_HexDigit(char ch);

u8 ASCII_To_u8Number(char High_Nibble, char Low_Nibble);

u8 Check_Record(u8 *record);

void Parse_Record(u8 *record);

#endif /* PARSER_H_ */
