#ifndef BIGINTEGER_H
#define BIGINTEGER_H

typedef struct Node{
    int data;
    struct Node *next, *prev;
}Node;

struct BigInteger{
    Node *head, *tail;
    int length;
    char sign;
};

struct BigInteger initialize(char *s);

struct BigInteger add(struct BigInteger num1, struct BigInteger num2);    
struct BigInteger sub(struct BigInteger num1, struct BigInteger num2);    
struct BigInteger mul(struct BigInteger num1, struct BigInteger num2);
struct BigInteger div1(struct BigInteger num1, struct BigInteger num2);
struct BigInteger mod(struct BigInteger num1,struct BigInteger num2);

void insertAtHead(struct BigInteger *num, int val);
void insertAtTail(struct BigInteger *num, int val);

int compare(struct BigInteger num1, struct BigInteger num2);
void trimZero(struct BigInteger *num);
void freeBigInteger(struct BigInteger *num);
int calculateLength(struct BigInteger *num);

void display(struct BigInteger num);

#endif
