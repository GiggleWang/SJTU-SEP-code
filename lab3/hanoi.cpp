#include <iostream>
#include <string>
#include <climits>

#include "board.h"

using namespace std;

int main() {
    while (true) {
        cout << "How many disks do you want? (1 ~ 5)" << endl;
        string input;
        getline(cin, input);
        if (input == "Q") {
            break;
        }
        // TODO
        if(input[1]!='\0'||input[0]<'1'||input[0]>'5')
            continue;
        int numOfDisk=input[0]-'0';
        Board m(numOfDisk);
        m.draw();

        while (true){
            cout<<"Move a disk. Format: x y"<<endl;
            string input1;
            getline(cin,input1);
            bool flag=true;
            if(input1[0]<'1'||input1[0]>'3')
                flag= false;
            if(input1[2]<'1'||input1[2]>'3')
                flag= false;
            if(input1[0]=='0'&&input1[2]=='0')
                flag=true;
            if(input1[1]!=' ')
                flag= false;
            if(input1[3]!='\0')
                flag= false;

            if(flag== false)
            {
                m.draw();
                continue;
            }

            int from,to;
            from=input1[0]-'0';
            to=input1[2]-'0';
            if(from==0&&to==0)
                m.autoplay();
            else
            {
                m.move(from,to);
            }
            bool winOrNot=m.win();
            if(winOrNot==true)
            {
                cout<<"Congratulations! You win!"<<endl;
                break;
            }
        }

    }
    return 0;
}
