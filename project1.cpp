#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <iterator>
#include <bits/stdc++.h>

using namespace std;

set<char> chars;
unsigned int num_chars;

struct states_struct { 
    vector< vector<int> > states;
};

vector<states_struct> nfa;
int nfa_size;
struct states_struct new_states;
int line_count;
stack<int> st;
bool epsilon;

int char_num(char c) {
    switch (c) {
        case 'E': return num_chars;
        default: return c - 'a';
    }
}

void getChars(string regex) {
    for (unsigned int i = 0; i < regex.size(); i++) {
        if (regex[i] != '*' && regex[i] != '&' && 
                regex[i] != '+' && regex[i] != '\n' &&
                regex[i] != 'E') {          
            chars.insert(regex[i]);
        }
    }
    num_chars = chars.size();
}

void push_new() {
    for (unsigned int i = 0; i <= num_chars; i++) {
        vector<int> row;
        new_states.states.push_back(row);
    }
    nfa.push_back(new_states);
}

void character(unsigned int i) {
    push_new();
    push_new();
    nfa[nfa_size].states[i].push_back(nfa_size + 1);
    st.push(nfa_size);
    nfa_size++;
    st.push(nfa_size);
    nfa_size++;
}

void union_() {
    push_new();
    push_new();
    int d = st.top(); st.pop();
    int c = st.top(); st.pop();
    int b = st.top(); st.pop();
    int a = st.top(); st.pop();
    nfa[nfa_size].states[num_chars].push_back(a);
    nfa[nfa_size].states[num_chars].push_back(c);
    nfa[b].states[num_chars].push_back(nfa_size + 1);
    nfa[d].states[num_chars].push_back(nfa_size + 1);
    st.push(nfa_size);
    nfa_size++;
    st.push(nfa_size);
    nfa_size++;
}

void concatenation() {
    int d = st.top(); st.pop();
    int c = st.top(); st.pop();
    int b = st.top(); st.pop();
    int a = st.top(); st.pop();
    nfa[b].states[num_chars].push_back(c);
    st.push(a);
    st.push(d);
}

void kleene_star() {
    push_new();
    push_new();
    int b = st.top();
    st.pop();
    int a = st.top();
    st.pop();
    nfa[nfa_size].states[num_chars].push_back(a);
    nfa[nfa_size].states[num_chars].push_back(nfa_size + 1);
    nfa[b].states[num_chars].push_back(a); 
    nfa[b].states[num_chars].push_back(nfa_size + 1);
    st.push(nfa_size);
    nfa_size++;
    st.push(nfa_size);
    nfa_size++;
}

void nextExp() {
    line_count--;
    nfa[nfa_size - 1].states[num_chars].push_back(nfa_size);
}

void postfix_to_nfa(string postfix) {
    for (unsigned int i = 0; i < postfix.size(); i++) {
        switch (postfix[i]) {
            case '*': kleene_star(); break;
            case '&': concatenation(); break;
            case '+': union_(); break;
            default: character(char_num(postfix[i]));
        }
    }
    if (line_count > 1) nextExp();
}

void display_nfa() {
    cout << "q\t";
    set<char>::iterator i;
    for (i = chars.begin(); i != chars.end(); i++) {
        char c = *i;
        cout << c << "\t";
    }
    cout << "E" << endl;
    for (unsigned int i = 0; i < nfa.size(); i++) {
        cout << i << "\t";
        for (unsigned int j = 0; j <= num_chars; j++) {
            for(unsigned int k = 0; k < nfa[i].states[j].size(); k++) {
                cout << nfa[i].states[j][k] << ' ';
            }
            cout << "\t";        
        }
        cout << "\n";
    }
}

void build_nfa(int i, string line) {
    if (i == 1) {
        line_count++;
        getChars(line);
    } else {
        getChars(line);
        postfix_to_nfa(line);
    }
}

int main() {
    line_count = 0;
    for (int i = 1; i <= 2; i++) {
        string line;
        ifstream file ("test.txt");
        if (file.is_open()) {
            while (getline (file, line)) {
                build_nfa(i, line);
            }
            file.close();
        }
    }
    display_nfa();
    return 0;
}
