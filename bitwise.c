#include <stdio.h>

int num1;   //initialize variable to be returned by prefix
//Function to shift first n bits of number x
int prefix(unsigned int n, unsigned int x){
    //Initialize input variable
    num1 = x;
    //Starting point
    int multiple = 1;
    int length = 0;

    //Calculate how many bits to shift
    while (multiple < x){
        multiple *= 2;
        length++;
    }

    //Shift bits and return
    num1 = num1 >> (length - n);
    return num1;
}

int num2;   //initialize number to be returned by suffix
//Function to shift last n bits of number x
int suffix(unsigned int x, unsigned int n){
    int multiple = 1;   //initialize multiple variable
    int i = 0;  //initialize loop variable

    //Calculate bit operation
    for (i; i< n; i++){
        multiple *= 2;
    }
    multiple -= 1;

    //Perform bit operation and return
    num2 = (x & multiple);
    return num2;
}


int newNum; //initialize toggled variable
//Function to toggle the nth bit of number x
int toggle(unsigned int n, unsigned int x){
    //Toggle number and return it
    newNum = x ^ (1 << n);
    return newNum;
}

void binaryConverter(int x){
    //Convert decimal number to binary
    long long bin = 0;
    int rem, i = 1;
    while (x != 0){
        rem = x % 2;
        x /= 2;
        bin += rem * i;
        i *= 10;
    }
    //Print number in binary
    printf("Output number in binary: %lld\n", bin);
}

int main(){

    //Call prefix
    int pre = prefix(3, 14);
    //Print prefix number in decimal
    printf("First n bits of x in decimal: %d\n", num1);
    //Convert to binary
    binaryConverter(pre);

    //Call suffix
    int suf = suffix(14, 3);
    //Print suffix number in decimal
    printf("Last n bits of x in decimal: %d\n", num2);
    //Convert to binary
    binaryConverter(suf);

    //Call toggle function
    int tog = toggle(2, 10);
    //Print toggled number in decimal
    printf("Toggled number in decimal: %d\n", newNum);
    //Convert to binary
    binaryConverter(tog);

}