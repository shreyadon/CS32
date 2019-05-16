//
//  main.cpp
//  32Homework2.5
//
//  Created by Shreya Donepudi on 05/02/17.
//  Copyright © 2017 Brute. All rights reserved.
//

#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;

int evaluate(string infix, const bool values[], string& postfix, bool& result);
// Evaluates a boolean expression
// Postcondition: If infix is a syntactically valid infix boolean
//   expression, then postfix is set to the postfix form of that
//   expression, result is set to the value of the expression (where
//   in that expression, each digit k represents element k of the
//   values array), and the function returns zero.  If infix is not a
//   syntactically valid expression, the function returns 1.  (In that
//   case, postfix may or may not be changed, but result must be
//   unchanged.)

#include <stack>//lines you need


bool evalPrecedence(char c1, char c2) // ! > & > |
{
    if (c2 == '!')
        return true;
    if (c2 == '&')
    {
        if(c1 == '!')
            return false;
        return true;
    }
    if (c2 == '|' && c1 == '|')
        return true;
    return false;
}

string deleteSpaces(string infix)   // Returns the temporary infix string with no spaces and all the characters of infix
{
    string infix_nospaces = "";
    for (int i = 0; i < infix.size(); i++)
        if (infix[i] != ' ')
            infix_nospaces += infix[i];
    return infix_nospaces;
}

bool isValid(string infix)
{
    
    string temp_infix = deleteSpaces(infix);
    if (temp_infix[temp_infix.size() - 1] == '|' || temp_infix[temp_infix.size() - 1] == '&') //  if the last char is these operators than return false
        return false;
    int nNumbers = 0;
    int nOpenPars = 0;
    for (int i = 0; i < temp_infix.size(); i++)
    {
        if (isdigit(temp_infix[i])) // count the numbers in the statement
            nNumbers++;
    }
    if (nNumbers == 0) // is there are no numbers in the statement return false
        return false;
    for (int i = 0; i < temp_infix.size(); i++) // Loop through the infix without spaces
    {
        if (temp_infix[i] == '(') // count the number of open paras
            nOpenPars++;
        if (temp_infix[i] == ')') // if you encounter a closed para reduce the open para count
            nOpenPars--;
        if (temp_infix[i] == '|' || temp_infix[i] == '&')
        {
            if ( (!isdigit(temp_infix[i-1]) && temp_infix[i-1] != ')') || (!isdigit(temp_infix[i+1]) &&  temp_infix[i+1] !='!' && temp_infix[i+1] !='('))
                return false;
        }
        else if (isdigit(temp_infix[i]))
        {
            if (isdigit(temp_infix[i+1]) || temp_infix[i+1] == '(' || temp_infix[i+1] == '!')
                return false;
        }
    }
    if (nOpenPars != 0)
        return false;
    return true;
}

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
    if (!isValid(infix))
        return 1;
    infix = deleteSpaces(infix);
    // the infix to postfix conversion step //
    postfix = ""; // Initialize postfix to empty
    stack<char> operatorStack; // Initialize the operator stack to empty
    for (int i = 0; i < infix.size(); i++) // For each character ch in the infix string
    {
        char ch = infix[i];
        switch (ch)
        {
            case '(':
            {
                operatorStack.push(ch);
                break;
            }
            case ')': // pop stack until matching '('
            {
                while(operatorStack.top() != '(') // While stack top is not '('
                {
                    postfix += operatorStack.top(); // append the stack top to postfix
                    operatorStack.pop(); // pop the stack
                }
                operatorStack.pop(); // remove the '('
                break;
            }
            case '!':
            case '&':
            case '|':
            {
                while (!operatorStack.empty() && operatorStack.top() != '(' && evalPrecedence(ch, operatorStack.top())) //While the stack is not empty and the stack top is not '(' and precedence of ch <= precedence of stack top
                {
                    postfix += operatorStack.top(); // append the stack top to postfix
                    operatorStack.pop(); // pop the stack
                }
                operatorStack.push(ch); // push ch onto the stack
                break;
            }
            default: //case operand
            {
                postfix += ch;//append ch to end of postfix
                break;
            }
        }
    }
    while(!operatorStack.empty()) // While the stack is not empty
    {
        postfix += operatorStack.top(); // append the stack top to postfix
        operatorStack.pop(); // pop the stack
    }
    // the evaluation of the postfix expression //
    stack<bool> operandStack;//Initialize the operand stack to empty
    for(int i = 0; i < postfix.size(); i++) //For each character ch in the postfix string
    {
        char ch = postfix[i];
        if (isdigit(ch))
        {
            int x = ch - '0';
            operandStack.push(values[x]);
        }
        else //(ch == '!' || ch == '&' || ch == '|')  ch is a binary operator
        {
            bool operand2 = operandStack.top(); //set operand2 to the top of the operand stack
            operandStack.pop(); //pop the stack
            if (ch == '!')
            {
                operandStack.push(!operand2);
            }
            else
            {
                bool operand1 = operandStack.top(); //set operand1 to the top of the operand stack
                operandStack.pop(); //pop the stack
                if(ch == '&')
                {
                    operandStack.push(operand1 && operand2); //push the result onto the stack
                }
                else //(ch == '|')
                {
                    operandStack.push(operand1 || operand2); //push the result onto the stack
                }
            }
        }
    }
    result = operandStack.top(); // When the loop is finished, the operand stack will contain one item, the result of evaluating the expression
    return 0;
}

int main()
{
    bool ba[10] = {
        //  0      1      2      3      4      5      6      7      8      9
        true,  true,  true,  false, false, false, true,  false, true,  false
    };
    string pf;
    bool answer;
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  answer);
    assert(evaluate("8|", ba, pf, answer) == 1);
    assert(evaluate("4 5", ba, pf, answer) == 1);
    assert(evaluate("01", ba, pf, answer) == 1);
    assert(evaluate("()", ba, pf, answer) == 1);
    assert(evaluate("2(9|8)", ba, pf, answer) == 1);
    assert(evaluate("2(&8)", ba, pf, answer) == 1);
    assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
    assert(evaluate("", ba, pf, answer) == 1);
    assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
           &&  pf == "43!03&&|"  &&  !answer);
    assert(evaluate(" 9  ", ba, pf, answer) == 0  &&  pf == "9"  &&  !answer);
    ba[2] = false;
    ba[9] = true;
    assert(evaluate("((9))", ba, pf, answer) == 0  &&  pf == "9"  &&  answer);
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  !answer);
    cout << "Passed all tests" << endl;
}
