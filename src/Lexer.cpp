/*
 * Lexer.cpp
 *
 *  Created on: Nov 29, 2014
 *      Author: thomas
 */

#include "Lexer.hpp"

Lexer::Lexer() {
	symbols.insert('+');
	symbols.insert('-');
	symbols.insert('*');
	symbols.insert('/');
	symbols.insert('!');
	symbols.insert(')');
	symbols.insert('(');
	symbols.insert(',');
	ops.insert("+");
	ops.insert("-");
	ops.insert("*");
	ops.insert("/");
	ops.insert("!");
	ops.insert("(");
	ops.insert(")");
	ops.insert(",");
	ops.insert("<");
	ops.insert("<>");
	ops.insert("<=");
	ops.insert(">");
	ops.insert(">=");
	ops.insert("==");
	ops.insert("&&");
	ops.insert("||");
}

/*EXAMPLE
 * select*from a__ where(1>0&&1<0||1<=0)
 * prettify:
 *    insert space before and after all symbols:
 *    select * from a__ where ( 1 > 0 && 1 < 0 || 1 <= 0 )
 * split:
 *    select
 *    *
 *    from
 *    a__
 *    where
 *    (
 *    1
 *    >
 *    0
 *    &&
 *    1
 *    <
 *    0
 *    ||
 *    1
 *    <=
 *    0
 *    )
 *
 * match keyword first:
 *    select from where
 *    to_upper
 *
 * match symbols:
 *    * ( > && < || <= )
 *
 * match numbers:
 *    [0-9]+
 *    illegal:
 *       01a
 * match id:
 *    [a-zA-Z_][a-z_0-9]*
 *    no illegal situation: all other symbols matched:
 *        a+/2b -> a + / 2b, 2b matched before
 */

void to_upper(char& a) {
	if (a >= 'a' && a <= 'z') {
		a = a - 'a' + 'A';
	}
}

bool is_keyword(const std::string& s) {
	std::string t = s;
	std::for_each(t.begin(), t.end(), to_upper);
	if (t == "SELECT" || t == "FROM" || t == "WHERE" || t == "DELETE"
			|| t == "TABLE") {
		return true;
	}
	return false;
}

std::list<Token> Lexer::GetTokens(const std::string& a) {
	std::list<std::string> tmp = split(a);
	std::list<std::string>::iterator ite;
	std::list<Token> ret;
	for (ite = tmp.begin(); ite != tmp.end(); ite++) {
		std::string t = *ite;
		if (is_keyword(t)) {
			std::for_each(t.begin(), t.end(), to_upper);
			ret.push_back(Token(KEYWORD, t));
		} else if (ops.find(t) != ops.end()) {
			ret.push_back(Token(OP, t));
		} else {
			//NUM
			if (t[0] >= '0' && t[0] <= '9') {
				for (int i = 1; i < t.size(); i++) {
					if (!(t[i] >= '0' && t[i] <= '9')) {
						std::cerr << "Illegal Tokens: " << t << std::endl;
						exit(EXIT_FAILURE);
					}
				}
				ret.push_back(Token(NUM, t));
				//ID
			} else if ((t[0] >= 'a' && t[0] <= 'z')
					|| (t[0] >= 'A' && t[0] <= 'Z') || t[0] == '_') {
				ret.push_back(Token(ID, t));
			}
		}
	}
	return ret;
}

std::list<std::string> Lexer::split(const std::string& s) {
	std::list<std::string> ret;
	std::string t = "";
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == ' ' || s[i] == '\t') {
			if (t != "") {
				ret.push_back(t);
				t = "";
			}
		} else if (symbols.find(s[i]) != symbols.end()) {
			if (t != "") {
				ret.push_back(t);
				t = "";
			}
			ret.push_back(std::string("") + s[i]);
		} else if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')
				|| (s[i] >= '0' && s[i] <= '9')) {
			t += s[i];
		} else if (s[i] == '<') {
			if (t != "") {
				ret.push_back(t);
				t = "";
			}
			if (s[i + 1] == '>') {
				ret.push_back("<>");
				i++;
			} else if (s[i + 1] == '=') {
				ret.push_back("<=");
				i++;
			} else {
				ret.push_back("<");
			}
		} else if (s[i] == '>') {
			if (t != "") {
				ret.push_back(t);
				t = "";
			}
			if (s[i + 1] == '=') {
				ret.push_back("<=");
				i++;
			} else {
				ret.push_back("<");
			}

		} else if (s[i] == '&' || s[i] == '|' || s[i] == '=') {
			if (t != "") {
				ret.push_back(t);
				t = "";
			}
			if (s[i + 1] == s[i]) {
				ret.push_back(std::string("") + s[i] + s[i]);
				i++;
			} else {
				std::cerr << "Illegal Token" << std::endl;
				exit(0);
			}
		}
	}
	if (t != "") {
		ret.push_back(t);
	}
	return ret;
}
