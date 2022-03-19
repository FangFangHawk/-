#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include<fstream>  //ifstream
#include <io.h>

using namespace std;

vector<char> cT;
vector<string> sT;

//�����
map<string , int> PT;
map<string , int> KT;

vector< pair<string , int> > res;

void init_all_map(); //��ʼ��ӳ���

int state_change(int state , char ch); //״̬ת��

int get_state_ch(char ch);  //��õ�ǰ�ַ���״̬

int State2Code(int state_before);   //״̬ת����

int parse(int code , string token);//��������

void show();    //չʾ��õ��������

bool check_legal_num(string token);//����Ƿ�Ϊ�Ϸ�������

bool all_zero(string token); //�������ȫΪ0��ʱ����ܳ��ֵĴ���

void Error_print(int state , int line);//���������Ϣ


int flag_cT = 0;
int flag_sT = 0;
int flag_anno = 0;
int flag_all_anno = 0;
int flag_e_count = 0;
int main(){
    init_all_map();
    int state = 1;
    int state_before = state;
    string token;
    //���ļ���ȡ�ַ�

    ifstream inf;
    ofstream outf;

    char in_file[100];

    cout<<"Please enter the path to the test file(Less than a hundred characters)"<<endl;
    cin>>in_file;
    inf.open(in_file);
    if (!inf.is_open())
    {
        cout << "can not open the  test in_file" << endl;
        return 0;
    }

    char out_file[100];
    cout<<"Please enter the path to the output file(Less than a hundred characters)"<<endl;
    cin>>out_file;
    //inf.open("D:\\������\\����ԭ��\\test.txt");

    if(_access(out_file, 0) == 0){
        cout<<"The file already exists. Whether to overwrite it"<<endl;
        cout<<"Yes(1) /t NO(0)"<<endl;
        int op = 0;
        cin>>op;
        if(op == 0)
            return 0;
    }
    else{
        cout<<"File does not exist"<<endl;
        cout<<"The file will be created for you in that path"<<endl;
    }
    outf.open(out_file);
    if(!outf.is_open()){
        cout << "Failed to create or can not open the result out_file" << endl;
        return 0;
    }
    char ch;
    string sline;
    int count_line = 1;
    while (getline(inf , sline)){
        for(int i = 0 ; i < sline.size() ; i++){
            if(flag_anno){
                state = 1;
                token.clear();
                flag_anno = 0;
                break;
            }
            ch = sline[i];
            state_before = state;
            state = state_change(state , ch);
            //cout<<state<<" "<<ch<<endl;
            //int tmp;
            //cout<<token<<endl;
            if(state > 0){
                token += ch;
            }
            else if(state == 0){
                state = 1;
                int code =  State2Code(state_before);
                int check_parse = parse(code , token);
                //cout<<check_parse<<" "<<endl;
                if(check_parse < 0){
                    Error_print(check_parse , count_line);
                    outf<<"Error_code is "<<check_parse<<"  Error_line: " << count_line<<endl;
                    return 0;
                }
                state = state_change(state , ch);
                token.clear();
                if(state){
                    token += ch;
                }
                else{
                    state = 1;
                }
            }
            if(state < 0){
                Error_print(state , count_line);
                outf<<"Error_code is "<<state<<"  Error_line: " << count_line<<endl;
                return 0;
            }
        }
        count_line++;
    }
    //cout<<token<<" asd"<<endl;
    if(!token.empty()){
        int code =  State2Code(state);
        parse(code , token);
        token.clear();
    }
    inf.close();
    show();
    for(int i = 0 ; i < res.size() ; i++){
        outf<<"< "<<res[i].first<<" , "<<res[i].second<<" >"<<endl;
    }
    outf.close();

    return 0;
}
void show(){
    for(int i = 0 ; i < res.size() ; i++){
        if(i%2 == 0)
            cout<<endl;
        cout<<"< "<<res[i].first<<" , "<<res[i].second<<" >"<<"\t\t";
    }
}
int state_change(int state , char ch){
    if(flag_sT == 1 && ch != '\"'){
        return 7;
    }
    int now_state;
    switch (state)
    {
        case 1:
            return get_state_ch(ch);
            break;
        case 2:
            return 0;
            break;
        case 3:
            if(flag_cT == 2){
                flag_cT = 0;
                return 0;
            }
            flag_cT = 1;
            return 0;
            break;
        case 4:
            if(flag_sT == 2){
                flag_sT = 0;
                return 0;
            }
            flag_sT = 1;
            return 0;
            break;
        case 5:
            if(get_state_ch(ch) != 5){
                return 0;
            }
            return 5;
            break;
        case 6:
            if(ch == 'e' || ch == 'E'){
                return 9;
            }
            now_state = get_state_ch(ch);
            if(now_state <= 5){
                if(now_state < 0){
                    return now_state;
                }
                return 0;
            }
            if(now_state == 8){
                //cout<<"asa"<<ch<<endl;
                return 8;
            }
            return 6;
            break;
        case 7:
            if(isalpha(ch) || ch == '_'|| isdigit(ch)){
                return 7;
            }
            return 0;
            break;
        case 8:
            if(ch == 'e' || ch == 'E'){
                return 9;
            }
            now_state = get_state_ch(ch);
            if(now_state <= 5){
                if(now_state < 0){
                    return now_state;
                }
                return 0;
            }
            //cout<<now_state<<"asa"<<ch<<endl;
            if(now_state == 8)
                return -6;
            return 8;
            break;
        case 9:
            if(ch == '+' || ch == '-'){
                if(flag_e_count > 1)
                    return -7;
                flag_e_count++;
                return 9;
            }
            now_state = get_state_ch(ch);
            if(now_state <= 5){
                if(now_state < 0){
                    return now_state;
                }
                return 0;
            }
            //cout<<now_state<<"asa"<<ch<<endl;
            if(now_state == 8)
                return -6;
            return 9;
            break;

    }
    return 0;
}
int get_state_ch(char ch){
    if((int)ch > 125 || (int)ch < 32){
        return -1;
    }
    if(ch == ' ' || ch == '\n'){\
        return 0;
    }
    string now;
    now += ch;
    int now_code = PT[now];
    if(now_code){
        if(now_code > 69){
            if(now_code == 76)
                return 3;
            else if(now_code == 77)
                return 4;
            else if(now_code == 100)
                return 8; //����С��
            return 2;
        }
        return 5;
    }
    if( ch >= '0' && ch <= '9'){
        return 6;
    }
    return 7;
}
int State2Code(int state_before){
    switch (state_before)
    {
    case 2:
        return 1; //���
        break;
    case 3:
        return 1; //������ ��'��
    case 4:
        return 1; //������ ��"��
    case 5:
        return 4; //�����
    case 6:
        return 5; //��������
    case 7:
        if(flag_cT == 1){
            flag_cT = 2;
            return 2;
        }
        else if(flag_sT == 1){
            flag_sT = 2;
            return 3;
        }
        return 6;// ��ʶ����ؼ���
    case 8:
        return 7;//����С��
    case 9:
        return 8;//��ѧ����
    default:
        break;
    }
    return -3;
}
int parse(int code , string token){
    pair<string , int> ans;
    string point_num;
    string left;
    int pos ;
    int PT_check;
    if(flag_all_anno){
        if(PT[token] != 83)
            return 0;
        flag_all_anno = 0;
        return 0;
    }
    switch (code)
    {
        case 1:
            ans = make_pair(token , PT[token]);
            res.push_back(ans);
            break;
        case 2:
            if(token.size() != 1)
                return -2;
            ans = make_pair(token , 1);
            res.push_back(ans);
            break;
        case 3:
            ans = make_pair(token , 2);
            res.push_back(ans);
            break;
        case 4:
            PT_check = PT[token];
            //cout<<PT_check<<endl;
            if(!PT_check){
                return -5;
            }
            else if(PT_check == 81){
                flag_anno = 1;
                break;
            }
            else if(PT_check == 82){
                flag_all_anno = 1;
                break;
            }
            ans = make_pair(token , PT[token]);
            res.push_back(ans);
            break;
        case 5:
            if(check_legal_num(token)){
                return -4;
            }
            ans = make_pair(token , 3);
            res.push_back(ans);
            break;
        case 6:
            if(KT[token]){
                ans = make_pair(token , KT[token]);
                res.push_back(ans);
            }
            else{
                ans = make_pair(token , 0);
                res.push_back(ans);
            }
            break;
        case 7:
            if(token[0] == '.') {
                point_num += '0';
                left += '0';
            }
            for(pos = 0 ; pos < token.size() ; pos++){
                if(token[pos] != '.'){
                    left += token[pos];
                }
                else
                    break;
            }
            //cout<<"-6"<<left<<endl;
            if(check_legal_num(left))
                return -6;
            ans = make_pair(token , 3);
            res.push_back(ans);
            break;
        case 8:
            if(token[0] == '.') {
                point_num += '0';
                left += '0';
            }
            for(pos = 0 ; pos < token.size() ; pos++){
                if(token[pos] != '.' && token[pos] != 'e' && token[pos] != 'E'){
                    left += token[pos];
                }
                else
                    break;
            }
            //cout<<"-6"<<left<<endl;
            if(check_legal_num(left))
                return -6;
            if(token[pos] == '.'){
                pos++;
                point_num.clear();
                if(!isdigit(token[pos]))
                    return -6;
            }
            //cout<<token<<endl;
            //cout<<pos<<endl;
            //cout<<"asdasd"<<endl;
            while(pos < token.size()){
                cout<<pos<<endl;
                if(token[pos] == 'e' || token[pos] =='E'){
                    break;
                }
                pos++;
            }
            pos++;
            //cout<<pos<<endl;
            if(pos >= token.size())
                return -7;
            if(token[pos] == '+' || token[pos] == '-')
                pos++;

            for(;pos < token.size() ; pos++){
                if(!isdigit(token[pos]))
                    return -7;
            }
            ans = make_pair(token , 3);
            res.push_back(ans);
            flag_e_count = 0;
            break;
    }
    return 0;
}
int dif_base(char first , char second){
    if(first == '0' && (second == 'b' || second == 'B')){
        return 2;
    }
    else if(first == '0' && isdigit(second)){
        return 8;
    }
    else if(first == '0' && (second == 'x' || second == 'X')){
        return 16;
    }
    else if(first != 0)
        return 10;
    return 0;
}
bool all_zero(string token){
    if(token.size() < 3)
        return false;
    int count = 0;
    for(int i = 0 ; i < token.size() ; i++){
        if(token[i] == '0'){
            count++;
        }
    }
    return (count == token.size());
}
bool check_legal_num(string token){
    if(token.size() < 2){
        return false;
    }
    int dif = dif_base(token[0] , token[1]);
    cout<<dif<<endl;
    if((dif != 10 && dif != 8) && token.size() < 3 ||(all_zero(token))){
        return true;
    }
    switch (dif)
    {
    case 0:
        return true;
    case 10:
        for(int i = 1 ; i < token.size() ; i++)
            if(!isdigit(token[i]))
                return true;
        break;
    case 8:

        for(int i = 1 ; i < token.size() ; i++){
            if( token[i] > '8' || token[i] < '0')
                return true;
        }
        break;
    case 16:
        for(int i = 2; i < token.size() ; i++){
            if(!(isdigit(token[i])) &&( (token[i] < 'a' || token[i] > 'f') && (token[i] < 'A' || token[i] > 'F') ))
                return true;
        }
        break;
    default:
        break;
    }
    return false;
}


void init_all_map(){
    //�ؼ���
    KT["auto"] = 4;
    KT["short"] = 5;
    KT["int"] = 6;
    KT["long"] = 7;
    KT["float"] = 8;
    KT["double"] = 9;
    KT["char"] = 10;
    KT["struct"] = 11;
    KT["union"] = 12;
    KT["enum"] = 13;
    KT["typedef"] = 14;
    KT["const"] = 15;
    KT["unsigned"] = 16 ;
    KT["signed"] = 17;
    KT["extern"] = 18;
    KT["register"] = 19;
    KT["static"] = 20;
    KT["volatile"] = 21;
    KT["void"] = 22;
    KT["if"] = 23;
    KT["else"] = 24;
    KT["switch"] = 25;
    KT["case"] = 26;
    KT["for"] = 27;
    KT["do"] = 28;
    KT["while"] = 29;
    KT["goto"] = 30;
    KT["continue"] = 31;
    KT["break"] = 32;
    KT["default"] = 33;
    KT["sizeof"] = 34;
    KT["return"] = 35;
    KT["main"] = 36;

    //PT���
    //���������
    PT["+"] = 37;
    PT["-"] = 38;
    PT["*"] = 39;
    PT["/"] = 40;
    PT["%"] = 41;
    PT["++"] = 42;
    PT["--"] = 43;
    //�Ƚ������
    PT[">"] = 44;
    PT["<"] = 45;
    PT["=="] = 46;
    PT[">="] = 47;
    PT["<="] = 48;
    PT["!="] = 49;
    //�߼������
    PT["&&"] = 50;
    PT["||"] = 51;
    PT["!"] = 52;
    //λ�����
    PT["&"] = 53;
    PT["|"] = 54;
    PT["~"] = 55;
    PT["^"] = 56;
    PT["<<"] = 57;
    PT[">>"] = 58;
    //��ֵ�����
    PT["="] = 59;
    PT["+="] = 60;
    PT["-="] = 61;
    PT["*="] = 62;
    PT["/="] = 63;
    PT["%="] = 64;
    PT["&="] = 65;
    PT["|="] = 66;
    PT["^="] = 67;
    PT[">>="] = 68;
    PT["<<="] = 69;
    //�ָ����
    PT["("] = 70;
    PT[")"] = 71;
    PT["["] = 72;
    PT["]"] = 73;
    PT["{"] = 74;
    PT["}"] = 75;
    PT["\'"] = 76;
    PT["\""] = 77;
    PT[";"] = 78;
    PT[","] = 79;
    PT["!"] = 80;
    PT["//"] = 81;
    PT["/*"] = 82;
    PT["*/"] = 83;
    PT["?"] = 84;
    PT["."] = 100;
}
void Error_print(int state , int line){
    switch (state)
    {
    case -1:
        cout<<"Error : Your input contains illegal characters in Line : "<<line<<" ."<<endl;
        break;
    case -2:
        cout<<"Error : Contains multiple characters in a variable of type \"char\" in Line : "<<line<<" ."<<endl;
        break;
    case -3:
        cout<<"Error : Contains unparsed characters  in Line : "<<line<<" ."<<endl;
        break;
    case -4:
        cout<<"Error : The representation of numbers is wrong in Line : "<<line<<" ."<<endl;
        break;
    case -5:
        cout<<"Error : Incorrect use of related symbols in Line : "<<line<<" ."<<endl;
        break;
    case -6:
        cout<<"Error : Incorrect decimal representation is used in the statement in Line : "<<line<<" ."<<endl;
        break;
    case -7:
        cout<<"Error : Incorrect use of scientific notation in Line : "<<line<<" ."<<endl;
    default:
        break;
    }
}
