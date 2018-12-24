#ifndef READERROREXCEPTION_H
#define READERROREXCEPTION_H
#include<exception>
#include<iostream>

class ReadErrorException : public std::exception{

public:
    ReadErrorException():std::exception(){

    }

};

#endif // READERROREXCEPTION_H
