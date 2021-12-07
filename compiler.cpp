#include<bits/stdc++.h>

using namespace std;

// status_map[k]: The value of all the var in status i; 
map<int,map<pair<string,bool>,int> > status_map;
map<string,int> env;
map<string,bool> vis;
int now_status=0;
bool isAlpha(char c){
	return (c>='a'&&c<='z')||(c>='A'&&c<='Z');
}
bool isDigit(char c){
	return c>='0'&&c<='9';
}
bool isSpecial(char c){
	return c=='+'||c=='-'||c=='*'||
	c=='='||c==':'||c==';'||c=='!'||
	c=='|'||c=='&'||c=='<';
}
//词法分析
/*{token,id}
	1:str like var_name
	2:num 1,2,231...
	3:others <=,:=....
*/
vector<pair<string,int> > lexer(string s){
	vector<pair<string,int>> tokens;

	string now_token;
	int now_status=0;
	for(char c:s){
		if(isAlpha(c)){
			if(now_status==0||now_status==1){
				now_token+=c;
				now_status=1;
			}else{
				int siz=(int)now_token.size();
				if(siz){
					tokens.push_back({now_token,now_status});
				}
				now_token="";
				now_token+=c;
				now_status=1;
			}
		}
		else if(isDigit(c)){
			if(now_status==0||now_status==2){
				now_token+=c;
				now_status=2;
			}else{
				int siz=(int)now_token.size();
				if(siz){
					tokens.push_back({now_token,now_status});
				}
				now_token="";
				now_token+=c;
				now_status=2;
			}
		}
		else if(isSpecial(c)){
			if(now_status==0||now_status==3){
				now_token+=c;
				now_status=3;
			}else{
				int siz=(int)now_token.size();
				if(siz){
					tokens.push_back({now_token,now_status});
				}
				now_token="";
				now_token+=c;
				now_status=3;
			}
		}else if(c==' '){
			int len=(int)now_token.size();
			if(len){
				tokens.push_back({now_token,now_status});
			}
			now_token="";
			now_status=0;
		}
	}
	if(now_token.size()) tokens.push_back({now_token,now_status});
	return tokens;
}

//return value of string s
int Eval(string s){
	int ans=0;
	for(char c:s){
		ans=ans*10+c-'0';
	}
	return ans;
}
//Parse Aexp and return the value.
int ParseAexp(string s){
	vector<pair<string,int>> tokens=lexer(s);
	int siz=(int)tokens.size();
	//a::=n
	if(tokens[0].second==2&&siz==1){
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
		}
		for(int i=add_id+1;i<siz;i++){
			a1+=tokens[i].first;
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
		}
		for(int i=sub_id+1;i<siz;i++){
			a1+=tokens[i].first;
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
		}
		for(int i=mul_id+1;i<siz;i++){
			a1+=tokens[i].first;
		}
		return ParseAexp(a0)*ParseAexp(a1);		
	}	
	return -1;
}

bool ParseBexp(string s){
	vector<pair<string,int> > tokens=lexer(s);
	int siz=(int)tokens.size();

	assert(siz>=2);
	if(siz==3){
		int left_val;
		int right_val;
		//calculate the left value
		if(tokens[0].second==1){
			left_val=env[tokens[0].first];
		}else{
			left_val=Eval(tokens[0].first);
		}
		//calculate the right value
		if(tokens[2].second==1){
			right_val=env[tokens[2].first];
		}else{
			right_val=Eval(tokens[2].first);
		}

		if(tokens[1].first=="=="){
			return left_val==right_val;
		}
		if(tokens[1].first=="<="){
			return left_val<=right_val;
		}
	}

	string b0="";
	string b1="";
	//!b
	if(tokens[0].first=="!"){
		for(int i=1;i<siz;i++){
			b0+=tokens[i].first;
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
		}
		for(int i=and_id+1;i<siz;i++){
			b1+=tokens[i].first;
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
		}
		for(int i=or_id+1;i<siz;i++){
			b1+=tokens[i].first;
		}
		return ParseBexp(b0)|ParseBexp(b1);
	}

	return 0;
}
//解析出tokens，根据tokens确定语句类型
void ParseCommandLine(vector<pair<string,int> > tokens){

	if(tokens[0].first=="if"){

	}else if(tokens[0].first=="while"){

	}else if(tokens[0].first=="skip"){
		
	}else{//X:=aexp

		string left=tokens[0].first;
		string aexp="";
		for(int i=2;i<(int)tokens.size();i++){
			aexp+=tokens[i].first;
		}
		int right=ParseAexp(aexp);
		vis[left]=1;
		env[left]=right;
	}
}
int main(){
	cout<<ParseBexp("X==Y")<<endl;
	string code_line;
	while(getline(cin,code_line)){
		vector<pair<string,int>> tokens=lexer(code_line);
		for(auto v:tokens){
			cout<<v.first<<" ";
		}
		cout<<endl;
		// ParseCommandLine(tokens);
	}	
	return 0;
}