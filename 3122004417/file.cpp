#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 10000 // ����ļ�����

// ��ȡ�ļ�����
char* read_file(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        printf("�޷����ļ�: %s\n", file_path);
        exit(1);
    }

    fseek(file, 0, SEEK_END);// ���ļ�ָ���ƶ����ļ�ĩβ
    long file_size = ftell(file);// ��ȡ�ļ���С
    fseek(file, 0, SEEK_SET);// ���ļ�ָ���ƶ����ļ���ͷ

    char* content = (char*)malloc((file_size + 1) * sizeof(char));
    if (content == NULL) {
        printf("�ڴ����ʧ��\n");
        exit(1);
    }

    size_t bytes_read = fread(content, sizeof(char), file_size, file);// ��ȡ�ļ����ݵ�������
    if (bytes_read != file_size) {
        printf("��ȡ�ļ�ʧ��\n");
        exit(1);
    }
    content[file_size] = '\0';// ���ļ�ĩβ��ӿ��ַ����Ա��ַ�������

    fclose(file);
    return content;
}

// �ָ������ı��ɴ���
char** tokenize(const char* text, int* num_tokens) {
    char* copy = strdup(text);
    if (copy == NULL) {
        printf("�ڴ����ʧ��\n");
        exit(1);
    }

    char* token = strtok(copy, " \t\n\r,.;!?����������");// ʹ�÷ָ����ָ��ı��ɴ���
    int count = 0;// ��ʼ�����������
    char** tokens = NULL;// ��ʼ����������

    while (token != NULL) {// �����ָ��Ĵ���
        char** temp =(char**) realloc(tokens, (count + 1) * sizeof(char*));
        if (temp == NULL) {
            printf("�ڴ����ʧ��\n");
            exit(1);
        }
        tokens = temp;// ���´�������ָ��

        tokens[count] = strdup(token);// ���ƴ��ﵽ����������
        if (tokens[count] == NULL) {
            printf("�ڴ����ʧ��\n");
            exit(1);
        }
        
        count++;// ���Ӵ��������
        token = strtok(NULL, " \t\n\r,.;!?����������");// �����ָ��ı�
    }

    free(copy);
    *num_tokens = count;
    return tokens;
}

// �������ƶ�
float calculate_similarity(char** original_tokens, int original_num_tokens, char** copied_tokens, int copied_num_tokens) {
    int total_words = original_num_tokens;
    int common_words = 0;

    for (int i = 0; i < original_num_tokens; i++) {
        for (int j = 0; j < copied_num_tokens; j++) {
            if (strcmp(original_tokens[i], copied_tokens[j]) == 0) {
                common_words++;
                break;
            }
        }
    }

    return (float)common_words / total_words * 100;
}

int main(int argc, char* argv[]) {
    if (argc != 4) { // ��������в�������
        printf("Usage: %s <original_file_path> <copied_file_path> <output_file_path>\n", argv[0]);
        return 1;
    }

    const char* original_file_path = argv[1];// ԭ���ļ�·��
    const char* copied_file_path = argv[2];// ��Ϯ���ļ�·��
    const char* output_file_path = argv[3];// ����ļ�·��

    char* original_text = read_file(original_file_path);// ��ȡԭ���ļ�����
    char* copied_text = read_file(copied_file_path);// ��ȡ��Ϯ���ļ�����

    int original_num_tokens, copied_num_tokens;
    char** original_tokens = tokenize(original_text, &original_num_tokens);
    char** copied_tokens = tokenize(copied_text, &copied_num_tokens);
    
    // �������ƶ�
    float similarity = calculate_similarity(original_tokens, original_num_tokens, copied_tokens, copied_num_tokens);

    FILE* output_file = fopen(output_file_path, "w");
    if (output_file == NULL) {
        printf("�޷�������ļ�\n");
        return 1;
    }

    fprintf(output_file, "�ظ��ʣ�%.2f%%\n", similarity);// �����ƶ�д������ļ�

    fclose(output_file);
    free(original_text);
    free(copied_text);

    for (int i = 0; i < original_num_tokens; i++) {
        free(original_tokens[i]);
    }
    free(original_tokens);

    for (int i = 0; i < copied_num_tokens; i++) {
        free(copied_tokens[i]);
    }
    free(copied_tokens);

    return 0;
}
