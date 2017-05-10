#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

int i; //number of propositional variables
int nComs; //number of combinations
int **arr; //array storing all possible combinations of propositional variable values
char *eq;
char *simEq;
char *stack;

int power(int base, int of) {
    int ans = base;
    for (int j = 1; j < of; j++) {
        ans *= base;
        //printf("%d^%d = %d\n", base, j, ans);
    }
    return ans;
}

void allotMem() {
    arr = (int **)malloc(nComs * sizeof(int *));
    for (int d = 0; d < nComs; d++) {
         arr[d] = (int *)malloc(i * sizeof(int));
    }
    simEq = malloc(strlen(eq) * sizeof(char));
}

/*
I had to be creative with this solution. I found that a truth table was essentially 
each column alternating between 0 and 1 as onestack goes down the rows, then alternates
faster and faster until finally the last column is alternating every time. So I used
that knowledge to create the different combinations in an array
*/
void fillArr() {
    int mult = nComs / 2, counter;
    bool ones;
    for (int col = 0; col < i; col++) {
        counter = 0;
        ones = false;
        for (int coms = 0; coms < nComs; coms++) { //for different combinations of values of prop values
            if (counter == mult) {
                counter = 0;
                if (ones) ones = false;
                else ones = true;
            }
            if (ones) arr[coms][col] = 1;
            counter++;
        }
        mult /= 2;
    }
}

/*
I made the input for the equation very forgiving in that it only allows valid 
variable letters and operators through to be used
*/
void optimizEquation() {
    int it = 0, stInd = 0;
    while (eq[it] != '\0') {
        if (isalpha(eq[it])) {
            if ((eq[it]- 97) >= 0 && (eq[it] - 97) < i) {
                simEq[stInd] = eq[it];
                stInd++;
            }
            else printf("Nice try, but this letter is out of bounds\n");
        }
        else if (isdigit(eq[it])) {
            if ((eq[it] - '0') >= 0 && (eq[it] - '0') < 2) {
                simEq[stInd] = eq[it];
                stInd++;
            }
            else printf("Nice try, but this number is out of bounds\n");
        } 
        else if (eq[it] == '|' || eq[it] == '&' || eq[it] == '#' || eq[it] == '>' || eq[it] == '=' || eq[it] == '-') {
            simEq[stInd] = eq[it];
            stInd++;
        }
        else {
            printf("Hey, the \'%c\' character isn\'t allowed!\n", eq[it]);
        }
        it++; 
    }
}

int pickUpVariable(int index) {
    bool found = false;
    for (int f = index; f >= 0; f--) {
        if (!found && isdigit(stack[f])) {
            found = true; // stops the search for the next value down
            int this = stack[f] - '0';
            stack[f] = '*';
            return this;
        }
    }
    if(!found) {
        return -1;
    }
}

void printTopRows() {
    for (int d = 0; d < i; d++) { // print letters
        char letter = d + 97;
        printf("%c ", letter);
    }
    printf(": %s : Result\n", simEq); // print rest of top line
    int eqLen = i * 2 + strlen(simEq) + 11;
    for (int k = 0; k < eqLen; k++) { // print proportional equals line
        printf("=");
    }
    printf("\n");
}

/*
I use the stack array as a base with all the stack info then use the bottom and top integers
to perform the operations before putting the values back into the stack array.
For all intensive purposes, this array is called a stack despite not being directly aligned 
with the definition and the top and bottom are simply used to represent the two number being
used during an operation.
*/
void crackTheStack() { 
    printTopRows();
    int bottom, top; //integers to be operated on
    int it;
    char c;
    for (int row = 0; row < nComs; row++) { // for each row in truth table
        bottom = -1, top = -1, it = 0;
        stack = malloc(strlen(simEq) * sizeof(char)); //reset stack to original equation
        for (int b = 0; b < strlen(simEq); b++) {
            stack[b] = simEq[b];
        }
        for (int j = 0; j < i; j++) {
            printf("%d ", arr[row][j]); 
        }
        printf(": ");
        while ((c = stack[it]) != '\0') { // go through equation
            if (isalpha(c)) {
                printf(" ");
                if (top < 0) { // if top is empty, meaning stack is empty
                    top = arr[row][(c - 97)]; // if bottom of stack is empty
                    stack[it] = '*';
                }
                else if (top >= 0 && bottom < 0) { // if top is full but bottom is empty
                    bottom = top; // top moves to bottom
                    top = arr[row][(c - 97)]; // top gets new variable
                    stack[it] = '*';
                }
                else if (bottom >= 0 && top >= 0) { // if both are filled and another variable comes in
                    stack[it] = bottom + '0'; // puts bottom number back in equation
                    bottom = top; // top moves to bottom
                    top = arr[row][(c - 97)]; // top gets new variable
                }

            }
            if (isdigit(c)) { // when checking around again
                printf(" ");
                if (top < 0) { // if top is empty, meaning stack is empty
                    top = c - '0'; // if bottom of stack is empty
                    stack[it] = '*';
                }
                else if (top >= 0 && bottom < 0) { // if top is full but bottom is empty
                    bottom = top; // top moves to bottom
                    top = c - '0'; // top gets new variable
                    stack[it] = '*';
                }
                else if (bottom >= 0 && top >= 0) { // if both are filled and another variable comes in
                    stack[it] = bottom + '0'; // puts bottom number back in equation
                    bottom = top; // top moves to bottom
                    top = c - '0'; // top gets new variable
                }
            }
            else {
                switch(c) {
                    case '#' :
                        if (bottom < 0) {
                            bottom = pickUpVariable(it);
                        }
                        if (bottom >= 0 || top >= 0) { // if top in stack for operation
                            if ((bottom == 0 && top == 0) || (bottom == 1 && top == 1)) {
                                printf("0");
                                bottom = -1;
                                top = 0;
                            }
                            else {
                                printf("1");
                                bottom = -1;
                                top = 1;
                            }
                            stack[it] = '*';
                        }
                        else stack[it] = '*'; // else dismiss operation
                        break;
                    case '|' :
                        if (bottom < 0) {
                            bottom = pickUpVariable(it);
                        }
                        if (bottom >= 0 && top >= 0) { // if two variables in stack for operation
                            if (bottom == 1 || top == 1) {
                                printf("1");
                                bottom = -1;
                                top = 1;
                            }
                            else {
                                printf("0");
                                bottom = -1;
                                top = 0;
                            }
                            stack[it] = '*';
                        }
                        else stack[it] = '*'; // else dismiss operation
                        break;
                    case '&' :
                        if (bottom < 0) {
                            bottom = pickUpVariable(it);
                        }
                        if (bottom >= 0 && top >= 0) { // if two variables in stack for operation
                            if (bottom == 0 || top == 0) {
                                printf("0");
                                bottom = -1;
                                top = 0;
                            }
                            else {
                                printf("1");
                                bottom = -1;
                                top = 1;
                            }
                            stack[it] = '*';
                        }
                        else stack[it] = '*'; // else dismiss operation
                        break;
                    case '>' :
                        if (bottom < 0) {
                            bottom = pickUpVariable(it);
                        }
                        if (bottom >= 0 && top >= 0) { // if two variables in stack for operation
                            if (bottom == 1 && top == 0) {
                                printf("0");
                                bottom = -1;
                                top = 0;
                            }
                            else {
                                printf("1");
                                bottom = -1;
                                top = 1;
                            }
                            stack[it] = '*';
                        }
                        else stack[it] = '*'; // else dismiss operation
                        break;
                    case '=' :
                        if (bottom < 0) {
                            bottom = pickUpVariable(it);
                        }
                        if (bottom >= 0 && top >= 0) { // if two variables in stack for operation
                            if ((bottom == 0 && top == 0) || (bottom == 1 && top == 1)) {
                                printf("1");
                                bottom = -1;
                                top = 1;
                            }
                            else {
                                printf("0");
                                bottom = -1;
                                top = 0;
                            }
                            stack[it] = '*';
                        }
                        else stack[it] = '*'; // else dismiss operation
                        break;
                    case '-' :
                        if (top >= 0) { // if variable in stack for operation
                            if (top == 1) {
                                printf("0");
                                top = 0;
                            }
                            else {
                                printf("1");
                                top = 1;
                            }
                            stack[it] = '*';
                        }
                        else stack[it] = '*'; // else dismiss operation
                        break;
                }
            }
            it++;
        }
        printf("     %d\n", top);
    }   
}

void main(int argc, char** argv) {
    i = atoi (argv[1]);
    nComs = power(2, i);
    eq = argv[2];
    allotMem();
    fillArr();
    optimizEquation();
    crackTheStack();
    //free(arr);
    //free(eq);
    //free(stack);
}