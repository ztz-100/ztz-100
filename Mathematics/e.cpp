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
	int num_or_Symbol;  //0�����֣�1�Ƿ��� 
	int Symbol;
	int numer;         //���� 
	int Den;           //��ĸ 
	int num;           //ϵ�� 
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
//����������� 
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
        if (node->num_or_Symbol == 0) // ��������ֽڵ�
        {
            // ���ϵ����Ϊ 1�����ϵ��
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
            // ��ӷ���
        }
        else // ����Ƿ��Žڵ�
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

// �����ʽ��ʽ
void printExpression(var* head)
{
    char buffer[100]; // ���ڴ洢��ʽ�ַ���
    buffer[0] = '\0'; // ȷ���ַ���Ϊ��

    expressionToString(head->next, buffer); // ������ڵ�ת��Ϊ��ʽ�ַ���

    printf("Expression: %s\n", buffer); // ��ӡ��ʽ�ַ���
}
//�������min��max������
int getRandom(int min ,int max)
{
//	Sleep(10);
//	srand(time(NULL));
    return rand() % (max + 1 - min) + min;
}

// ���������
int getnum(int max ,var *v)
{
    v->num_or_Symbol = 0;  // ����
    v->Symbol = -1;        // �޷���
    v->Den = 0;  // ������ɷ�ĸ
    v->numer = 0;  // ������ɷ���
    v->num = getRandom(0, max - 1);  // ������ɷ���ǰ��ϵ��
}

// �����������
void getFraction(int max ,var *v)
{
    v->num_or_Symbol = 0;  // ����
    v->Symbol = -1;
    v->Den = getRandom(2, max);  // ������ɷ�ĸ
    v->numer = getRandom(1, v->Den - 1);  // ������ɷ���
    v->num = getRandom(0, max - 1);  // ������ɷ���ǰ��ϵ��
}

// ������������
void getRandomOperator(var *v)
{
    v->num_or_Symbol = 1;  // ����
    v->Symbol = getRandom(0, 3); // 0-3����{+,-,*,/}
    v->Den = 0;  // ������ɷ�ĸ
    v->numer = 0;  // ������ɷ���
    v->num = 0;  // ������ɷ���ǰ��ϵ��
}
//�����������
#define L 4
#define R 5
void getparenthesis(var *temp,int flag){
    
    temp->num_or_Symbol=1;
    temp->Symbol=flag;
    temp->Den=0;
    temp->numer=0;
    temp->num=0;
}

// ���ɴ����ŵ�����������ʽ
void generateExpression(var* head, int max)
{
    int length = getRandom(2, 4);  //���ɵ����ָ���
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


//�Ƚ���������ȼ� 
int prio(var* node) {
    if (node->Symbol == 0 || node->Symbol == 1) // '+' �� '-'
        return 1;
    else if (node->Symbol == 2 || node->Symbol == 3) // '*' �� '��'
        return 2;
    else // '(' �� ')'
        return 0;
}


Status Infix_to_Postfix(var *p, var *Postfix) {
    var* stack = NULL; // ����һ��ջ�����ڴ洢�����
    var* q = p->next; // ��׺���ʽ
    Postfix->next = NULL;
    // ������׺���ʽ��ÿ��Ԫ��
    while (q != NULL) {
        var* v = q;
        q = q->next;
        if (v->num_or_Symbol == 0) { // ���������
            // �����ַ����׺���ʽ��
            v->next = Postfix->next;
            Postfix->next = v;
        }
        else if (v->num_or_Symbol == 1 && v->Symbol == 4) { // �����������
            // ����������ջ
            v->next = stack;
            stack = v;
        }
        else if (v->num_or_Symbol == 1 && v->Symbol == 5) { // �����������
            // ѭ��ֱ�����������Ż�ջΪ��
            while (stack->Symbol != 4 && stack != NULL) {
                var* s = stack;
                stack = stack->next; // ��ջ��Ԫ�س�ջ
                // ��ջ��Ԫ�ط����׺���ʽ��
                s->next = Postfix->next;
                Postfix->next = s;
            }
            if (stack == NULL) { // ���ջΪ�գ�˵��ȱ��������
                free(stack); // �ͷ�ջ�ڴ�
                return ERROR;
            }
            stack = stack->next; // �������ų�ջ
            free(v); // �ͷ������Žڵ��ڴ�
        }
        else { // ����������
            // ��ǰ��������ȼ�С�ڵ���ջ����������ȼ�ʱ����ջ��Ԫ�س�ջ�������׺���ʽ��
            while (stack != NULL && prio(v) <= prio(stack)) {
                var* s = stack;
                stack = stack->next; // ��ջ��Ԫ�س�ջ
                // ��ջ��Ԫ�ط����׺���ʽ��
                s->next = Postfix->next;
                Postfix->next = s;
            }
            // ����ǰ�������ջ
            v->next = stack;
            stack = v;
        }
    }

    // ��ջ��ʣ�����������γ�ջ�������׺���ʽ��
    while (stack != NULL) {
        var* s = stack;
        stack = stack->next;
        s->next = Postfix->next;
        Postfix->next = s;
    }
        // �˴���תPostfix����
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
  //  Postfix->next = prev; // ����ͷ�ڵ��ָ��
    return SUCCESS;
}

// �����Լ��
int gcd(int a, int b) {
	return !b ? a : gcd(b, a % b);	
}

//ͨ�� 
void preprocess(var *a, var *b)
{
	//��ǰ���ϵ���ӽ���ĸ
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
	//��ĸͨ��
	a->numer *= b->Den;
	b->numer *= a->Den;
	a->Den = b->Den = a->Den * b->Den;
}

//���� 
Status operate(var *a, var *b, var *oper, var *redu)
{
	preprocess(a, b);
	//�ж�ִ�еĲ���
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
	else if (oper->Symbol == 3)// ��
	{
		redu->numer = a->numer * b->Den;
		redu->Den = a->Den * b->numer;
	}
	//printf("%d %d/%d\n",redu->num ,redu->numer, redu->Den);
	if (redu->numer < 0 || redu->Den == 0)//��������г����˸��� || ��������г����˳� 0 �ķǷ�����
	{
		return ERROR;
	}
	//����
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
	
// �����׺���ʽ
Status calc(var* Postfix, var* ans) {
    var* stack = NULL; // ����һ��ջ�����ڴ�Ų�����
    var* postfix = Postfix->next;
    while (postfix != NULL) { // ������׺���ʽ
        var* v = postfix;
        postfix = postfix->next;
        if (v->num_or_Symbol == 0) { // ����ǲ�����
            v->next = stack;
            stack = v; // ��������ѹ��ջ��
        } else { // ����������
            if (stack == NULL || stack->next == NULL) { // �жϲ������Ƿ��㹻��������������򱨴�
                printf("�������󣬲��������㣬�޷��������\n");
                return ERROR;
            }

            var* ret;
			ret = (var*)malloc(sizeof(var)); // ���ڴ洢�������Ľڵ�
            if (ret == NULL) 
				return OVERFLOW; 

            // ��ջ�е�������������
            var* operand2 = stack;
            stack = stack->next;
            var* operand1 = stack;
            stack = stack->next;

            // ִ���������
            if (!operate(operand1, operand2, v, ret)) { // �����������г����˸����������ķǷ�����
           // printf("��������г����˸����������ķǷ�����");
                return ERROR; // ���ش��� 
            }
            // ��������ѹ��ջ��
            ret->next = stack;
            stack = ret;
        }
    }
    	if (stack->next == NULL) // ���ջ��ֻʣ��һ��Ԫ�أ������յļ�����
	{
		stack->num_or_Symbol = 0;
		ans->next = stack; // �����յļ������洢�� ans ��
		return SUCCESS; // ���سɹ���־
	}
	printf("����������\n");
	return ERROR;
}
//����Ŀ��������ļ� 
int M_instructions(int amount,int max) {
	FILE *fpexe;
    FILE *fpgrade;
    fpexe = fopen("Exercises.txt", "w");
    fpgrade = fopen("Gradefile.txt", "w");
    if (fpgrade == NULL) {
    	printf("�޷������ļ���\n");
        return ERROR; 
    }
    if (fpexe == NULL) {
        perror("�޷������ļ���\n");
        return ERROR;
    }
	srand(time(NULL));
	int i = 0;
	char res[amount][100];
	while(i<amount){
    var *infixExpression;  // ��׺���ʽ�����ͷָ��
    var *postfixExpression;  // ��׺���ʽ�����ͷָ��
    var *result;  // �洢�������������ͷָ��

    // �����ڴ�ռ�
    infixExpression = (var *)malloc(sizeof(var));
    postfixExpression = (var *)malloc(sizeof(var));
    postfixExpression->next = NULL;
    result = (var *)malloc(sizeof(var));
    result->next = NULL;
    if (infixExpression == NULL || postfixExpression == NULL || result == NULL) {
        printf("�ڴ����ʧ��\n");
        return -1;
    }
	    infixExpression->next = NULL;
	    infixExpression->num_or_Symbol = -1; 
	    generateExpression(infixExpression, max);  // ���ɱ��ʽ
	   char infix[100];
	   infix[0] = '\0';
	    expressionToString(infixExpression->next, infix);
    if (Infix_to_Postfix(infixExpression, postfixExpression) == ERROR) {
        //printf("��׺���ʽת��׺���ʽʧ��\n");
        //return -1;
        continue;
    }

    // �����׺���ʽ
    if (calc(postfixExpression, result) == ERROR) {
        //printf("�����׺���ʽʧ��\n");
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
    printf("������: %s\n\n",res[i]);
    fprintf(fpgrade, "%d:%s \n",i+1,res[i]);
    fprintf(fpexe, "%d:%s=\n",i+1,infix);
    // �ͷŶ�̬������ڴ�
    // ��ȷ���ڳ������ʱ�ͷ��ڴ棬�����ڴ�й©
    free(infixExpression);
    free(postfixExpression);
    free(result);
    i++;
}
    fclose(fpexe);
    fclose(fpgrade);
    return SUCCESS;

}

//�½������� �������������ָ�� 
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

// �����ʽ���ַ���ת��Ϊ�����ʾ
var* expressionToList(char* expression) {
    var* head = newNode(-1, -1, -1, -1, -1);  // ͷ�ڵ�
    var* current = head;
    int len = strlen(expression);
    int i = 0;
    while (i < len) {
        if (expression[i] >= '0' && expression[i] <= '9') {
            // ����
            int j = i;
            int num = 0;
            int numer = 0;
            int Den = 0;
            int n = 0;
            // ������������
            while (expression[j] >= '0' && expression[j] <= '9') {
                n = n * 10 + expression[j] - '0';
                j++;
            }
            num = n;
            // ��������д���������
            if (expression[j] == '\'') {
                j++; // ������������
                n = 0;
                // �������Ӳ���
                while (expression[j] >= '0' && expression[j] <= '9') {
                    n = n * 10 + expression[j] - '0';
                    j++;
                }
                numer = n;
                // ������ĸ����
                if (expression[j] == '/') {
                    j++; // ����������
                    n = 0;
                    // ������ĸ
                    while (expression[j] >= '0' && expression[j] <= '9') {
                        n = n * 10 + expression[j] - '0';
                        j++;
                    }
                    Den = n;
                }
            }
            // ������������������
            else if (expression[j] == '/') {
                j++; // ����������
                numer = num;
                num = 0;
                n = 0;
                // ��������
                while (expression[j] >= '0' && expression[j] <= '9') {
                    n = n * 10 + expression[j] - '0';
                    j++;
                }
                Den = n;
        		//������ĸ 
            }
            // �����ڵ㲢��������
            current->next = newNode(0, -1, num, numer, Den);
            current = current->next;
            i = j - 1; // ��������
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
// �Ƚ��������Ƿ���ͬ
int compareNodes(var* list1, var* list2) {
    // ��������Ƿ�Ϊ��
    if (list1 == NULL || list2 == NULL || list1->next == NULL || list2->next == NULL) {
        return 0; // ����κ�һ������Ϊ�ջ�������ֻ��һ���ڵ㣬����ͬ
    }

    // ���ڶ����ڵ��ֵ�Ƿ���ͬ
    if (list1->next->num != list2->next->num ||
        list1->next->numer != list2->next->numer ||
        list1->next->Den != list2->next->Den) {
        	if(list1->next->num != list2->next->num && list1->next->numer == 0 && list1->next->numer == list2->next->numer){
        		return 1;//���������������ϵ����ͬ�ҷ��Ӷ�Ϊ0 
			}
        return 0; // ����нڵ��ֵ��ͬ������ͬ
    }

    // ������ͬ
    return 1;
}
//�ļ��𰸱ȶԣ������ӡ���ļ��� 
Status  answerdetection(){
	FILE *fpexe;
    FILE *fpgrade;
    char line[1000]; // ��������г���Ϊ100���ַ�
    fpexe = fopen("Exercises.txt", "r");
    fpgrade = fopen("Grade.txt", "w");
    if (fpgrade == NULL) {
    	printf("�޷������ļ���\n");
        return ERROR; 
    }
    if (fpexe == NULL) {
        perror("�޷����ļ���\n");
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
        printf("��׺���ʽת��׺���ʽʧ��\n");
        return ERROR;
	    }
	    // �����׺���ʽ
	    if (calc(postfixExpression, result) == ERROR) {
	        printf("�����׺���ʽʧ��\n");
	        return ERROR;
	    }
//	     ��ӡ������
	    if(compareNodes(expressionToList(ans), result)==1){
			CorrectAndWrong[c] = k;
			c++;
		}else{
			CorrectAndWrong[w] = k;
			w--;
		}
		k++;
		// �ͷŶ�̬������ڴ�// ��ȷ���ڳ������ʱ�ͷ��ڴ棬�����ڴ�й©
	    free(postfixExpression);
	    free(result);	// ��ÿһ���н����� "=" Ϊ�ָ����Ĳ��
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
	if (argc != 5) {/// ��������в�������
        printf("Usage: %s -n <number> -r <number>\n", argv[0]);
        return 1;
    }
	int max =atoi(argv[4]);
	int amount = atoi(argv[2]);
	int i;
	printf("-----------------------------\n");
	printf("\t1.������Ŀ\n");
	printf("\t2.�ж���Ŀ�Դ�\n");
	printf("-----------------------------\n");
	printf("����: ");
	scanf("%d",&i);
	printf("\n"); 
    switch(i){
    	case 1:
    		if(M_instructions(amount,max)) printf("������Ŀ�ɹ�\n");
		    break;
		case 2:
			if(answerdetection() == SUCCESS)   printf("�ж����\n");
	        break;
	}
	return 0;
}
