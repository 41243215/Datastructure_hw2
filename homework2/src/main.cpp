#include "Polynomial.h"

int main(){
    Polynomial a, b, c;

    cout << "請輸入a多項式的項數，再依序輸入各項係數與指數：\n";

    if(!(cin >> a)){
        cout << "輸入格式錯誤\n";
        return 1;
    }
    cout << "請輸入b多項式的項數，再依序輸入各項係數與指數：\n";
    if(!(cin >> b)){
        cout << "輸入格式錯誤\n";
        return 1;
    }
    
    char type;
    bool keepRun = true;

    cout << "輸入'1'列出多項式\n" << "輸入'+'相加多項式\n" << "輸入'*'相乘多項式\n";
    if(!(cin >> type)){
        cout << "無法讀取選項\n";
        return 1;
    }

    while(keepRun){
        switch (type){
            case '1':
                cout << "Polynomial a : " << a << "\n";
                cout << "Polynomial b : " << b << "\n";
                keepRun = false;
                break;
            case '+':
                c = a.Add(b);
                cout << "(" << a << ")" << " + " << "(" << b << ")" << " = " << c << "\n";
                type = 'x';
                break;
            case '*':
                c = a.Mult(b);
                cout << "(" << a << ")" << " * " << "(" << b << ")" << " = " << c << "\n";
                type = 'x';
                break;
            case 'x':
                float xx;
                cout << "輸入 x = ";
                if(!(cin >> xx)){
                    keepRun = false;
                    break;
                }
                else{
                    cout << "x = " << xx << ", answer = " << c.Eval(xx) << "\n";
                    keepRun = false;
                    break;
                }
            default:
                cout << "請輸入 1 / + / * / x 來選擇要如何實作Polynomial" << "\n";
                keepRun = false;
                break;
            
        }
    }
    
    return 0;
}
