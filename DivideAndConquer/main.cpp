//John Handley
//Date: 10/13/14
//Program Name: Program 2
/*Program Description:
    The Program uses strings to represent
    numbers that would otherwise be unable
    to be represented with smaller data storage.
    We are implementing the various functions
    needed to operate with the numeric string
    as well as implementing strassen's algorithm
    for efficient multiplication.
*/
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int toNum(char a){
    return a - '0';
}
string toString(int a){
    ostringstream converter;
    converter << a;
    string temp = converter.str();
    converter.clear();
    return temp;
}

class ReallyBigInteger{
public:
    string data;
    ostringstream converter;
    
    ReallyBigInteger(){
        data = "0";
    }
    ReallyBigInteger(ReallyBigInteger& other){
        data.assign(other.data);
    }
    ReallyBigInteger(string& other){
        for(int i = 0; i < data.length(); i++) {
            if(!std::isdigit(data[i])){
                data.assign("0");
                return;
            }
        }
        data.assign(other);
        return;
    }
    
    ReallyBigInteger(int m){
        if(m < 0){
            data = toString(0);
        }
        else{
            data = toString(m);
        }
    }
    
    void removeZeroes(){
        while(data[0] == '0'){
            data = data.substr(1);
        }
    }
    
    ReallyBigInteger  addUtil(const ReallyBigInteger& smaller, const ReallyBigInteger& larger){
        ReallyBigInteger temp;
        temp.data.assign("");
        
        int previous = 0;
        for(double i = smaller.data.length()-1; i >= 0; i--){
            int carry = 0;
            int total = toNum(smaller.data[i]) + toNum(larger.data[i]);
            if( total >= 10){
                carry = 1;
                total = total - 10;
            }
            total += previous;
            temp.data.insert(0, toString(total));
            previous = carry;
        }
        if(previous == 1){
            temp.data.insert(0, toString(1));
        }
        
        //temp.removeZeroes();
        return temp;
    }
    
    ReallyBigInteger  Add(const ReallyBigInteger& s1 ){
        if(s1.data.length() == 0){
            return *this;
        }
        ReallyBigInteger temp;
        temp.data.assign("");
        if(data.length() != s1.data.length()){
            unsigned long l = labs(data.length()-s1.data.length());
            if(data.length() > s1.data.length()){
                temp.data.append(s1.data);
                for(unsigned long i = 0; i < l; i++){
                    temp.data.insert(0, "0");
                }
                temp.data.assign(addUtil(temp, *this).data);
                return temp;
            }
            else{
                temp.data.append(data);
                for(unsigned long i = 0; i < l; i++){
                    temp.data.insert(0, "0");
                }
                temp.data.assign(addUtil(temp, s1).data);
                return temp;
            }
        }
        temp.data.assign(addUtil(*this, s1).data);
        return temp;
    }
    int compare(const ReallyBigInteger& smaller, const ReallyBigInteger& larger){
        long count = 0;
        for(;count < smaller.data.length() && smaller.data[count] == '0'; count++){}
        long count2 = 0;
        for(;count2 < larger.data.length() && larger.data[count2] == '0'; count2++){}
        if(count != count2){
            if(count < count2){
                return 1;
            }
            else{
                return -1;
            }
        }
        for(int i = 0; i < larger.data.length(); i++){
            for(;i < larger.data.length() &&
                smaller.data[i] == larger.data[i]; i++){}
            if(i < larger.data.length()){
                if(larger.data[i] < smaller.data[i]){
                    return 1;
                }
                else if(larger.data[i] > smaller.data[i]){
                    return -1;
                }
            }
        }
        return 0;
    }
    ReallyBigInteger subUtil(const ReallyBigInteger& smaller, const ReallyBigInteger& larger){
        ReallyBigInteger temp;
        temp.data.assign(smaller.data);
        if(compare(smaller, larger) <= 0){
            temp.data.assign("0");
        }
        else{
            for(double i = temp.data.length()-1; i >= 0; i--){
                if(toNum(temp.data[i]) < toNum(larger.data[i])){
                    double k = i-1;
                    for(; k > 0 && temp.data[k]=='0'; k--){
                        temp.data[k] = '9';
                    }
                    temp.data[k] = toString(toNum(temp.data[k])-1)[0];
                    temp.data[i] = toString((toNum(temp.data[i])+10) - toNum(larger.data[i]))[0];
                }
                else{
                    temp.data[i] = toString(toNum(temp.data[i])
                                            - toNum(larger.data[i]))[0];
                }
            }
        }
        return temp;
    }
    
    ReallyBigInteger  Subtract(const ReallyBigInteger& s1 ){
        ReallyBigInteger temp;
        temp.data.assign("");
        if(data.length() != s1.data.length()){
            
            unsigned long l = labs(data.length()-s1.data.length());
            if(data.length() > s1.data.length()){
                temp.data.append(s1.data);
                for(unsigned long i = 0; i < l; i++){
                    temp.data.insert(0, "0");
                }
                temp.data.assign(subUtil(*this, temp).data);
                return temp;
            }
            else{
                temp.data.append(data);
                for(unsigned long i = 0; i < l; i++){
                    temp.data.insert(0, "0");
                }
                temp.data.assign(subUtil(temp, s1).data);
                return temp;
            }
        }
        temp.data.assign(subUtil(*this, s1).data);
        return temp;
    }
    ReallyBigInteger mulUtil(const ReallyBigInteger& x, const ReallyBigInteger& y){
        ReallyBigInteger temp, result;
        temp.data.assign("");
        
        for(double i = 0 ; i < x.data.length(); i++){
            int previous = 0;
            for(double k = 0; k < i; k++){
                temp.data.insert(0,"0");
            }
            for(double j = y.data.length()-1; j >= 0; j--){
                int carry = 0;
                int total = Multiply1(toNum(x.data[i]), toNum(y.data[j]));
                carry = total / 10;
                total %= 10;
                total += previous;
                if(total > 10){
                    carry++;
                    total -= 10;
                }
                temp.data.insert(0, toString(total));
                previous = carry;
            }
            if(previous > 0){
                temp.data.insert(0, toString(previous));
            }
            temp.removeZeroes();
            result.data.assign(result.Add(temp).data);
            temp.data.clear();
        }
        return result;
    }
    ReallyBigInteger  Multiply(const ReallyBigInteger& s1 ){
        ReallyBigInteger temp;
        temp.data.assign("");
        if(data.length() != s1.data.length()){
            unsigned long l = labs(data.length()-s1.data.length());
            if(data.length() > s1.data.length()){
                temp.data.append(s1.data);
                for(unsigned long i = 0; i < l; i++){
                    temp.data.insert(0, "0");
                }
                temp.data.assign(mulUtil(*this, temp).data);
                return temp;
            }
            else{
                temp.data.append(data);
                for(unsigned long i = 0; i < l; i++){
                    temp.data.insert(0, "0");
                }
                temp.data.assign(mulUtil(s1, temp).data);
                return temp;
            }
        }
        temp.data.assign(mulUtil(*this, s1).data);
        return temp;
    }
    ReallyBigInteger partOf(const ReallyBigInteger& x , int i){
        ReallyBigInteger temp;
        temp.data.assign(x.data.substr(i, 1));
        return temp;
    }
    
    void fastMult(ReallyBigInteger& x, ReallyBigInteger& y){
        ReallyBigInteger A11, A12, A21, A22, B11, B12, B21, B22;
        ReallyBigInteger M1, M2, M3, M4, M5, M6, M7;
        ReallyBigInteger C11, C12, C21, C22;
        
        A11.data.assign(partOf(x, 0).data);
        A12.data.assign(partOf(x, 1).data);
        A21.data.assign(partOf(y, 0).data);
        A22.data.assign(partOf(y, 1).data);
        B11.data.assign(partOf(x, 2).data);
        B12.data.assign(partOf(x, 3).data);
        B21.data.assign(partOf(y, 2).data);
        B22.data.assign(partOf(y, 3).data);
        
        M1.data.assign((A11.Add(A22)).Multiply((B11.Add(B22))).data);
        M2.data.assign((A21.Add(A22)).Multiply(B11).data);
        M3.data.assign((A21.Add(A22)).Multiply(B11).data);
        M4.data.assign(A22.Multiply((B21.Subtract(B11))).data);
        M5.data.assign((A11.Add(A12)).Multiply(B22).data);
        M6.data.assign((A21.Subtract(A11)).Multiply((B11.Add(B12))).data);
        M7.data.assign((A12.Subtract(A22)).Multiply((B21.Add(B22))).data);
        
        C11.data.assign(M7.Add(M5.Subtract(M1.Add(M4))).data);
        C12.data.assign(M2.Add(M4).data);
        C21.data.assign(M3.Add(M5).data);
        C22.data.assign(M6.Add(M2.Subtract(M1.Add(M3))).data);
        
        C11.data.append(C12.data);
        C21.data.append(C22.data);
        x.data.assign(C11.data);
        y.data.assign(C21.data);
        return;
    }
    
    void DnCUtil(ReallyBigInteger& x, ReallyBigInteger& y){
        long count = data.length();
        ReallyBigInteger temp1, temp2, temp3, temp4;
        ReallyBigInteger result;
        
        temp1.data.assign(x.data);
        temp2.data.assign(y.data);
        
        if(count > 4){
            temp1.data.assign(x.data.substr(0,(data.length()/2)-1));
            temp2.data.assign(y.data.substr(0,(data.length()/2)-1));
            DnCUtil(temp1, temp2);
            temp3.data.assign(x.data.substr((data.length()/2), data.length()-1));
            temp4.data.assign(y.data.substr((data.length()/2), data.length()-1));
            DnCUtil(temp3, temp4);
        }

        fastMult(temp1, temp2);
        fastMult(temp3,temp4);
        temp1.data.append(temp3.data);
        temp2.data.append(temp4.data);
        
        x.data.assign(temp1.data);
        y.data.assign(temp2.data);
        
        return;
    }
    
    ReallyBigInteger MultiplyDivideAndConquer(const ReallyBigInteger& s1 ){
        ReallyBigInteger temp, temp2, total;
        temp.data.assign("");
        if(data.length() != s1.data.length()){
            unsigned long l = labs(data.length()-s1.data.length());
            if(data.length() > s1.data.length()){
                temp.data.append(s1.data);
                temp2.data.assign(data);
            }
            else{
                temp.data.append(data);
                temp2.data.assign(s1.data);
            }
            for(unsigned long i = 0; i < l; i++){
                temp.data.insert(0, "0");
            }
            unsigned long x = temp.data.length();
            while((x & (x - 1)) != 0){
                temp.data.insert(0, "0");
                temp2.data.insert(0, "0");
            }
            DnCUtil(temp2, temp);
            total.data.assign(temp2.Multiply(temp).data);
            return total;
        }
        temp.data.assign(data);
        temp2.data.assign(s1.data);
        unsigned long x = data.length();
        while((x & (x - 1)) != 0){
            temp.data.insert(0, "0");
            temp2.data.insert(0, "0");
        }
        if(compare(temp, temp2) >= 0){
            DnCUtil(temp, temp2);
            total.data.assign(temp2.Multiply(temp).data);
        }
        else{
            DnCUtil(temp2, temp);
            total.data.assign(temp2.Multiply(temp).data);
        }
        return total;
    }
    
    int Multiply1(int x, int y){
        if ((x >= 0) && (x <= 18) && (y >= 0) && (y <=18))
            return x * y;
        exit(-1);
    }
};

ostream& operator <<(ostream& os, const ReallyBigInteger& s1){
    os << s1.data;
    return os;
}

istream& operator >>(istream& is, ReallyBigInteger& s1){
    string temp;
    is >> temp;
    for(int i = 0; i < temp.length(); i++) {
        if(!std::isdigit(temp[i])){
            return is;
        }
    }
    s1.data.assign(temp);
    return is;
}

int main(int argc, char** argv){
    ReallyBigInteger* r = new ReallyBigInteger();
    ReallyBigInteger* r2 = new ReallyBigInteger();
    ostringstream s;
    cin >> *r;
    cin >> *r2;
    cout << r->Add(*r2) << endl;
    cout << r->Subtract(*r2) << endl;
    cout << r->Multiply(*r2) << endl;
    /****** Doesnt Work ************/
    //cout << r->MultiplyDivideAndConquer(*r2) << endl
    return 0;
}

