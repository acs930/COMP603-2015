class Visitor {
    public:
        virtual void visit(const CommandNode * leaf) = 0;
        virtual void visit(const Loop * loop) = 0;
        virtual void visit(const Program * program) = 0;
};

class Printer : public Visitor {
    public:
        void visit(const CommandNode * leaf) {
            switch (leaf->command) {
                case INCREMENT:   cout << "++*ptr;\n";break;//'+'; break;
                case DECREMENT:   cout << "--*ptr;\n";break;//'-'; break;
                case SHIFT_LEFT:  cout << "--ptr;\n";break;//'<'; break;
                case SHIFT_RIGHT: cout << "++ptr;\n";break;//'>'; break;
                case INPUT:       cout << "*ptr=getchar();\n"; break;//','; break;
                case OUTPUT:      cout << "putchar(*ptr);\n";break;//'.'; break;
            }
        }
        void visit(const Loop * loop) {
            cout << '[';
                cout << "while (*ptr) {";
            for (vector<Node*>::const_iterator it = loop->children.begin(); it != loop->children.end(); ++it) {
                (*it)->accept(this);
            }
            cout << "}";
            //cout << ']';
        }
        void visit(const Program * program) {
            cout << "#include <stdio.h>\n";
            cout << "char array[30000] = {0};";
            cout << "char *ptr=array;\n";
            cout << "int main(int argc, char **argv){\n";

            for (vector<Node*>::const_iterator it = program->children.begin(); it != program->children.end(); ++it) {
                (*it)->accept(this);
            }

            cout << "}\n";
        }
};