#include <vector>
#include <iostream>


// DONE: Separate clients from Domain
// #include "Checker.h"

#include "Domain.h"

#include <g3log/g3log.hpp>



using namespace std;
using namespace domain;


/******
* Space
******/

Space &Domain::mkSpace(const string &name)
{
    Space *s = new Space(name);
    spaces.push_back(s);
    return *s;
}


std::vector<Space*> &Domain::getSpaces() 
{
    return spaces; 
}

// TODO: MOVE THIS STUFF
std::string Space::getName() const { return name_; }

const Space &domain::VecExpr::getSpace() const { return space_; }


/****
Ident
****/


VecIdent *Domain::mkVecIdent(Space &s)
{
    VecIdent *id = new VecIdent(s);
    idents.push_back(id);
    return id;
}



/****
 Expr
****/


domain::VecVarExpr *Domain::mkVecVarExpr(Space &s)
{
    domain::VecVarExpr *var = new domain::VecVarExpr(s);
    exprs.push_back(var);
    return var;
}

domain::VecVecAddExpr *Domain::mkVecVecAddExpr(Space &s, domain::VecExpr *mem, domain::VecExpr *arg)
{
    domain::VecVecAddExpr *be = new domain::VecVecAddExpr(s, mem, arg);
    exprs.push_back(be);
    return be;
}

domain::VecExpr *VecVecAddExpr::getMemberVecExpr()
{
    return mem_;
}

domain::VecExpr *VecVecAddExpr::getArgVecExpr()
{
    return arg_;
}

// KEVIN: Added for VecParen module, has to stay in Domain.h
domain::VecParenExpr *Domain::mkVecParenExpr(Space &s, domain::VecExpr *expr)
{
		domain::VecParenExpr *var = new domain::VecParenExpr(s, expr);
		exprs.push_back(var);
		return var;
}


/******
* Value
*******/

Vector_Lit* Domain::mkVector_Lit(Space& space, float x, float y, float z) {
    Vector_Lit* vec = new domain::Vector_Lit(space, x, y, z); 
    vectors.push_back(vec); 
    return vec;
}


Vector_Expr* Domain::mkVector_Expr(Space& s, domain::VecExpr* exp) {
    Vector_Expr* vec = new domain::Vector_Expr(s, exp);
    vectors.push_back(vec);
    return vec;
}


/****
* Def
*****/


// TODO: Should be binding to Vector, not Expr
// 
Vector_Def *Domain::mkVector_Def(domain::VecIdent* i, domain::Vector* v)
{
    LOG(DEBUG) <<"Domain::mkVector_Def ";
    Vector_Def *bd = new Vector_Def(i, v);  
    defs.push_back(bd); 
    return bd;
}

/********
* Details
********/


// TODO: Use pointers everywhere here?
void Domain::dumpVecIdents()
{
    for (auto i: idents ){
        LOG(DEBUG) <<"Domain::dumpVecIdents: An identifier (STUB)\n";
    }
}

void Domain::dumpExpressions()
{
    for (auto e: exprs ){
        LOG(DEBUG) <<"Domain::dumpVecExpressions. An expression. (STUB).\n";
    }
}

void Domain::dumpVector_Defs()
{
    for (auto b: defs ){
        LOG(DEBUG) <<"Domain::dumpVector_Defs. A def. (STUB)\n";
    }
}

void Domain::dump() {
    LOG(DEBUG) <<"Domain expressions:\n";
    dumpExpressions(); // print contents on std::cerr
    LOG(DEBUG) <<"Domain VecIdents\n";
    dumpVecIdents(); // print contents on std::cerr
    LOG(DEBUG) <<"Domain Vector_Defs\n";
    dumpVector_Defs(); // print contents on std::cerr
}
