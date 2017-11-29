/*******************************************************************************
* Assignment: Project 1 - Lexical Analyzer for Scheme to C++ Translator        *
* Author: Dr. Watts                                                             *
* Date: Fall 2017                                                              *
* File: LexicalAnalyzer.h                                                      *
*                                                                              *
* Description: This file contains the                                          *
*******************************************************************************/

#ifndef SYN_H
#define SYN_H

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

using namespace std;

class SyntacticalAnalyzer 
{
    public:
	SyntacticalAnalyzer (char * filename);
	~SyntacticalAnalyzer ();
    private:
	LexicalAnalyzer * lex;
	ofstream p2file;
	token_type token;
	int Program (); // test
    int Define (); // test
    int More_Defines (); // test
    int Param_List (); //test
    int Statement (); // test
    int Statement_List (); // test
    int Statement_Pair (); // test
    int Statement_Pair_Body (); // check for issue with Literal
    int Literal ();  // test
    int Quoted_Lit (); // test
    int Any_Other_Token (); // test
    int More_Tokens (); // test
    int Action (); // test
    int Else_Part (); // test
    int More_Pairs (); // test
};
	
#endif
