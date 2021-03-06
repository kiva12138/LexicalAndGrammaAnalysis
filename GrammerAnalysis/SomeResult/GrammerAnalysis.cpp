#include "pch.h"
#include <iostream>
#include <string>
#include <sstream> 
#include <fstream> 
#include <map>
#include <typeinfo>

#define MAXNUM 1000
#define ERROR "err"
#define ACCUTATE "acc"
#define MAXNUMOFCLOUSURE 100
#define MAXNUMOFITEM 100
#define MAXNUMOFCHAR 30

using namespace std;

class StatusStack{
protected:
	int list[MAXNUM];
	int currentPos;
public :
	StatusStack() {
		currentPos = -1;
	}

	int getTop() {
		return this->list[currentPos];
	}

	int getCurrentPos() {
		return this->currentPos;
	}

	int getElement(int pos) {
		return this->list[pos];
	}

	void clear() {
		currentPos = -1;
	}

	void pop() {
		currentPos = currentPos - 1;
	}

	void push(int status) {
		list[currentPos+1] = status;
		currentPos++;
	}
};

class SymbolStack {
protected:
	char list[MAXNUM];
	int currentPos;
public:
	SymbolStack() {
		list[0] = '#';
		currentPos = 0;
	}

	char getTop() {
		return this->list[currentPos];
	}

	int getCurrentPos() {
		return this->currentPos;
	}

	char getElement(int pos) {
		return this->list[pos];
	}

	void clear() {
		list[0] = '#';
		currentPos = 0;
	}

	void pop() {
		currentPos = currentPos - 1;
	}

	void push(char str) {
		list[currentPos + 1] = str;
		currentPos++;
	}
};

class ActionMap {//table[status, inputChar]
protected:
	map <int, map<int, string> > table;
	int numOfStatus;
	int numOfInputChar;

public:
	ActionMap(int numOfStatus, int numOfInputChar){
		this->numOfInputChar = numOfInputChar;
		this->numOfStatus = numOfStatus;
		
		for (int i = 0; i < numOfStatus; i++) {
			for (int j = 0; j < numOfInputChar; j++) {
				table[i][j] = ERROR;
			}
		}
	}

	void insert(int status, int inputChar, string ele) {
		table[status][inputChar] = ele;
	}

	void remove(int status, int inputChar) {
		table[status][inputChar] = ERROR;
	}

	string getElement(int status, int inputChar) {
		return table[status][inputChar];
	}
};

class GotoMap {//table[status, inputStatus]
protected:
	map <int, map<int, int> > table;
	int numOfStatus;
	int numOfInputStatus;

public:
	GotoMap(int numOfStatus, int numOfInputStatus) {
		this->numOfInputStatus = numOfInputStatus;
		this->numOfStatus = numOfStatus;

		for (int i = 0; i < numOfStatus; i++) {
			for (int j = 0; j < numOfInputStatus; j++) {
				table[i][j] = -1;
			}
		}
	}

	void insert(int status, int inputChar, int ele) {
		table[status][inputChar] = ele;
	}

	void remove(int status, int inputChar) {
		table[status][inputChar] = -1;
	}

	int getElement(int status, int inputChar) {
		return table[status][inputChar];
	}
};

class DFAMap {//[a, b]=B 从状态a接受B到达状态b
protected:
	map <int, map<int, char> > table;
	int firstStatus;
	int secondStatus;
public:
	DFAMap(int numOfStatus) {
		this->firstStatus = numOfStatus;
		this->secondStatus = numOfStatus;

		for (int i = 0; i < numOfStatus; i++) {
			for (int j = 0; j < numOfStatus; j++) {
				table[i][j] = '*';
			}
		}
	}

	void insert(int status1, int status2, char ele) {
		table[status1][status2] = ele;
	}

	void remove(int status1, int status2) {
		table[status1][status2] = '*';
	}

	char getElement(int status1, int status2) {
		return table[status1][status2];
	}
};

class NumOfInputChar {
protected:
	map<int, char> table;
	int sum;
public :
	NumOfInputChar() {
		this->sum = 0;
	}
	int getSum() {
		return this->sum;
	}
	void inset(char ch) {
		table [sum++] = ch;
	}
	int getNumber(char ch) {
		for (int i = 0; i <= sum; i++) {
			if (table[i] == ch) {
				return i;
			}
		}
		return -1;
	}
};

class NumOfInputStatus {
protected:
	map<int, char> table;
	int sum;
public:
	NumOfInputStatus() {
		this->sum = 0;
	}
	void inset(int i) {
		table[sum++] = i;
	}
	int getNumber(int in) {
		for (int i = 0; i <= sum; i++) {
			if (table[i] == in) {
				return i;
			}
		}
		return -1;
	}
};

class Item {
public :
	int posOfDot;
	string item;
public :
	Item() {
		for (int i = 0; i < 30; i++) {
			this->item += '\0';
		}
	}
};

class Clousure {
public:
	int numOfItem;
	Item grammer[MAXNUMOFITEM];
};

void printSymbolStack(SymbolStack symbolStack) {
	int num = symbolStack.getCurrentPos();
	cout << "符号栈的内容为:" << endl;
	for (int i = 0; i <= num; i++) {
		cout << symbolStack.getElement(i) << " ";
	}
	cout << endl;
}

void printStatusStack(StatusStack statusStack) {
	int num = statusStack.getCurrentPos();
	cout << "状态栈的内容为:" << endl;
	for (int i = 0; i <= num; i++) {
		cout << statusStack.getElement(i) << " ";
	}
	cout << endl;
}

void getInputSymbol(string strInput[], int * numOfSymbol) {
	ifstream fp("input3.txt");
	//ifstream fp("input_test.txt");
	string strRead[MAXNUM];
	string temp;
	if (fp.fail()) {
		std::cout << "Can not open the source file";
		exit(0);
	}
	int num = 0;
	while (getline(fp, temp)) {
		strInput[num++] = temp;
	}
	*numOfSymbol = num;
}

void getGrammer(string grammerInput[], int * numOfGrammer) {
	ifstream fp("grammer1.txt");
	//ifstream fp("grammer_test.txt");
	string temp;
	int grammerHasRead = 0;
	if (fp.fail()) {
		std::cout << "Can not open the source file";
		exit(0);
	}
	int num = 0;
	while (getline(fp, temp)) {
		grammerInput[num++] = temp;
	}
	*numOfGrammer = num;
}

bool grammerAnalysis(string strInput[], ActionMap actionMap, GotoMap gotoMap, 
					StatusStack statusStack, SymbolStack symbolStack,
					NumOfInputStatus numOfInputStatus, NumOfInputChar numOfInputChar, 
					string grammerInput[]) {

	statusStack.push(0);
	int i = 0;
	int lastAction = 0;//记录上一动作，0表示移进， 1表示归约

	while (1) {
		if (lastAction == 0) {
			cout << "Action " ;
			char temp[1];
			strInput[i].copy(temp, 1, 0);
			char inputChar = temp[0];
			//cout << inputChar << statusStack.getTop() <<"-" << numOfInputChar.getNumber(inputChar) << endl;
			string actionToDo = "";
			actionToDo = actionMap.getElement(statusStack.getTop(), numOfInputChar.getNumber(inputChar));
			cout << actionToDo << endl;
			if (actionToDo == ACCUTATE) {
				return true;
			}
			else if (actionToDo == ERROR) {
				return false;
			}
			if (actionToDo[0] == 'S') {
				//移进操作
				actionToDo[0] = '0';
				statusStack.push(stoi(actionToDo));
				symbolStack.push(inputChar);
				i++;
				lastAction = 0;

				//打印信息
				cout << "移进操作:" <<"移进"<< inputChar << endl;
				printStatusStack(statusStack);
				printSymbolStack(symbolStack);
				cout << endl;
			}
			else if (actionToDo[0] == 'R') {
				//归约操作 使用actionToDo[1]产生式
				actionToDo[0] = '0';
				int numOfGrammer = stoi(actionToDo);
				int startPosOfGrammer = 0;//产生式开始的位置
				int endPosOfGrammer = 0;//产生式结束的的分号的位置
				
				char temp[1];
				grammerInput[numOfGrammer-1].copy(temp, 1, endPosOfGrammer);
				char todoChar = temp[0];
				while (todoChar != ';') {
					grammerInput[numOfGrammer-1].copy(temp, 1, endPosOfGrammer);
					todoChar = temp[0];
					endPosOfGrammer++;
				}
				endPosOfGrammer--;

				int numOfPop = endPosOfGrammer - startPosOfGrammer - 1;
				for (; numOfPop > 0; numOfPop--) {
					symbolStack.pop();
					statusStack.pop();
				}
				grammerInput[numOfGrammer-1].copy(temp, 1, startPosOfGrammer);
				char charToPush = temp[0];
				symbolStack.push(charToPush);

				lastAction = 1;

				//打印信息
				cout << "归约操作:" << "使用第" << stoi(actionToDo) << "个产生式" << endl;
				printStatusStack(statusStack);
				printSymbolStack(symbolStack);
				cout << endl;
			}
			else {
				cout << "符号表识别错误" << endl;
				getchar();
				exit(0);
			}
			continue;
		}
		else if (lastAction == 1) {
			int statusToGo = gotoMap.getElement(statusStack.getTop(), numOfInputStatus.getNumber(symbolStack.getTop()));
			if (statusToGo == -1) {
				return false;
			}
			statusStack.push(statusToGo);
			lastAction = 0;
			//打印信息
			cout << "状态转移:" << "转移到第" << statusToGo << "状态" << endl;
			printStatusStack(statusStack);
			printSymbolStack(symbolStack);
			cout << endl;
			continue;
		}
	}
}

void makeAnalysisChart(string grammerInput[], ActionMap * actionMap, GotoMap * gotoMap, 
						NumOfInputChar * numOfInputChar, NumOfInputStatus * numOfInputStatus, int numOfGrammer) {	
	//构造Clousure, s代表S', 其余为大写S
	//首先构造最基本的0与1号状态
	int numOfClousure = 0;
	Clousure clousure[MAXNUMOFCLOUSURE];
	clousure[0].numOfItem = 1;
	clousure[0].grammer[0].posOfDot = 1;
	clousure[0].grammer[0].item = "s.S";
	clousure[1].numOfItem = 1;
	clousure[1].grammer[0].item = "sS.";
	clousure[1].grammer[0].posOfDot = 2;

	int numOfInitClo = 2;
	for (int i = 0; i < numOfGrammer; i++) {
		char chs[MAXNUMOFCHAR];
		int numOfCh = 0;
		for (int j = 0; j<MAXNUMOFCHAR; j++) {
				char temp[1];
				grammerInput[i].copy(temp, 1, j);
				if (temp[0] == ';') {
					break;
				}
				chs[j] = temp[0];
				numOfCh = j;
			}
		for (int j = 2; j <= numOfCh+1; j++) {
			clousure[numOfInitClo].numOfItem = 1;
			bool b = false;
			for (int k = 0; k <= numOfCh+1; k++) {
				if (k == j) {
					b = true;
					clousure[numOfInitClo].grammer[0].item[j] = '.';
					continue;
				}
				if (b) {
					clousure[numOfInitClo].grammer[0].item[k] = chs[k-1];
				}
				else {
					clousure[numOfInitClo].grammer[0].item[k] = chs[k];
				}
			}
			clousure[numOfInitClo].grammer[0].posOfDot = j;
			numOfInitClo++;
		}
	}

	while (numOfClousure < numOfInitClo) {
		int lastNumOfItem = clousure[numOfClousure].numOfItem;
		int posToInsert = clousure[numOfClousure].numOfItem;
		while (1) {
			//char toDoChar = clousure[numOfClousure].grammer[clousure[numOfClousure].numOfItem - 1].item[clousure[numOfClousure].grammer[clousure[numOfClousure].numOfItem - 1].posOfDot + 1];
			char toDoChar = clousure[numOfClousure].grammer[posToInsert - 1].item[clousure[numOfClousure].grammer[posToInsert - 1].posOfDot + 1];
			if (toDoChar == '\0') {
				numOfClousure++;
				break;
			}
			for (int i = 0; i < numOfGrammer; i++) {
				char temp[1];
				grammerInput[i].copy(temp, 1, 0);
				char firstGrammerChar = temp[0];

				if (firstGrammerChar == toDoChar) {
					stringstream stream;
					stream << firstGrammerChar;
					string itemToInsert = stream.str() + '.';
					bool ifExist = false;
					for (int k = 1; ; k++) {
						char tempChs[1];
						grammerInput[i].copy(tempChs, 1, k);
						char tempCh = tempChs[0];
						if (tempCh == ';') {
							break;
						}
						itemToInsert = itemToInsert + tempCh;
					}
					for (int k = 0; k < clousure[numOfClousure].numOfItem; k++) {
						if (itemToInsert == clousure[numOfClousure].grammer[k].item) {
							ifExist = true;
							break;
						}
					}
					if (ifExist) {
						continue;
					}
					clousure[numOfClousure].grammer[clousure[numOfClousure].numOfItem].item = itemToInsert;
					clousure[numOfClousure].grammer[clousure[numOfClousure].numOfItem].posOfDot = 1;
					clousure[numOfClousure].numOfItem++;
				}
			}
			
			posToInsert++;
			if (posToInsert >= clousure[numOfClousure].numOfItem) {
				if (lastNumOfItem == clousure[numOfClousure].numOfItem) {
					numOfClousure++;
					break;
				}
			}
			
			lastNumOfItem = clousure[numOfClousure].numOfItem;

		}
	}

	cout << "计算得出的闭包:" << endl << endl;
	for (int ik = 0; ik < numOfInitClo; ik++) {
		cout << "Clousure" << ik << ":" << endl;
		for (int j = 0; j < clousure[ik].numOfItem; j++) {
			cout << clousure[ik].grammer[j].item[0] << " -> ";
			for (int i = 1; i <= clousure[ik].grammer[j].item.length(); i++) {
				cout << clousure[ik].grammer[j].item[i];
			}
			cout << endl;
		}
	}
	cout << endl;

	//下面构造DFA
	DFAMap * dfaMap = new DFAMap(numOfInitClo);
	for (int n = 0; n < numOfInitClo; n++) {
		for (int m = 0; m < clousure[n].numOfItem; m++) {
			string itemToCal = clousure[n].grammer[m].item;
			char chToMove = clousure[n].grammer[m].item[clousure[n].grammer[m].posOfDot+1];//需要使用的符号
			if (chToMove == '\0') {
				continue;
			}
			itemToCal[clousure[n].grammer[m].posOfDot+1] = '.';
			itemToCal[clousure[n].grammer[m].posOfDot] = chToMove;
			for (int t = 0; t < numOfInitClo; t++) {
				for (int e = 0; e < clousure[t].numOfItem; e++) {
					string itemToCmp = clousure[t].grammer[e].item;
					//cout << itemToCal.c_str() << "==" << itemToCmp.c_str() << "=" << endl;
					if (strcmp(itemToCal.c_str() , itemToCmp.c_str()) == 0) {
						//cout << "OK"<< endl;
						dfaMap->insert(n, t, chToMove);
					}
				}
			}
		}
	}

	/*for (int n = 0; n < numOfInitClo; n++) {
		for (int m = 0; m < numOfInitClo; m++) {
			cout << dfaMap->getElement(n, m);
		}
		cout << endl;
	}*/

	//下面构造分析表
	for (int k = 0; k < numOfInitClo; k++) {
		if (k == 1) {//第1号一定是sS.
			actionMap->insert(k, numOfInputChar->getNumber('#'), ACCUTATE);
			continue;
		}
		for (int g = 0; g < clousure[k].numOfItem; g++) {
			string tempItem = clousure[k].grammer[g].item;
			if (tempItem[clousure[k].grammer[g].posOfDot+1] == '\0') {
				tempItem[clousure[k].grammer[g].posOfDot] = '\0';
				for (int h = 0; h < numOfGrammer; h++) {
					string tempGrammmer = grammerInput[h];
					tempGrammmer[tempGrammmer.length() - 1] = '\0';
					if (strcmp(tempGrammmer.c_str(), tempItem.c_str()) == 0) {
						//k状态 全部为R(h+1)
						//cout << k << "->" << h << "->" << tempItem.c_str() << endl;
						string str = "R" + to_string(h+1);
						for (int v = 0; v < numOfInputChar->getSum(); v++) {
							actionMap->insert(k, v, str);
						}
						break;
					}
				}
				continue;
			}
			char tempCh = tempItem[clousure[k].grammer[g].posOfDot + 1];
			if (numOfInputChar->getNumber(tempCh) >= 0) {
				for (int d = 0; d < numOfClousure; d++) {
					if (dfaMap->getElement(k, d) == tempCh) {
						string tempStr = "S"+ to_string(d);
						actionMap->insert(k, numOfInputChar->getNumber(tempCh), tempStr);
					}
				}
				continue;
			}
			if (numOfInputStatus->getNumber(tempCh) >= 0) {
				for (int d = 0; d < numOfClousure; d++) {
					if (dfaMap->getElement(k, d) == tempCh) {
						gotoMap->insert(k, numOfInputStatus->getNumber(tempCh),d);
					}
				}
				continue;
			}
			cout << tempItem; 
			cout << endl << "识别符号出错！" << endl;
			exit(0);
		}
	}
}

int main()
{
	int symbolNum = 0;
	string inputSymbol[MAXNUM];
	getInputSymbol(inputSymbol, &symbolNum);
	cout << "输入序列：" << endl;
	for (int i = 0; i <= symbolNum; i++) {
		cout << inputSymbol[i];
	}
	cout << endl << endl;

	int grammerNum;
	string grammerInput[MAXNUM];
	getGrammer(grammerInput, &grammerNum);
	cout << "语法序列：" << grammerNum << "个" << endl;
	for (int i = 0; i < grammerNum; i++) {
		cout << grammerInput[i][0] ;
		cout << " -> ";
		for (int j = 1; j < grammerInput[i].length(); j++) {
			cout << grammerInput[i][j];
		}
		cout << endl;
	}
	cout << endl;

	cout << "构造语法分析表......" << endl << endl;
	//ActionMap *action = new ActionMap(7, 3);//7个状态， 3个终结符
	ActionMap *action = new ActionMap(100, 13);
	NumOfInputChar *numOfInputChar = new NumOfInputChar;//与上面相对应的终结符集合
	//numOfInputChar->inset('a');
	//numOfInputChar->inset('b');
	//numOfInputChar->inset('#');
	numOfInputChar->inset('a');
	numOfInputChar->inset('b');
	numOfInputChar->inset('d');
	numOfInputChar->inset('e');
	numOfInputChar->inset('f');
	numOfInputChar->inset('g');
	numOfInputChar->inset('w');
	numOfInputChar->inset('i');
	numOfInputChar->inset('=');
	numOfInputChar->inset('(');
	numOfInputChar->inset(')');
	numOfInputChar->inset('{');
	numOfInputChar->inset('}');
	numOfInputChar->inset('#');

	/*
	action->insert(0, numOfInputChar->getNumber('a'), "S3");
	action->insert(0, numOfInputChar->getNumber('b'), "S4");
	action->insert(1, numOfInputChar->getNumber('#'), ACCUTATE);
	action->insert(2, numOfInputChar->getNumber('a'), "S3");
	action->insert(2, numOfInputChar->getNumber('b'), "S4");
	action->insert(3, numOfInputChar->getNumber('a'), "S3");
	action->insert(3, numOfInputChar->getNumber('b'), "S4");
	action->insert(4, numOfInputChar->getNumber('a'), "R3");
	action->insert(4, numOfInputChar->getNumber('b'), "R3");
	action->insert(4, numOfInputChar->getNumber('#'), "R3");
	action->insert(5, numOfInputChar->getNumber('a'), "R1");
	action->insert(5, numOfInputChar->getNumber('b'), "R1");
	action->insert(5, numOfInputChar->getNumber('#'), "R1");
	action->insert(6, numOfInputChar->getNumber('a'), "R2");
	action->insert(6, numOfInputChar->getNumber('b'), "R2");
	action->insert(6, numOfInputChar->getNumber('#'), "R2");



	cout << "Action表：" << endl;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 3; j++) {
			cout << action->getElement(i, j) << " ";
		}
		cout << endl;
	}

	cout << endl;

	*/
	
	//GotoMap *gotoMap = new GotoMap(7, 2);//7个状态，2个非终结符
	GotoMap *gotoMap = new GotoMap(100, 18);
	NumOfInputStatus *numOfInputStatus = new NumOfInputStatus;//非终结符集合
	//numOfInputStatus->inset('S');
	//numOfInputStatus->inset('B');
	numOfInputStatus->inset('S');
	numOfInputStatus->inset('A');
	numOfInputStatus->inset('B');
	numOfInputStatus->inset('C');
	numOfInputStatus->inset('D');
	numOfInputStatus->inset('E');
	numOfInputStatus->inset('F');
	numOfInputStatus->inset('G');
	numOfInputStatus->inset('H');
	numOfInputStatus->inset('J');
	numOfInputStatus->inset('L');
	numOfInputStatus->inset('U');
	numOfInputStatus->inset('W');
	numOfInputStatus->inset('M');
	numOfInputStatus->inset('N');
	numOfInputStatus->inset('O');
	numOfInputStatus->inset('P');
	numOfInputStatus->inset('R');
	numOfInputStatus->inset('I');
	//numOfInputStatus->inset('=');
	//numOfInputStatus->inset('(');
	//numOfInputStatus->inset(')');
	//numOfInputStatus->inset('{');
	//numOfInputStatus->inset('}');


	/*
	gotoMap->insert(0, numOfInputStatus->getNumber('S'), 1);
	gotoMap->insert(0, numOfInputStatus->getNumber('B'), 2);
	gotoMap->insert(2, numOfInputStatus->getNumber('B'), 5);
	gotoMap->insert(3, numOfInputStatus->getNumber('B'), 6);

	cout << "Goto表：" << endl;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 2; j++) {
			cout << gotoMap->getElement(i, j) << " ";
		}
		cout << endl;
	}
	cout << endl;

	*/
	
	makeAnalysisChart(grammerInput, action, gotoMap, numOfInputChar, numOfInputStatus, grammerNum);

	//Test
	/*
	cout << "Action表：" << endl;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 3; j++) {
			cout << action->getElement(i, j) << " ";
		}
		cout << endl;
	}
	cout << endl;

	cout << "Goto表：" << endl;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 2; j++) {
			cout << gotoMap->getElement(i, j) << " ";
		}
		cout << endl;
	}
	cout << endl;
	*/
	cout << "Action表：" << endl;
	for (int i = 0; i < 69; i++) {
		for (int j = 0; j < 14; j++) {
			cout << action->getElement(i, j) << " ";
		}
		cout << endl;
	}
	cout << endl;

	cout << "Goto表：" << endl;
	for (int i = 0; i < 69; i++) {
		for (int j = 0; j < 19; j++) {
			cout << gotoMap->getElement(i, j) << " ";
		}
		cout << endl;
	}
	cout << endl;
	
	cout << "语法分析表构造完毕......" << endl << endl;

	cout << "开始语法分析......" << endl << endl;
	StatusStack *statusStack = new StatusStack;
	SymbolStack *symbolStack = new SymbolStack;

	bool result = grammerAnalysis(inputSymbol, *action, *gotoMap,
									*statusStack, *symbolStack,
									*numOfInputStatus, *numOfInputChar,
									grammerInput);

	if (result) {
		cout << endl << "成功完成语法分析!"  << endl << endl;
	}
	else {
		cout << endl << "语法分析失败!" << endl << endl;
	}
	cout << "结束语法分析......" << endl;
	
	
	
	getchar();
	return 0;
}

