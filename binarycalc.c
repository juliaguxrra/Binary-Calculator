#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_DIGITS 8
#define MAX_FORMULA_LEN (1 + MAX_DIGITS + 3 + 1 + MAX_DIGITS)

// Checks if any char in the formula is invalid
int checkformula(char formula[], int *val1length, int *val2length){
    int operatorcount = 0;
    for(int i = 0; formula[i] != '\0'; i++){
        if(formula[i] != '1' && formula[i] != '0' && formula[i] != '+' && formula[i] != '-' && formula[i] != '&' && formula[i] != '|'&& formula[i] != '^'){
        return 1;
        }
        // Counts the number of operators in the formula
        if(formula[i] == '+' || formula[i] == '-' || formula[i] == '|' || formula[i] == '&'|| formula[i] == '^'){
            operatorcount++;
        }
        else{
            // Counts the length of the two individual values
            if(operatorcount == 0){
                (*val1length)++;
            }
            else{
                (*val2length)++;
            }
        }
    }
    // Checks if there is only 1 operator in the formula
    if(operatorcount != 1){
        return 1;
    }
    return 0;
}

void separatevals(char formula[], char *val1, char* val2, int *val1length, int *val2length, char *operator){
    int operatorcount = 0;
    for(int i = 0; i < strlen(formula); i++){
        if(formula[i] == '+' || formula[i] == '-' || formula[i] == '|' || formula[i] == '&'|| formula[i] == '^'){
            *operator = formula[i];
            operatorcount++;
        }
        else{
            if(operatorcount == 0){
                val1[MAX_DIGITS - *val1length] = formula[i];
                (*val1length)--;
            }else{
                val2[MAX_DIGITS - *val2length] = formula[i];
                (*val2length)--;
            }
        }
    }
}

// Adds together the two values and creates the result
void addition(char val1[], char val2[], int carryin, int *overflow, char *result){
    for(int i = strlen(val1) -1; i >= 0; i--){
        if(!carryin && val1[i] == '0' && val2[i] == '0'){
            result[i] = '0';
        }else if((carryin && val1[i] == '0' && val2[i] == '0') || (!carryin && val1[i] == '1' && val2[i] == '0') || (!carryin && val1[i] == '0' && val2[i] == '1')){
            result[i] = '1';
            carryin = 0;
        }else if((carryin && val1[i] == '1' && val2[i] == '0') || (carryin && val1[i] == '0' && val2[i] == '1') || (!carryin && val1[i] == '1' && val2[i] == '1')){
            result[i] = '0';
            carryin = 1;
        }else{
            result[i] = '1';
            carryin = 1;
        }
    }
    if((val1[0] == val2[0]) && (val1[0] != result[0])){
        *overflow = 1;
    }
}

// Swaps all bits
char* negate(char val[]){
    for(int i = 0; i < strlen(val); i++){
        if(val[i] == '0'){
            val[i] = '1';
        }else{
            val[i] = '0';
        }
    }
    return val;
}

// Subtracting val2 from val1 by negating val2 and adding them
void subtraction(char val1[], char val2[], int *overflow, char *result){
    val2 = negate(val2);
    addition(val1, val2, 1, overflow, result);
    val2 = negate(val2); // Negating it back to its original value
}

//if at least one bit is 1 the result will set to 1 
void bitwiseOr(char val1[], char val2[], char *result){
    for (int i= 0; i<MAX_DIGITS; i++) {
        if(val1[i]=='1'||val2[i]=='1'){
            result[i] ='1';
        } 
        else{
            result[i]='0';
        }
    }
}

//same as || just changed operator for if statement 
void bitwiseAND(char val1[], char val2[], char *result){
    for(int i = 0; i<MAX_DIGITS; i++) {
        if (val1[i]== '1'&&val2[i]== '1'){
            result[i]= '1';
        } 
        else{
            result[i]='0';
        }
    }
}

// if bits are different the result will set to 1 / if they're the same then the result is 0
void bitwiseXor(char val1[], char val2[], char *result){
    for(int i= 0; i< MAX_DIGITS; i++) {
        if((val1[i]== '0' && val2[i]== '1')||(val1[i]=='1' && val2[i]=='0')){
            result[i]= '1';
        } 
        else{
            result[i]='0';
        }
    }
}

int main(int argc, char *argv[]) {    
    if (argc != 2) {
        printf("Provide a single, valid binary formula\n");
        return 1;
    }
    if (strlen(argv[1]) > MAX_FORMULA_LEN) {
        printf("Invalid binary formula\n");
        return 1;
    }
    // Copy formula from command line into formula_str variable
    char formula_str[MAX_FORMULA_LEN + 1];
    strcpy(formula_str, argv[1]);
    
    // Stores the operator as a char for the extended formula
    char operator;
    int overflow = 0;
    int value1length = 0;
    int value2length = 0;
    // Generates two 8-bit strings full of zeroes
    char value1[MAX_DIGITS + 1];
    char value2[MAX_DIGITS + 1];
    char result[MAX_DIGITS + 1];
    memset(value1, '0', MAX_DIGITS); // Sets all the initial chars to '0' - found on Stack Overflow
    memset(value2, '0', MAX_DIGITS);
    value1[MAX_DIGITS] = '\0';
    value2[MAX_DIGITS] = '\0';
    result[MAX_DIGITS] = '\0';

    // Checking if any char in the formula is invalid
    if(checkformula(formula_str, &value1length, &value2length) == 1){
        printf("Invalid binary formula\n");
        return 1;
    }

    // Checks if either value is greater than the max digits allowed or less than 1
    if(value1length > MAX_DIGITS || value2length > MAX_DIGITS || value1length < 1 || value2length < 1){
        printf("Invalid binary formula\n");
        return 1;
    }

    separatevals(formula_str, value1, value2, &value1length, &value2length, &operator);

    if(operator== '+'){
        addition(value1, value2, 0, &overflow, result);
    } 
    else if(operator== '-'){
        subtraction(value1, value2, &overflow, result);
    }
    else if(operator== '|'){
        bitwiseOr(value1, value2, result);
    } 
    else if(operator== '&'){
        bitwiseAND(value1, value2, result);
    }
    else if(operator== '^'){
        bitwiseXor(value1, value2, result);
    } 
    else{
        printf("Invalid binary formula\n");
        return 1;
    }
    printf("%s%c%s\n", value1, operator, value2);
    printf("%s\n", result);
    if (overflow) {
        printf("Overflow\n");
    }
    return 0;
}