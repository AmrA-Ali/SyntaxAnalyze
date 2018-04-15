//
//  C-Parser.h
//  Compiler_Design
//
//  Created by Badr AlKhamissi on 4/14/18.
//  Copyright © 2018 Badr AlKhamissi. All rights reserved.
//

#ifndef C_Parser_h
#define C_Parser_h

//
//  main.cpp
//  Compiler_Design
//
//  Created by Badr AlKhamissi on 3/10/18.
//  Copyright © 2018 Badr AlKhamissi. All rights reserved.
//

#include <iostream>
using namespace std;

/*** Terminals ***/
enum token{
    ID, NUM,
    INT, VOID,
    LET, IF, WHILE, RETURN, ELSE,
    ADDOP, MULOP, // + - * /
    LTE, LT, BT, BTE, EQ, NEQ, ASSIGN, // <= < > >= == != =
    SLB, SRB, LB, RB, CLB, CRB, // [ ] ( ) { }
    SEMI_COLON, COMMA, // ; ,
    $
};

string token2Str[26] = {
    "id", "num",
    "int", "void",
    "let", "if", "while", "return",
    "+/-", "*/\\",
    "<=", "<", ">", ">=", "==", "!=", "=",
    "[", "]", "(", ")", "{", "}",
    ";", ",",
    "$"
};

class SyntaxAnalyzer{

private:
    struct node{
        node *left, *right; string op;
        node(string o, node*l, node*r) : op(o), left(l), right(r) {}
    };
    node* root;
    token* tokens;
    int ptr;
    int currToken;
    bool error;
    
    /**** Procedures ***/
    node* program();
    node* declaration_list();
    node* declaration();
    node* var_declaration();
    node* var_tail();
    node* type_specifier();
    node* fun_declaration();
    node* params();
    node* param_list();
    node* param();
    node* compound_stmt();
    node* local_declarations();
    node* statement_list();
    node* statement();
    node* expression_stmt();

    node* selection_stmt();
    node* iteration_stmt();
    node* return_stmt();
    node* expression();
    node* var();
    node* simple_expression();
    node* relop();
    node* additive_expression();
    node* addop();
    node* term();
    node* mulop();
    node* factor();
    node* call();
    node* args();
    node* arg_list();
    
    /**** Helper Functions ****/
    token scan();
    void match(token);
    void syntaxError(token);
    void traverse(node*);

public:
    SyntaxAnalyzer(token[], int);
    ~SyntaxAnalyzer();
    
    /**** Helper Functions ****/
    bool create();
    void traverse();
};

SyntaxAnalyzer::SyntaxAnalyzer(token tokens[], int size){
    ptr = 0;
    error = false;
    this->tokens = new token[size];
    for(int i = 0; i<size; i++)
        *(this->tokens+i) = tokens[i];
    currToken = scan();
}

SyntaxAnalyzer::~SyntaxAnalyzer(){
    delete[] tokens;
}

bool SyntaxAnalyzer::create(){
    root = program();
    scan();
    error = error || currToken != $;
    if(error)
        syntaxError($);
    return !error;
}

token SyntaxAnalyzer::scan(){
    return tokens[ptr++];
}

void SyntaxAnalyzer::match(token expectedToken){
    if(currToken == expectedToken)
        currToken = scan();
    else
        syntaxError(expectedToken);
}

void SyntaxAnalyzer::traverse(){
    cout<<"Pre-Order Traversal: "<<endl;
    traverse(root);
    cout<<endl;
}

void SyntaxAnalyzer::traverse(node* ptr){
    if(ptr){
        cout<<ptr->op<<" ";
        if(ptr->left)
            traverse(ptr->left);
        if(ptr->right)
            traverse(ptr->right);
    }
}

void SyntaxAnalyzer::syntaxError(token expected){
    error = true;
    if(!(expected == $))
        cerr<<"Syntax Error! Expected: "<<(token)expected<<" Found:"<<(token)currToken<<endl;
    else
        cerr<<"Syntax Error! Expecting Terminal"<<endl;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::program(){
    return declaration_list();
}

SyntaxAnalyzer::node* SyntaxAnalyzer::declaration_list(){
    node* left = declaration();
    // TODO: while declaration
    return left;
}

//node* var_declaration();
//node* var_tail();
//node* type_specifier();
//node* fun_declaration();
//node* params();
//node* param_list();


SyntaxAnalyzer::node* SyntaxAnalyzer::selection_stmt() {
	node* ifptr;
	match(IF);
	match(CLB);
	node* exprptr = expression();
	match(CRB);
	node* thenptr = statement();
	if (currToken == ELSE)
	{
		match(ELSE);
		node* elseptr = new node("ELSE", thenptr, elseptr);
		ifptr = new node("IF", exprptr, elseptr);
	}
	else {
		ifptr = new node("IF", exprptr, thenptr);
	}
	return ifptr;
}
//node* iteration_stmt();
//node* return_stmt();
//node* expression();
//node* var();
//node* simple_expression();
//node* relop();
//node* additive_expression();
//node* addop();
//node* term();
//node* mulop();
//node* factor();
//node* call();
//node* args();
//node* arg_list();

#endif /* C_Parser_h */