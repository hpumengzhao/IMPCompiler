#include<map>
#include<iostream>
#include<vector>
#include <cassert>
#include "lexer.h"


//env[key] is the value of variable key 
map<string,int> env;
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
	cout<<"<"<<s<<","<<"status["<<status<<"]>--->"<<"status["<<now_status<<"]"<<endl;
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
		cout<<"<"<<s<<","<<"status"<<now_status<<">--->"<<res<<endl;
		return res;
	}
	//a::=X
	if(tokens[0].second==1&&siz==1){
		int res=env[tokens[0].first];
		cout<<"<"<<s<<","<<"status"<<now_status<<">--->"<<res<<endl;
		return res;
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
		int res=ParseAexp(a0)+ParseAexp(a1);
		cout<<"<"<<s<<","<<"status"<<now_status<<">--->"<<res<<endl;
		return res;
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
		int res=ParseAexp(a0)-ParseAexp(a1);
		cout<<"<"<<s<<","<<"status"<<now_status<<">--->"<<res<<endl;
		return res;	
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
		int res=ParseAexp(a0)*ParseAexp(a1);
		cout<<"<"<<s<<","<<"status"<<now_status<<">--->"<<res<<endl;
		return res;		
	}	
	cout<<"Syntax Error"<<endl;
	exit(-1);
}
/*
	Parse Bexp and return the value.
	b::=true|false|a0==a1|a_0<=a1|!b|b0 and b1|b0 or b1
*/
bool ParseBexp(string s){
	vector<pair<string,int> > tokens=lexer(s);
	int siz=(int)tokens.size();
	if(siz<2){
			cout<<"Syntax Error"<<endl;
		exit(-1);
	}
	string b0="";
	string b1="";
	//!b
	if(tokens[0].first=="!"){
		for(int i=1;i<siz;i++){
			b0+=tokens[i].first;
			b0+=" ";
		}
		bool res=!ParseBexp(b0);
		cout<<"<"<<s<<","<<"status"<<now_status<<">--->"<<(res?"true":"false")<<endl;
		return res;
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
		bool res=ParseBexp(b0)&ParseBexp(b1);
		cout<<"<"<<s<<","<<"status"<<now_status<<">--->"<<(res?"true":"false")<<endl;
		return res;
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
		bool res=ParseBexp(b0)|ParseBexp(b1);
		cout<<"<"<<s<<","<<"status"<<now_status<<">--->"<<(res?"true":"false")<<endl;
		return res;
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
		bool res=ParseAexp(a0)==ParseAexp(a1);
		cout<<"<"<<s<<","<<"status"<<now_status<<">--->"<<(res?"true":"false")<<endl;
		return res;
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
		bool res=ParseAexp(a0)<=ParseAexp(a1);
		cout<<"<"<<s<<","<<"status"<<now_status<<">--->"<<(res?"true":"false")<<endl;
		return res;
	}

	cout<<"Syntax Error"<<endl;
	exit(-1);
}

void ParseCommandLine(string s,int status){
	// cout<<s<<endl;
	if(s.size()==0){
		return ;		
	}
	vector<pair<string,int> > tokens=lexer(s);
	int siz=(int)tokens.size();
	string c0="";
	string c1="";
	if(siz<3&&tokens[0].first!="skip"){
		cout<<"Syntax Error!"<<endl;
		exit(-1);		
	}
	int cut_id=-1;
	if(tokens[0].first!="if"&&tokens[0].first!="while"){
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
			ParseCommandLine(c0,now_status);
			// cout<<"dasd"<<" " <<c1<<endl;
			ParseCommandLine(c1,now_status);
			// debug(s,status);
		}else{
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
			int left_val=ParseAexp(left);
			int right=ParseAexp(aexp);
			++now_status;
			debug(s,status);
			env[left]=right;
			return ;
		}
	}else if(tokens[0].first=="if"){// if b then c0 else c1 fi;nxt
		string b;
		string c0;
		string c1;

		string nxt="";
		int then_id=-1;
		for(int i=1;i<siz;i++){
			if(tokens[i].first=="then"){
				then_id=i;
				break;
			}
			b+=tokens[i].first;
			b+=" ";
		}

		int balance=0;
		int else_id=-1;
		for(int i=then_id+1;i<siz;i++){
			if(tokens[i].first=="else"&&balance==0){
				else_id=i;
				break;
			}
			if(tokens[i].first=="if"){
				++balance;
			}
			if(tokens[i].first=="else"){
				--balance;
			}
			c0+=tokens[i].first;
			c0+=" ";
		}
		int fi_id=-1;
		for(int i=else_id+1;i<siz;i++){
			if(tokens[i].first=="fi"){
				fi_id=i;
				break;
			}
			c1+=tokens[i].first;
			c1+=" ";
		}

		if(fi_id<siz-1){
			//Parse if 
			if(ParseBexp(b)){
				ParseCommandLine(c0,now_status);
				debug(s,status);
			}else{
				ParseCommandLine(c1,now_status);
				debug(s,status);
			}
			for(int i=fi_id+1;i<siz;i++){
				nxt+=tokens[i].first;
				nxt+=" ";
			}
			ParseCommandLine(nxt,now_status);
		}
		else{//over 
			if(ParseBexp(b)){
				ParseCommandLine(c0,now_status);
				debug(s,status);
			}else{
				ParseCommandLine(c1,now_status);
				debug(s,status);
			}
			return ;
		}
	}else if(tokens[0].first=="while"){//while b do c1;c2 od;nxt
		string b="";
		string c="";
		string nxt="";
		int do_id=-1;
		for(int i=1;i<siz;i++){
			if(tokens[i].first=="do"){
				do_id=i;
				break;
			}
			b+=tokens[i].first;
			b+=" ";
		}

		int balance=0;
		int od_id=-1;
		for(int i=do_id+1;i<siz;i++){
			if(tokens[i].first=="od"&&balance==0){
				od_id=i;
				break;
			}
			c+=tokens[i].first;
			c+=" ";
		}
		if(od_id<siz-1){
			while(ParseBexp(b)){
				ParseCommandLine(c,now_status);
			}	
			for(int i=od_id+1;i<siz;i++){
				nxt+=tokens[i].first;
				nxt+=" ";
			}			
			ParseCommandLine(nxt,now_status);
		}else{
			while(ParseBexp(b)){
				ParseCommandLine(c,now_status);
			}			
		}
	}else{
		cout<<"Syntax Error!"<<endl;
		exit(-1);
	}
}