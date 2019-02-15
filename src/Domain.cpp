#include <vector>
#include <iostream>
#include "Checker.h"
#include "Domain.h"

using namespace std;
using namespace domain;

std::string Space::getName() const { return name_; }

const Space &domain::VecExpr::getSpace() const { return space_; }


std::string VecIdent::getName() const
{
    std::cerr << "VecIdent::getName(): vardecl_  addr is " << std::hex << vardecl_->getVarDecl() << "\n";
    return "(" + vardecl_->toString() + " : " + space_->getName() + ")";
}

// TODO: currently UNUSED
void VecLitExpr::addFloatLit(float num)
{
    std::cerr << "Stub: add floats to litvalexpr\n";
}

const domain::VecExpr &VecVecAddExpr::getVecVecAddExprArgL()
{
    return arg_left_;
}

const domain::VecExpr &VecVecAddExpr::getVecVecAddExprArgR()
{
    return arg_right_;
}

/*const VecIdent &VecDef::getVecIdent()
{
    return identifier_;
}
*/

Space &Domain::addSpace(const string &name)
{
    Space *s = new Space(name);
    spaces.push_back(*s);
    //std::cerr << "Added space to domain at address " << std::hex << s << "\n";
    return *s;
}


domain::VecExpr *Domain::addVecLitExpr(Space &s, const coords::VecLitExpr *e)
{
    domain::VecExpr *be = new domain::VecExpr(s, e);
    //std::cerr << "Adding Vec Lit Expr to domain, address " << std::hex << be << " dump before is ... \n";
    //dump();
    expressions.push_back(be);
    //std::cerr << "... dump after is ...\n";
    //dump();
    return be;
}



// precondition: variable already interpreted
Space &getSpaceOfVarExpr(const coords::VecExpr *ast)
{
    // look up variable (ast) in identifiers table
    // Space& s = getSpaceOf(ast,identifiers);
    // return s;

    // get and return the space assigned to that object
    //std::cerr << "STUB: Domain: getSpaceOfVarExpr in Domain.cpp\n";
    return *new Space("_");
}

// TODO: Change arg type to be more precise
domain::VecExpr *Domain::addVecVarExpr(Space &s, const coords::VecVarExpr *ast)
{
    domain::VecExpr *var = new domain::VecExpr(s, ast);
    std::cerr << "Adding Vec Var Expr to domain, address " << std::hex << var << ": " << var->toString() << "\n";
    //dump();
    expressions.push_back(var);
    //std::cerr << "... dump after is ...\n";
    //dump();
    return var;
}

domain::VecExpr *Domain::addVecVecAddExpr(Space &s, coords::VecVecAddExpr *e, domain::VecExpr *mem, domain::VecExpr *arg)
{
    domain::VecExpr *be = new domain::VecVecAddExpr(s, e, mem, arg);
    expressions.push_back(be);
    std::cerr << "Domain::addVecVecAddExpr:: Add. Coords are "
        << std::hex << e << " Domain VecExpr at " << std::hex << be << "\n";
    std::cerr << "Domain::addVecVecAddExpr:: VecExpr string is " << be->toString() << "\n";
    return *be;
}

VecIdent *Domain::addVecIdent(Space &s, const coords::VecIdent *ast)
{
    VecIdent *id = new VecIdent(s, ast);
    identifiers.push_back(*id);
    return id;
}

// TODO: Should be binding to Vector, not Expr
// 
VecDef &Domain::addVecDef(const coords::VecDef *v, const VecIdent* i, const domain::VecExpr* e)
{
    std::cerr << "Domain::addVecDef ";
    std::cerr << "identifier is " << i->toString();
    std::cerr << " expression is " << e->toString() << "\n";
    VecDef *bd = new VecDef(v, i, e);
/*
Domain.cpp:116:38: error: no matching function for call to 'domain::VecDef::VecDef(const coords::VecDef*&, const domain::VecIdent*&, const domain::VecExpr*&)'
     VecDef *bd = new VecDef(v, i, e);
                                      ^
*/
    bindings.push_back(*bd);
    return *bd;
}

Vector* Domain::addVector(coords Vector* coords, domain::VecExpr *expr) {
    Vector* vec = new Vector(coords, expr);
    vectors.push_back(*vec);
}


// TODO: Use pointers everywhere here?
void Domain::dumpIdentifiers()
{
    for (auto i: identifiers ){
        std::cerr << i.toString() << "\n";
    }
}

void Domain::dumpExpressions()
{
    for (auto e: expressions ){
        std::cerr << e->toString() << "\n";
    }
}

void Domain::dumpVecDefs()
{
    for (auto b: bindings ){
        std::cerr << b.toString() << "\n";
    }
}

// Check domain for consistency
// Precondition: true
// Postcondition: return value true indicates presence of inconsistencies
// Implementation: Call Lean-specific checking code below (make virtual)
bool Domain::isConsistent()
{
    Checker *c = new Checker(*this);
    bool result = c->Check();
    delete c;
    return result;
}

vector<Space> &Domain::getAllSpaces()
{
    return spaces;
}
