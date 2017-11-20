/*******************************************************************************
* Assignment: Project 1 - Lexical Analyzer for Scheme to C++ Translator        *
* Author: Dr. Watts                                                             *
* Date: Fall 2017                                                              *
* File: LexicalAnalyzer.h                                                      *
*                                                                              *
* Description: This file contains the                                          *
*******************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

/*******************************************************************************
* Function:                                                                    *
*                                                                              *
* Parameters:                                                                  *
* Return value:                                                                *
* Description: This function will                                              *
*******************************************************************************/
SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	lex = new LexicalAnalyzer (filename);
	string name = filename;
	string p2name = name.substr (0, name.length()-3) + ".p2"; 
	p2file.open (p2name.c_str());
	token = lex->GetToken();
    cout << "cons with filename: " << filename << endl;
	Program ();
}

/*******************************************************************************
* Function:                                                                    *
*                                                                              *
* Parameters:                                                                  *
* Return value:                                                                *
* Description: This function will                                              *
*******************************************************************************/
SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
	p2file.close ();
}

/*******************************************************************************
* Function:                                                                    *
*                                                                              *
* Parameters:                                                                  *
* Return value:                                                                *
* Description: This function will                                              *
*******************************************************************************/
int SyntacticalAnalyzer::Program ()
{
	p2file << "Entering Program function; current token is: "
					<< lex->GetTokenName (token) << endl;
	int errors = 0;
	// token should be in firsts of Program
	// Body of function goes here.
    errors += Define ();
    errors += More_Defines ();
	if (token != EOF_T)
	{
		errors++;
		lex->ReportError ("Missing end of file at end of program");
	}
	// token should be in follows of Program
	p2file << "Exiting Program function; current token is: "
					<< lex->GetTokenName (token) << endl;
	return errors;
}


/*******************************************************************************
* Function:                                                                    *
*                                                                              *
* Parameters:                                                                  *
* Return value:                                                                *
* Description: This function will                                              *
*******************************************************************************/
int SyntacticalAnalyzer::Define ()
{
	p2file << "Entering Define function; current token is: "
					<< lex->GetTokenName (token) << endl;
	int errors = 0;
	// token should be in firsts of Define
	// Body of function goes here.

    if (token == LPAREN_T) {
		token = lex->GetToken ();
    }
	else
	{
		errors++;
		lex->ReportError ("Expecting '('; saw " + lex->GetLexeme ());
	}
	
	while (token != DEFINE_T && token != EOF_T)
	{
		errors++;
		lex->ReportError ("Expecting 'define'; saw " + lex->GetLexeme ());
		token = lex->GetToken ();
	}
	//else
		token = lex->GetToken ();
	// token should be in follows of Define
	p2file << "Exiting Define function; current token is: "
					<< lex->GetTokenName (token) << endl;
	return errors;
}


int SyntacticalAnalyzer::More_Defines () {
    int errors = 0;
    token = lex->GetToken();
    if (token != DEFINE_T) {
        return errors;
    }
    errors += Define();
    return errors + More_Defines();
}

int SyntacticalAnalyzer::Param_List () {
    token = lex->GetToken();
    if (token != IDENT_T) { // null check
        return 0;
    }
    return Param_List();
}

int SyntacticalAnalyzer::Statement () {
    int errors = 0;
    token = lex->GetToken();
    if (token == IDENT_T) {
        return 0;
    }
    else if(token == LPAREN_T) {
        // call action
    }
    else {
        errors += Literal();
    }
    
    return errors;
}

int SyntacticalAnalyzer::Literal () {
    int errors = 0;
    token = lex->GetToken();
    if (token == NUMLIT_T || token == STRLIT_T) {
        return 0;
    }
    else if (QUOTE_T) {
        errors += Quoted_Lit();
    }
    else {
        return errors++;
    }
}

int SyntacticalAnalyzer::Quoted_Lit () {
    int errors = 0;
    token = lex->GetToken();
    // errors += Any_Other_Token();
    return errors;
}

int SyntacticalAnalyzer::Any_Other_Token () {
    int errors = 0;
    token = lex->GetToken();
    if (token == LPAREN_T) {
        // more tokens and rparent_t
    }
    else if (token == QUOTE_T) {
        errors += Any_Other_Token();
    }
    else if (token > 0 && token != RPAREN_T && token != LAMBDA && token != EOF_T) {
        return errors;
    }
    return ++errors;
}

int SyntacticalAnalyzer::More_Tokens () {
    int errors = 0;
    token = lex->GetToken();
    if (token == LAMBDA) {
        return 0;
    }
    errors += Any_Other_Token();
    return errors + More_Tokens();
}

