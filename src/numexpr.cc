/*******************************************************************
(C) 2010 by Radu Stefan
radu124@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*******************************************************************/

#include "numexpr.h"
#include "scene.h"
#include "verbosity.h"
#include "message.h"
#include "scnGuitar.h"

GLfloat tNumExpr::val() { return 0; }
void tNumExpr::print() { DBG(NUMEXPR,"0"); }
tNumExpr::~tNumExpr() {;}

class tNXconst:public tNumExpr
{
public:
	GLfloat v;
	tNXconst(GLfloat vv):v(vv) {;}
	GLfloat val() { return v; }
	void print() { DBG(NUMEXPR,"%f",v); }
};

class tNXreference:public tNumExpr
{
public:
	GLfloat *v;
	tNXreference(GLfloat *vv):v(vv) {;}
	GLfloat val() { return *v; }
	void print() { DBG(NUMEXPR,"ptr"); }
};

class tNXtrigmiss:public tNumExpr
{
public:
	GLfloat val() { return (guitarScene.timenow-guitarScene.timelastmiss)*2.267e-04; }
	void print() { DBG(NUMEXPR,"trigmiss"); }
};

class tNXsin:public tNumExpr
{
public:
	tNumExpr * operand1;
	tNXsin(tNumExpr *op1):operand1(op1){;}
	GLfloat val() { return sin(operand1->val()); }
	void print() { DBG(NUMEXPR,"sin("); operand1->print(); DBG(NUMEXPR,")"); }
	~tNXsin()
	{
		if (operand1) delete operand1;
	}
};

class tNXsinstep:public tNXsin
{
public:
	tNXsinstep(tNumExpr *op1):tNXsin(op1){;}
	GLfloat val() { GLfloat tv=operand1->val(); return tv<1?sin(tv*3.14):0; }
	void print() { DBG(NUMEXPR,"sinstep("); operand1->print(); DBG(NUMEXPR,")"); }
};

class tNXplus:public tNumExpr
{
public:
	tNumExpr *operand1;
	tNumExpr *operand2;
	GLfloat val() { return operand1->val()+operand2->val(); }
	void print() { DBG(NUMEXPR,"("); operand1->print(); DBG(NUMEXPR,"+"); operand2->print(); DBG(NUMEXPR,")"); }
	tNXplus(tNumExpr *op1,tNumExpr *op2):operand1(op1),operand2(op2){;}
	~tNXplus()
	{
		if (operand1) delete operand1;
		if (operand2) delete operand2;
	}
};

class tNXminus:public tNXplus
{
public:
	tNXminus(tNumExpr *op1,tNumExpr *op2):tNXplus(op1,op2){;}
	GLfloat val() { return operand1->val()-operand2->val(); }
	void print() { DBG(NUMEXPR,"("); operand1->print(); DBG(NUMEXPR,"-"); operand2->print(); DBG(NUMEXPR,")"); }
};

class tNXmult:public tNXplus
{
public:
	tNXmult(tNumExpr *op1,tNumExpr *op2):tNXplus(op1,op2){;}
	GLfloat val() { return operand1->val()*operand2->val(); }
	void print() { DBG(NUMEXPR,"("); operand1->print(); DBG(NUMEXPR,"*"); operand2->print(); DBG(NUMEXPR,")"); }
};

enum NX_TOK_TYPE {
	NXTT_NULL,
	NXTT_VALUE,
	NXTT_SIN,
	NXTT_SINSTEP,
	NXTT_time,
	NXTT_trigmiss,
	NXTT_ERR};

char NXgetnexttok(const char *&expr, string &res)
{
	char c;
	res="";
	while (*expr==' ' || *expr=='\t') expr++;
	if (!*expr) return NXTT_NULL;
	c=*expr;
	if ((*expr>='0' && *expr<='9') || *expr=='.')
	{
		while ((*expr>='0' && *expr<='9') || *expr=='.')
		{
			res+=*expr;
			expr++;
		}
		return NXTT_VALUE;
	}
	if ((*expr>='a' && *expr<='z') || (*expr>='A' && *expr<='Z') || *expr=='_')
	{
		while ((*expr>='a' && *expr<='z') || (*expr>='A' && *expr<='Z')
			|| *expr=='_' || (*expr>='0' && *expr<='9'))
		{
			res+=*expr;
			expr++;
		}
		if (res=="sin") return NXTT_SIN;
		if (res=="trigmiss") return NXTT_trigmiss;
		if (res=="sinstep") return NXTT_SINSTEP;
		if (res=="t" || res=="time") return NXTT_time;

		return NXTT_ERR;
	}
	if (*expr=='-') { expr++; return '-'; }
	if (*expr=='+') { expr++; return '+'; }
	if (*expr=='*') { expr++; return '*'; }
	if (*expr=='(') { expr++; return '('; }
	if (*expr==')') { expr++; return ')'; }

	return NXTT_ERR;
}

char NXpeeknexttok(const char *expr)
{
	// not efficient, but since it's init only...
	string unused;
	return NXgetnexttok(expr,unused);
}


tNumExpr *parseNumExpressionInternal(const char *&expr, int pri=0);

tNumExpr *parseNumExpression(const char *expr)
{
	const char *p=expr;
	tNumExpr *res=parseNumExpressionInternal(p);
	DBG(NUMEXPR,"expr:");
	res->print();
	DBG(NUMEXPR,"\n");
	if (res) return res;
	return new tNumExpr();
}

tNumExpr *parseNumExpressionInternal(const char *&expr, int pri)
{
	char tok;
	tNumExpr *stored=NULL, *swp;
	const char *p=expr;
	string v;

	tok=NXgetnexttok(expr,v);
	if (tok==NXTT_VALUE) stored=new tNXconst(atof(v.c_str()));
	else if (tok==NXTT_time) stored=new tNXreference(&scn.time);
	else if (tok==NXTT_trigmiss) stored=new tNXtrigmiss();
	else if (tok=='(') stored=parseNumExpressionInternal(expr,0);
	else if (tok==NXTT_SIN) stored=new tNXsin(parseNumExpressionInternal(expr,2));
	else if (tok==NXTT_SINSTEP) stored=new tNXsinstep(parseNumExpressionInternal(expr,2));
	else if (tok=='-') stored=new tNXminus(new tNumExpr(),parseNumExpressionInternal(expr,1));
	else {
		WARN(NUMEXPR,"error reading expression, expecting value. rest: %s\n", expr);
		return new tNumExpr();
	}
	while (1)
	{
		tok=NXpeeknexttok(expr);
		if (tok=='+' && pri>=1) return stored;
		if (tok=='-' && pri>=1) return stored;
		if (tok=='*' && pri>=2) return stored;
		if (tok==NXTT_NULL) return stored;
		if (tok==NXTT_ERR)
		{
			WARN(NUMEXPR,"erroneous token: %s\n",expr);
			return stored;
		}
		NXgetnexttok(expr,v);
		if (tok==')') { return stored; }
		if (tok=='+')
		{
			stored=new tNXplus(stored,parseNumExpressionInternal(expr,1));
			continue;
		}
		if (tok=='-')
		{
			stored=new tNXminus(stored,parseNumExpressionInternal(expr,1));
			continue;
		}
		if (tok=='*')
		{
			stored=new tNXmult(stored,parseNumExpressionInternal(expr,1));
			continue;
		}
		WARN(NUMEXPR,"expecting operand, got: %s\n",expr);
	}
}




