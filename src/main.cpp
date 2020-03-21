#include <iostream>
#include <string>
#include <fstream>
#include "Line.h"

using namespace std;

void getInput(const string& filename) {
    ifstream file;
    cout<<"reading " << filename<<endl;
    file.open(filename);

    if(!file) handle_error("cannot open file: "+filename);
    char s;
    int num, x0, y0, x1, y1;

    if(file >> num) {
        cout<<"input " <<num<< " diagrams."<<endl;
    } else {
        handle_error("why not input a N?");
    }

    for (int i = 0; i < num; i++) {
        if (file >> s) {
            if (s == 'L' || s == 'R' || s == 'S') {
                if (file >> x0 >> y0 >> x1 >> y1) add_diagram(s, x0, y0, x1, y1);
            } else if (s == 'C') {
                if (file >> x0 >> y0 >> x1) add_diagram(s, x0, y0, x1, 0);
            } else {
                handle_error("line " + DoubleToString(i + 1) + " undefined type!");
            }
        } else {
            handle_error("need more lines");
        }
    }
}

int main(int argc, char **argv) {
    if (argc == 5) {
        ofstream out;
        if (argv[1][0] == '-' && argv[1][1] == 'i' && argv[3][0] == '-' && argv[3][1] == 'o') {
            getInput(argv[2]);
            out.open(argv[4]);
        } else if (argv[1][0] == '-' && argv[1][1] == 'o' && argv[3][0] == '-' && argv[3][1] == 'i') {
            getInput(argv[4]);
            out.open(argv[2]);
        } else handle_error("invalid cmd parameters!");
        out << point_map.size();
        out.close();
        return 0;
    }
    handle_error("please type right input!");
    return 0;
}