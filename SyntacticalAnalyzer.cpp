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

    while (token != LPAREN_T && token != EOF_T)
    {
        errors++;
        lex->ReportError ("Expecting '('; saw " + lex->GetLexeme ());
        token = lex->GetToken ();
    }
    token = lex->GetToken ();
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
    
    token = lex->GetToken ();
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
    p2file << "Entering More_Defines function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token == EOF_T) {
        return errors; // lambda
    }
    token = lex->GetToken();
    // has to be this becaus it is not lambda
    errors += Define();
    errors += More_Defines();
    
    p2file << "Exiting More_Defines function; current token is: " << lex->GetTokenName (token) << endl;
    return errors;
}

int SyntacticalAnalyzer::Param_List () {
    p2file << "Entering Param_List function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token == EOF_T)
        return errors;
    token = lex->GetToken();
    if (token == IDENT_T) {
        return Param_List();
    }
    else if (token == RPAREN_T) {
        p2file << "Exiting Param_List function; current token is: " << lex->GetTokenName (token) << endl;
        return errors; // lambda
    }
    errors++;
    lex->ReportError ("Expecting ')'; saw " + lex->GetLexeme ());
    p2file << "Exiting Param_List function; current token is: " << lex->GetTokenName (token) << endl;
    return errors;
}

int SyntacticalAnalyzer::Statement () {
    p2file << "Entering Statement function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token == EOF_T)
        return errors;
    token = lex->GetToken();
    if (token == IDENT_T) {
        return errors;
    }
    else if(token == LPAREN_T) {
        errors += Action(); // getting token at end of action.
        while (token != RPAREN_T && token != EOF_T) {
            errors++;
            lex->ReportError ("Expecting ')'; saw " + lex->GetLexeme ());
            token = lex->GetToken ();
        }
        return errors;
    }
    while (token != NUMLIT_T && token != STRLIT_T && token != QUOTE_T) {
        errors++;
        lex->ReportError ("Expecting NUMLIT_T, STRLIT_T, QUOTE_T; saw " + lex->GetLexeme ());
        token = lex->GetToken ();
    }
    errors += Literal();
    
    return errors;
}

int SyntacticalAnalyzer::Statement_List () {
    p2file << "Entering Statement_List function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token == EOF_T)
        return errors;
    token = lex->GetToken();
    if (token == RPAREN_T) { // check for lambda
        token = lex->GetToken();
        return errors;
    }
    if (token != EOF_T) {
        errors += Statement();
        errors += Statement_List();
    }
    return errors;
}

int SyntacticalAnalyzer::Statement_Pair () {
    p2file << "Entering Statement_Pair function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token == EOF_T)
        return errors;
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
    p2file << "Entering Statement_Pair_Body function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token == EOF_T)
        return errors;
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
    p2file << "Entering Literal function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token == EOF_T)
        return errors;
    token = lex->GetToken(); // should we be getting this token before
    if (token == NUMLIT_T || token == STRLIT_T) {
        return errors;
    }
    else if (token == QUOTE_T) {
        errors += Quoted_Lit();
    }
    errors++;
    lex->ReportError ("Expecting 'LITERAL'; saw " + lex->GetLexeme ());
    return errors;
}

int SyntacticalAnalyzer::Quoted_Lit () {
    p2file << "Entering Quoted_Lit function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token != EOF_T)
        errors += Any_Other_Token();
    return errors;
}

int SyntacticalAnalyzer::More_Tokens () {
    p2file << "Entering More_Tokens function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token == EOF_T)
        return errors;
    token = lex->GetToken();
    if (token == LAMBDA || token == EOF_T) {
        return 0;
    }
    errors += Any_Other_Token();
    return errors + More_Tokens();
}

int SyntacticalAnalyzer::Any_Other_Token () {
    p2file << "Entering Any_Other_Token function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token == EOF_T)
        return errors;
    token = lex->GetToken();
    switch (token) {
        case LPAREN_T:
            errors += More_Tokens();
            token = lex->GetToken();
            if (token != RPAREN_T) {
                errors++;
                lex->ReportError ("Expecting ')'; saw " + lex->GetLexeme ());
            }
            break;
        case IDENT_T:  break;
        case NUMLIT_T:  break;
        case STRLIT_T:  break;
        case CONS_T:  break;
        case IF_T:  break;
        case DISPLAY_T:   break;
        case NEWLINE_T:  break;
        case LISTOP_T:  break;
        case AND_T:  break;
        case OR_T:  break;
        case NOT_T:   break;
        case DEFINE_T:  break;
        case NUMBERP_T:  break;
        case SYMBOLP_T:  break;
        case LISTP_T:  break;
        case ZEROP_T:   break;
        case NULLP_T:  break;
        case STRINGP_T:  break;
        case PLUS_T:  break;
        case MINUS_T:  break;
        case DIV_T:   break;
        case MULT_T:  break;
        case MODULO_T:  break;
        case EQUALTO_T:  break;
        case GT_T:  break;
        case LT_T:   break;
        case GTE_T:  break;
        case LTE_T:  break;
        case QUOTE_T:
            errors += Any_Other_Token();
            break;
        default:
            errors++;
            lex->ReportError ("Expecting TOKEN; saw " + lex->GetLexeme ());
    }
    return errors; 
}

int SyntacticalAnalyzer::Action () {
    // add gget token to each switch //done
    p2file << "Entering Action function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token == EOF_T)
        return errors;
    token = lex->GetToken();
    cout << "Action: getting token " << lex->GetTokenName(token) << endl;
    switch (token) {
        case IF_T:
            errors += Statement();
            errors += Statement();
            errors += Else_Part();
            break;
        case COND_T:
            errors += Statement_Pair();
            errors += More_Pairs();
            break;
        case LISTOP_T:
            errors += Statement();
            break;
        case CONS_T:
            errors += Statement();
            errors += Statement();
            break;
        case AND_T:
            errors += Statement_List();
            break;
        case OR_T:
            errors += Statement_List();
            break;
        case NOT_T:
            errors += Statement();
            break;
        case NUMBERP_T:
            errors += Statement();
            break;
        case SYMBOLP_T:
            errors += Statement();
            break;
        case LISTP_T:
            errors += Statement();
            break;
        case ZEROP_T:
            errors += Statement();
            break;
        case NULLP_T:
            errors += Statement();
            break;
        case STRINGP_T:
            errors += Statement();
            break;
        case PLUS_T:
            errors += Statement_List();
            break;
        case MINUS_T:
            errors += Statement();
            errors += Statement_List();
            break;
        case DIV_T:
            errors += Statement();
            errors += Statement_List();
            break;
        case MULT_T:
            errors += Statement_List();
            break;
        case MODULO_T:
            errors += Statement();
            errors += Statement();
            break;
        case EQUALTO_T:
            errors += Statement_List();
            return errors;
        case GT_T:
            errors += Statement_List();
            break;
        case LT_T:
            errors += Statement_List();
            break;
        case GTE_T:
            errors += Statement_List();
            break;
        case LTE_T:
            errors += Statement_List();
            break;
        case IDENT_T:
            errors += Statement_List();
            break;
        case DISPLAY_T:
            errors += Statement();
            break;
        case NEWLINE_T:
            break;
        default:
            errors++;
            lex->ReportError ("Expecting 'ACTION'; saw " + lex->GetLexeme ());
    }
//    token = lex->GetToken();
    return errors;
}

int SyntacticalAnalyzer::Else_Part() {
    p2file << "Entering Else_Part function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token == EOF_T)
        return errors;
    token = lex->GetToken();
    if (token == RPAREN_T) {
        return errors;
    }
    errors += Statement();
    return errors;
}

int SyntacticalAnalyzer::More_Pairs() {
    p2file << "Entering More_Pairs function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token == EOF_T)
        return errors;
    token = lex->GetToken();
    if (token == RPAREN_T) {
        return errors; // lambda
    }
    errors += Statement_Pair();
    errors += More_Pairs();
    return errors;
}




