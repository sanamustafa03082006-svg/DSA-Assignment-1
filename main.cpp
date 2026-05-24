#include <iostream>
#include <string>
using namespace std;

class StackChar
{
    char* arr;
    int top;
    int capacity;

public:

    StackChar(int size = 100)
    {
        capacity = size;
        arr = new char[capacity];
        top = -1;
    }

    void push(char value)
    {
        arr[++top] = value;
    }

    void pop()
    {
        if (top != -1)
            top--;
    }

    char Top()
    {
        return arr[top];
    }

    bool isEmpty()
    {
        return top == -1;
    }
};

class StackInt
{
    int* arr;
    int top;
    int capacity;

public:

    StackInt(int size = 100)
    {
        capacity = size;
        arr = new int[capacity];
        top = -1;
    }

    void push(int value)
    {
        arr[++top] = value;
    }

    void pop()
    {
        if (top != -1)
            top--;
    }

    int Top()
    {
        return arr[top];
    }

    bool isEmpty()
    {
        return top == -1;
    }

    int size()
    {
        return top + 1;
    }
};

bool isDigit(char ch)
{
    return ch >= '0' && ch <= '9';
}

bool isLetter(char ch)
{
    return (ch >= 'a' && ch <= 'z') ||
        (ch >= 'A' && ch <= 'Z');
}

bool isOperator(char ch)
{
    return ch == '+' ||
        ch == '-' ||
        ch == '*' ||
        ch == '/';
}

bool isOpening(char ch)
{
    return ch == '(' ||
        ch == '[' ||
        ch == '{';
}

bool isClosing(char ch)
{
    return ch == ')' ||
        ch == ']' ||
        ch == '}';
}

bool matching(char open, char close)
{
    return (open == '(' && close == ')') ||
        (open == '[' && close == ']') ||
        (open == '{' && close == '}');
}

int precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;

    if (op == '*' || op == '/')
        return 2;

    return 0;
}

int stringToInt(string str)
{
    int num = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        num = num * 10 + (str[i] - '0');
    }

    return num;
}
// converting infix expression to postfix
string infixToPostfix(string exp)
{
    StackChar st;

    string postfix = "";

    bool lastWasOperator = true;
    bool lastWasOpening = false;

    for (int i = 0; exp[i] != '\0'; i++)
    {
        char ch = exp[i];

        if (ch == ' ')
            continue;

        
        if (isDigit(ch))
        {
            while (isDigit(exp[i]))
            {
                postfix += exp[i];
                i++;
            }

            postfix += ' ';
            i--;

            lastWasOperator = false;
            lastWasOpening = false;
        }

        
        else if (isLetter(ch) || ch == '_')
        {
            while (isLetter(exp[i]) ||
                isDigit(exp[i]) ||
                exp[i] == '_')
            {
                postfix += exp[i];
                i++;
            }

            postfix += ' ';
            i--;

            lastWasOperator = false;
            lastWasOpening = false;
        }

        
        else if (isOpening(ch))
        {
            st.push(ch);

            lastWasOperator = true;
            lastWasOpening = true;
        }


        else if (isClosing(ch))
        {
            if (lastWasOpening)
            {
                cerr << "Syntax Error\n";
                exit(1);
            }

            while (!st.isEmpty() &&
                !isOpening(st.Top()))
            {
                postfix += st.Top();
                postfix += ' ';
                st.pop();
            }

            if (st.isEmpty())
            {
                cerr << "Syntax Error\n";
                exit(1);
            }

            if (!matching(st.Top(), ch))
            {
                cerr << "Logical Error\n";
                exit(3);
            }

            st.pop();

            lastWasOperator = false;
            lastWasOpening = false;
        }

        
        else if (isOperator(ch))
        {
            if (lastWasOperator)
            {
                cerr << "Syntax Error\n";
                exit(1);
            }

            while (!st.isEmpty() &&
                precedence(st.Top()) >= precedence(ch))
            {
                if (isOpening(st.Top()))
                    break;

                postfix += st.Top();
                postfix += ' ';
                st.pop();
            }

            st.push(ch);

            lastWasOperator = true;
            lastWasOpening = false;
        }

        else
        {
            cerr << "Syntax Error\n";
            exit(1);
        }
    }

    if (lastWasOperator)
    {
        cerr << "Syntax Error\n";
        exit(1);
    }

    while (!st.isEmpty())
    {
        if (isOpening(st.Top()))
        {
            cerr << "Logical Error\n";
            exit(3);
        }

        postfix += st.Top();
        postfix += ' ';
        st.pop();
    }

    return postfix;
}

int getVariableValue(string varNames[],
    int values[],
    int& count,
    string variable)
{
    for (int i = 0; i < count; i++)
    {
        if (varNames[i] == variable)
            return values[i];
    }

    int value;

    cerr << "Enter value for "
        << variable << ": ";

    cin >> value;

    varNames[count] = variable;
    values[count] = value;

    count++;

    return value;
}

int evaluatePostfix(string postfix)
{
    StackInt st;

    string varNames[100];
    int values[100];
    int count = 0;

    for (int i = 0; postfix[i] != '\0'; i++)
    {
        char ch = postfix[i];

        if (ch == ' ')
            continue;

        // Number
        if (isDigit(ch))
        {
            string num = "";

            while (isDigit(postfix[i]))
            {
                num += postfix[i];
                i++;
            }

            st.push(stringToInt(num));

            i--;
        }

        // Variable
        else if (isLetter(ch) || ch == '_')
        {
            string var = "";

            while (isLetter(postfix[i]) ||
                isDigit(postfix[i]) ||
                postfix[i] == '_')
            {
                var += postfix[i];
                i++;
            }

            int value = getVariableValue(
                varNames,
                values,
                count,
                var
            );

            st.push(value);

            i--;
        }

        // Operator
        else if (isOperator(ch))
        {
            if (st.size() < 2)
            {
                cerr << "Syntax Error\n";
                exit(1);
            }

            int b = st.Top();
            st.pop();

            int a = st.Top();
            st.pop();

            int result = 0;

            if (ch == '+')
                result = a + b;

            else if (ch == '-')
                result = a - b;

            else if (ch == '*')
                result = a * b;

            else if (ch == '/')
            {
                if (b == 0)
                {
                    cerr << "Runtime Error\n";
                    exit(2);
                }

                result = a / b;
            }

            st.push(result);
        }
    }

    if (st.size() != 1)
    {
        cerr << "Syntax Error\n";
        exit(1);
    }

    return st.Top();
}

int main()
{
    string expression;

    getline(cin, expression);

    string postfix =
        infixToPostfix(expression);

    cerr << "Postfix: "
        << postfix << endl;

    int result =
        evaluatePostfix(postfix);

    cout << result;

    return 0;
}
