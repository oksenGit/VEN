#include<iostream>
#include<fstream>
#include<string>
#include<vector>

using namespace std;

class STable;

enum TokenType
{
	BEGIN_SY, END_SY, PROC_SY, EQUAL_SY,
	LEFTPAR_SY, RIGHTPAR_SY, INTEGER_SY,
	BOOLEAN_SY, COMMA_SY, SEMICOLON_SY,
	ASSIGMENT_SY, READ_SY, WRITE_SY,
	IF_SY, THEN_SY, ELSE_SY, WHILE_SY,
	DO_SY, CALL_SY, OR_SY, AND_SY, NOT_SY,
	MINUS_SY, ID_SY, LESSTHAN_SY, GREATERTHAN_SY,
	GREATERTHANOREQUAL_SY, LESSTHANOREQUAL_SY,
	NOTEQUAL_SY, PLUS_SY, STAR_SY, DIVSION_SY,
	NUMERAL_SY, $_SY, ERROR_SY, EOF_SY
};


class Token {
public:
	TokenType type;
	string name;
	Token() {
		type = ERROR_SY;
		name = "";
	}
};

Token current_token;
int SIZE = 10;

enum Type { T_INTEGER, T_BOOLEAN, T_PROC, T_EMPTY };


struct Param
{
	string name;
	Type type;
	Param(string n, Type t) {
		name = n;
		type = t;
	}
};


class Symbol {
public:
	string name;
	Type type;
	vector<Param> paramater;
	Symbol* next;
	Symbol(string n, Type t, vector<Param> param) {
		name = n;
		type = t;
		paramater = param;
		next = NULL;
	}
};
class STable {
public:
	Symbol** table;
	STable() {
		table = new Symbol*[SIZE];

		for (int i = 0; i < SIZE; i++)
		{
			table[i] = NULL;
		}
	}

	int hashFunc(string name) {
		int sum = 0;
		for (int i = 0; i < name.length(); i++)
		{
			sum += name[i];
		}
		return sum % SIZE;
	}
	void insert(string name, Type type, vector<Param> param) {
		Symbol *p = new Symbol(name, type, param);
		int index = hashFunc(name);
		Symbol* entry = table[index];
		if (entry == NULL) {
			table[index] = p;
		}
		else {
			while (entry->next != NULL) {
				entry = entry->next;
			}
			entry->next = p;
		}
	}
	bool search(string name) {
		int index = hashFunc(name);
		Symbol* entry = this->table[index];
		while (entry != NULL) {
			if (entry->name == name)
				return true;
			else
				entry = entry->next;
		}
		return false;
	}
	Type getType(string name) {
		int index = hashFunc(name);
		Symbol* entry = this->table[index];
		while (entry != NULL) {
			if (entry->name == name)
				return entry->type;
			else
				entry = entry->next;
		}
		return T_EMPTY;
	}
	vector<Param> getParam(string name) {
		int index = hashFunc(name);
		Symbol* entry = this->table[index];
		while (entry != NULL) {
			if (entry->name == name)
				return entry->paramater;
			else
				entry = entry->next;
		}
		return{};
	}
	bool intersect(STable sub) {
		for (int i = 0; i < SIZE; i++)
		{
			Symbol* entry = sub.table[i];
			while (entry != NULL) {
				if (this->search(entry->name)) {
					cout << "search result:" << entry->name << endl;
					return true;
				}
				entry = entry->next;
			}
		}
		return false;
	}
	void union_table(STable sub) {
		Symbol* entry;
		for (int i = 0; i < SIZE; i++)
		{
			entry = sub.table[i];
			while (entry != NULL)
			{
				this->insert(entry->name, entry->type, entry->paramater);
				entry = entry->next;
			}
		}
	}
};

class Parser {
private: fstream f;
public:
	Parser(string filename)
	{
		f.open(filename);
		current_token = get_token();
		if (!f)
		{
			cout << "Unable to open f" << endl;
			system("pause");
			exit(1);
		}
	}
	~Parser()
	{
		f.close();
	}
	Token check_reserved(string name) {
		Token tokenHolder;
		if (name == "proc") {
			tokenHolder.name = name;
			tokenHolder.type = PROC_SY;
			return tokenHolder;
		}
		else if (name == "call") {
			tokenHolder.name = name;
			tokenHolder.type = CALL_SY;
			return tokenHolder;
		}
		else if (name == "begin") {
			tokenHolder.name = name;
			tokenHolder.type = BEGIN_SY;
			return tokenHolder;
		}
		else if (name == "end") {
			tokenHolder.name = name;
			tokenHolder.type = END_SY;
			return tokenHolder;
		}
		else if (name == "integer") {
			tokenHolder.name = name;
			tokenHolder.type = INTEGER_SY;
			return tokenHolder;
		}
		else if (name == "boolean") {
			tokenHolder.name = name;
			tokenHolder.type = BOOLEAN_SY;
			return tokenHolder;
		}
		else if (name == "read") {
			tokenHolder.name = name;
			tokenHolder.type = READ_SY;
			return tokenHolder;
		}
		else if (name == "write") {
			tokenHolder.name = name;
			tokenHolder.type = WRITE_SY;
			return tokenHolder;
		}
		else if (name == "if") {
			tokenHolder.name = name;
			tokenHolder.type = IF_SY;
			return tokenHolder;
		}
		else if (name == "then") {
			tokenHolder.name = name;
			tokenHolder.type = THEN_SY;
			return tokenHolder;
		}
		else if (name == "else") {
			tokenHolder.name = name;
			tokenHolder.type = ELSE_SY;
			return tokenHolder;
		}
		else if (name == "while") {
			tokenHolder.name = name;
			tokenHolder.type = WHILE_SY;
			return tokenHolder;
		}
		else if (name == "do") {
			tokenHolder.name = name;
			tokenHolder.type = DO_SY;
			return tokenHolder;
		}
		else if (name == "and") {
			tokenHolder.name = name;
			tokenHolder.type = AND_SY;
			return tokenHolder;
		}
		else if (name == "or") {
			tokenHolder.name = name;
			tokenHolder.type = OR_SY;
			return tokenHolder;
		}
		else {
			tokenHolder.name = name;
			tokenHolder.type = ID_SY;
			return tokenHolder;
		}
	}
	Token get_token() {
		string stringHolder = "";
		Token tokenHolder;
		char ch;
		f.get(ch);
		while (!f.eof() && isspace(ch)) {
			f.get(ch);
		}

		if (f.eof()) {
			tokenHolder.name = "End of f";
			tokenHolder.type = EOF_SY;
			return tokenHolder;
		}
		else if (ch == ':') {
			f.get(ch);
			if (ch == '=') {
				tokenHolder.name = ":=";
				tokenHolder.type = ASSIGMENT_SY;
				return tokenHolder;
			}
			else {
				tokenHolder.name = ch;
				tokenHolder.type = ERROR_SY;
				return tokenHolder;
			}
		}
		else if (ch == ';') {
			tokenHolder.name = ch;
			tokenHolder.type = SEMICOLON_SY;
			return tokenHolder;
		}
		else if (ch == ',') {
			tokenHolder.name = ch;
			tokenHolder.type = COMMA_SY;
			return tokenHolder;
		}
		else if (ch == '(') {
			tokenHolder.name = ch;
			tokenHolder.type = LEFTPAR_SY;
			return tokenHolder;
		}
		else if (ch == ')') {
			tokenHolder.name = ch;
			tokenHolder.type = RIGHTPAR_SY;
			return tokenHolder;
		}
		else if (ch == '-') {
			tokenHolder.name = ch;
			tokenHolder.type = MINUS_SY;
			return tokenHolder;
		}
		else if (ch == '<') {
			f.get(ch);
			if (isspace(ch)) {
				tokenHolder.name = ch;
				tokenHolder.type = LESSTHAN_SY;
				return tokenHolder;
			}
			else if (ch == '=') {
				tokenHolder.name = "<=";
				tokenHolder.type = LESSTHANOREQUAL_SY;
				return tokenHolder;
			}
			else if (ch == '>')
			{
				tokenHolder.name = "<>";
				tokenHolder.type = NOTEQUAL_SY;
				return tokenHolder;
			}
			else {
				f.putback(ch);
				{
					tokenHolder.name = ch;
					tokenHolder.type = LESSTHAN_SY;
					return tokenHolder;
				}
			}
		}
		else if (ch == '>') {
			f.get(ch);
			if (isspace(ch)) {
				tokenHolder.name = '>';
				tokenHolder.type = GREATERTHAN_SY;
				return tokenHolder;
			}
			else if (ch == '=') {
				tokenHolder.name = ">=";
				tokenHolder.type = GREATERTHANOREQUAL_SY;
				return tokenHolder;
			}
			else {
				f.putback(ch);
				{
					tokenHolder.name = '>';
					tokenHolder.type = GREATERTHAN_SY;
					return tokenHolder;
				}
			}
		}
		else if (ch == '=') {
			tokenHolder.name = ch;
			tokenHolder.type = EQUAL_SY;
			return tokenHolder;
		}
		else if (ch == '+') {
			tokenHolder.name = ch;
			tokenHolder.type = PLUS_SY;
			return tokenHolder;
		}
		else if (ch == '*') {
			tokenHolder.name = ch;
			tokenHolder.type = STAR_SY;
			return tokenHolder;
		}
		else if (ch == '/') {
			tokenHolder.name = ch;
			tokenHolder.type = DIVSION_SY;
			return tokenHolder;
		}
		else if (ch == '$') {
			tokenHolder.name = ch;
			tokenHolder.type = $_SY;
			return tokenHolder;
		}
		else if (isdigit(ch)) { //the start of a number
			stringHolder = ch;
			f.get(ch);
			if (isspace(ch)) {
				tokenHolder.name = stringHolder; //"numeral" 
				tokenHolder.type = NUMERAL_SY;
				return tokenHolder;
			} //the number ended
			while (!f.eof() && isdigit(ch)) //loops till the end of that number
			{
				stringHolder += ch;
				f.get(ch);
			}
			f.putback(ch); //to leave the extra char that made us leave the loop
			{
				tokenHolder.name = stringHolder;//"numeral"
				tokenHolder.type = NUMERAL_SY;
				return tokenHolder;
			} //it didn't find any char inside the number then it's not an error		
		}
		else if (isalpha(ch)) { //the start of ident
			stringHolder = ch;
			f.get(ch);
			if (isspace(ch)) {
				tokenHolder.name = stringHolder;
				tokenHolder.type = ID_SY;
				return tokenHolder;
			}//it's only 1 char so we know it's a ident and not a reserved word so we don't need to check
			while (!f.eof() && isalnum(ch)) //loops till the end of that string
			{
				stringHolder += ch;
				f.get(ch);
			}
			f.putback(ch); //to leave the extra char that made us leave the loop
			tokenHolder = check_reserved(stringHolder);
			return tokenHolder;
		}
		else {
			cout << "here scanner ";
			current_token.type = ERROR_SY;
			current_token.name = ch;
		}

	}
	void syntax_error() {
		cout << "syntax error in the program in token:" << current_token.name << " type: " << current_token.type << endl;
		current_token = get_token();//to continue the program
									/*system("pause");
									exit(1);*/

									//we won't exit the program for reading more errors not just the first one
	}
	Token current_token;
	void match(TokenType t) {
		if (current_token.type == t) {
			cout << "current token  " << current_token.name << " " << current_token.type << endl;
			current_token = get_token();
		}
		else if (current_token.type == ERROR_SY) cout << "lexical error at :  " << current_token.name << " " << current_token.type << endl;

		else syntax_error();
	}
	//==============================================================

	//nest functions
	vector<STable> nest;
	bool searchNest(string name) {
		for (int i = nest.size() - 1; i >= 0; i--) {
			if (nest[i].search(name)) return true;
		}
		return false;
	}

	Type getTypeNest(string name) {
		for (int i = nest.size() - 1; i >= 0; i--) {
			if (nest[i].search(name)) {
				return nest[i].getType(name);
			}
		}
		return T_EMPTY;
	}

	vector<Param> getParamNest(string name) {
		for (int i = nest.size() - 1; i >= 0; i--) {
			if (nest[i].search(name) && getTypeNest(name) == T_PROC) {
				return nest[i].getParam(name);
			}
		}
		return{};
	}

	vector<Type> getNameListType() {
		vector<Type> types = {};
		types.push_back(getTypeNest(current_token.name));
		match(ID_SY);
		while (current_token.type == COMMA_SY) {
			match(COMMA_SY);
			types.push_back(getTypeNest(current_token.name));
			match(ID_SY);
		}
		return types;
	}

	bool matchParams(string name) {
		vector<Param> nameParams = getParamNest(name);
		vector<Type> nameLtype = getNameListType();
		if (nameParams.size() != nameLtype.size()) {
			return false;
		}
		for (int i = 0; i < nameParams.size(); i++) {
			if (nameLtype[i] != nameParams[i].type) {
				return false;
			}
		}
		return true;
	}

	//==============================================================

	//<program> ::= <block>
	void program() {
		block();
		match($_SY);
	}

	//<block> ::= begin <declseq> <comseq> end
	void block()
	{
		STable st = STable();
		match(BEGIN_SY);
		declSq(st);
		nest.push_back(st);
		comSq(st);
		match(END_SY);
	}
	//<declseq> ::= <decl>{<decl>} 
	void declSq(STable &st) {
		decl(st);
		while (current_token.type == BOOLEAN_SY || current_token.type == INTEGER_SY || current_token.type == PROC_SY)
		{
			STable s2 = STable();
			decl(s2);
			if (st.intersect(s2))
			{
				cout << "Error var already declared\n";
				return;
			}
			st.union_table(s2);
		}
	}

	//decl ::= <type> <name - list> |
	//		proc <name>[(<parameter - list>)] = <command> 
	void decl(STable &st)
	{
		if (current_token.type == BOOLEAN_SY || current_token.type == INTEGER_SY)
		{
			constructName(st);
		}
		else if (current_token.type == PROC_SY)
		{
			match(PROC_SY);
			string idName = current_token.name;
			vector<Param> params = {};
			match(ID_SY);
			if (current_token.type == LEFTPAR_SY) {
				match(LEFTPAR_SY);
				params = param_list();
				match(RIGHTPAR_SY);
			}
			st.insert(idName, T_PROC, params);
			match(EQUAL_SY);
			command(st);
		}
	}
	vector<Param> constructName(STable &st)
	{
		Type t = type();
		vector<Param> params;
		st.insert(current_token.name, t, {});
		params.push_back(Param(current_token.name, t));
		match(ID_SY);
		while (current_token.type == COMMA_SY)
		{
			match(COMMA_SY);
			if (st.search(current_token.name)) {
				cout << "Error name list Var Already declared\n";
				return{};
			}
			st.insert(current_token.name, t, {});
			params.push_back(Param(current_token.name, t));
			match(ID_SY);
		}
		return params;
	}

	//<parameter-list> ::= <type> <name-list>{; <type> <namelist>} 
	vector<Param> param_list() {
		STable stP = STable();
		vector<Param> params = constructName(stP);
		while (current_token.type == SEMICOLON_SY)
		{
			match(SEMICOLON_SY);
			vector<Param> temp = constructName(stP);
			for (int i = 0; temp.size(); i++) {
				params.push_back(temp[i]);
			}
		}
		return params;
	}

	Type type()
	{
		Type t;
		switch (current_token.type)
		{
		case INTEGER_SY:
			t = T_INTEGER;
		case BOOLEAN_SY:
			t = T_BOOLEAN;
		case PROC_SY:
			t = T_PROC;
		default:
			t = T_EMPTY;
		}
		match(current_token.type);
		return t;
	}

	//<command-seq> ::= <command> {; <command>}
	void comSq(STable st) {
		command(st);
		while (current_token.type == SEMICOLON_SY) {
			match(SEMICOLON_SY);
			command(st);
		}
	}
	/*
	<command> ::= <name> := <expr>
				| read <name>
				| write <expr>
				| if <expr> then <command-seq> [else <command-seq>] end if
				| while <expr> do <command-seq> end while
				| call <name> [(<name-list>)]
				|<block>
	*/


	void command(STable st) {
		if (current_token.type == ID_SY) {
			if (searchNest(current_token.name) == false) {
				cout << "id must be declared\n";
				return;
			}
			if (getTypeNest(current_token.name) != T_INTEGER && getTypeNest(current_token.name) != T_BOOLEAN) {
				cout << "id must be variable\n";
				return;
			}
			if (getTypeNest(current_token.name) != expr()) {
				cout << "id type must be the same type of the expr\n";
				return;
			}
			match(ID_SY);
		}
		else if (current_token.type == READ_SY) {
			match(READ_SY);
			if (getTypeNest(current_token.name) != T_INTEGER && getTypeNest(current_token.name) != T_BOOLEAN) {
				cout << "id in read must be variable\n";
				return;
			}
			match(ID_SY);
		}
		else if (current_token.type == WRITE_SY) {
			match(WRITE_SY);
			expr();
		}
		else if (current_token.type == IF_SY) {
			match(IF_SY);
			if (expr() != T_BOOLEAN) {
				cout << "expr in if must be boolean\n";
				return;
			}
			match(THEN_SY);
			comSq(st);
			if (current_token.type == ELSE_SY) {
				match(ELSE_SY);
				comSq(st);
			}
			match(END_SY);
			match(IF_SY);
		}
		else if (current_token.type == WHILE_SY) {
			match(WHILE_SY);
			if (expr() != T_BOOLEAN) {
				cout << "expr in while must be boolean\n";
				return;
			}
			match(DO_SY);
			comSq(st);
			match(END_SY);
			match(IF_SY);
		}
		else if (current_token.type == CALL_SY) {
			match(CALL_SY);
			if (getTypeNest(current_token.name) != T_PROC) {
				cout << current_token.name << " must be declared as proc\n";
				return;
			}
			string idName = current_token.name;
			match(ID_SY);
			if (current_token.type == LEFTPAR_SY) {
				match(LEFTPAR_SY);
				if (matchParams(idName) == false) {
					cout << "params don't match\n";
					return;
				}
				match(RIGHTPAR_SY);
			}
		}
		else if (current_token.type == BEGIN_SY) {
			block();
		}
		else {
			syntax_error();
		}

	}


	Type expr() {

	}

	Type getExpType() {


	}

};

int main() {
	//Parser pa("parse.txt");
	//pa.program();
	system("pause");
	return 0;
}