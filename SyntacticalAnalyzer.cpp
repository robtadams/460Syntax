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
    token = lex->GetToken ();
    while (token != LPAREN_T && token != EOF_T)
    {
        errors++;
        lex->ReportError ("Expecting '('; saw " + lex->GetLexeme ());
        token = lex->GetToken ();
    }
    token = lex->GetToken ();
    while (token != IDENT_T && token != EOF_T)
    {
        errors++;
        lex->ReportError ("Expecting 'IDENT_T'; saw " + lex->GetLexeme ());
        token = lex->GetToken ();
    }
    
    errors += Param_List();
    
    while (token != RPAREN_T && token != EOF_T)
    {
        errors++;
        lex->ReportError ("Expecting ')'; saw " + lex->GetLexeme ());
        token = lex->GetToken ();
    }
    
    errors += Statement();
    errors += Statement_List();
    
    while (token != RPAREN_T && token != EOF_T)
    {
        errors++;
        lex->ReportError ("Expecting ')'; saw " + lex->GetLexeme ());
        token = lex->GetToken (); // should we get this?
    }
    
    
	// token should be in follows of Define
	p2file << "Exiting Define function; current token is: "
					<< lex->GetTokenName (token) << endl;
	return errors;
}


int SyntacticalAnalyzer::More_Defines () {
    int errors = 0;
    token = lex->GetToken();
    if (token == EOF_T) {
        return errors; // lambda
    }
    // has to be this becaus it is not lambda
    errors += Define();
    errors += More_Defines();
    return errors;
}

int SyntacticalAnalyzer::Param_List () {
    int errors = 0;
    token = lex->GetToken();
    if (token == IDENT_T) {
        return Param_List();
    }
    else if (token == RPAREN_T) {
        return errors; // lambda
    }
    errors++;
    lex->ReportError ("Expecting ')'; saw " + lex->GetLexeme ());
    return errors;
}

int SyntacticalAnalyzer::Statement () {
    int errors = 0;
    token = lex->GetToken();
    if (token == IDENT_T) {
        return errors;
    }
    else if(token == LPAREN_T) {
        errors += Action();
        while (token != RPAREN_T && token != EOF_T) {
            errors++;
            lex->ReportError ("Expecting ')'; saw " + lex->GetLexeme ());
            token = lex->GetToken (); // should we get this?
        }
        return errors;
    }
    
    errors += Literal();
    
    return errors;
}

int SyntacticalAnalyzer::Statement_List () {
    int errors = 0;
    token = lex->GetToken();
    if (token == RPAREN_T) { // check for lambda
        return errors;
    }
    errors += Statement();
    errors += Statement_List();
    return errors;
}

int SyntacticalAnalyzer::Statement_Pair () {
    int errors = 0;
    token = lex->GetToken();
    if (token == LPAREN_T) {
        errors += Statement_Pair_Body();
        token = lex->GetToken();
        if (token == RPAREN_T) {
            return errors;
        }
        else {
            errors++;
            lex->ReportError ("Expecting ')'; saw " + lex->GetLexeme ());
            return errors;
        }
    }
    errors++;
    lex->ReportError ("Expecting '('; saw " + lex->GetLexeme ());
    return errors;
}

int SyntacticalAnalyzer::Statement_Pair_Body () {
    int errors = 0;
    token = lex->GetToken();
    if (token == LPAREN_T) {
        errors += Action();
        token = lex->GetToken();
        if (token != RPAREN_T) {
            errors++;
            lex->ReportError ("Expecting ')'; saw " + lex->GetLexeme ());
            return errors;
        }
        token = lex->GetToken();
        errors += Statement();
        return errors;
    }
    else if (token == NUMLIT_T || token == STRLIT_T || token == QUOTE_T) {
        errors += Literal();  // token will be an issue that we already have it.
        return errors;
    }
    errors += Action();
    return errors;
}

int SyntacticalAnalyzer::Literal () {
    int errors = 0;
    token = lex->GetToken(); // should we be getting this token before
    if (token == NUMLIT_T || token == STRLIT_T) {
        return errors;
    }
    else if (QUOTE_T) {
        errors += Quoted_Lit();
    }
    errors++;
    lex->ReportError ("Expecting 'LITERAL'; saw " + lex->GetLexeme ());
    return errors;
}

int SyntacticalAnalyzer::Quoted_Lit () {
    int errors = 0;
    errors += Any_Other_Token();
    return errors;
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

int SyntacticalAnalyzer::Any_Other_Token () {
    int errors = 0;
    token = lex->GetToken();
    switch (token) {
        case LPAREN_T:
            errors += More_Tokens();
            token = lex->GetToken();
            if (token != RPAREN_T) {
                errors++;
                lex->ReportError ("Expecting ')'; saw " + lex->GetLexeme ());
            }
            return errors;
        case IDENT_T:   return errors;
        case NUMLIT_T:  return errors;
        case STRLIT_T:  return errors;
        case CONS_T:  return errors;
        case IF_T:  return errors;
        case DISPLAY_T:   return errors;
        case NEWLINE_T:  return errors;
        case LISTOP_T:  return errors;
        case AND_T:  return errors;
        case OR_T:  return errors;
        case NOT_T:   return errors;
        case DEFINE_T:  return errors;
        case NUMBERP_T:  return errors;
        case SYMBOLP_T:  return errors;
        case LISTP_T:  return errors;
        case ZEROP_T:   return errors;
        case NULLP_T:  return errors;
        case STRINGP_T:  return errors;
        case PLUS_T:  return errors;
        case MINUS_T:  return errors;
        case DIV_T:   return errors;
        case MULT_T:  return errors;
        case MODULO_T:  return errors;
        case EQUALTO_T:  return errors;
        case GT_T:  return errors;
        case LT_T:   return errors;
        case GTE_T:  return errors;
        case LTE_T:  return errors;
        case QUOTE_T:
            errors += Any_Other_Token();
            return errors;
        default:
            errors++;
            lex->ReportError ("Expecting TOKEN; saw " + lex->GetLexeme ());
    }
}

int SyntacticalAnalyzer::Action () {
    int errors = 0;
    token = lex->GetToken();
    switch (token) {
        case IF_T:
            errors += Statement();
            errors += Statement();
            errors += Else_Part();
            return errors;
        case COND_T:
            errors += Statement_Pair();
            errors += More_Pairs();
            return errors;
        case LISTOP_T:
            errors += Statement();
            return errors;
        case CONS_T:
            errors += Statement();
            errors += Statement();
            return errors;
        case AND_T:
            errors += Statement_List();
            return errors;
        case OR_T:
            errors += Statement_List();
            return errors;
        case NOT_T:
            errors += Statement();
            return errors;
        case NUMBERP_T:
            errors += Statement();
            return errors;
        case SYMBOLP_T:
            errors += Statement();
            return errors;
        case LISTP_T:
            errors += Statement();
            return errors;
        case ZEROP_T:
            errors += Statement();
            return errors;
        case NULLP_T:
            errors += Statement();
            return errors;
        case STRINGP_T:
            errors += Statement();
            return errors;
        case PLUS_T:
            errors += Statement_List();
            return errors;
        case MINUS_T:
            errors += Statement();
            errors += Statement_List();
            return errors;
        case DIV_T:
            errors += Statement();
            errors += Statement_List();
            return errors;
        case MULT_T:
            errors += Statement_List();
            return errors;
        case MODULO_T:
            errors += Statement();
            errors += Statement();
            return errors;
        case EQUALTO_T:
            errors += Statement_List();
            return errors;
        case GT_T:
            errors += Statement_List();
            return errors;
        case LT_T:
            errors += Statement_List();
            return errors;
        case GTE_T:
            errors += Statement_List();
            return errors;
        case LTE_T:
            errors += Statement_List();
            return errors;
        case IDENT_T:
            errors += Statement_List();
            return errors;
        case DISPLAY_T:
            errors += Statement();
            return errors;
        case NEWLINE_T:
            return errors;
        default:
            errors++;
            lex->ReportError ("Expecting 'ACTION'; saw " + lex->GetLexeme ());
            return errors;
    }
}

int SyntacticalAnalyzer::Else_Part() {
    int errors = 0;
    token = lex->GetToken();
    if (token == RPAREN_T) {
        return errors;
    }
    errors += Statement();
    return errors;
}

int SyntacticalAnalyzer::More_Pairs() {
    int errors = 0;
    token = lex->GetToken();
    if (token == RPAREN_T) {
        return errors; // lambda
    }
    errors += Statement_Pair();
    errors += More_Pairs();
    return errors;
}




