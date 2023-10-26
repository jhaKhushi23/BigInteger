#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BigInteger.h"

struct BigInteger initialize(char *s){
    struct BigInteger *newBig = (struct BigInteger *)malloc(sizeof(struct BigInteger));

    // handling memory allocation failure
    if(newBig == NULL){
        printf("Memory Allocation Failed\n");
        exit(0);
    }

    // initializing properties of newBig
    newBig->length = 0;
    newBig->head = newBig->tail = NULL;
    newBig->sign = '+';

    // required variables
    char loop_control = s[0];
    unsigned short int isFirstDigit = 1;

    while(loop_control != '\0' && loop_control != '\n'){

        // handling sing of inputted integer
        if(isFirstDigit){
            if(s[0] == '-'){
                newBig->sign = '-';
                s += 1;
            } 

            else if(s[0] == '+'){
                newBig->sign = '+';
                s += 1;
            }
            
            else{
                newBig->sign = '+';
                s += 0;
            }

            isFirstDigit = 0;
        }

        // inserting digits into linked num
        if(s[0] >= '0' && s[0] <= '9'){
            insertAtTail(newBig, (s[0] - '0'));
        } 
        else {
            break;
        }

        s += 1;
        loop_control = s[0];
    }

    return *newBig;
}



struct BigInteger add(struct BigInteger num1, struct BigInteger num2){

    // perform this operation only for like signed nums
    if(num1.sign == '-' && num2.sign == '+'){
        num2.sign = '-';
        return sub(num1, num2);
    }
    else if(num1.sign == '+' && num2.sign == '-'){
        num2.sign = '+';
        return sub(num1, num2);
    }

    struct BigInteger *newBig = (struct BigInteger *)malloc(sizeof(struct BigInteger));

    // handling memory allocation failure
    if(newBig == NULL){
        printf("Memory Allocation Failed\n");
        exit(0);
    }

    // initializing properties of newBinary
    newBig->head = newBig->tail = NULL;
    newBig->length = 0;
    newBig->sign = num2.sign;

    // required variables
    Node *itr1 = num1.tail, *itr2 = num2.tail;
    unsigned short int carry = 0;


    // loop runs till both the nums becomes null
    while(itr1 != NULL || itr2 != NULL){
        unsigned short int val = 0;

        // handling cases of nums being null
        if(itr1 != NULL && itr2 != NULL){
            val = (itr1->data + itr2->data + carry);
        }
        else if(itr1 != NULL && itr2 == NULL){
            val = (itr1->data + carry);
        }
        else if(itr1 == NULL && itr2 != NULL){
            val = (itr2->data + carry);
        }
        else{
            break;
        }
        
        // checking for value and carry
        if(val >= 10){
            carry = 1;
            val -= 10;
        } else{
            carry = 0;
            val = val;
        }

        // pushing a newNode with new Data into the head of this BigInt
        insertAtHead(newBig, val);

        itr1 = (itr1 != NULL) ? itr1->prev : NULL;
        itr2 = (itr2 != NULL) ? itr2->prev : NULL;

        newBig->length += 1;
    }

    if(carry){
        insertAtHead(newBig, 1);
        carry = 0;
        newBig->length += 1;
    }

    return *newBig;
}

struct BigInteger sub(struct BigInteger num1, struct BigInteger num2){

    // comparing two nums
    short int comparision_result = compare(num1, num2);

    // handling cases for different magnitudes
    if(num1.sign == '+' && num2.sign == '-'){
        num2.sign = '+';
        return add(num1, num2);
    }
    else if(num1.sign == '-' && num2.sign == '+'){
        num2.sign = '-';
        return add(num1, num2);
    }
    else{
        // pass
    }

    struct BigInteger *newBig = (struct BigInteger *)malloc(sizeof(struct BigInteger));

    // handling memory allocation failure
    if(newBig == NULL){
        printf("Memory Allocation Failed\n");
        exit(0);
    }

    // initializing properties of newBinary
    newBig->head = newBig->tail = NULL;
    newBig->length = 0;


    // interchanign num1 and num2 to keep smaller num at num2
    if(comparision_result == -1){
        struct BigInteger tmp = num1;
        num1 = num2;
        num2 = tmp;
        num2.sign = '-';
    } else{
        // pass
    }

    // required variables
    Node *itr1 = num1.tail, *itr2 = num2.tail;
    short int borrow = 0;
    
    while(itr1 != NULL){
        int diff = 0;
        int digit1 = (itr1 != NULL) ? itr1->data : 0;
        int digit2 = (itr2 != NULL) ? itr2->data : 0;

        if (borrow == 1) {
            digit1 -= 1;
            borrow = 0; 
        }

        if (digit1 < digit2) {
            digit1 += 10;
            borrow = 1;
        }

        diff = digit1 - digit2;
        insertAtHead(newBig, diff);

        itr1 = itr1->prev;
        if (itr2 != NULL) {
            itr2 = itr2->prev;
        }
    }

    newBig->sign = num2.sign;
    trimZero(newBig);
    return *newBig;
}

struct BigInteger mul(struct BigInteger num1, struct BigInteger num2) {
    struct BigInteger *newBig = (struct BigInteger*)malloc(sizeof(struct BigInteger));

    // handling memory allocation failure
    if(newBig == NULL){
        printf("Memory Allocation Failed\n");
        exit(0);
    }

    // initializing newBig
    newBig->head = newBig->tail = NULL;
    newBig->length = 0;

    //handling sign
    if(num1.sign == num2.sign){
        newBig->sign = '+';
    } else{
        newBig->sign = '-';
    }

    // putting num with greater length as num1
    if(num1.length < num2.length){
        struct BigInteger swap = num1;
        num1 = num2;
        num2 = swap;
    }

    // required variables
    struct BigInteger tmp;
    int trailing_zeroes = 0;

    // for accessing elements of num2
    while (num2.tail != NULL) {

        int carry = 0;

        // initializing tmp every time
        tmp.head = tmp.tail = NULL;
        tmp.length = 0;
        tmp.sign = '+';

        // adding zeroes
        for (int i = 0; i < trailing_zeroes; i++) {
            insertAtTail(&tmp, 0);
        }

        Node *itr = num1.tail;

        // for accessing elements of num1
        while (itr != NULL) {
            int product = (itr->data * num2.tail->data + carry);
            
            carry = product / 10;
            product = product % 10;
            
            insertAtHead(&tmp, product);
            itr = itr->prev;
        }

        // adding last carry-if
        if (carry != 0) {
            insertAtHead(&tmp, carry);
        }

        // finally adding to the main num
        *newBig = add(*newBig, tmp);
        // recalculating length
        newBig->length = calculateLength(newBig);

        num2.tail = num2.tail->prev;
        trailing_zeroes += 1;
    }

    trimZero(newBig);
    return *newBig;
}

struct BigInteger div1(struct BigInteger num1, struct BigInteger num2){

    // required variables
    struct BigInteger ans = initialize("");
    struct BigInteger quotient = initialize("0");
    struct BigInteger one = initialize("1");
    
    // handling sign
    char keep_sign = '+';
    if(num1.sign != num2.sign){
        keep_sign = '-';
    }

    // for the sake of simplicity
    num1.sign = num2.sign = '+';

    // div by 1
    if(compare(num2, one) == 0){
        num1.sign = keep_sign;
        return num1;
    }

    // div by zero
    else if(compare(num1, quotient) == 0){
        return quotient;
    }

    // div by zero
    else if(compare(num2, quotient) == 0){
        return ans;
    }

    // div by greater number
    else if(compare(num1, num2) == -1){
        return quotient;
    }

    // div by same number
    else if(compare(num1, num2) == 0){
        return one;
    }

    // div greater by smaller
    else{
        ans = num1;
        
        while(compare(ans, num2) != -1){
            ans = sub(ans, num2);
            quotient = add(quotient, one);
        }
        
        quotient.sign = keep_sign;
        return quotient;
    }

    return ans;
}

struct BigInteger mod(struct BigInteger num1, struct BigInteger num2) {
    
    // required variables
    struct BigInteger zero = initialize("0");
    struct BigInteger one = initialize("1");
    struct BigInteger nothing = initialize("");

    // mod by same number
    if(compare(num1, num2) == 0){
        return nothing;
    }

    // mod by greater number
    else if(compare(num1, num2) == -1){
        return num1;
    }

    // mod by zero
    else if(compare(num1, zero) == 0 || compare(num2, zero) == 0){
        return zero;
    }

    // mod by one
    else if(compare(num2, one) == 0){
        return zero;
    }

    // mod of greater by smaller number
    else{
        struct BigInteger quotient = div1(num1, num2);
        struct BigInteger product = mul(quotient, num2);
        struct BigInteger modulus = sub(num1, product);
        return modulus;
    }

    return nothing;
}

void insertAtHead(struct BigInteger *num, int val){
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));

    // handling memory allocation failure
    if(newNode == NULL){
        printf("Memory Allocation Failed\n");
        exit(0);
    }

    newNode->data = val;

    // corner case : no head node
    if(num->head == NULL){
        newNode->next = newNode->prev = NULL;
        num->head = num->tail = newNode;
        num->length += 1;
        return;
    }

    (num->head)->prev = newNode;
    newNode->prev = NULL;

    newNode->next = (num->head);
    (num->head) = newNode;
    num->length += 1;
}

void insertAtTail(struct BigInteger *num, int val){
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));

    // handling memory allocation failure
    if(newNode == NULL){
        printf("Memory Allocation Failed\n");
        exit(0);
    }

    newNode->data = val;

    if(num->head == NULL){
        newNode->next = newNode->prev = NULL;
        num->head = num->tail = newNode;
        num->length += 1;
        return;
    }

    newNode->next = NULL;
    newNode->prev = num->tail;
    (num->tail)->next = newNode;
    num->tail = newNode;
    num->length += 1;
}

int compare(struct BigInteger num1, struct BigInteger num2){
    // num1 > lsit2
    if(num1.length > num2.length){
        return 1;
    }

    // num2 > num1
    if(num1.length < num2.length){
        return -1;
    }

    Node *itr1 = num1.head, *itr2 = num2.head;

    while(itr1 != NULL && itr2 != NULL){
        if(itr1->data > itr2->data){
            return 1;
        }

        if(itr1->data < itr2->data){
            return -1;
        }

        itr1 = itr1->next;
        itr2 = itr2->next;
    }
    
    if(itr1 == NULL && itr2 == NULL){
        return 0;
    }
    return 0;
}

void trimZero(struct BigInteger *num){
    if(num->head == NULL){
        return;
    }

    while(num->head->data == 0 && num->head->next != NULL){
        Node* tmp = num->head;
        num->head = num->head->next;
        num->head->prev = NULL;
        num->length -= 1;
        free(tmp);
    }
}

void freeBigInteger(struct BigInteger *num)
{
    Node *current = num->head;
    while (current != NULL)
    {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}

int calculateLength(struct BigInteger *num) {
    // required variables
    int len = 0;
    Node *current = num->head;

    while (current != NULL) {
        len++;
        current = current->next;
    }
    return len;
}



void display(struct BigInteger num){
    
    Node *head = num.head;
    
    // corner case : no head node
    if(head == NULL){
        printf("NULL\n");
        return;
    }

    printf("%c", num.sign);

    while(head != NULL){
        printf("%d", head->data);
        head = head->next;
    }
    printf("\n");
}
