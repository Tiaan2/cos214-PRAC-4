#include <iostream>
#include <cstdlib>
#include <string>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h> 


using namespace std;

int nonceGenerator() {
    return (rand() % 100);
}

struct Token {
    int value;
    string privatekey;
};

string reverse(string s) {
    string rev = "";
    for (int i = s.length() - 1; i >= 0; i--) {
        rev += s.at(i);
    }
    return rev; 
}

struct User {
    string userName;
    Token token;
    string key;
    int nonceValue;
    int requestNumber;
    bool authenticated;
};

bool validation(User* user) {
    cout << "Please enter your nonce value: ";
    int val;
    cin >> val;

    cout << "Please enter your private key: ";
    string k;
    cin >> k;

    if (val == user->nonceValue && k == user->key)
        return true;
    else {
        cout << "Validation failed, please try again!" << endl;
        return false; 
    }
}

class Handler {
public:
    Handler() {
        next = nullptr;
    };

    void setNext(Handler* n) {
        next = n;
    };

    virtual void handleRequest(User* user) = 0;

protected:
    Handler* next;
};

class NonceHandler : public Handler {
public:
    void handleRequest(User* user) {

        //! Nonce Value was already created so pass on to next handler
        if (user->nonceValue != -1)
            next->handleRequest(user);
        else {
        //! Generate a nonce value and pass to next handler
            user->nonceValue = nonceGenerator();
            cout << "Nonce value generated and persisting..." << endl;
            cout << "Your nonce value is: " << user->nonceValue << endl;
            next->handleRequest(user);
        }
    }
};

class ValidateHandler : public Handler {
public:
    void handleRequest(User* user) {
        if (user->nonceValue != -1) {
            bool validated = false;
            while (!validated) {
                validated = validation(user);
            }

            string reversed = reverse(user->key);
            cout << "This is the token value: " << reversed << " and id: " << user->nonceValue << endl;

            user->token.privatekey = reversed;
            user->token.value = user->nonceValue;

            next->handleRequest(user);
        } else {
            next->handleRequest(user);
        }
    }
};

bool checkToken(User* user) {
    string rev = reverse(user->key);
    if (user->token.privatekey == rev && user->token.value == user->nonceValue)
        return true;
    return false;
}

class TokenHandler : public Handler {
public:
    void handleRequest(User* user) {
        cout << "==== Doing Token validation ====\n";
        if (checkToken(user)) {
            user->authenticated = true;
            next->handleRequest(user);
        } else 
            cout << "Token is not valid!\n";
    }
};

class RequestHandler : public Handler {
public:
    void handleRequest(User* user) {
        cout << "Handling your Request...\n";
        if (user->requestNumber == 1) {
            cout << "You have 0.001 bitcoin\n";
        } else {
            cout << "You have 123.2877 dogecoin\n";
        }
    }
};

//! =================== Testing of the handlers ===================
int main () {
    //* Create a user
    User* newUser = new User;
    newUser->nonceValue = -1;
    
    //* Create handlers
    NonceHandler* nonce = new NonceHandler();
    ValidateHandler* validate = new ValidateHandler();
    TokenHandler* token = new TokenHandler();
    RequestHandler* processRequest = new RequestHandler();

    nonce->setNext(validate);
    validate->setNext(token);
    token->setNext(processRequest);

    //* Take a user name
    string name;
    cout << "Please enter your name: ";
    cin >> name;
    newUser->userName = name;

    //* NonceHandler checks if a nonce is already created or not

    //* Ask for private key
    cout << "Please enter your private key: ";
    string privatekey;
    cin >> privatekey;
    newUser->key = privatekey; 

    //* Output what they want to request
    cout << "========== Request Menu ==========" << endl;
    cout << "1. Print bitcoin\n";
    cout << "2. Print dogecoin\n";
    int requestNo;
    cout << "Please select 1 or 2 for your request: "; 
    cin >> requestNo;
    newUser->requestNumber = requestNo;

    //* Handle chain
    nonce->handleRequest(newUser);

    return 0;
}