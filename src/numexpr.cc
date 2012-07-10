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
	GLfloat val() { return (guitarScene.timenow-guitarScene.timelastmiss)*2.267e-05; }
	void print() { DBG(NUMEXPR,"trigmiss"); }
};

class tNXtrigpick:public tNumExpr
{
public:
	GLfloat val() { return (guitarScene.timenow-guitarScene.timelasthit)*2.267e-05; }
	void print() { DBG(NUMEXPR,"trigpick"); }
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

class tNXbeatprofile:public tNXsin
{
public:
	tNXbeatprofile(tNumExpr *op1):tNXsin(op1){;}
	GLfloat val() {
		GLfloat x=operand1->val();
		if (x<0) return 0;
		if (x>1) return 0;
		if (x<0.2) return x*5;
		return 1.25-x*1.25;
	}
	void print() { DBG(NUMEXPR,"beatprofile("); operand1->print(); DBG(NUMEXPR,")"); }
};

class tNXmodf:public tNXsin
{
public:
	tNXmodf(tNumExpr *op1):tNXsin(op1){;}
	GLfloat val() { double u; double tv=operand1->val(); return modf(tv,&u); }
	void print() { DBG(NUMEXPR,"modf("); operand1->print(); DBG(NUMEXPR,")"); }
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

string NXgetnexttok(const char *&expr, GLfloat &val)
{
	string res="";
	char c;
	res="";
	while (*expr==' ' || *expr=='\t') expr++;
	if (!*expr) return "";
	c=*expr;
	if ((*expr>='0' && *expr<='9') || *expr=='.')
	{
		while ((*expr>='0' && *expr<='9') || *expr=='.')
		{
			res+=*expr;
			expr++;
		}
		val=atof(res.c_str());
		return "1";
	}
	if ((*expr>='a' && *expr<='z') || (*expr>='A' && *expr<='Z') || *expr=='_')
	{
		while ((*expr>='a' && *expr<='z') || (*expr>='A' && *expr<='Z')
			|| *expr=='_' || (*expr>='0' && *expr<='9'))
		{
			res+=*expr;
			expr++;
		}
		if (res=="time") return "t";
		if (res=="frac") return "modf";
		return res;
	}
	res=*expr;
	if (*expr=='-'
		||*expr=='+'
		||*expr=='*'
		||*expr=='('
		||*expr==')'
		) { expr++; return res; }
	return "ERR";
}

string NXpeeknexttok(const char *expr)
{
	// not efficient, but since it's init only...
	GLfloat unused;
	return NXgetnexttok(expr,unused);
}


tNumExpr *parseNumExpressionInternal(const char *&expr, int pri=0);

tNumExpr *parseNumExpression(const char *expr)
{
	const char *p=expr;
	tNumExpr *res=parseNumExpressionInternal(p);
	DBG(NUMEXPR,"expr:%s =>",expr);
	res->print();
	DBG(NUMEXPR,"=%f\n",res->val());
	if (res) return res;
	return new tNumExpr();
}

tNumExpr *parseNumExpressionInternal(const char *&expr, int pri)
{
	string tok;
	tNumExpr *stored=NULL, *swp;
	const char *p=expr;
	GLfloat v;

	tok=NXgetnexttok(expr,v);
	if (tok=="1") stored=new tNXconst(v);
	else if (tok=="t") stored=new tNXreference(&scn.time);
	else if (tok=="trigmiss")    stored=new tNXtrigmiss();
	else if (tok=="trigpick")    stored=new tNXtrigpick();
	else if (tok=="(")           stored=parseNumExpressionInternal(expr,0);
	else if (tok=="sin")         stored=new tNXsin(parseNumExpressionInternal(expr,2));
	else if (tok=="beatprofile") stored=new tNXbeatprofile(parseNumExpressionInternal(expr,2));
	else if (tok=="sinstep")     stored=new tNXsinstep(parseNumExpressionInternal(expr,2));
	else if (tok=="modf")        stored=new tNXmodf(parseNumExpressionInternal(expr,2));
	else if (tok=="-")           stored=new tNXminus(new tNumExpr(),parseNumExpressionInternal(expr,1));
	else {
		WARN(NUMEXPR,"NUMEXP: expecting value, got %s before %s\n", tok, expr);
		return new tNumExpr();
	}
	while (1)
	{
		tok=NXpeeknexttok(expr);
		if (tok=="+" && pri>=1) return stored;
		if (tok=="-" && pri>=1) return stored;
		if (tok=="*" && pri>=2) return stored;
		if (tok=="") return stored;
		if (tok=="ERR")
		{
			WARN(NUMEXPR,"NUMEXP: erroneous token: %s before %s\n", tok, expr);
			return stored;
		}
		NXgetnexttok(expr,v);
		if (tok==")") { return stored; }
		if (tok=="+")
		{
			stored=new tNXplus(stored,parseNumExpressionInternal(expr,1));
			continue;
		}
		if (tok=="-")
		{
			stored=new tNXminus(stored,parseNumExpressionInternal(expr,1));
			continue;
		}
		if (tok=="*")
		{
			stored=new tNXmult(stored,parseNumExpressionInternal(expr,1));
			continue;
		}
		WARN(NUMEXPR,"NUMEXP: expecting operand, got: %s before %s\n", tok, expr);
	}
}




