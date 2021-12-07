#include<vector>
#include<iostream>
#include "Parser.h"

int main(){
	string source_code;
	string code_line;
	vector<string> all;
	while(getline(cin,code_line)){
		all.push_back(code_line);
		for(char c:code_line){
			if((int)c==13){
				continue;
			}
			source_code+=c;
		}
	}	
	ParseCommandLine(source_code);
	cout<<"final result: "<<endl;
	for(auto v:env){
		cout<<v.first<<": "<<v.second<<endl;
	}
	return 0;
}