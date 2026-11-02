# 41243215

作業二 Polynomial

## 解題說明

本次作業要求依照課本與投影片的 Figure 1、Figure 2 實作 `Polynomial` 類別，並多載 `>>`、`<<`，讓多項式可以直接用 `cin` 輸入、`cout` 輸出。

程式需要完成下列功能：

1. 建立初始值為 0 的多項式。
2. 計算兩個多項式的加法與乘法。
3. 將數值代入多項式，計算結果。
4. 輸入與輸出多項式。

### 資料表示方式

我使用課本的 `Term` 類別儲存一項，每一項包含係數 `coef` 與指數 `exp`。`Polynomial` 再使用動態陣列存放這些項，只保留非零係數的項，並按照指數由大到小排列。

例如 $3x^2+2x+1$ 會存成：

| 陣列位置 | `coef` | `exp` | 代表的項 |
| --- | ---: | ---: | --- |
| `termArray[0]` | 3 | 2 | $3x^2$ |
| `termArray[1]` | 2 | 1 | $2x$ |
| `termArray[2]` | 1 | 0 | $1$ |

`termArray` 是指標，用來保存動態陣列的起始位址。陣列的每一格是一個 `Term`，所以同一格可以透過 `.coef` 與 `.exp` 存取係數和指數。

另外使用兩個整數記錄陣列狀態：

- `capacity`：陣列總共能放幾項。
- `terms`：目前實際存了幾個非零項，不是最高次方。

建構子先設定 `capacity = 1`、`terms = 0`，再配置一格陣列。雖然已經有空間，但還沒有存入任何項，因此代表零多項式。

### 解題策略

1. **新增項目 `NewTerm()`**：將係數和指數存到 `termArray[terms]`。如果 `terms == capacity`，先建立容量為原本兩倍的新陣列，用 `copy()` 複製舊資料，再釋放舊陣列。最後才增加項數。
2. **多項式加法 `Add()`**：使用 `aPos`、`bPos` 分別記錄兩邊目前處理的位置。指數相同就把係數相加，不為零才存入結果；指數不同就先存指數比較大的那一項。任一邊處理完後，再把另一邊剩下的項接到結果後面。
3. **多項式乘法 `Mult()`**：左邊每一項都乘上右邊每一項，係數相乘、指數相加。每一輪先存成 `temp`，再用 `c = c.Add(temp)` 累加並合併同類項。
4. **代入求值 `Eval()`**：逐項計算「係數乘上輸入值的指數次方」，把結果累加後回傳。
5. **輸入 `operator>>`**：先讀項數，再讀每一項的係數與指數。檢查通過後先存進暫存多項式，全部成功才指定給原物件，避免中途失敗時改到原本的資料。
6. **輸出 `operator<<`**：零多項式輸出 `0`；其他情況依序輸出符號、係數、`x` 和指數。指數為 0 不印 `x`，指數為 1 不印 `^1`。目前係數為 1 時仍會印出，例如 `1x`，不會省略成 `x`。

### 輸入與操作方式

多項式輸入格式是先輸入項數，再輸入各項的「係數 指數」。例如：

```text
2
2
1
3
0
```

代表 $2x+3$。項數為 0 代表零多項式，不需要再輸入係數與指數。

輸入條件如下：

- 項數不可為負數。
- 每一項的係數不可為 0，但可以是負數或小數。
- 指數必須是非負整數。
- 指數必須由大到小，不能重複。

主程式先讀入 `a`、`b`，再使用 `switch` 選擇操作。輸入 `1` 列出兩個多項式；輸入 `+` 或 `*` 後，將計算結果存進 `c`，再讀取 `x`，對結果多項式求值。完成一次操作後結束程式，不會重新顯示選單。

## 程式實作

程式分成三個檔案：

| 檔案 | 用途 |
| --- | --- |
| `src/Polynomial.h` | 宣告 `Term`、`Polynomial`、資料成員與函式 |
| `src/Polynomial.cpp` | 實作記憶體管理、計算與輸入輸出 |
| `src/main.cpp` | 讀取兩個多項式，讓使用者選擇操作 |

`Term` 將 `Polynomial` 宣告為友好類別，讓它的成員函式可以存取係數與指數。`operator<<` 也需要分別成為兩個類別的 `friend` ，因為友好關係不會自動傳遞。

因為類別使用動態陣列，所以另外實作解構子、複製建構子與複製指定運算子。複製物件時會配置自己的陣列，再複製各項內容，避免只複製位址，造成兩個物件共用陣列或重複釋放同一塊空間。

### Polynomial.h

```cpp
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <iostream>

using namespace std;

class Polynomial;
class Term{
    friend Polynomial;
    friend ostream& operator<<(ostream& out, const Polynomial& poly);

private:
    float coef;
    int exp;
};

class Polynomial{
public:
    Polynomial();
    Polynomial(const Polynomial& poly);
    ~Polynomial();

    Polynomial& operator=(const Polynomial& poly);

    Polynomial Add(Polynomial poly);
    Polynomial Mult(Polynomial poly);
    float Eval(float f);

    friend ostream& operator<<(ostream& out, const Polynomial& poly);
    friend istream& operator>>(istream& in, Polynomial& poly);

private:
    Term* termArray;
    int capacity;
    int terms;

    void NewTerm(const float theCoeff, const int theExp);
};

#endif
```

### Polynomial.cpp

```cpp
#include "Polynomial.h"
#include <algorithm>
#include <cmath>

Polynomial::Polynomial(){
    capacity = 1;
    terms = 0;
    termArray = new Term[capacity];
}

Polynomial::~Polynomial(){
    delete[] termArray;
}

Polynomial::Polynomial(const Polynomial& poly){
    capacity = poly.capacity;
    terms = poly.terms;
    termArray = new Term[capacity];

    for(int i = 0; i < terms; i++)
        termArray[i] = poly.termArray[i];
}

Polynomial& Polynomial::operator=(const Polynomial& poly){
    if(this == &poly)
        return *this;

    Term* newArray = new Term[poly.capacity];

    for(int i = 0; i < poly.terms; i++)
        newArray[i] = poly.termArray[i];

    delete[] termArray;

    termArray = newArray;
    capacity = poly.capacity;
    terms = poly.terms;

    return *this;
}

void Polynomial::NewTerm(const float theCoeff, const int theExp){
    if(terms == capacity){
        capacity *= 2;
        Term* temp = new Term[capacity];

        copy(termArray, termArray + terms, temp);
        delete[] termArray;
        termArray = temp;
    }

    termArray[terms].coef = theCoeff;
    termArray[terms++].exp = theExp;
}

Polynomial Polynomial::Add(Polynomial b){
    Polynomial c;
    int aPos = 0;
    int bPos = 0;
    
    while((aPos < terms) && (bPos < b.terms)){
        if(termArray[aPos].exp == b.termArray[bPos].exp){
            float t = termArray[aPos].coef + b.termArray[bPos].coef;

            if(t)
                c.NewTerm(t, termArray[aPos].exp);
            
            aPos++;
            bPos++;
        }
        else if(termArray[aPos].exp < b.termArray[bPos].exp){
            c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
            bPos++;
        }
        else{
            c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
    }
    for(; aPos < terms; aPos++)
        c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);

    for(; bPos < b.terms; bPos++)
        c.NewTerm((b.termArray[bPos].coef), b.termArray[bPos].exp);

    return c;
}

float Polynomial::Eval(float f){
    float result = 0;
    
    for(int i = 0; i < terms; i++)
        result += termArray[i].coef * pow(f, termArray[i].exp);
    return result;
}

Polynomial Polynomial::Mult(Polynomial b){
    Polynomial c;
    for(int i = 0; i < terms; i++){
        Polynomial temp;
        
        for(int j = 0; j < b.terms; j++){
            float coefficient = termArray[i].coef * b.termArray[j].coef;
            int exponent = termArray[i].exp + b.termArray[j].exp;

            if(coefficient != 0)
                temp.NewTerm(coefficient, exponent);
        }

        c = c.Add(temp);
    }
    return c;
}

ostream& operator<<(ostream& out, const Polynomial& poly){
    if(poly.terms == 0)
        out << 0;
    
    else{
        for(int i = 0; i < poly.terms; i++){
            float coefficient = poly.termArray[i].coef;

            if(i == 0){
                if(coefficient < 0)
                    out << "-";
            }
            else{
                if(coefficient > 0)
                    out << " + ";
                else
                    out << " - ";
            }
            out << fabs(coefficient);  //取浮點數絕對值

            int exponent = poly.termArray[i].exp;
            
            if(exponent > 0){
                out << "x";
                if(exponent > 1){
                    out << "^" << exponent;
                }
            }
        }
    }
    return out;
}

istream& operator>>(istream& in, Polynomial& poly){
    int numberOfTerms;

    if(!(in >> numberOfTerms))
        return in;
    if(numberOfTerms < 0){
        in.setstate(ios::failbit);
        return in;
    }

    Polynomial temp;
    int previousExponent = 0;

    for(int i = 0; i < numberOfTerms; i++){
        float coefficient;
        int exponent;

        if(!(in >> coefficient >> exponent))
            return in;
        
        if(coefficient == 0 || exponent < 0){
            in.setstate(ios::failbit);
            return in;
        }
        if(i > 0 && exponent >= previousExponent){
            in.setstate(ios::failbit);
            return in;
        }
        previousExponent = exponent;
        temp.NewTerm(coefficient, exponent);
    }
    poly = temp;
    
    return in;
}
```

### main.cpp

```cpp
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
```

## 效能分析

設左邊多項式有 $m$ 個非零項，右邊有 $n$ 個非零項；單一多項式的項數以 $t$ 表示：

| 功能 | 時間複雜度 | 空間說明 |
| --- | --- | --- |
| 建構子 | $O(1)$ | 初始只配置一格，為 $O(1)$ |
| 複製建構子、複製指定運算子 | 複製項目的時間為 $O(t)$ | 新陣列依來源的 `capacity` 配置；目前正常建立的物件容量與項數同階，為 $O(t)$ |
| `NewTerm()` | 有空位時 $O(1)$；擴充時 $O(t)$；連續新增平均攤銷為 $O(1)$ | 擴充時新舊陣列暫時同時存在，為 $O(t)$ |
| `Add()` | $O(m+n)$ | 包含參數副本與結果陣列，為 $O(m+n)$ |
| `Mult()` | 非空多項式的最壞情況為 $O(m^2n)$ | 中間結果最多有 $mn$ 項，包含暫存與複製，為 $O(mn+m+n)$ |
| `Eval()` | 呼叫 `pow()` 共 $t$ 次；將單次 `pow()` 視為固定成本時為 $O(t)$ | 累加只用固定數量變數，額外空間為 $O(1)$ |
| 輸入 | 正常情況下，新增 $t$ 項合計為 $O(t)$，最後指定也為 $O(t)$ | 暫存及指定時的新陣列為 $O(t)$ |
| 輸出 | 固定每項文字長度下為 $O(t)$ | 固定數量的區域變數，額外空間為 $O(1)$ |

空間分析以額外建立的資料為主。輸入輸出的字元讀寫成本另受數字位數影響；`pow()` 的實際耗時也受函式庫及參數影響，這裡主要比較走訪項數的次數。

### 加法分析

每次比較後，`aPos`、`bPos` 至少有一個往下一格移動，不會回頭重新處理相同的項。兩邊剩下的項也只各複製一次，所以是 $O(m+n)$。

雖然 `NewTerm()` 偶爾需要擴充陣列，但容量以兩倍增加，所有擴充的複製成本加起來仍與新增總項數同階，不會讓整個加法變成平方時間。

### 乘法分析

兩層迴圈本身會做 $mn$ 次項目相乘，但不能因此直接把目前的乘法寫成 $O(mn)$，因為每一輪還會執行：

```cpp
c = c.Add(temp);
```

如果不同項的乘積沒有相同指數，第 $i$ 輪的累計結果可能已經有約 $in$ 項。反覆合併的成本上界會像：

$$
n+2n+3n+\cdots+mn=O(m^2n)
$$

`operator=` 複製各輪結果的成本也包含在這個上界裡。這是目前以左邊作為外層迴圈的分析；若乘積有很多同類項，實際結果會比較短，執行時間也可能較少。若其中一邊是零多項式，項目相乘與合併的工作也會減少，仍需考慮參數複製與迴圈檢查的成本。

## 測試與驗證

### 測試案例

本次先編譯目前的.cpp檔，再以固定輸入執行測試。一般測試使用 $a=2x+3$、$b=x+1$；其他案例在表內另列。表內「實際結果」節錄結果文字，不重複列出每次的選單提示。

| 測試案例 | 操作或輸入 | 預期結果 | 實際結果 |
| --- | --- | --- | --- |
| 測試一 | 一般測試，選 `1` | 列出兩個多項式 | `2x + 3`、`1x + 1` |
| 測試二 | 一般測試，選 `+`，代入 `2` | $3x+4$，求值為 10 | `3x + 4`、`answer = 10` |
| 測試三 | 一般測試，選 `*`，代入 `2` | $2x^2+5x+3$，求值為 21 | `2x^2 + 5x + 3`、`answer = 21` |
| 測試四 | 一般測試，選 `+`，代入 `0.5` | 求值為 5.5 | `answer = 5.5` |
| 測試五 | $(3x^2+2)+(-3x^2-2)$，代入 `2` | 同類項全部抵消，結果為 0 | 多項式為 `0`，`answer = 0` |
| 測試六 | $0\times(2x+3)$，代入 `2` | 結果為 0 | 多項式為 `0`，`answer = 0` |
| 測試七 | $a=10^{20}$、$b=10^{20}$，選 `+`，代入 `0`；係數可輸入 `1e20` | 結果為 $2\times10^{20}$ | 多項式為 `2e+20`，`answer = 2e+20` |
| 測試八 | 某一項輸入 `0 2` | 拒絕零係數項 | `輸入格式錯誤` |
| 測試九 | 兩項指數都輸入 `2` | 拒絕重複指數 | `輸入格式錯誤` |
| 測試十 | $a=x^{2147483647}$、$b=0$，選 `+`，代入 `1` | 保留最高邊界指數，求值為 1 | `1x^2147483647`，`answer = 1` |
| 測試十一 | 指數依序輸入 `1`、`2` | 拒絕未遞減的順序 | `輸入格式錯誤` |
| 測試十二 | 選單輸入 `?` | 顯示提示並結束，不會卡住 | 顯示選項提示後結束 |
| 測試十三 | 一開始直接選 `x`，代入 `2` | 此路徑沒有先產生運算結果，作為限制檢查 | 對初始的零多項式 `c` 求值，得到 `answer = 0` |

測試十三記錄的是目前程式仍存在的限制，不把它當成已完成的正常求值功能。主選單原本只提供 `1`、`+`、`*`，但程式仍允許直接進入 `case 'x'`。之後可以限制初始選項，或加入判斷，確認 `c` 已經有運算結果才允許求值。

### 編譯與執行指令

在 `homework2/src` 資料夾中執行：

```powershell
g++ -std=c++17 -Wall -Wextra main.cpp Polynomial.cpp -o main.exe
.\main.exe
```

兩個 `.cpp` 需要一起編譯。`Polynomial.h` 由 `#include` 載入，不另外當成程式執行。

以加法為例，輸入：

```text
2
2
1
3
0
2
1
1
1
0
+
2
```

運算結果如下，省略前面的輸入提示與選單：

```text
(2x + 3) + (1x + 1) = 3x + 4
輸入 x = 2
x = 2, answer = 10
```

### 效能量測

每組資料讓左右兩個多項式都有 $n$ 項，係數都是 1。左邊指數為 $i(n+1)$，右邊指數為 $j$，其中 $i,j$ 從 0 到 $n-1$，輸入時按遞減順序排列。這樣不同項的乘積具有不同指數，能觀察結果項數增加時的成本。

先執行一次乘法暖機，再重複加法 2000 次、乘法 100 次，將總時間除以次數。資料建立和終端機輸出不包含在計時內；每次計時包含結果物件的建立、銷毀，以及一次 `Eval(0)`，用來讀取結果。因此下表是這組操作的平均耗時，不是完全排除其他成本的單獨函式時間。

| 左右各自項數 $n$ | 加法組合操作平均時間（微秒） | 乘法組合操作平均時間（微秒） |
| ---: | ---: | ---: |
| 10 | 0.273 | 7.374 |
| 20 | 0.458 | 25.222 |
| 40 | 0.782 | 153.431 |

## 申論及開發報告

這次一開始我比較不熟悉類別的寫法，所以先從 `.h` 的外框開始，把建構子、加法、乘法和求值分開宣告，再到 `.cpp` 一個一個實作。這樣比較能分清楚「這個類別有哪些功能」和「每個功能實際怎麼做」，不會一開始就把所有程式混在一起。

我原本不太懂，為什麼同一個陣列可以存係數，又可以存指數。後來才理解，不是一格交替存兩種數字，而是每一格都是一個 `Term`，一個 `Term` 裡面本來就有 `coef` 和 `exp`。`Term* termArray` 則是宣告指標，真正的陣列是用 `new Term[capacity]` 建立，指標再保存第一格的位址。

參照和指標也是這次花比較多時間釐清的地方。傳值會建立物件副本，但如果物件裡有指標，預設複製不會自動把整個陣列複製一份。所以我補上複製建構子和 `operator=`，讓不同物件各自管理自己的陣列。解構子再負責用 `delete[]` 釋放空間。

加法主要參考課本的比較方式與 `NewTerm()`。乘法則先讓左邊一項乘上右邊每一項，形成 `temp`，再用已經完成的加法合併。我一開始把 `c = c.Add(temp)` 裡面的兩個 `c` 搞混，後來才知道，這一行的兩個 `c` 都是乘法裡的同一個物件；`Add()` 裡面宣告的區域變數雖然也叫 `c`，但那是另外一個儲存加法結果的物件。先算出結果，再透過 `operator=` 存回去，才能把每一輪的乘積累加起來。

輸入輸出運算子的寫法一開始看起來很長，拆開後其實仍然是回傳型別、函式名稱和參數。`in`、`out` 是自己取的參數名稱，呼叫 `cin >> a`、`cout << a` 時，才分別參考原本的 `cin`、`cout`。另外，`if(!(cin >> type))` 判斷的是讀取是否失敗，不是在判斷使用者是不是輸入 0；能讀進一個字元，也不代表它就是合法選項。

主程式的部分，我把列出多項式、加法和乘法放進 `switch`，運算後再轉到求值。檢查時發現乘法的顯示符號原本寫成加號，代入的變數原本也用了 `int`，所以改成乘號與 `float`。另外，原本為了顯示和儲存結果，會重複呼叫兩次加法或乘法，現在先存到 `c` 再輸出，避免做兩次相同計算。

目前程式可以完成這次作業要求的基本操作，但還有一些待改進的地方。輸入需要自己按指數遞減排列，程式不會自動排序或合併輸入的重複項；主選單直接選 `x` 也還沒有擋下來。極大指數相加的整數溢位、浮點數超出範圍，以及記憶體配置失敗後的恢復，也沒有完整處理。這些地方不能因為一般測試通過，就當成所有輸入都一定正確。

這次比較重要的是，我能逐步說明每個變數在記錄什麼，以及它為什麼要放在那個位置。像 `terms` 不只是項數，也能指出新增項目的位置；`temp` 則讓我先準備好資料，再交給原本的物件。接下來如果要修改程式，也比較能知道應該從哪個函式開始看。
