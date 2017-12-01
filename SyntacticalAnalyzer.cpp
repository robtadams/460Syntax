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
    token = lex->GetToken();
    
    errors += Param_List();
    
    while (token != RPAREN_T && token != EOF_T)
    {
        errors++;
        lex->ReportError ("Expecting ')'; saw " + lex->GetLexeme ());
        token = lex->GetToken ();
    }
    
    token = lex->GetToken();
    errors += Statement();
    errors += Statement_List();
    
    while (token != RPAREN_T && token != EOF_T)
    {
        errors++;
        lex->ReportError ("Expecting ')'; saw " + lex->GetLexeme ());
        token = lex->GetToken (); // should we get this?
    }
    token = lex->GetToken();
    
    
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
    // has to be this becaus it is not lambda
    if (token == LPAREN_T) {
        errors += Define();
        errors += More_Defines();
    }
    
    p2file << "Exiting More_Defines function; current token is: " << lex->GetTokenName (token) << endl;
    return errors;
}

int SyntacticalAnalyzer::Param_List () {
    p2file << "Entering Param_List function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token == EOF_T)
        return errors;
    
    if (token == IDENT_T) {
        token = lex->GetToken();
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
    
    if (token == IDENT_T) {
        token = lex->GetToken();
        return errors;
    }
    else if (token == LPAREN_T) {
        token = lex->GetToken();
        errors += Action(); // Return here 2
        if (token != RPAREN_T) {
            errors++;
            lex->ReportError ("Expecting ); saw " + lex->GetLexeme ());
            return errors;
        }
        token = lex->GetToken();
        return errors;
    }
    errors += Literal();
    
    return errors;
}

int SyntacticalAnalyzer::Statement_List () {
    p2file << "Entering Statement_List function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token == RPAREN_T) {
        return errors; // done
    }
    
    errors += Statement();
    errors += Statement_List();
    
    return errors;
}

int SyntacticalAnalyzer::Statement_Pair () {
    p2file << "Entering Statement_Pair function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token == EOF_T)
        return errors;
    
    if (token == LPAREN_T) {
        token = lex->GetToken();
        errors += Statement_Pair_Body();
        if (token == RPAREN_T) {
            token = lex->GetToken();
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
    if (token == LPAREN_T) {
        token = lex->GetToken();
        errors += Action();
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
    if (token == NUMLIT_T || token == STRLIT_T) {
        token = lex->GetToken();
        return errors;
    }
    else if (token == QUOTE_T) {
        token = lex->GetToken();
        errors += Quoted_Lit();
    }
    else {
        errors++;
        lex->ReportError ("Expecting 'LITERAL'; saw " + lex->GetLexeme ());
    }
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
    if (token == RPAREN_T)
        return errors;
    
    errors += Any_Other_Token();
    errors += More_Tokens();
    return errors;
}

int SyntacticalAnalyzer::Any_Other_Token () {
    p2file << "Entering Any_Other_Token function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token == EOF_T)
        return errors;
    switch (token) {
        case LPAREN_T:
            token = lex->GetToken();
            errors += More_Tokens();
            if (token != RPAREN_T) {
                errors++;
                lex->ReportError ("Expecting ')'; saw " + lex->GetLexeme ());
            }
            token = lex->GetToken();
            break;
        case IDENT_T:   token = lex->GetToken(); break;
        case NUMLIT_T:  token = lex->GetToken(); break;
        case STRLIT_T:  token = lex->GetToken(); break;
        case CONS_T:  token = lex->GetToken(); break;
        case IF_T:  token = lex->GetToken(); break;
        case DISPLAY_T:   token = lex->GetToken(); break;
        case NEWLINE_T:  token = lex->GetToken(); break;
        case LISTOP_T:  token = lex->GetToken(); break;
        case AND_T:  token = lex->GetToken(); break;
        case OR_T:  token = lex->GetToken(); break;
        case NOT_T:   token = lex->GetToken(); break;
        case DEFINE_T:  token = lex->GetToken(); break;
        case NUMBERP_T:  token = lex->GetToken(); break;
        case SYMBOLP_T:  token = lex->GetToken(); break;
        case LISTP_T:  token = lex->GetToken(); break;
        case ZEROP_T:   token = lex->GetToken(); break;
        case NULLP_T:  token = lex->GetToken(); break;
        case STRINGP_T:  token = lex->GetToken(); break;
        case PLUS_T:  token = lex->GetToken(); break;
        case MINUS_T:  token = lex->GetToken(); break;
        case DIV_T:   token = lex->GetToken(); break;
        case MULT_T:  token = lex->GetToken(); break;
        case MODULO_T:  token = lex->GetToken(); break;
        case EQUALTO_T:  token = lex->GetToken(); break;
        case GT_T:  token = lex->GetToken(); break;
        case LT_T:   token = lex->GetToken(); break;
        case GTE_T:  token = lex->GetToken(); break;
        case LTE_T:  token = lex->GetToken(); break;
        case QUOTE_T:
            token = lex->GetToken();
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
    cout << "Action: getting token " << lex->GetTokenName(token) << endl;
    switch (token) {
        case IF_T:
            token = lex->GetToken();
            errors += Statement();
            errors += Statement();
            errors += Else_Part();
            break;
        case COND_T:
            token = lex->GetToken();
            errors += Statement_Pair();
            errors += More_Pairs();
            break;
        case LISTOP_T:
            token = lex->GetToken();
            errors += Statement();
            break;
        case CONS_T:
            token = lex->GetToken();
            errors += Statement();
            errors += Statement();
            break;
        case AND_T:
            token = lex->GetToken();
            errors += Statement_List();
            break;
        case OR_T:
            token = lex->GetToken();
            errors += Statement_List();
            break;
        case NOT_T:
            token = lex->GetToken();
            errors += Statement();
            break;
        case NUMBERP_T:
            token = lex->GetToken();
            errors += Statement();
            break;
        case SYMBOLP_T:
            token = lex->GetToken();
            errors += Statement();
            break;
        case LISTP_T:
            token = lex->GetToken();
            errors += Statement();
            break;
        case ZEROP_T:
            token = lex->GetToken();
            errors += Statement();
            break;
        case NULLP_T:
            token = lex->GetToken();
            errors += Statement();
            break;
        case STRINGP_T:
            token = lex->GetToken();
            errors += Statement();
            break;
        case PLUS_T:
            token = lex->GetToken();
            errors += Statement_List();
            break;
        case MINUS_T:
            token = lex->GetToken();
            errors += Statement();
            errors += Statement_List();
            break;
        case DIV_T:
            token = lex->GetToken();
            errors += Statement();
            errors += Statement_List();
            break;
        case MULT_T:
            token = lex->GetToken();
            errors += Statement_List();
            break;
        case MODULO_T:
            token = lex->GetToken();
            errors += Statement();
            errors += Statement();
            break;
        case EQUALTO_T:
            token = lex->GetToken();
            errors += Statement_List();
            return errors;
        case GT_T:
            token = lex->GetToken();
            errors += Statement_List();
            break;
        case LT_T:
            token = lex->GetToken();
            errors += Statement_List();
            break;
        case GTE_T:
            token = lex->GetToken();
            errors += Statement_List();
            break;
        case LTE_T:
            token = lex->GetToken();
            errors += Statement_List();
            break;
        case IDENT_T:
            token = lex->GetToken();
            errors += Statement_List();
            break;
        case DISPLAY_T:
            token = lex->GetToken();
            errors += Statement();
            break;
        case NEWLINE_T:
            token = lex->GetToken();
            break;
        default:
            errors++;
            lex->ReportError ("Expecting 'ACTION'; saw " + lex->GetLexeme ());
    }
    return errors;
}

int SyntacticalAnalyzer::Else_Part() {
    p2file << "Entering Else_Part function; current token is: " << lex->GetTokenName (token) << endl;
    int errors = 0;
    if (token == EOF_T)
        return errors;
    
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
    
    if (token == RPAREN_T) {
        return errors; // lambda
    }
    
    errors += Statement_Pair();
    errors += More_Pairs();
    return errors;
}




