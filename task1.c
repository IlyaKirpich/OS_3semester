#include <stdio.h>

enum status_codes{
    ok = 0,
    wrong_file = -1,
    file_write_error = -2
};

int file_write(char* filename){
    FILE* file = fopen(filename, "wb");
    if (!file) return wrong_file;
    unsigned char sequence[11] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    int count = fwrite(sequence, sizeof(unsigned char), 11, file);
    fclose(file);
    if (count != 11) return file_write_error;
    return ok;
}

int byte_read(char* filename){
    FILE* file = fopen(filename, "rb");
    if (!file) return wrong_file;
    unsigned char buffer;
    while (fread(&buffer, sizeof(unsigned char), 1, file) == 1){
        printf("Byte: %u\n", buffer);
        printf("File field status:\n");
        printf("bufsiz: %d\n", file -> _bufsiz);
        printf("charbuf: %d\n", file -> _charbuf);
        printf("cnt: %d\n", file -> _cnt);
        printf("file: %d\n", file -> _file);
        printf("flag: %d\n", file -> _flag);
        printf("ptr: %u\n", *(file -> _ptr));
        printf("tmpfname: %s\n\n", file -> _tmpfname);
    }
    fclose(file);
    return ok;
}

int move_pointer(char* filename, unsigned char buffer[]){
    FILE* file = fopen(filename, "rb");
    if (!file) return wrong_file;
    int ok_flag = fseek(file, 3, SEEK_SET);
    if (ok_flag != 0){
        fclose(file);
        return wrong_file;
    }
    if(fread(buffer, sizeof(unsigned char), 4, file) != 4) {
        fclose(file);
        return wrong_file;
    }
    fclose(file);
    return ok;
}



int main(int argc, char* argv[]){
    if (argc != 2){
        printf("Wrong amount of arguments");
        return 1;
    }
    if (file_write(argv[1]) != ok){
        printf("Wrong file detected");
        return 2;
    }
    if (byte_read(argv[1]) != ok){
        printf("File error detected");
        return 2;
    }
    unsigned char buffer[4];
    if (move_pointer(argv[1], buffer) != ok){
        printf("File error detected");
        return 2;
    }
    printf("Elements from 4 to 7:\n");
    for (int i = 0; i < 4; i++){
        printf("%u ", buffer[i]);
    }
    return 0;
}