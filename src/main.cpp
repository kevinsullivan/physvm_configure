#include <string>
#include <iostream>
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"

#include "CodeCoordinate.h"
#include "Interpretation.h"
#include "Oracle.h"
#include "Bridge.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;
using namespace std;
using namespace llvm;
using namespace bridge;

/**************************************************
Fundamental data structure constructed by this tool
***************************************************/

/*
Sharing data via global variables is a bad idea, but we'll 
do it to get a working system. These variables are initialized 
in main() and updated during the parse tree traversal, as AST
node handlers are triggered. 
*/
Oracle *oracle;
Bridge *bridge_domain;
Interpretation *interp;

/****************************
Standard Clang Tooling Set-up
*****************************/

/*
Set up a custom category for all command-line options; a help 
message with Clang's standard common command-line options; and 
a tool-specific help message.
*/
static llvm::cl::OptionCategory MyToolCategory("Peirce options");
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static cl::extrahelp MoreHelp("No additional options available for Peirce.");

/***************************
 * CXXConstructExpr HANDLERS
 ***************************/

/*
Function: Add interpretation for Vector identifier

create a bridge variable object 
add interpretation from vardecl to bridge variable object
maybe return a bool or something to indicate success or failure?
*/
bridge::Identifier *handleCXXConstructIdentifier(const VarDecl *vardecl, ASTContext *context, SourceManager &sm)
{
  bridge::Identifier *bIdent = new Identifier(vardecl);
  interp->putIdentifier(vardecl, bIdent);
  cout << "Created bridge identifier\n";
  return bIdent;
}

// Function: Add interpretation for binding of Vector identifier to Vector Expression
void handleCXXConstructIdentifierBinding(bridge::Identifier *bv, bridge::Expr *be)
{
  cout << "Adding interpretation for binding of identifier to expression (STUB)\n";
}

// Class: Match Callback class for handling Vector Literal Expressions
class HandlerForCXXConstructLitExpr : public MatchFinder::MatchCallback
{
public:
  virtual void run(const MatchFinder::MatchResult &Result)
  {
    const auto *litexpr = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("VectorLitExpr");
    ASTContext *context = Result.Context;
    SourceManager &sm = context->getSourceManager();
    cout << "Handing Vector Lit Expression (STUB)\n";
  }
};

/*********************/

class HandlerForCXXMemberCallExprRight_DeclRefExpr {
};

class HandlerForCXXMemberCallExprRight_MemberCallExpr {
};

class CXXMemberCallExprRightMatcher
{
public:
  CXXMemberCallExprRightMatcher()
  {

    // DeclRefExpr()

    // CXXMemberCallExpr
    
  };

  void match(const clang::Expr *call_rhs, ASTContext *context)
  {
    CXXMemberCallExprRightMatcher_.match(*call_rhs, *context);
  }

private:
  MatchFinder CXXMemberCallExprRightMatcher_;
  HandlerForCXXMemberCallExprRight_DeclRefExpr dcrHandler_;
  HandlerForCXXMemberCallExprRight_MemberCallExpr mceHandler_;
};


bridge::Expr *handle_left_of_add_call(const clang::Expr *left)
{
  cout << "Handling LEFT of add call\n";

  // create left matcher
  // call matcher->match(left, context)
}

bridge::Expr *handle_right_of_add_call(const clang::Expr *right)
{
  cout << "Handling RIGHT of add call\n";

}

/*
CXXMemberCallExpr := CXXMemberCallExprLeft  + CXXMemberCallExprRight 
CXXMemberCallExprLeft := MemberExpr
CXXmemberCallExprRight := (DeclRefExpr | CXXMemberCallExpr)
*/
// Class: Callback for handling Vector Add Expression (CXXMemberCallExpr)
class HandlerForCXXConstructAddExpr : public MatchFinder::MatchCallback
{
public:
  virtual void run(const MatchFinder::MatchResult &Result)
  {
    const auto *addexpr = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("MemberCallExpr");
    if (addexpr == NULL) {
      cout << "Error in HandlerForCXXConstructAddExpr::run. No add expression pointer\n";
      return;
    }
    ASTContext *context = Result.Context;
    SourceManager &sm = context->getSourceManager();
    cout << "Handling Vector Add (Member Call) Expr. Recurse on implicit parameter and argument. STUB.\n";

    const clang::Expr *left = addexpr->getImplicitObjectArgument();
    const clang::Expr *right = addexpr->getArg(0);
    if (!left)
    {
      cout << "Null left clang pointer\n";
      return;
    }
    else if (!right)
    {
      cout << "Null right clang pointer\n";
      return;
    }
    bridge::Expr *left_br = handle_left_of_add_call(left);
    bridge::Expr *right_br = handle_right_of_add_call(right);
    if (!left)
    {
      cout << "Null left bridge pointer\n";
      return;
    }
    else if (!right)
    {
      cout << "Null right bridge pointer\n";
      return;
    }

    Space &s = oracle->getSpaceForAddExpression(left_br, right_br);
    bridge_domain->addVecAddExpr(s, addexpr, *left_br, *right_br);
  }
private:
  CXXMemberCallExprRightMatcher call_right_matcher;
  CXXMemberCallExprLeftMatcher call_left_matcher;
};

class CXXConstructExprMatcher
{
public:
  CXXConstructExprMatcher()
  {

    // literal
    CXXConstructExprMatcher_.addMatcher(cxxConstructExpr(argumentCountIs(3)),
                                        &litHandler_);

    // member call
    CXXConstructExprMatcher_.addMatcher(cxxConstructExpr(hasDescendant(cxxMemberCallExpr(hasDescendant(memberExpr(hasDeclaration(namedDecl(hasName("vec_add")))))).bind("MemberCallExpr"))), &addHandler_);
  };

  /*
(cxxConstructExpr (hasDescendant (cxxMemberCallExpr)), (hasDescendant (memberExpr)), (hasDeclaration(namedDecl(hasName("add")))))
*/
  void match(const clang::CXXConstructExpr *consdecl, ASTContext *context)
  {
    CXXConstructExprMatcher_.match(*consdecl, *context);
  }

private:
  MatchFinder CXXConstructExprMatcher_;
  HandlerForCXXConstructLitExpr litHandler_;
  HandlerForCXXConstructAddExpr addHandler_;
};

/*
Precondition: consdecl of type CXXConstructExpr* is a pointer to an 
expression, the value of which is being assigned to a variable in a 
declaration statement. 

Explanation: By the time control reaches this code, we are assured 
the argument is an AST node for a Vector-valued expression that is
going to be used to initialize the value of a variable. The purpose 
of this code is to make sure that this C++ expression is "lifted" to
a corresponding expression in the domain/bridge, and that the
interpretation links this code/AST-node to that domain/bridge object.

Postcondition: the return value of this function is pointer to a new 
object of type bridge::Expr; that object is in the bridge; it might
itself represent a complex expression tree; it links back to consdecl;
and the interpretation is updated to like consdecl to the new bridge
object. This function works recursively to make sure that all of the
work of handling the top-level CXXConstructExpr is finished by the 
time this function returns.

Explanation: the way in which this consdecl is turned into a bridge 
object depends on the specific form of the expression being handled.
The cases to be handled include literal and add expressions.
- Vec v1(0.0,0.0,0.0) is a literal expression
- (v1.add(v2)).(v3.add(v4)) is an add expression (recursive)
*/
bridge::Expr *handleCXXConstructExpr(const clang::CXXConstructExpr *consdecl, ASTContext *context, SourceManager &sm)
{
  cout << "Pattern matching Vector CXXConstructExpr and calling appropriate handler.\n";
  CXXConstructExprMatcher *matcher = new CXXConstructExprMatcher();
  matcher->match(consdecl, context);
  // postcondition: consdecl now has an interpretation
  // How do we get BI to return to user? Look it up
  // bridge::Expr* bi = interp->getExpr(consdecl);
  cout << "Returning domain expression object (STUBBED OUT)\n";
  return NULL; /* STUB */

  /*
  match consdecl with
  | literal <CXX three-argument pattern>, handle_literal(consdecl)
  | variable, handle_var
  | add expr, handle_add_expr
 
  StatementMatcher matchLit = ...;
  StatementMatcher matchVar = ...;
  Statement Matcher matchAdd = ...;

  handleXXXLit() {}
  handleCXXVar() {}
  HandleCXXAdd() { get left; get right part; handle_left; handle_right; glue them together }
  */
}

/*************************
 * Handle Vector DeclStmts
 *************************/

class VectorDeclStmtHandler : public MatchFinder::MatchCallback
{
public:
  virtual void run(const MatchFinder::MatchResult &Result)
  {
    /*
    get DeclStatement from matcher
    get context 
    get source manager 
    get handle on variable declaration, vardecl
    be sure it has an initializer, then get the CXXConstructExpr initializer
    get handle on expression used to initialize the variable
    establish interpretation from consdecl to corresponding expression in the domain bridge
    establish interpretation from variable in code to corresponding var object in domain bridge
    finally establish interpretation linking overall declstmt in code to corresponding binding in domain
    */
    const auto *declstmt = Result.Nodes.getNodeAs<clang::DeclStmt>("VectorStatement");
    ASTContext *context = Result.Context;
    SourceManager &sm = context->getSourceManager();
    if (declstmt->isSingleDecl())
    {
      const VarDecl *vardecl = dyn_cast<VarDecl>(declstmt->getSingleDecl());
      const clang::CXXConstructExpr *consdecl;
      if (vardecl->hasInit())
      {
        consdecl = static_cast<const clang::CXXConstructExpr *>(vardecl->getInit());
      }
      // TODO: Fill in logic for error condition in preceding conditional
      cout << "Handling vector declaration statement\n";
      bridge::Expr *be = handleCXXConstructExpr(consdecl, context, sm);
      bridge::Identifier *bi = handleCXXConstructIdentifier(vardecl, context, sm);
      handleCXXConstructIdentifierBinding(bi, be);
      cout << "Done handling vector declaration statement\n\n";
    }
    else
    {
      cout << "STUB: Something's wrong\n";
    }
  }
};

/********************************************
 * Top-level analyzer: Match Vector DeclStmts
 ********************************************/

class MyASTConsumer : public ASTConsumer
{
public:
  MyASTConsumer()
  {
    StatementMatcher match_Vector_general_decl =
        declStmt(hasDescendant(varDecl(hasDescendant(cxxConstructExpr(hasType(asString("class Vec"))))))).bind("VectorStatement");
    VectorDeclStmtMatcher.addMatcher(match_Vector_general_decl, &HandlerForVectorDeclStmt);
  }
  void HandleTranslationUnit(ASTContext &Context) override
  {
    VectorDeclStmtMatcher.matchAST(Context);
  }

private:
  MatchFinder VectorDeclStmtMatcher;
  VectorDeclStmtHandler HandlerForVectorDeclStmt;
};

/*******************************
* Main Clang Tooling entry point
********************************/

class MyFrontendAction : public ASTFrontendAction
{
public:
  MyFrontendAction() {}
  void EndSourceFileAction() override
  {
    bool consistent = bridge_domain->isConsistent();
    cerr << (consistent ? "STUB: Good\n" : "STUB: Bad\n");
  }
  std::unique_ptr<ASTConsumer>
  CreateASTConsumer(CompilerInstance &CI, StringRef file) override
  {
    return llvm::make_unique<MyASTConsumer>();
  }
};

/*****
* Main
******/

int main(int argc, const char **argv)
{
  CommonOptionsParser op(argc, argv, MyToolCategory);
  ClangTool Tool(op.getCompilations(), op.getSourcePathList());
  bridge_domain = new Bridge();
  bridge_domain->addSpace("Space1");
  bridge_domain->addSpace("Space2");
  interp = new Interpretation();
  oracle = new Oracle(*bridge_domain);
  return Tool.run(newFrontendActionFactory<MyFrontendAction>().get());
}