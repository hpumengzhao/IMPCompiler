#include<map>
#include<iostream>
#include<vector>
#include <cassert>
#include "lexer.h"


// status_map[k]: The value of all the var in status i; 
map<int,map<pair<string,bool>,int> > status_map;
map<string,int> env;
map<string,bool> vis;
int now_status=0;
//return value of string s
int Eval(string s){
	int ans=0;
	for(char c:s){
		ans=ans*10+c-'0';
	}
	return ans;
}
void debug(string s,int status){
	cout<<"<"<<s<<","<<"status["<<status<<"]>->"<<"Status["<<status+1<<"]"<<endl;
}
/*
	Parse Aexp and return the value.
	Aexp: a::=n|X|a0+a1|a0-a1|a0*a1
*/
int ParseAexp(string s){
	vector<pair<string,int>> tokens=lexer(s);
	int siz=(int)tokens.size();
	//a::=n
	if(tokens[0].second==2&&siz==1){
		int res=Eval(tokens[0].first);

		return Eval(tokens[0].first);
	}
	//a::=X
	if(tokens[0].second==1&&siz==1){
		return env[tokens[0].first];
	}

	string a0="";
	string a1="";
	//a::=a0+a1
	int add_id=-1;
	for(int i=0;i<siz;i++){
		if(tokens[i].first=="+"){
			add_id=i;
			break;
		}
	}

	if(add_id!=-1){
		for(int i=0;i<add_id;i++){
			a0+=tokens[i].first;
			a0+=" ";
		}
		for(int i=add_id+1;i<siz;i++){
			a1+=tokens[i].first;
			a1+=" ";
		}
		return ParseAexp(a0)+ParseAexp(a1);
	}

	//a::=a0-a1
	int sub_id=-1;
	for(int i=0;i<siz;i++){
		if(tokens[i].first=="-"){
			sub_id=i;break;
		}
	}
	if(sub_id!=-1){
		for(int i=0;i<sub_id;i++){
			a0+=tokens[i].first;
			a0+=" ";
		}
		for(int i=sub_id+1;i<siz;i++){
			a1+=tokens[i].first;
			a1+=" ";
		}
		return ParseAexp(a0)-ParseAexp(a1);		
	}
	//a::=a0*a1
	int mul_id=-1;
	for(int i=0;i<siz;i++){
		if(tokens[i].first=="*"){
			mul_id=i;break;
		}
	}
	if(mul_id!=-1){
		for(int i=0;i<mul_id;i++){
			a0+=tokens[i].first;
			a0+=" ";
		}
		for(int i=mul_id+1;i<siz;i++){
			a1+=tokens[i].first;
			a1+=" ";
		}
		return ParseAexp(a0)*ParseAexp(a1);		
	}	
	return -1;
}
/*
	Parse Bexp and return the value.
	b::=true|false|a0==a1|a_0<=a1|!b|b0&b1|b0|b1
*/
bool ParseBexp(string s){
	vector<pair<string,int> > tokens=lexer(s);
	int siz=(int)tokens.size();
	assert(siz>=2);
	string b0="";
	string b1="";
	//!b
	if(tokens[0].first=="!"){
		for(int i=1;i<siz;i++){
			b0+=tokens[i].first;
			b0+=" ";
		}
		return !ParseBexp(b0);
	}
	//b0 & b1;

	int and_id=-1;
	for(int i=0;i<siz;i++){
		if(tokens[i].first=="&"){
			and_id=i;
			break;
		}
	}
	if(and_id!=-1){
		for(int i=0;i<and_id;i++){
			b0+=tokens[i].first;
			b0+=" ";
		}
		for(int i=and_id+1;i<siz;i++){
			b1+=tokens[i].first;
			b1+=" ";
		}
		return ParseBexp(b0)&ParseBexp(b1);
	}
	// b0 | b1
	int or_id=-1;
	for(int i=0;i<siz;i++){
		if(tokens[i].first=="|"){
			or_id=i;
			break;
		}
	}
	if(or_id!=-1){
		for(int i=0;i<or_id;i++){
			b0+=tokens[i].first;
			b0+=" ";
		}
		for(int i=or_id+1;i<siz;i++){
			b1+=tokens[i].first;
			b1+=" ";
		}
		return ParseBexp(b0)|ParseBexp(b1);
	}

	string a0="";
	string a1="";
	//a0==a1
	int eq_id=-1;
	for(int i=0;i<siz;i++){
		if(tokens[i].first=="=="){
			eq_id=i;break;
		}
	}
	if(eq_id!=-1){
		for(int i=0;i<eq_id;i++){
			a0+=tokens[i].first;
			a0+=" ";
		}
		for(int i=eq_id+1;i<siz;i++){
			a1+=tokens[i].first;
			a1+=" ";
		}
		return ParseAexp(a0)==ParseAexp(a1);
	}
	//a0<=a1
	int lq_id=-1;
	for(int i=0;i<siz;i++){
		if(tokens[i].first=="<="){
			lq_id=i;break;
		}
	}
	if(lq_id!=-1){
		for(int i=0;i<lq_id;i++){
			a0+=tokens[i].first;
			a0+=" ";
		}
		for(int i=lq_id+1;i<siz;i++){
			a1+=tokens[i].first;
			a1+=" ";
		}
		return ParseAexp(a0)<=ParseAexp(a1);
	}

	return 0;
}
/*
	Parse command
	c::=skip|X:=a|c0;c1|if b then c0 else c1|while b do c
*/
void ParseCommandLine(string s){
	vector<pair<string,int> > tokens=lexer(s);
	int siz=(int)tokens.size();
	string c0="";
	string c1="";

	int cut_id=-1;
	for(int i=0;i<siz;i++){
		if(tokens[i].first==";"){
			cut_id=i;
			break;
		}
	}
	//c=c0,c1
	if(cut_id!=-1){
		for(int i=0;i<cut_id;i++){
			c0+=tokens[i].first;
			c0+=" ";
		}
		for(int i=cut_id+1;i<siz;i++){
			c1+=tokens[i].first;
			c1+=" ";
		}
		ParseCommandLine(c0);
		ParseCommandLine(c1);
	}else{
		if(tokens[0].first=="if"){//If should be parsed like Bracket matching
			string b;
			string c0;
			string c1;
			int cut_id=-1;
			for(int i=1;i<siz;i++){//b is between the first if and then...
				if(tokens[i].first!="then"){
					b+=tokens[i].first;
					b+=" ";
				}else{
					cut_id=i;
					break;
				}
			}
			int nxt_cut_id=-1;//the position of else in (if ... then ... else ...)
			int bracket=0;
			for(int i=cut_id+1;i<siz;i++){
				if(tokens[i].first=="else"&&bracket==0){
					nxt_cut_id=i;
					break;		
				}
				if(tokens[i].first=="if") ++bracket;
				if(tokens[i].first=="else") --bracket;
				c0+=tokens[i].first;
				c0+=" ";
			}

	
			for(int i=nxt_cut_id+1;i<siz;i++){
				if(tokens[i].first==";") break;
				c1+=tokens[i].first;
				c1+=" ";
			}
			if(ParseBexp(b)){
				ParseCommandLine(c0);
			}else{
				ParseCommandLine(c1);
			}
		}else if(tokens[0].first=="while"){
			//while b do c ;
			string b="";
			string c="";
			int cut_id=-1;
			for(int i=1;i<siz;i++){
				if(tokens[i].first!="do"){
					 b+=tokens[i].first;
					 b+=" ";
				}
				else{
					cut_id=i;
					break;
				}
			}
			for(int i=cut_id+1;i<siz;i++){
				 c+=tokens[i].first;
				 c+=" ";
			}
			// ParseCommandLine(c);
			while(ParseBexp(b)){
				ParseCommandLine(c);
			}
		}else if(tokens[0].first=="skip"){
			//we do nothing here		
		}else{//X:=aexp;
			string left=tokens[0].first;
			string aexp="";
			for(int i=2;i<(int)tokens.size();i++){
				if(tokens[i].first!=";"){
					aexp+=tokens[i].first;
					aexp+=" ";
				}else{
					break;
				}
			}
			int right=ParseAexp(aexp);

			vis[left]=1;
			env[left]=right;
		}
	}
}