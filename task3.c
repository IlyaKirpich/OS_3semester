#include <stdio.h>

enum status_codes{
    ok = 0,
    file_error = -1,
    arguments_error = -2
};

int copy_file(char* file_input, char* file_output){
    FILE* in = fopen(file_input, "rb");
    if (!in) return file_error;
    FILE* out = fopen(file_output, "wb");
    if (!out){
        fclose(in);
        return file_error;
    }
    unsigned char buffer[1];
    while (fread(buffer, sizeof(unsigned char), 1, in)){
        if(fwrite(buffer, sizeof(unsigned char), 1, out) != 1){
            fclose(in);
            fclose(out);
            return file_error;
        }
    }
    fclose(in);
    fclose(out);
    return ok;
}

int main(int argc, char* argv[]){
    if (argc != 3){
        printf("Wrong amount of arguments");
        return arguments_error;
    }
    if(copy_file(argv[1], argv[2]) != ok){
        printf("Error file");
        return file_error;
    }
    return ok;
}