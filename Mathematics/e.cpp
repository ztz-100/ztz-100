#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <string.h>
#define SUCCESS 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef struct variable
{
	int num_or_Symbol;  //0是数字，1是符号 
	int Symbol;
	int numer;         //分子 
	int Den;           //分母 
	int num;           //系数 
	struct variable *next;
}var;

void Free(var* head){
	while(head->next!=NULL){
		var* temp=head;
		head=head->next;
		free(temp);
	}
	free(head);
}

void printvar(var* h){
    printf("%d %d %d %d %d\n", h->num_or_Symbol, h->Symbol, h->numer, h->Den,h->num);
}
//输出链表内容 
void Print(var* head){
    printf("--------\n");
    while(head->next!=NULL){
        printvar(head->next);
        head=head->next;
    }
    printf("--------\n");
}
void expressionToString(var* node, char* buffer)
{
    while (node != NULL)
    {
        if (node->num_or_Symbol == 0) // 如果是数字节点
        {
            // 如果系数不为 1，添加系数
            if (node->Den==0){
            	sprintf(buffer, "%s%d", buffer, node->num);
			}
			else if(node->num == 0 && node->Den!=0){
				sprintf(buffer, "%s%d/%d", buffer, node->numer, node->Den);
			}else{
				sprintf(buffer, "%s%d", buffer, node->num);
				sprintf(buffer, "%s'", buffer);
				sprintf(buffer, "%s%d/%d", buffer, node->numer, node->Den);
			}
            // 添加分数
        }
        else // 如果是符号节点
        {
            switch (node->Symbol)
            {
            case 0: // '+'
                sprintf(buffer, "%s+", buffer);
                break;
            case 1: // '-'
                sprintf(buffer, "%s-", buffer);
                break;
            case 2: // '*'
                sprintf(buffer, "%s*", buffer);
                break;
            case 3: // '/'
                sprintf(buffer, "%s%%", buffer);
                break;
            case 4: // '('
                sprintf(buffer, "%s(", buffer);
                break;
            case 5: // ')'
                sprintf(buffer, "%s)", buffer);
                break;
            default:
                break;
            }
        }
        node = node->next;
    }
}

// 输出算式形式
void printExpression(var* head)
{
    char buffer[100]; // 用于存储算式字符串
    buffer[0] = '\0'; // 确保字符串为空

    expressionToString(head->next, buffer); // 将链表节点转换为算式字符串

    printf("Expression: %s\n", buffer); // 打印算式字符串
}
//随机生成min到max的整数
int getRandom(int min ,int max)
{
//	Sleep(10);
//	srand(time(NULL));
    return rand() % (max + 1 - min) + min;
}

// 生成随机数
int getnum(int max ,var *v)
{
    v->num_or_Symbol = 0;  // 数字
    v->Symbol = -1;        // 无符号
    v->Den = 0;  // 随机生成分母
    v->numer = 0;  // 随机生成分子
    v->num = getRandom(0, max - 1);  // 随机生成分数前的系数
}

// 生成随机分数
void getFraction(int max ,var *v)
{
    v->num_or_Symbol = 0;  // 数字
    v->Symbol = -1;
    v->Den = getRandom(2, max);  // 随机生成分母
    v->numer = getRandom(1, v->Den - 1);  // 随机生成分子
    v->num = getRandom(0, max - 1);  // 随机生成分数前的系数
}

// 生成随机运算符
void getRandomOperator(var *v)
{
    v->num_or_Symbol = 1;  // 符号
    v->Symbol = getRandom(0, 3); // 0-3符号{+,-,*,/}
    v->Den = 0;  // 随机生成分母
    v->numer = 0;  // 随机生成分子
    v->num = 0;  // 随机生成分数前的系数
}
//随机生成括号
#define L 4
#define R 5
void getparenthesis(var *temp,int flag){
    
    temp->num_or_Symbol=1;
    temp->Symbol=flag;
    temp->Den=0;
    temp->numer=0;
    temp->num=0;
}

// 生成带括号的四则运算表达式
void generateExpression(var* head, int max)
{
    int length = getRandom(2, 4);  //生成的数字个数
    var *p = head;
    for(int i = 0; i < length; i++){
        var *NumNode;
        NumNode = (var*)malloc(sizeof(var));
        if(getRandom(0 ,2)==0){
            getFraction(max,NumNode);
        }else{
            getnum(max,NumNode);
        }
        p->next = NumNode;
        p = NumNode;
        NumNode->next = NULL;
    }

    p=head;
    if(length >= 3){
    	int left = getRandom(1, length-1);
		int right = getRandom(left+1, length);
		var *templ;
		templ=(var *) malloc(sizeof(var));
		getparenthesis(templ, L);
		for(int i = 1; i < left; i++){
			p = p->next;
		}
		templ->next = p->next;
	    p->next = templ;
	    p = templ->next;
	    p=head->next;
	    var *tempr;
	    tempr=(var *) malloc(sizeof(var));
	    getparenthesis(tempr, R);
	    for(int i = 0; i < right; i++){
			p = p->next;
		}
		tempr->next = p->next;
	    p->next = tempr;
	    p = tempr->next;
	}
	
    p=head->next;
    while(p->next!=NULL){
        if((p->num_or_Symbol==0&&p->next->num_or_Symbol==0) || (p->num_or_Symbol==0&&p->next->Symbol==4) || (p->Symbol==5&&p->next!=NULL&&p->next->num_or_Symbol==0))
        {
            var *SymbolNode;
            SymbolNode = (var*)malloc(sizeof(var));
            getRandomOperator(SymbolNode);
            SymbolNode->next=p->next;
            p->next=SymbolNode;
            p= SymbolNode;
        }
        p=p->next;
    }
}


//比较运算符优先级 
int prio(var* node) {
    if (node->Symbol == 0 || node->Symbol == 1) // '+' 或 '-'
        return 1;
    else if (node->Symbol == 2 || node->Symbol == 3) // '*' 或 '÷'
        return 2;
    else // '(' 或 ')'
        return 0;
}


Status Infix_to_Postfix(var *p, var *Postfix) {
    var* stack = NULL; // 定义一个栈，用于存储运算符
    var* q = p->next; // 中缀表达式
    Postfix->next = NULL;
    // 遍历中缀表达式的每个元素
    while (q != NULL) {
        var* v = q;
        q = q->next;
        if (v->num_or_Symbol == 0) { // 如果是数字
            // 将数字放入后缀表达式中
            v->next = Postfix->next;
            Postfix->next = v;
        }
        else if (v->num_or_Symbol == 1 && v->Symbol == 4) { // 如果是左括号
            // 将左括号入栈
            v->next = stack;
            stack = v;
        }
        else if (v->num_or_Symbol == 1 && v->Symbol == 5) { // 如果是右括号
            // 循环直到遇到左括号或栈为空
            while (stack->Symbol != 4 && stack != NULL) {
                var* s = stack;
                stack = stack->next; // 将栈顶元素出栈
                // 将栈顶元素放入后缀表达式中
                s->next = Postfix->next;
                Postfix->next = s;
            }
            if (stack == NULL) { // 如果栈为空，说明缺少左括号
                free(stack); // 释放栈内存
                return ERROR;
            }
            stack = stack->next; // 将左括号出栈
            free(v); // 释放右括号节点内存
        }
        else { // 如果是运算符
            // 当前运算符优先级小于等于栈顶运算符优先级时，将栈顶元素出栈并放入后缀表达式中
            while (stack != NULL && prio(v) <= prio(stack)) {
                var* s = stack;
                stack = stack->next; // 将栈顶元素出栈
                // 将栈顶元素放入后缀表达式中
                s->next = Postfix->next;
                Postfix->next = s;
            }
            // 将当前运算符入栈
            v->next = stack;
            stack = v;
        }
    }

    // 将栈中剩余的运算符依次出栈并放入后缀表达式中
    while (stack != NULL) {
        var* s = stack;
        stack = stack->next;
        s->next = Postfix->next;
        Postfix->next = s;
    }
        // 此处反转Postfix链表
   /* var *prev = NULL;
    var *current = Postfix->next;//lambad
    var *next = NULL;
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }*/
    	var *l,*k;
	l = Postfix -> next;
	Postfix -> next = NULL;
	while(l != NULL){
		k = l->next;
		l->next = Postfix->next;
		Postfix->next = l;
		l = k;
	}
  //  Postfix->next = prev; // 更新头节点的指针
    return SUCCESS;
}

// 求最大公约数
int gcd(int a, int b) {
	return !b ? a : gcd(b, a % b);	
}

//通分 
void preprocess(var *a, var *b)
{
	//将前面的系数加进分母
	if (a->Den == 0){
		a->numer = a->num;
		a->Den = 1;
		a->num = 0;
	}
	else if(a->num != 0){
		a->numer += a->Den*a->num;
		a->num = 0;
	}
	if (b->Den == 0){
		b->numer = b->num;
		b->Den = 1;
		b->num = 0;
	}
	else if(b->num != 0){
		b->numer += b->Den*b->num;
		b->num = 0;
	}
	//分母通分
	a->numer *= b->Den;
	b->numer *= a->Den;
	a->Den = b->Den = a->Den * b->Den;
}

//计算 
Status operate(var *a, var *b, var *oper, var *redu)
{
	preprocess(a, b);
	//判断执行的操作
	if (oper->Symbol == 0)//+
	{
		redu->numer=a->numer + b->numer;
		redu->Den = a->Den;
	}
	else if (oper->Symbol == 1)//-
	{
		redu->numer = a->numer - b->numer;
		redu->Den = a->Den;
	}
	else if (oper->Symbol == 2)// *
	{
		redu->numer = a->numer * b->numer;
		redu->Den = a->Den * b->Den;
	}
	else if (oper->Symbol == 3)// ÷
	{
		redu->numer = a->numer * b->Den;
		redu->Den = a->Den * b->numer;
	}
	//printf("%d %d/%d\n",redu->num ,redu->numer, redu->Den);
	if (redu->numer < 0 || redu->Den == 0)//计算过程中出现了负数 || 计算过程中出现了除 0 的非法操作
	{
		return ERROR;
	}
	//化简
	if(redu->numer == 0){
		redu->Den = 0;
		redu->num = 0;
	}
	else{
		int d = gcd(redu->numer, redu->Den);
	    redu->numer /= d;
		redu->Den /= d;
	    redu->num = redu->numer / redu->Den;
	    redu->numer %= redu->Den;
	    if(redu->numer == 0) redu->Den = 0;
    }
	//printf("%d %d/%d\n",redu->num ,redu->numer, redu->Den);
	return SUCCESS;
}
	
// 计算后缀表达式
Status calc(var* Postfix, var* ans) {
    var* stack = NULL; // 定义一个栈，用于存放操作数
    var* postfix = Postfix->next;
    while (postfix != NULL) { // 遍历后缀表达式
        var* v = postfix;
        postfix = postfix->next;
        if (v->num_or_Symbol == 0) { // 如果是操作数
            v->next = stack;
            stack = v; // 将操作数压入栈中
        } else { // 如果是运算符
            if (stack == NULL || stack->next == NULL) { // 判断操作数是否足够两个，如果不足则报错
                printf("发生错误，操作数不足，无法完成运算\n");
                return ERROR;
            }

            var* ret;
			ret = (var*)malloc(sizeof(var)); // 用于存储计算结果的节点
            if (ret == NULL) 
				return OVERFLOW; 

            // 从栈中弹出两个操作数
            var* operand2 = stack;
            stack = stack->next;
            var* operand1 = stack;
            stack = stack->next;

            // 执行运算操作
            if (!operate(operand1, operand2, v, ret)) { // 如果计算过程中出现了负数或除以零的非法操作
           // printf("计算过程中出现了负数或除以零的非法操作");
                return ERROR; // 返回错误 
            }
            // 将计算结果压入栈中
            ret->next = stack;
            stack = ret;
        }
    }
    	if (stack->next == NULL) // 最后栈中只剩下一个元素，即最终的计算结果
	{
		stack->num_or_Symbol = 0;
		ans->next = stack; // 将最终的计算结果存储到 ans 中
		return SUCCESS; // 返回成功标志
	}
	printf("计算结果错误\n");
	return ERROR;
}
//将题目答案输出到文件 
int M_instructions(int amount,int max) {
	FILE *fpexe;
    FILE *fpgrade;
    fpexe = fopen("Exercises.txt", "w");
    fpgrade = fopen("Gradefile.txt", "w");
    if (fpgrade == NULL) {
    	printf("无法生成文件。\n");
        return ERROR; 
    }
    if (fpexe == NULL) {
        perror("无法生成文件。\n");
        return ERROR;
    }
	srand(time(NULL));
	int i = 0;
	char res[amount][100];
	while(i<amount){
    var *infixExpression;  // 中缀表达式链表的头指针
    var *postfixExpression;  // 后缀表达式链表的头指针
    var *result;  // 存储计算结果的链表的头指针

    // 分配内存空间
    infixExpression = (var *)malloc(sizeof(var));
    postfixExpression = (var *)malloc(sizeof(var));
    postfixExpression->next = NULL;
    result = (var *)malloc(sizeof(var));
    result->next = NULL;
    if (infixExpression == NULL || postfixExpression == NULL || result == NULL) {
        printf("内存分配失败\n");
        return -1;
    }
	    infixExpression->next = NULL;
	    infixExpression->num_or_Symbol = -1; 
	    generateExpression(infixExpression, max);  // 生成表达式
	   char infix[100];
	   infix[0] = '\0';
	    expressionToString(infixExpression->next, infix);
    if (Infix_to_Postfix(infixExpression, postfixExpression) == ERROR) {
        //printf("中缀表达式转后缀表达式失败\n");
        //return -1;
        continue;
    }

    // 计算后缀表达式
    if (calc(postfixExpression, result) == ERROR) {
        //printf("计算后缀表达式失败\n");
        //return -1;
        continue;
    }
    
    res[i][0] = '\0';
    expressionToString(result->next, res[i]);
    int j = 0;
	while(j<i){
		if(strcmp(res[j],res[i]) == 0) break;
			j++;
	}
	if(j<i) continue;
    printf("Expression: %s\n", infix);
    printf("计算结果: %s\n\n",res[i]);
    fprintf(fpgrade, "%d:%s \n",i+1,res[i]);
    fprintf(fpexe, "%d:%s=\n",i+1,infix);
    // 释放动态分配的内存
    // 请确保在程序结束时释放内存，避免内存泄漏
    free(infixExpression);
    free(postfixExpression);
    free(result);
    i++;
}
    fclose(fpexe);
    fclose(fpgrade);
    return SUCCESS;

}

//新建链表结点 ，返回这个结点的指针 
var* newNode(int num_or_Symbol, int Symbol, int num, int numer, int Den) {
    var* node = (var*)malloc(sizeof(var));
    if (node != NULL) {
        node->num_or_Symbol = num_or_Symbol;
        node->Symbol = Symbol;
        node->numer = numer;
        node->Den = Den;
        node->num = num;
        node->next = NULL;
    }
    return node;
}

// 将表达式从字符串转换为链表表示
var* expressionToList(char* expression) {
    var* head = newNode(-1, -1, -1, -1, -1);  // 头节点
    var* current = head;
    int len = strlen(expression);
    int i = 0;
    while (i < len) {
        if (expression[i] >= '0' && expression[i] <= '9') {
            // 数字
            int j = i;
            int num = 0;
            int numer = 0;
            int Den = 0;
            int n = 0;
            // 解析整数部分
            while (expression[j] >= '0' && expression[j] <= '9') {
                n = n * 10 + expression[j] - '0';
                j++;
            }
            num = n;
            // 如果后面有带分数部分
            if (expression[j] == '\'') {
                j++; // 跳过分数符号
                n = 0;
                // 解析分子部分
                while (expression[j] >= '0' && expression[j] <= '9') {
                    n = n * 10 + expression[j] - '0';
                    j++;
                }
                numer = n;
                // 解析分母部分
                if (expression[j] == '/') {
                    j++; // 跳过分数线
                    n = 0;
                    // 解析分母
                    while (expression[j] >= '0' && expression[j] <= '9') {
                        n = n * 10 + expression[j] - '0';
                        j++;
                    }
                    Den = n;
                }
            }
            // 如果后面有真分数部分
            else if (expression[j] == '/') {
                j++; // 跳过分数线
                numer = num;
                num = 0;
                n = 0;
                // 解析分子
                while (expression[j] >= '0' && expression[j] <= '9') {
                    n = n * 10 + expression[j] - '0';
                    j++;
                }
                Den = n;
        		//解析分母 
            }
            // 创建节点并连接链表
            current->next = newNode(0, -1, num, numer, Den);
            current = current->next;
            i = j - 1; // 更新索引
        } else if (expression[i] == '+') {
            current->next = newNode(1, 0, -1, -1, -1);
            current = current->next;
        } else if (expression[i] == '-') {
            current->next = newNode(1, 1, -1, -1, -1);
            current = current->next;
        } else if (expression[i] == '*') {
            current->next = newNode(1, 2, -1, -1, -1);
            current = current->next;
        } else if (expression[i] == '%') {
            current->next = newNode(1, 3, -1, -1, -1);
            current = current->next;
        } else if (expression[i] == '(') {
            current->next = newNode(1, 4, -1, -1, -1);
            current = current->next;
        } else if (expression[i] == ')') {
            current->next = newNode(1, 5, -1, -1, -1);
            current = current->next;
        }
        i++; 
    }
    return head;
}
// 比较两个答案是否相同
int compareNodes(var* list1, var* list2) {
    // 检查链表是否为空
    if (list1 == NULL || list2 == NULL || list1->next == NULL || list2->next == NULL) {
        return 0; // 如果任何一个链表为空或者链表只有一个节点，则不相同
    }

    // 检查第二个节点的值是否相同
    if (list1->next->num != list2->next->num ||
        list1->next->numer != list2->next->numer ||
        list1->next->Den != list2->next->Den) {
        	if(list1->next->num != list2->next->num && list1->next->numer == 0 && list1->next->numer == list2->next->numer){
        		return 1;//特殊情况：两个数系数相同且分子都为0 
			}
        return 0; // 如果有节点的值不同，则不相同
    }

    // 链表相同
    return 1;
}
//文件答案比对，结果打印到文件中 
Status  answerdetection(){
	FILE *fpexe;
    FILE *fpgrade;
    char line[1000]; // 假设最大行长度为100个字符
    fpexe = fopen("Exercises.txt", "r");
    fpgrade = fopen("Grade.txt", "w");
    if (fpgrade == NULL) {
    	printf("无法生成文件。\n");
        return ERROR; 
    }
    if (fpexe == NULL) {
        perror("无法打开文件。\n");
        return ERROR;
    }
    int k = 1; 
    int CorrectAndWrong[10002];
    int c = 0;
    int w = 10001;
    while (fgets(line, sizeof(line), fpexe) != NULL) {
    	char equation[100];
    	char ans[50];
        int i = 0;
        while(line[i]!=':'){
        	i++;
		}
		i++;
		int j = 0;
		while(line[i]!='='){
			equation[j] = line[i];
			j++;
        	i++;
		}
		equation[j] = '\0';
		i++;
		j = 0;
		while(i < strlen(line)+1){
			ans[j] = line[i];
			j++;
        	i++;
		}
		ans[j] = '\0';
		//(expressionToList(ans));
		var *postfixExpression;
		var *result;
    	postfixExpression = (var *)malloc(sizeof(var));
    	postfixExpression->next = NULL;
    	result = (var *)malloc(sizeof(var));
    	result->Symbol = -1;
    	result->next = NULL;
    	
    	if (Infix_to_Postfix(expressionToList(equation), postfixExpression) == ERROR) {
        printf("中缀表达式转后缀表达式失败\n");
        return ERROR;
	    }
	    // 计算后缀表达式
	    if (calc(postfixExpression, result) == ERROR) {
	        printf("计算后缀表达式失败\n");
	        return ERROR;
	    }
//	     打印计算结果
	    if(compareNodes(expressionToList(ans), result)==1){
			CorrectAndWrong[c] = k;
			c++;
		}else{
			CorrectAndWrong[w] = k;
			w--;
		}
		k++;
		// 释放动态分配的内存// 请确保在程序结束时释放内存，避免内存泄漏
	    free(postfixExpression);
	    free(result);	// 在每一行中进行以 "=" 为分隔符的拆分
	}
	fprintf(fpgrade, "Correct: %d (",c);
	for(int i = 0; i < c; i++){
		fprintf(fpgrade,"%d ",CorrectAndWrong[i]);
	}    
	fprintf(fpgrade,")\n");
	fprintf(fpgrade, "Wrong: %d (",10001-w);
	for(int i = 10001; i > w; i--){
		fprintf(fpgrade,"%d ",CorrectAndWrong[i]);
	}    
	fprintf(fpgrade,")\n");
    fclose(fpexe);
    fclose(fpgrade);
    return SUCCESS;
}
int main(int argc, char *argv[]){
	if (argc != 5) {/// 检查命令行参数数量
        printf("Usage: %s -n <number> -r <number>\n", argv[0]);
        return 1;
    }
	int max =atoi(argv[4]);
	int amount = atoi(argv[2]);
	int i;
	printf("-----------------------------\n");
	printf("\t1.创建题目\n");
	printf("\t2.判断题目对错\n");
	printf("-----------------------------\n");
	printf("输入: ");
	scanf("%d",&i);
	printf("\n"); 
    switch(i){
    	case 1:
    		if(M_instructions(amount,max)) printf("创建题目成功\n");
		    break;
		case 2:
			if(answerdetection() == SUCCESS)   printf("判断完成\n");
	        break;
	}
	return 0;
}
