#include<vector>
using namespace std;

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
	now_token="";
	return tokens;
}