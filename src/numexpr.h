#ifndef __HDR_numexpr_h
#define __HDR_numexpr_h

#include "includes.h"

class tNumExpr
{
public:
	virtual GLfloat val();
	virtual void print();
	virtual ~tNumExpr();
};

tNumExpr* parseNumExpression(const char *expr);

#endif
