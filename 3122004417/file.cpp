#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 10000 // 最大文件长度

// 读取文件内容
char* read_file(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        printf("无法打开文件: %s\n", file_path);
        exit(1);
    }

    fseek(file, 0, SEEK_END);// 将文件指针移动到文件末尾
    long file_size = ftell(file);// 获取文件大小
    fseek(file, 0, SEEK_SET);// 将文件指针移动回文件开头

    char* content = (char*)malloc((file_size + 1) * sizeof(char));
    if (content == NULL) {
        printf("内存分配失败\n");
        exit(1);
    }

    size_t bytes_read = fread(content, sizeof(char), file_size, file);// 读取文件内容到缓冲区
    if (bytes_read != file_size) {
        printf("读取文件失败\n");
        exit(1);
    }
    content[file_size] = '\0';// 在文件末尾添加空字符，以便字符串结束

    fclose(file);
    return content;
}

// 分割中文文本成词语
char** tokenize(const char* text, int* num_tokens) {
    char* copy = strdup(text);
    if (copy == NULL) {
        printf("内存分配失败\n");
        exit(1);
    }

    char* token = strtok(copy, " \t\n\r,.;!?，。；！？");// 使用分隔符分割文本成词语
    int count = 0;// 初始化词语计数器
    char** tokens = NULL;// 初始化词语数组

    while (token != NULL) {// 遍历分割后的词语
        char** temp =(char**) realloc(tokens, (count + 1) * sizeof(char*));
        if (temp == NULL) {
            printf("内存分配失败\n");
            exit(1);
        }
        tokens = temp;// 更新词语数组指针

        tokens[count] = strdup(token);// 复制词语到词语数组中
        if (tokens[count] == NULL) {
            printf("内存分配失败\n");
            exit(1);
        }
        
        count++;// 增加词语计数器
        token = strtok(NULL, " \t\n\r,.;!?，。；！？");// 继续分割文本
    }

    free(copy);
    *num_tokens = count;
    return tokens;
}

// 计算相似度
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
    if (argc != 4) { // 检查命令行参数数量
        printf("Usage: %s <original_file_path> <copied_file_path> <output_file_path>\n", argv[0]);
        return 1;
    }

    const char* original_file_path = argv[1];// 原文文件路径
    const char* copied_file_path = argv[2];// 抄袭版文件路径
    const char* output_file_path = argv[3];// 输出文件路径

    char* original_text = read_file(original_file_path);// 读取原文文件内容
    char* copied_text = read_file(copied_file_path);// 读取抄袭版文件内容

    int original_num_tokens, copied_num_tokens;
    char** original_tokens = tokenize(original_text, &original_num_tokens);
    char** copied_tokens = tokenize(copied_text, &copied_num_tokens);
    
    // 计算相似度
    float similarity = calculate_similarity(original_tokens, original_num_tokens, copied_tokens, copied_num_tokens);

    FILE* output_file = fopen(output_file_path, "w");
    if (output_file == NULL) {
        printf("无法打开输出文件\n");
        return 1;
    }

    fprintf(output_file, "重复率：%.2f%%\n", similarity);// 将相似度写入输出文件

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
