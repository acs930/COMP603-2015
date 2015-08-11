/*
= Brainfuck

If you have gcc:

----
g++ -o brainfuck.exe brainfuck.cpp
brainfuck.exe helloworld.bf
----
*/

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

/**
 * Primitive Brainfuck commands
 */
typedef enum { 
    INCREMENT, // +
    DECREMENT, // -
    SHIFT_LEFT, // <
    SHIFT_RIGHT, // >
    INPUT, // ,
    OUTPUT // .
} Command;

// Forward references. Silly C++!
class CommandNode;
class Loop;
class Program;

void printOutput();


//output vector (so visit commands can access it)
vector<int> output(1,0);
int outputIterator = 0;
int setupNewVect = 0;
int tempIterator = outputIterator;


/**
 * Visits?!? Well, that'd indicate visitors!
 * A visitor is an interface that allows you to walk through a tree and do stuff.
 */
class Visitor {
    public:
        virtual void visit(const CommandNode * leaf) = 0;
        virtual void visit(const Loop * loop) = 0;
        virtual void visit(const Program * program) = 0;
};

/**
 * The Node class (like a Java abstract class) accepts visitors, but since it's pure virtual, we can't use it directly.
 */
class Node {
    public:
        virtual void accept (Visitor *v) = 0;
};

/**
 * CommandNode publicly extends Node to accept visitors.
 * CommandNode represents a leaf node with a primitive Brainfuck command in it.
 */
class CommandNode : public Node {
    public:
        Command command;
        CommandNode(char c) {
            switch(c) {
                case '+': command = INCREMENT; break;
                case '-': command = DECREMENT; break;
                case '<': command = SHIFT_LEFT; break;
                case '>': command = SHIFT_RIGHT; break;
                case ',': command = INPUT; break;
                case '.': command = OUTPUT; break;
            }
        }
        CommandNode(char c, int count) {
            for(int i = 0; i < count; i++){
                switch(c) {
                    case '+': command = INCREMENT; break;
                    case '-': command = DECREMENT; break;
                    case '<': command = SHIFT_LEFT; break;
                    case '>': command = SHIFT_RIGHT; break;
                    case ',': command = INPUT; break;
                    case '.': command = OUTPUT; break;
                }
            }
        }
        void accept (Visitor * v) {
            v->visit(this);
        }
};

class Container: public Node {
    public:
        vector<Node*> children;
        virtual void accept (Visitor * v) = 0;
};

/**
 * Loop publicly extends Node to accept visitors.
 * Loop represents a loop in Brainfuck.
 */
class Loop : public Container {
    public:
        void accept (Visitor * v) {
            v->visit(this);
        }
};


/**
 * Program is the root of a Brainfuck program abstract syntax tree.
 * Because Brainfuck is so primitive, the parse tree is the abstract syntax tree.
 */
class Program : public Container {
    public:
        void accept (Visitor * v) {
            v->visit(this);
        }
};

void parseInnerLoop(fstream & file, Container * container, Loop* outerLoop);


/**
 * Read in the file by recursive descent.
 * Modify as necessary and add whatever functions you need to get things done.
 */
void parse(fstream & file, Container * container) {
    char c;
    int commandCount;
    // How to peek at the next character
    //c = (char)file.peek();

    if (file >> c){

    if(c == '['){
        Loop* ermergerdlerp = new Loop();

        //program->children.push_back(new Loop());


        while((char)file.peek() != ']')
        {
            if((char)file.peek() == '['){
                file >> c;
                parseInnerLoop(file, container, ermergerdlerp);
            }
            
            file >> c;
            //cout << c << endl;
            ermergerdlerp->children.push_back(new CommandNode(c));
            
        }
        file >> c;
        container->children.push_back(ermergerdlerp);
        //file >> c;
        //program->children.push_back(new Loop());

         //program->children.push_back(new CommandNode(c));
    }
    else{
                            cout << "stuck";

            if(c == (char)file.peek()){
                while(c == (char)file.peek())
                {
                    cout << "stuck";
                    file >> c;
                    commandCount++;
                }
                container->children.push_back(new CommandNode(c, commandCount));
            }
            else
                container->children.push_back(new CommandNode(c));
    }

    parse(file, container);
    }
    else
        return;
    

    // How to print out that character
    //cout << c;
    // How to read a character from the file and advance to the next character
    //file >> c;
    // How to print out that character
    //cout << c;
    // How to insert a node into the program.
    //program->children.push_back(new CommandNode(c));

    //cout << c;
    // How to insert a node into the container.
    //container->children.push_back(new CommandNode(c));

}



/**
 * A printer for Brainfuck abstract syntax trees.
 * As a visitor, it will just print out the commands as is.
 * For Loops and the root Program node, it walks trough all the children.
 */
class Printer : public Visitor {
    public:
        void visit(const CommandNode * leaf) {
            switch (leaf->command) {
                case INCREMENT:   cout << '+'; break;
                case DECREMENT:   cout << '-'; break;
                case SHIFT_LEFT:  cout << '<'; break;
                case SHIFT_RIGHT: cout << '>'; break;
                case INPUT:       cout << ','; break;
                case OUTPUT:      cout << '.'; break;
            }
        }
        void visit(const Loop * loop) {
            //cout << '[';
            for (vector<Node*>::const_iterator it = loop->children.begin(); it != loop->children.end(); ++it) {
                (*it)->accept(this);
            }
            //cout << ']';
        }
        void visit(const Program * program) {
            for (vector<Node*>::const_iterator it = program->children.begin(); it != program->children.end(); ++it) {
                (*it)->accept(this);
            }
            //cout << '\n';
        }
};

class Interpreter : public Visitor {
    public:
        void visit(const CommandNode * leaf) {
            //int temp2 = outputIterator;
            switch (leaf->command) {
                case INCREMENT:
                    output.at(outputIterator)++;
                    //cout << "plus|";
                    break;
                case DECREMENT:
                    output.at(outputIterator)--;
                    //cout << "IT: " << outputIterator << " val: " <<output.at(outputIterator) << endl;
                    //cout << "heoufsdsde";
                    break;
                case SHIFT_LEFT:
                    //int temp = outputIterator;
                   // cout << outputIterator << " -> ";
                    if(outputIterator <= 0){
                        //cout << "yo";
                        return;
                    }
                    else
                        outputIterator--;
                        //cout << "IT: " << outputIterator << " val: " <<output.at(outputIterator) << endl;
                        //cout << "left" << endl;

                   //cout << "itL: " << outputIterator << endl;

                    break;
                case SHIFT_RIGHT:
                   // cout << outputIterator << " -> ";
                    
                    //cout << output.size() << "|";

                    if(outputIterator+1 >= output.size()){
                        //cout << output.at(outputIterator) << "amhere|";
                        output.push_back(0);
                    }

                    outputIterator++;
                  //  outputIterator++;
                   // cout << "itR: " << outputIterator << endl;

                    //cout << outputIterator << "|";
                    //cout << output.at(outputIterator) << "|";
                    break;
                case INPUT:
                    //cout << "in";
                    break;
                case OUTPUT:
                    //cout << "Iterator: " << outputIterator << " Val: " << (char)(output.at(outputIterator)) << endl;
                    cout << (char)output.at(outputIterator);
                    break;
            }
        }
        void visit(const Loop * loop) {
            //cout <<"enter" <<endl;
            tempIterator = outputIterator;
            //cout << tempIterator << endl;
            //cout << "begin: " << output.at(tempIterator) << endl;
            while(output.at(tempIterator) != 0){
                //cout << tempIterator << endl;
                //cout << output.at(tempIterator) << endl;
                //cout <<outputIterator << endl;
                //cout << output.at(tempIterator) << endl;
                //tempIterator = outputIterator;
                for (vector<Node*>::const_iterator it = loop->children.begin(); it != loop->children.end(); ++it) {
                    (*it)->accept(this);
                }
                //cout << tempIterator <<endl;
                //cout << "op: " << output.at(tempIterator) << endl;
                tempIterator = outputIterator;
            }
            //cout <<"exit" <<endl;
        }
        void visit(const Program * program) {
            for (vector<Node*>::const_iterator it = program->children.begin(); it != program->children.end(); ++it) {
                (*it)->accept(this);
            }
        }
};

//first inner loop should run 4 times


int main(int argc, char *argv[]) {
    fstream file;
    Program program;
    Printer printer;
    Interpreter interpreter;
    

//optimization
    if (argc == 1) {
        cout << argv[0] << ": No input files." << endl;
    } else if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            file.open(argv[i], fstream::in);
            parse(file, & program);
            //program.accept(&printer);
            program.accept(&interpreter);
            file.close();
        }
    }
    //printOutput();
}




void printOutput()
{
    for (int i =0; i < output.size(); i++)
    {
        cout << output.at(i);
    }
}
void parseInnerLoop(fstream & file, Container * container, Loop* outerLoop)
{
    char c;
    int commandCount = 0;
    Loop* ermergerdirnnerlerp = new Loop();
    while((char)file.peek() != ']')
        {
            if((char)file.peek() == '['){
                file >> c;
                parseInnerLoop(file, container, ermergerdirnnerlerp);
            }
            else{
                file >> c;
                //cout << c << endl;
                if(c == (char)file.peek()){
                    while(c == (char)file.peek())
                    {
                        file >> c;
                        cout << "stuck";
                        commandCount++;
                    }
                    ermergerdirnnerlerp->children.push_back(new CommandNode(c, commandCount));
                }
                else
                    ermergerdirnnerlerp->children.push_back(new CommandNode(c));

            }
        }
    file >> c;
    outerLoop->children.push_back(ermergerdirnnerlerp);
    return;
}
