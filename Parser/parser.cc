//
//  parser.cc
//  Parser
//
//  Created by Jack on 7/28/17.
//  Copyright © 2017 Ru Zhang. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "parser.h"
#include "lexer.h"
#include "inputbuf.h"

using namespace std;

LexicalAnalyzer lexer;


int main(int argc, char ** argv)
{
    parse_program();
    
}



void parse_program(){
// global_vars scope
    Token token1, token2;
    token1 = lexer.GetToken();
    if (token1.token_type == ID) {
        token2 = lexer.GetToken();
        if (token2.token_type == COMMA || token2.token_type == SEMICOLON) {
            lexer.UngetToken(token2);
            lexer.UngetToken(token1);
            parse_global_vars();
            parse_scope();
            return;
        }
        else if(token2.token_type == LBRACE){
            lexer.UngetToken(token2);
            lexer.UngetToken(token1);
            parse_scope();
        }
        else{
            cout << "Syntax Error" << endl;
            exit(0);
        }
        
    }
    else{
        cout << "Syntax Error" << endl;
        exit(0);

    }
    
}

void parse_global_vars(){
// var_list semicolon
    
    current_scope = new char[1];
    current_scope[0] = ':';
    PuborPri = 1;
 
    Token token1;
    token1 = lexer.GetToken();
    if (token1.token_type == ID) {
        lexer.UngetToken(token1);
        //cout << "Global variable:" << endl;
        parse_var_list();
    }
    else{
        cout << "Syntax Error" << endl;
        exit(0);
    }
    
    Token token2;
    token2 = lexer.GetToken();
    if (token2.token_type == SEMICOLON) {
        //cout << "Global variable" << endl;
    }
    else{
        cout << "Syntax Error" << endl;
        exit(0);
    }
    
    
}

void parse_var_list(){
// ID
// ID comma var_list
    
    Token token1;
    token1 = lexer.GetToken();
    if (token1.token_type == ID) {
        
        // add variable into list based on name, scope, public or private
        
        char *variableN = new char[token1.lexeme.length() + 1];
        strcpy(variableN, token1.lexeme.c_str());
        addList(variableN, current_scope, PuborPri);
        
        
        //cout << token1.lexeme << " add" << endl;
        Token token2 = lexer.GetToken();
        if (token2.token_type == COMMA) {
            parse_var_list();
        } else if(token2.token_type == SEMICOLON){
            lexer.UngetToken(token2);
        }
        else{
            cout << "Syntax Error" << endl;
            exit(0);
        }
    }
    else{
        cout << "Syntax Error" << endl;
        exit(0);
    }
    
    
}

void parse_public_vars(){
// PUBLIC COLON var_list SEMICOLON
    
    PuborPri = 1;
    Token token1 = lexer.GetToken();
    if (token1.token_type == PUBLIC) {
        token1 = lexer.GetToken();
        if (token1.token_type == COLON) {
            token1 = lexer.GetToken();
            if (token1.token_type == ID) {
                lexer.UngetToken(token1);
                //cout << "public variable:" << endl;
                parse_var_list();
                token1 = lexer.GetToken();
                if (token1.token_type == SEMICOLON) {
                    //cout << "public variables" << endl;
                }else{
                    cout << "Syntax Error" << endl;
                    exit(0);
                }
            }else{
                cout << "Syntax Error" << endl;
                exit(0);
            }
        }else{
            cout << "Syntax Error" << endl;
            exit(0);
        }
    }
    else if (token1.token_type == PRIVATE || token1.token_type == ID){
        lexer.UngetToken(token1);
        // no public variables
    }
    else{
        cout << "Syntax Error" << endl;
        exit(0);
    }
    
    
}

void parse_private_vars(){
// PRIVATE COLON var_list SEMICOLON
    
    PuborPri = 0;
    Token token1 = lexer.GetToken();
    if (token1.token_type == PRIVATE) {
        token1 = lexer.GetToken();
        if (token1.token_type == COLON) {
            token1 = lexer.GetToken();
            if (token1.token_type == ID) {
                lexer.UngetToken(token1);
                //cout << "private variable:" << endl;
                parse_var_list();
                token1 = lexer.GetToken();
                if (token1.token_type == SEMICOLON) {
                    //cout << "public variables" << endl;
                }else{
                    cout << "Syntax Error" << endl;
                    exit(0);
                }
            }else{
                cout << "Syntax Error" << endl;
                exit(0);
            }
        }else{
            cout << "Syntax Error" << endl;
            exit(0);
        }
    }
    else if (token1.token_type == ID){
        lexer.UngetToken(token1);
        // no private variables
    }
    else{
        cout << "Syntax Error" << endl;
        exit(0);
    }
    
    
}

void parse_scope(){
// ID LBRACE public_vars private_vars stmt_list RBRACE

    Token token1 = lexer.GetToken();
    if (token1.token_type == ID) {
        
        // current scope name changed
        current_scope = new char[token1.lexeme.length() + 1];
        strcpy(current_scope, token1.lexeme.c_str());
        
        token1 = lexer.GetToken();
        if (token1.token_type == LBRACE) {
            
            //cout << current_scope << ":" << endl;
            
            parse_public_vars();
            parse_private_vars();
            parse_stmt_list();
            
            token1 = lexer.GetToken();
            if (token1.token_type == RBRACE) {
                // delete variable in this scope
                deletelastList(current_scope);
                
                //cout << "end of scope" << endl;
            }else{
                cout << "Syntax Error" << endl;
                exit(0);
            }
        }else{
            cout << "Syntax Error" << endl;
            exit(0);
        }
    }else{
        cout << "Syntax Error" << endl;
        exit(0);
    }
    
}

void parse_stmt_list(){
// STMT
// STMT STMT_LIST
    
    Token token1 = lexer.GetToken();
    
    if (token1.token_type == ID) {
        lexer.UngetToken(token1);
        parse_stmt();
        parse_stmt_list();
    }
    
    else{
        if(token1.token_type == RBRACE){
            //cout << "find }";
            lexer.UngetToken(token1);
        }else{
            cout << "Syntax Error" << endl;
            exit(0);
        }
    }
    
}

void parse_stmt(){
// ID EQUAL ID SEMICOLON
// scope
    
    Token token1 = lexer.GetToken();
    if (token1.token_type == ID) {
        Token token2 = lexer.GetToken();
        if (token2.token_type == EQUAL) {
            Token token3 = lexer.GetToken();
            
            // scope check
            string name1 = searchlist(token1.lexeme);
            string name2 = searchlist(token3.lexeme);
            
            
            
            Token token4 = lexer.GetToken();
            if (token4.token_type == SEMICOLON) {
                if (name1.compare(":") == 0) {
                    cout << "::" << token1.lexeme << " = ";
                }else{
                    cout << name1 << "." << token1.lexeme << " = ";
                }
                
                if (name2.compare(":") == 0) {
                    cout << "::" << token3.lexeme << endl;
                }else{
                    cout << name2 << "." << token3.lexeme << endl;
                }
            }
            else{
                cout << "Syntax Error" << endl;
                exit(0);
            }
            
            
        } else if(token2.token_type == LBRACE){
            lexer.UngetToken(token2);
            lexer.UngetToken(token1);
            parse_scope();
        }
        else{
            cout << "Syntax Error" << endl;
            exit(0);
        }
    }else if(token1.token_type == RBRACE){
        lexer.UngetToken(token1);
        return;
        
    }else{
        cout << "Syntax Error" << endl;
        exit(0);
    }
    
    
}






void addList(char* n, char* sc, int p){
    
    
    if (symbolTable == NULL) {
        symbolTable = (sTable*) malloc (sizeof(sTable));
        symbolTable -> item = (sTableItem*) malloc (sizeof(sTableItem));
        
        symbolTable -> item -> name = n;
        //strcpy(symbolTable->item->name, n);
        symbolTable -> item -> scope = sc;
        //strcpy(symbolTable->item->scope, sc);
        
        symbolTable -> item -> permission = p;
        last = symbolTable;
       
    }
    else{
        last -> next = (sTable*) malloc (sizeof(sTable));
        last -> next -> prev = last;
        last -> next -> item = (sTableItem*) malloc (sizeof(sTableItem));
        
        //strcpy(last ->next ->item ->name, n);
        //strcpy(last ->next ->item ->scope, sc);
        last -> next -> item -> name = n;
        last -> next -> item -> scope = sc;
        
        last -> next -> item -> permission = p;
        last = last ->next;
    	}
    
}

void deletelastList(string CurScope){

    if (symbolTable == NULL) {
        return;
    }
    
    while (last != NULL && strcmp(current_scope,last -> item -> scope) == 0) {
        struct sTable* temp = last;
        last = last -> prev;

        delete temp;
    }
    if (last == NULL) {
        symbolTable = last;
    }
    
}


string searchlist(string n){
    struct sTable* current = last;
    
    if (symbolTable == NULL) {
        return "?";
    }
    
    while (current != NULL) {
        if (n.compare(current -> item -> name) == 0) {
            if (strcmp(current_scope, current -> item -> scope) != 0) {
                if (current -> item -> permission == 1) {
                    //cout << current -> item ->scope << endl;
                    return current -> item -> scope;
                }
            }
            else if(strcmp(current_scope,current -> item -> scope) == 0){
                return current -> item -> scope;
            }
        }
        current = current->prev;
    }
    
    return "?";
}





