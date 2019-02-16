#ifndef COORDSTODOMAIN_H
#define COORDSTODOMAIN_H

#include <iostream>
#include "Coords.h"
#include "Domain.h"

#include <unordered_map>

namespace coords2domain {

class CoordsToDomain
{
  public:

// Ident

	void putVecIdent(const coords::VecIdent *key, domain::VecIdent *bi);
	const domain::VecIdent *getVecIdent(const coords::VecIdent *key);

// Expr

	void PutVecExpr(const coords::VecExpr *n, domain::VecExpr *e);
	domain::VecExpr *getVecExpr(const coords::VecExpr* n);


	void putVecLitExpr(const coords::VecLitExpr &n, domain::VecLitExpr &v);
	domain::VecLitExpr *getLitInterp(const coords::VecLitExpr &n) const;

	void PutVecVarExpr(const coords::VecVarExpr *n, domain::VecVarExpr *e);
	domain::VecVarExpr *getVecExpr(const coords::VecVarExpr* n);

	void PutVecVecAddExpr(const coords::VecVarExpr *n, domain::VecVecAddExpr *e);
	domain::VecVecAddExpr *getVecVecAddExpr(const coords::VecVarExpr* n);

// Value

	void putVector(coords::Vector *vardecl_wrapper, domain::Vector *b);
	const domain::Vector *getVector(const coords::VecDef* vardecl_wrapper);

// Def

	void putVecDef(coords::VecDef *vardecl_wrapper, domain::VecDef *b);
	const domain::VecDef *getVecDef(const coords::VecDef* vardecl_wrapper);

	void dump();

  private:
	/* 
	We implement an interpretation as a collection of typed maps. 
	The keys are "Code Coordinate" objects, which, in turn, are 
	currently just containers for pointers to AST nodes, basically
	just adding operator==() and hash functions.

	TODO: Compare with ast2coords. There it's clear that every
	AST node maps to a coords::VecExpr. But here we distinguish
	between different kinds of coords. Re-evaluate.
	*/
	std::unordered_map<coords::VecIdent, domain::VecIdent *, coords::VecIdentHasher> interpIdent;
	std::unordered_map<coords::VecExpr, domain::VecExpr *, coords::VecExprHasher> interpExpression;
	std::unordered_map<coords::VecDef, domain::VecDef *, coords::VecDefHasher> interpVecDef;
};

} // namespace

#endif