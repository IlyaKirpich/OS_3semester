#include <stdio.h>
#include <string.h>

enum status_codes{
    ok = 0,
    wrong_flag = -1,
    wrong_mask = -2,
    wrong_amount_arguments = -3,
    wrong_file = -4
};

int flag_validation(char* flag){
    if (strlen(flag) == 4){
        if (strcmp(flag, "xor8") == 0) return ok;
        if (strcmp(flag, "mask") == 0) return ok;
        return wrong_flag;
    }
    if (strlen(flag) == 5){
        if (strcmp(flag, "xor32") == 0) return ok;
    }
    return wrong_flag;
}

int xor8(FILE* file){
    unsigned char buffer[1];
    unsigned char result = 0;
    while (fread(buffer, sizeof(unsigned char), 1, file))
        result = result ^ buffer[0];
    return result;
}

int xor32(FILE* file){
    unsigned char array[4] = {0, 0, 0, 0};
    unsigned char buffer[1];
    int byte_flag = 0;
    while (fread(buffer, sizeof(unsigned char), 1, file)){
        array[byte_flag] = array[byte_flag] ^ buffer[0];
        byte_flag = (byte_flag + 1) % 4;
    }
     long long int result = 0;
    for(int i = 0; i < 4; i++){
        result = result * 128 + array[i];
    }
    return result;
}

int mask_comparison(FILE* file, unsigned char* mask){
    unsigned char buffer[4];
    int result = 0;
    int byte_flag = 0;
    int equal_flag = 0;
    while (fread(buffer, sizeof(unsigned char), 4, file) == 4){
        for (int i = 0; i < 4; i++){
            if (buffer[i] / 16 == mask[2*i] && buffer[i] % 16 == mask[2*i + 1])
                equal_flag++;
            else equal_flag = 0;
        }
        if (equal_flag == 4) result++;
        equal_flag = 0;
    }
    return result;
}

int mask_to_array(unsigned char* mask, char* argv){
    int mask_length = strlen(argv);
    if (mask_length != 8) return wrong_mask;
    for (int i = 0; i < mask_length; i++){
        if (argv[i] >= '0' && argv[i] < '9') mask[i] = argv[i] - '0';
        else if (argv[i] >= 'A' && argv[i] <= 'F') mask[i] = argv[i] - 'A' + 10;
        else return wrong_mask;
    }
    return ok;
}

int arguments_amount_validation(int argc, char flag){
    if (flag == 'k' && argc == 4)
        return ok;
    if (argc != 3)
        return wrong_amount_arguments;
    return ok;
}

int main(int argc, char* argv[]){
    if(argc < 3){
        printf("Wrong amount of arguments");
        return wrong_amount_arguments;
    }
    if (flag_validation(argv[2]) != ok){
        printf("Wrong flag detected");
        return wrong_flag;
    }
    char flag = argv[2][3];
    printf("%d ", argc);
    if (arguments_amount_validation(argc, flag) != ok){
        printf("Wrong amount of arguments");
        return wrong_amount_arguments;
    }
    FILE* file = fopen(argv[1], "rb");
    if (!file){
        printf("No such file or directory");
        return wrong_file;
    }
    unsigned char mask[8];
    switch (flag) {
    case '8':
        printf("%d", xor8(file));
        break;
    case '3':
        printf("%d", xor32(file));
        break;
    case 'k':
        if (mask_to_array(mask, argv[3]) == wrong_mask){
            printf("Wrong mask detected");
            fclose(file);
            return wrong_mask;
        }
        printf("%d",mask_comparison(file, mask));
        break;
    }
    return ok;
}