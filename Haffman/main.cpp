//
//  main.cpp
//  Haffman
//
//  Created by Елизавета Трифонова on 26.04.17.
//  Copyright © 2017 Елизавета Трифонова. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>
#include <list>

using namespace std;

struct Node{
    int a; //частота символа
    char c; //символ
    Node *left; //указатель на левый потомок
    Node *right; //указатель на правый потомок
};

struct  cod_table{
    char symbol; //символ
    char code[9]; //код
};

struct MyCompare{
    bool operator()(Node* l, Node* r) const{
        return (l->a<r->a);
    }
};

int SetZero(int* a){
    for(int i=0; i<256; i++){
        a[i]=0;
    }
    return 0;
}

int CountSymbol(ifstream& text, int* a){
    char c;
    while((c=text.get()) != EOF){
        a[(int)c]++;
    }
    int n=0;
    for(int i=0; i<256; i++){
        if(a[i]!=0){
            n++;
            
        }
    }
    return n;
}

void CreateList(list<Node*>& tree, int* a){
    for(int i=0; i<256; i++){
        if(a[i]!=0){
            Node* element=new Node;
            element->a=a[i];
            element->c=i;
            element->right=element->left=NULL;
            tree.push_back(element);
        }
    }
}
void CreateTree(list<Node*>& tree){
    while(tree.size()!=1){
        tree.sort(MyCompare());
        Node* LeftSon=tree.front();
        tree.pop_front();
        Node* RightSon=tree.front();
        tree.pop_front();
        Node* dad=new Node;
        dad->a=LeftSon->a+RightSon->a;
        dad->left=LeftSon;
        dad->right=RightSon;
        tree.push_back(dad);
    }
}

void CreateCode(Node* root, char c, vector<int>& line, ofstream& code) {
    if( (*root).left != NULL ) {
        line.push_back(0);
        CreateCode((*root).left, c, line, code);
    }
    if( (*root).right != NULL ) {
        line.push_back(1);
        CreateCode((*root).right, c, line, code);
    }
    if( ((*root).right == NULL) && ((*root).left == NULL) && ((*root).c == c) ) {
        for(int j=0; j<line.size(); j++){
            code<<line[j];
        }
        return;
    }
        line.pop_back();
}


void Encode(ifstream& texts, Node* root, ofstream& code){
    char c;
    vector<int> line;
    c=texts.get();
    while(c != EOF){
        CreateCode(root, c, line, code);
        line.clear();
        c=texts.get();
    }
    code<<endl;
}
void print(Node* root,int k=0)
{
    if(root!=NULL)
    {
        print(root->left,k+7);
        for(int i=0;i<k;i++)
        {
            cout<<" ";
        }
        if (root->c) cout<<root->a<<" ("<<root->c<<")"<<endl;
        else cout<<root->a<<endl;
        print(root->right,k+7);
    }
}


void decodeLine(ifstream& code, Node* root, ofstream& encode){
    Node* elem=root;
    char c;
    while((c=code.get()) != EOF){
        if(c=='0'){
            elem=elem->left;
        }
        if(c=='1'){
            elem=elem->right;
        }
        if((elem->right==NULL)&&(elem->left==NULL)){
            encode<<(char)elem->c;
            elem=root;
        }
    }
}

int main(){
    ifstream text("text.txt");
    int a[256]; //массив для частот
    SetZero(a); //обнуление массива частот
    int n;
    n=CountSymbol(text,a); //подсчет частот строки, возвращается количество различных символов строки
    text.close();
    list<Node*>tree;
    CreateList(tree, a);
    CreateTree(tree);
    Node* root=tree.front();
    //print(root);
    ifstream texts("text.txt");
    ofstream code("code.txt");
    Encode(texts,root,code);
    texts.close();
    code.close();
    ifstream codes("code.txt");
    ofstream encode("encode.txt");
    decodeLine(codes, root, encode);// декодирование строки по дереву
    codes.close();
    encode.close();
    
}

