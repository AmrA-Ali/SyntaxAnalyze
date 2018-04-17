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
#include<vector>
using namespace std;

/*** Terminals ***/
enum token {
    ID, NUM,
    INT, VOID,
    LET, IF, WHILE, RETURN, ELSE,
    ADDOP, MULOP, // + - * /
    LTE, LT, BT, BTE, EQ, NEQ, ASSIGN, // <= < > >= == != =
    SLB, SRB, LB, RB, CLB, CRB, // [ ] ( ) { }
    SEMI_COLON, COMMA, // ; ,
    EMPTY, $
};

enum nonTerm {
    declaration_list, declaration, var_declaration, var_tail,
    type_specifier, fun_declaration,
    params, param_list, param,
    compound_stmt, local_declarations, statement_list, statement,
    expression_stmt, selection_stmt, iteration_stmt, return_stmt, expression,
    var, simple_expression, relop, additive_expression, addop, term, mulop, factor,
    call, args, arg_list
};

string token2Str[28] = {
    "id", "num",
    "int", "void",
    "let", "if", "while", "return", "else",
    "+/-", "*/\\",
    "<=", "<", ">", ">=", "==", "!=", "=",
    "[", "]", "(", ")", "{", "}",
    ";", ",",
    "empty", "$"
};

struct symbol{
    token tok;
    string value;
    symbol(){};
    symbol(token t, string v) : tok(t), value(v) {}
    symbol& operator=(const symbol &rhs){
        this->tok = rhs.tok;
        this->value = rhs.value;
        return *this;
    }
};

vector<vector<token>> First(30);

void BuildFirstSet() {
    First[declaration_list] = { INT,VOID };
    First[declaration] = { INT,VOID };
    First[var_declaration] = { INT,VOID };
    First[var_tail] = { SEMI_COLON, SLB };
    First[type_specifier] = { INT, VOID };
    First[fun_declaration] = { INT, VOID };
    First[params] = { INT, VOID };
    First[param_list] = { INT, VOID };
    First[param] = { INT, VOID };
    First[compound_stmt] = { CLB };
    First[local_declarations] = { EMPTY };
    First[statement_list] = { EMPTY };
    First[statement] = { LET,LB,ID,NUM,SEMI_COLON,IF,WHILE,CLB,RETURN };
    First[expression_stmt] = { LET,LB,ID,NUM,SEMI_COLON };
    First[selection_stmt] = { IF };
    First[iteration_stmt] = { WHILE };
    First[return_stmt] = { RETURN };
    First[expression] = { LET,LB,ID,NUM };
    First[var] = { ID };
    First[simple_expression] = { LB,ID, NUM };
    First[relop] = { LTE, LT, BT, BTE, EQ, NEQ, ASSIGN };
    First[additive_expression] = { LB, ID, NUM };
    First[addop] = { ADDOP };
    First[term] = { LB, ID, NUM };
    First[mulop] = { MULOP };
    First[factor] = { LB, ID, NUM };
    First[call] = { ID };
    First[args] = { LET, LB, ID, NUM, EMPTY };
    First[arg_list] = { LET,LB,ID,NUM };
}


class SyntaxAnalyzer {

private:
    struct node {
        node *left, *right; string op;
        node(string o, node*l, node*r) : op(o), left(l), right(r) {}
    };
    node* root;
    vector<vector<symbol>> tokens;
    int ptr, linenum;
    bool error;
    symbol currToken;

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
    symbol scan();
    void match(token);
    void syntaxError(string);
    void traverse(node*);
    bool check(nonTerm);


public:
    SyntaxAnalyzer(const vector<vector<symbol>>&);
    ~SyntaxAnalyzer();

    /**** Helper Functions ****/
    bool create();
    void traverse();
};


SyntaxAnalyzer::SyntaxAnalyzer(const vector<vector<symbol>>& program) {
    ptr = linenum = 0;
    error = false;
    currToken = symbol($, "$");
    BuildFirstSet();
    for(int i = 0; i<program.size(); i++){
        vector<symbol>temp;
        for(int j =0; j<program[i].size(); j++)
            temp.push_back(symbol(program[i][j].tok, program[i][j].value));
        tokens.push_back(temp);
    }
    currToken = scan();
}

SyntaxAnalyzer::~SyntaxAnalyzer() {

}

bool SyntaxAnalyzer::create() {
    root = program();
    error = error || currToken.tok != $;
    if (error)
        syntaxError("$");
    return !error;
}

symbol SyntaxAnalyzer::scan() {
    if(tokens[linenum].size() == ptr){
        ptr = 0;
        linenum++;
    }
    return tokens[linenum][ptr++];
}

void SyntaxAnalyzer::match(token expectedToken) {
    if (currToken.tok == expectedToken)
        currToken = scan();
    else
        syntaxError(token2Str[expectedToken]);
}

void SyntaxAnalyzer::traverse() {
    cout << "Pre-Order Traversal: " << endl;
    traverse(root);
    cout << endl;
}

void SyntaxAnalyzer::traverse(node* ptr) {
    if (ptr) {
        cout << "("<<ptr->op;
        if (ptr->left)
            traverse(ptr->left);
        if (ptr->right)
            traverse(ptr->right);
        cout<<")";
    }
}

bool SyntaxAnalyzer::check(nonTerm nT) {
    for (int i = 0; i < First[nT].size(); i++)
        if (First[nT][i] == currToken.tok)
            return true;
    return false;
}

void SyntaxAnalyzer::syntaxError(string error) {
    error = true;
    cerr << "Syntax Error @ line " << linenum << "! Expecting: " << error << endl;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::program() {
    return declaration_list();
}

SyntaxAnalyzer::node* SyntaxAnalyzer::declaration_list() {
    node* left = declaration();
    while (check(nonTerm::declaration)) {
        string op = currToken.value;
        node* right = declaration();
        left = new node(op, left, right);
    }
    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::declaration() {
    node* left = nullptr;
    if (check(nonTerm::type_specifier)) {
//        if(currToken == check(nonTerm::var_declaration))
            left = var_declaration();
//        else
//            left = fun_declaration();
    }
    else {
        syntaxError("ID | VOID");
    }
    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::var_declaration() {
    node* left = type_specifier();
    string op = currToken.value;
    match(ID);
    node* right = var_tail();
    left = new node(op, left, right);
    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::var_tail() {
    symbol prevToken = currToken;
    switch (currToken.tok) {
        case SEMI_COLON:
            match(SEMI_COLON);
            break;
        case SLB:
            match(SLB);
            match(NUM);
            match(SRB);
            break;
        default:
            syntaxError("; | [");
            break;
    }

    return new node(prevToken.value, nullptr, nullptr);
}

SyntaxAnalyzer::node* SyntaxAnalyzer::type_specifier() {
    node* left = nullptr;
    switch (currToken.tok) {
        case INT:
            left = new node(currToken.value, nullptr, nullptr);
            match(INT);
            break;
        case VOID:
            left = new node(currToken.value, nullptr, nullptr);
            match(VOID);
            break;
        default:
            syntaxError("int | void");
            break;
    }
    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::fun_declaration() {
    node* left = type_specifier();
    match(ID);
    match(LB);
    node* right = params();
    match(RB);
    node* el = compound_stmt();
    node* rightel = new node(token2Str[ID], el, right);
    left = new node(token2Str[ID], left, rightel);
    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::params() {
    node* left;
    if (check(nonTerm::param_list)) left = param_list();
    else { match(VOID); left = new node(token2Str[VOID], NULL, NULL); }
    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::param_list(){
    node* left = param();
    while (currToken.tok == COMMA){
        match(COMMA);
        node* right = param();
        left = new node(token2Str[COMMA], left, right);
    }

    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::param() {
    node* left = type_specifier();
    match(ID);
    if (currToken.tok == SLB){
        match(SLB);
        match(SRB);
    }
    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::compound_stmt(){
    match(CLB);
    node* left = local_declarations();
    node* right = statement_list();
    match(CRB);
    left = new node(token2Str[CLB], left, right);
    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::local_declarations() {
    match(EMPTY);
    node* left = nullptr;
    while (check(nonTerm::var)){
        token tok = currToken.tok;
        left = var();
        node* right = declaration();
        left = new node(token2Str[tok], left, right);
    }
    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::statement_list() {
    match(EMPTY);
    node* left = nullptr;
    while (check(nonTerm::statement)){
        left = statement();
    }
    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::statement() {
    match(EMPTY);
    node* left = nullptr;
    if (check(nonTerm::expression_stmt)) left = expression_stmt();
    else if (check(nonTerm::compound_stmt)) left = compound_stmt();
    else if (check(nonTerm::selection_stmt)) left = selection_stmt();
    else if (check(nonTerm::iteration_stmt)) left = iteration_stmt();
    else if (check(nonTerm::return_stmt)) left = return_stmt();

    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::expression_stmt() {
    node* left = nullptr;
    if (check(nonTerm::expression)) left = expression();
    match(SEMI_COLON);
    return left;
}



/**** Procedures 16 ----> 30***/
SyntaxAnalyzer::node* SyntaxAnalyzer::selection_stmt() {
    node* ifptr;
    match(IF);
    match(CLB);
    node* exprptr = expression();
    match(CRB);
    node* thenptr = statement();
    if (currToken.tok == ELSE) {
        match(ELSE);
        node* elseptr = statement();
        elseptr = new node("ELSE", thenptr, elseptr);
        ifptr = new node("IF", exprptr, elseptr);
    }
    else
        ifptr = new node("IF", exprptr, thenptr);
    return ifptr;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::iteration_stmt() {
    match(WHILE);
    match(CLB);
    node* exprptr = expression();
    match(CRB);
    node* stmtptr = statement();
    node* whileptr = new node("WHILE", exprptr, stmtptr);
    return whileptr;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::return_stmt() {
    match(RETURN);
    node* ptr = nullptr;
    if (check(nonTerm::expression))
        ptr = expression();
    match(SEMI_COLON);
    return ptr;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::expression() {
    node* left = nullptr;
    if (currToken.tok == LET) {
        match(LET);
        left = var();
        match(ASSIGN);
        node* el = expression();
        left = new node(token2Str[ASSIGN], left, el);
    }else if (check(nonTerm::simple_expression)) {
        left = simple_expression();
    }else
        syntaxError("$");
    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::var() {
    match(ID);
    node* ptr = nullptr;
    switch (currToken.tok) {
        case LB: { match(LB); ptr = expression(); match(RB); break; }
        default: syntaxError("(");
    }
    return ptr;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::simple_expression() {
    node* left = additive_expression();
    node* right;
    if (check(nonTerm::relop)){
        node* el = relop();
        right = additive_expression();
        right = new node(currToken.value, el, right);
        left = new node(currToken.value, left, right);
    }
    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::relop() {
    symbol prevToken = currToken;
    switch (currToken.tok){
        case LTE: match(LTE); break;
        case LT:  match(LT); break;
        case BT:  match(BT); break;
        case BTE: match(BTE); break;
        case EQ:  match(EQ); break;
        case NEQ: match(NEQ); break;
        case ASSIGN: match(ASSIGN); break;
        default:
            syntaxError("<= | < | > | >= | == | != | =");
    }
    return new node(prevToken.value, nullptr, nullptr);
}

SyntaxAnalyzer::node* SyntaxAnalyzer::additive_expression() {
    node* left = term();
    node* right = nullptr;
    while (check(nonTerm::addop)){
        token tok = currToken.tok;
        node* el = addop();
        right = term();
        node* rightel = new node(token2Str[tok], el, right);
        left = new node(token2Str[tok], left,  rightel);
    }
    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::mulop() {
    symbol prevToken = currToken;
    switch (currToken.tok){
        case MULOP: match(MULOP); break;
        default:
            syntaxError("* | \\");
    }
    return new node(prevToken.value, nullptr, nullptr);
}

SyntaxAnalyzer::node* SyntaxAnalyzer::addop() {
    symbol prevToken = currToken;
    switch (currToken.tok){
        case ADDOP: match(ADDOP); break;
        default:
            syntaxError("+ | -");
    }
    return new node(prevToken.value, nullptr, nullptr);
}

SyntaxAnalyzer::node* SyntaxAnalyzer::term() {
    node* left = factor();
    node* right;
    while (check(nonTerm::mulop)){
        token tok = currToken.tok;
        right = factor();
        left = new node(token2Str[tok], left, right);
    }
    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::factor() {
    node* left = nullptr;
    switch (currToken.tok){
        case LB: {
            match(LB);
            left = expression();
            match(RB);
        }; break;
        case ID: if (true) left = call(); else left = var(); break;
        case NUM: match(NUM); break;
        default:
            syntaxError("[ | ID | NUM");
    }
    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::call() {
    match(ID);
    match(LB);
    node* left = args();
    match(RB);
    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::args() {
    node* left = nullptr;
    if (check(nonTerm::arg_list))
        left = arg_list();
    else
        match(EMPTY);
    return left;
}

SyntaxAnalyzer::node* SyntaxAnalyzer::arg_list() {
    node* left = expression();
    while (COMMA) {
        match(COMMA);
        node* right = expression();
        left = new node(token2Str[COMMA], left, right);
    }
    return left;
}

#endif /* C_Parser_h */
