#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MaxSize 10
typedef struct fractions{
    int up;
    int down;
}fraction;

// 化成最简分数
fraction simplify(fraction a){
    int divide = 2;
    do{
    while(a.up%divide==0 && a.down%divide==0){
            a.down /= divide;
            a.up /= divide;
    }
    divide++;
    }while(divide <= a.up);
    return a;
}
//产生一个随机最简真分数或100以内的整数
fraction GenerateAnOperant(){
    fraction a;
    if (rand()%2) {//产生[1,20]整数
        a.down = 1;
        a.up = rand()%20 + 1;
    }
    else{
        a.down = rand()%10 + 1;//[1,10]
        do{
            a.up = rand()%(a.down) ;
        }while(a.up == 0);//[1,down-1]
        a = simplify(a);
    }
    return a;
}
// 打印一个操作数
void PrintFraction(fraction a){
    if (a.down == 1) {
        printf("%d", a.up);
    }
    else{
        printf("%d/%d", a.up, a.down);
    }
}
// 产生一个运算符
char GenerateOperator(){
    char ch[] = {'+', '-', '*', '/'};
    return (ch[rand()%4]);
}
// 判断运算符优先级
int priority(char a, char b){//若a优于b，则返回1
    int result = 0;
    switch (a) {
        case '*':
        case '/':
            if (b == '+' || b == '-') {
                result = 1;
            }
            break;
        default:
            result = 0;
            break;
    }
    return result;
}
// 分数四则运算
fraction calculate(char op, fraction a, fraction b){
    fraction result;
    switch (op) {
        case '+':
            result.down = a.down * b.down;
            result.up = a.up * b.down + a.down * b.up;
            break;
        case '-':
            result.down = a.down * b.down;
            result.up = a.up * b.down - a.down * b.up;
            break;
        case '*':
            result.down = a.down * b.down;
            result.up = a.up * b.up;
            break;
        case '/':
            result.down = a.down * b.up;
            result.up = a.up * b.down;
            break;
        default:
            break;
    }
    result = simplify(result);
    return result;
}

int main(int argc, char const *argv[]) {
        int count = 0;//需产生的题目数
        int j = 0;
        while (argv[2][j] != '\0') {
            count = count * 10 + (argv[2][j] - '0');
            j++;
        }
    char op[MaxSize];
    fraction num[MaxSize+1];
    srand((unsigned)time(NULL));
    printf("本次共%d题，满分100分\n",count);
    double each = 100 / count;
    double grade = 0;
        for(int k=0; k<count; k++){
            printf("%d: ", k+1);
    //产生并打印一个题目
            int OperatorQuantity = rand()%10 + 1;
for(int i=0; i<OperatorQuantity; i++){
        num[i] = GenerateAnOperant();
        op[i] = GenerateOperator();
        PrintFraction(num[i]);
        switch (op[i]) {
            case '+':
                printf(" + ");
                break;
            case '-':
                printf(" - ");
                break;
            case '*':
                printf(" × ");
                break;
            case '/':
                printf(" ÷ ");
                break;
            default:
                break;
        }
    }
    num[OperatorQuantity] = GenerateAnOperant();
    PrintFraction(num[OperatorQuantity]);
    printf(" = ");
    // 求值
    fraction fSt[OperatorQuantity+1];//操作数栈
    int ftop = -1;
    char oSt[OperatorQuantity];//运算符栈
    int otop = -1;
    int fi = 0;//num数组元素下标
    int oi = 0;//op数组元素下标
    fraction temp;//中间运算结果
    fraction finalresult;
    fSt[++ftop] = num[fi++];
    do{
        if (otop == -1) {//若栈空，直接进栈
            oSt[++otop] = op[oi++];
            if (fi < OperatorQuantity + 1) {
                fSt[++ftop] = num[fi++];
            }
        }
        else if(priority(op[oi], oSt[otop]) == 1){//若当前运算符优先级高于栈顶运算符，则操作数栈弹出一个与操作数数组中下一个元素做op[oi]运算
            temp = calculate(op[oi++],fSt[ftop--], num[fi++]);
            fSt[++ftop] = temp;
        }
        else{//若当前运算符优先级不高于栈顶运算符，则弹出两个操作数计算后把结果进栈
            temp = fSt[ftop--];//为了在下一句的参数中少出现自减运算
            temp = calculate(oSt[otop--], fSt[ftop--],temp);
            fSt[++ftop] = temp;
        }
    }while(oi < OperatorQuantity);
    if (fi < OperatorQuantity + 1) {
        fSt[++ftop] = num[fi++];
    }
    while(otop != -1) {
        temp = fSt[ftop--];
        temp = calculate(oSt[otop--], fSt[ftop--], temp);
    }
    finalresult = simplify(temp);
//    printf("%d/%d", finalresult.up, finalresult.down);
    // 判断用户输入正误
    int down, up;
    if (finalresult.down == 1) {
        scanf("%d", &up);
        down = 1;
    }
    else{
        scanf("%d/%d", &up, &down);
    }
    if (finalresult.up == up && finalresult.down == down) {
        printf("正确！\n");
        grade += each;
    }
    else{
        printf("不正确！正确答案 = ");
        PrintFraction(finalresult);
        printf("\n");
    }
 }

    // 输出得分
    printf("本次得分：%.2lf分\n", grade);
    return 0;
}
