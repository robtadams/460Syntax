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
	int Program (); // done
    int Define (); // done
    int More_Defines (); // done
    int Param_List (); //done
    int Statement (); // done
    int Statement_List (); // done
    int Statement_Pair (); // done
    int Statement_Pair_Body (); // check for issue with Literal
    int Literal ();  // done
    int Quoted_Lit (); // done
    int Any_Other_Token (); //
    int More_Tokens ();
    int Action (); // done
    int Else_Part (); // done
    int More_Pairs ();
};
	
#endif
