//
//  parser.h
//  Parser
//
//  Created by Jack on 7/28/17.
//  Copyright © 2017 Ru Zhang. All rights reserved.
//

#ifndef parser_h
#define parser_h

#include <stdio.h>
#include <string.h>

using namespace std;

struct sTableItem {
    char* name;
    char* scope;
    int permission;
};

struct sTable{
    sTableItem* item;
    sTable* next;
    sTable* prev;
};

struct sTable* symbolTable = NULL;
struct sTable* last = symbolTable;


char* current_scope;
int PuborPri;


void addList(char*, char*, int);
void deletelastList(string);
string searchlist(string);

void parse_program();
void parse_global_vars();
void parse_var_list();
void parse_public_vars();
void parse_private_vars();
void parse_scope();
void parse_stmt_list();
void parse_stmt();


#endif /* parser_h */
