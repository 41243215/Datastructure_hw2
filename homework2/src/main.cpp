#include "Polynomial.h"

int main(){
    Polynomial a;

    cout << "請輸入項數，再依序輸入各項係數與指數：\n";

    if(!(cin >> a)){
        cout << "輸入格式錯誤\n";
        return 1;
    }
    cout << "Polynomial: " << a << "\n";

    return 0;
}
