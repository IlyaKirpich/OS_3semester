#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



typedef struct {
    char* login;
    int pin_code;
    int sanctions;
}user;

enum state{
    menu = 1,
    registrate,
    login,
    logout,
    get_time,
    date,
    how_much,
    sanctions,
    exit_programm,
    menu_logged
};

enum status{
    ok = 0,
    already_registered = -1,
    allocation_error = -2,
    wrong_string = -3,
};

int registration(user** data_base, int* data_base_size, int* array_size, char* new_login, int pin);
enum state registrate_user(user** data_base, int* data_base_size, int* array_size);
int login_user(user** data_base, int data_base_size, int array_size, char* new_login, int pin, int* actions);
enum state login_print(user** data_base, int data_base_size, int array_size, int* actions);
int is_delimiter(char symbol);
int read(char string[], int length);
user* user_init(char* login, int pincode);
int login_validation(char* new_login);
int pincode_validation(char* new_pincode);
int command_validation(char* command);
enum state print_menu();
enum state print_menu_functions();
enum state time_function();
enum state date_function();
enum state how_much_function();
long long int how_much_year(int year, int month, int day, int current_year, int current_month, int current_day);
long long int how_much_hour(int year, int month, int day, int current_year, int current_month, int current_day, int current_hour);
long long int how_much_minute(int year, int month, int day, int current_year, int current_month, int current_day, int current_hour, int current_minute);
long long int how_much_second(int year, int month, int day, int current_year, int current_month, int current_day, int current_hour, int current_minute, int current_second);
int flag_validation(char* flag);
int month_validation(struct tm* local_time, int year, int month, int day);
int date_validation(char* string);

int registration(user** data_base, int* data_base_size, int* array_size, char* new_login, int pin){
    int size = *array_size;
    for (int i = 0; i < size; i++){
        if (strcmp(new_login, data_base[i] -> login) == 0){
            return already_registered;
        }
    }
    size++;
    if (size == *data_base_size){
        (*data_base_size) *= 2;
        user** ptr = (user**)realloc(data_base, (*data_base_size));
        if (!ptr){
            free(data_base);
            return allocation_error;
        }
        data_base = ptr;
    }
    user* new_user = user_init(new_login, pin);
    if(new_user == NULL){
        return allocation_error;
    }
    data_base[size-1] = new_user;
    *array_size = size;
    return ok;
}

enum state registrate_user(user** data_base, int* data_base_size, int* array_size){
    printf("\nEnter login <= 6 symbol. Use only numbers or letters\n");
    char login[7];
    char pincode[7];
    read(login, 7);
    printf("\nNow enter pin code 0 <= n <= 100000\n");
    read(pincode, 7);
    if(login_validation(login) == wrong_string){
        printf("\nLogin must be <= 6 symbols and can include only numbers or letters\nTry again\n\n");
        return menu;
    }
    int pin = pincode_validation(pincode);
    if (pin == wrong_string){
        printf("\nPin code must be integer number 0 <= 300000\nTry again\n\n");
        return menu;
    }
    printf("%d", *array_size);
    int result = registration(data_base, &(*data_base_size), &(*array_size), login, pin);
    if (result == allocation_error){
        printf("\nAllocation error detected\n");
        return exit_programm;
    }
    if (result == already_registered){
        printf("\nThis user is already registered\nTry again\n\n");
        return menu;
    }
    if (result == ok){
        printf("\nRegistration completed. Now you can log in\n\n");
        return menu;
    }
}

int login_user(user** data_base, int data_base_size, int array_size, char* new_login, int pin, int* actions){
    for (int i = 0; i < array_size; i++){
        if (strcmp(new_login, data_base[i] -> login) == 0)
            if(data_base[i]->pin_code == pin){
                *actions = data_base[i]->sanctions;
                return ok;
            }else{
                return wrong_string;
            }
    }
    return wrong_string;
}

enum state login_print(user** data_base, int data_base_size, int array_size, int* actions){
    printf("\nEnter your login:\n");
    char login[7];
    char pincode[7];
    read(login, 7);
    printf("\nEnter pin code:\n");
    read(pincode, 7);
    if(login_validation(login) == wrong_string){
        printf("\nAre you really? Did you forget your stupid login?\n\n");
        return menu;
    }
    int pin = pincode_validation(pincode);
    if (pin == wrong_string){
        printf("\nAre you really? Did you forget your stupid pin?\n\n");
        return menu;
    }
    int result = login_user(data_base, data_base_size, array_size, login, pin, &(*actions));
    if (result == wrong_string){
        printf("\nWrong data please check your brains in vetclinic\n\n");
        return menu;
    }
    printf("\nYou're cool!!!\n\n");
    return menu_logged;
}

int is_delimiter(char symbol){
    return (symbol == ' ' || symbol == '\n' || symbol == '\t');
}

int read(char string[], int length){
    char a = getchar();
    while (is_delimiter(a)) a = getchar();
    int string_length = 0;
    for (string_length = 0; string_length < length; string_length++){
        if (is_delimiter(a)){
            string[string_length] = '\0';
            return ok;
        }
        string[string_length] = a;
        a = getchar();
    }

    if (!is_delimiter(a)){
        while(!is_delimiter(a)){
            a = getchar();
        }
        return wrong_string;
    }
}

user* user_init(char* login, int pincode){
    user* user1 = (user*)malloc(sizeof(user));
    char* log_in = (char*)malloc(sizeof(char) * 7);
    strcpy(log_in, login);
    if (!user1) return NULL;
    user1->login = log_in;
    user1->pin_code = pincode;
    user1->sanctions = -1;
}

int login_validation(char* new_login){
    int length = strlen(new_login);
    if (length > 6){
        return wrong_string;
    }
    for (int i = 0; i < length; i++){
        if ((new_login[i] < '0' || new_login[i] > '9') && (new_login[i] < 'a' || new_login[i] > 'z'))
            return wrong_string;
    }
    return ok;
}

int pincode_validation(char* new_pincode){
    int pin_length = strlen(new_pincode);
    if (pin_length > 6) return wrong_string;
    int result = 0;
    for (int i = 0; i < pin_length; i++){
        if(new_pincode[i] < '0' || new_pincode[i] > '9')
            return wrong_string;
        result = result * 10 + new_pincode[i] - '0';
    }
    if (result > 100000) return wrong_string;
    return result;
}

int command_validation(char* command){
    if (strlen(command) != 1)
        return wrong_string;
    if (command[0] != 'R' && command[0] != 'L' && command[0] != 'E')
        return wrong_string;
    return ok;
}

enum state print_menu(){
    printf("\nTo registrate enter R\nTo login enter L\nTo exit enter E:\n");
    char command[2];
    read(command, 2);
    if (strcmp(command, "R") == 0)
        return registrate;
    if (strcmp(command, "L") == 0)
        return login;
    if (strcmp(command, "E") == 0)
        return exit_programm;
}

enum state print_menu_functions(){
    printf("\nYou can use this commands:\n");
    printf("Time (to find out current time)\n");
    printf("Date (to find out current date)\n");
    printf("Howmuch <time> flag (to find out time after date, use flag -s or -m or -h or -y)\n");
    printf("Logout (to logout)\n");
    printf("Sanction username <number> (to limit requests for this user)\n");
    char command[9];
    read(command, 9);
    if (strcmp(command, "Time") == ok) return get_time;
    if (strcmp(command, "Date") == ok) return date;
    if (strcmp(command, "Howmuch") == ok) return how_much;
    if (strcmp(command, "Logout") == ok) return menu;
    if (strcmp(command, "Sanctions") == ok) return sanctions;
    printf("\nWrong command. Try again\n");
    return menu_logged;
}

enum state time_function(){
    time_t current_time;
    time(&current_time);
    struct tm* local_time = localtime(&current_time);
    printf("Current time is %02d:%02d:%02d\n", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
    return menu_logged;
}

enum state date_function(){
    time_t current_time;
    time(&current_time);
    struct tm* local_time = localtime(&current_time);
    printf("Current date is %02d/%02d/%d\n", local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900);
    return menu_logged;
}

enum state how_much_function(){
    char date_string[11];
    char flag[3];
    time_t current_time;
    time(&current_time);
    struct tm* local_time = localtime(&current_time);
    int current_year = local_time->tm_year + 1900;
    int current_month = local_time->tm_mon + 1;
    int current_day = local_time->tm_mday;
    int current_hour = local_time->tm_hour;
    int current_minute = local_time->tm_min;
    int current_second = local_time->tm_sec;
    read(date_string, 11);
    read(flag, 3);
    int result = date_validation(date_string);
    if (result == wrong_string){
        printf("\nWrong date. Try again\n");
        return menu_logged;
    }
    int year = 0, month = 0, day = 0;
    for (int i = 6; i < 10; i++){
        year = year * 10 + (date_string[i] - '0');
    }
    month = (date_string[3] - '0') * 10 + (date_string[4] - '0');
    day = (date_string[0] - '0') * 10 + (date_string[1] - '0');
    result = month_validation(local_time, year, month, day);
    if (result == wrong_string){
        printf("\nWrong date. Try again\n");
        return menu_logged;
    }
    if (flag_validation(flag) != ok){
        printf("\nWrong flag. Try again\n");
        return menu_logged;
    }
    long long int answer;
    switch(flag[1]){
        case 'y':
            answer = how_much_year(year, month, day, current_year, current_month, current_day);
            printf("\n%d years have passed since your date\n", answer);
            break;
        case 'h':
            answer = how_much_hour(year, month, day, current_year, current_month, current_day, current_hour);
            printf("\n%lld hours have passed since your date\n", answer);
            break;
        case 'm':
            answer = how_much_minute(year, month, day, current_year, current_month, current_day, current_hour, current_minute);
            printf("\n%lld minutes have passed since your date\n", answer);
            break;
        case 's':
            answer = how_much_second(year, month, day, current_year, current_month, current_day, current_hour, current_minute, current_second);
            printf("\n%lld seconds have passed since your date\n", answer);
            break;
    }
    return menu_logged;
}

long long int how_much_year(int year, int month, int day, int current_year, int current_month, int current_day){
    if (month > current_month) return current_year - year - 1;
    if (month == current_month)
        if (day >= current_day) return current_year - year - 1;
        return current_year - year;
    return current_year - year;
}

long long int how_much_hour(int year, int month, int day, int current_year, int current_month, int current_day, int current_hour){
    long long int result = how_much_year(year, month, day, current_year, current_month, current_day) * 8760;
    if (month < current_month){
        if (day >= current_day)
            result += (current_month - month) * 43800;
        else result += (current_month - month - 1) * 43800;
    }
    else if (day >= current_day)
            result += (12 - month + current_month) * 43800;
        else result += (12 - month + current_month - 1) * 43800;
    return result + current_hour;
}

long long int how_much_minute(int year, int month, int day, int current_year, int current_month, int current_day, int current_hour, int current_minute){
    return how_much_hour(year, month, day, current_year, current_month, current_day, current_hour) * 60 + current_minute;
}

long long int how_much_second(int year, int month, int day, int current_year, int current_month, int current_day, int current_hour, int current_minute, int current_second){
    return how_much_minute(year, month, day, current_year, current_month, current_day, current_hour, current_minute) * 60 + current_second;
}

int flag_validation(char* flag){
    int length = strlen(flag);
    if (length != 2) return wrong_string;
    if (flag[0] != '-') return wrong_string;
    if (flag[1] != 'm' && flag[1] != 's' && flag[1] != 'h' && flag[1] != 'y')
        return wrong_string;
    return ok;
}

int month_validation(struct tm* local_time, int year, int month, int day){
    if (year > local_time->tm_year + 1900)
        return wrong_string;
    if (year == local_time->tm_year + 1900)
        if (month > local_time->tm_mon + 1)
            return wrong_string;
        if (month == local_time->tm_mon + 1)
            if (day >= local_time->tm_mday)
                return wrong_string;
    if (month == 2){
        if (year % 4 == 0)
            if (day > 29) return wrong_string;
        else if (day > 28) return wrong_string;
    }
    if (month == 4 || month == 6 || month == 9 || month == 11)
        if (day > 30)
            return wrong_string;
    else if (day > 31)
        return wrong_string;
    return ok;
}

int date_validation(char* string){
    int length = strlen(string);
    if (length != 10)
        return wrong_string;
    for (int i = 0; i < length; i++){
        if (i == 2 || i == 5)
            if (string[i] != '/')
                return wrong_string;
            continue;
        if (string[i] < '0' || string[i] > '9')
            return wrong_string;
    }
    return ok;
}

enum state sanctions_function(user** data_base, int users_count){
    char username[7];
    char actions[10];
    read(username, 7);
    read(actions, 10);
    int act = atoi(actions);
    for (int i = 0; i < users_count; i++){
        if (strcmp(username, data_base[i]->login)){
            data_base[i]->sanctions = act;
            break;
        }
    }
    printf("\nTo use sanction you need to relogin\n");
    return ok;
}

int actions_controle(int* actions){
    (*actions)--;
    if (*actions == 0)
        return 0;
}

int main(int argc, char* argv[]){
    enum state status = menu;
    user** data_base = (user**)malloc(sizeof(user*) * 2);
    int data_base_size = 2;
    int users_count = 0;
    int actions;
    while (status){
        switch (status){
            case menu:
                status = print_menu();
                break;
            case registrate:
                status = registrate_user(data_base, &data_base_size, &users_count);
                break;
            case login:
                status = login_print(data_base, data_base_size, users_count, &actions);
                break;
            case menu_logged:
                status = print_menu_functions();
                break;
            case get_time:
                status = time_function();
                if (actions_controle(&actions) == 0) status = menu;
                break;
            case date:
                status = date_function();
                if (actions_controle(&actions) == 0) status = menu;
                break;
            case how_much:
                status = how_much_function();
                if (actions_controle(&actions) == 0) status = menu;
                break;
            case sanctions:
                status = sanctions_function(data_base, users_count);
                if (actions_controle(&actions) == 0) status = menu;
                break;
            case logout:
                status = menu;
                break;
            case exit_programm:
                printf("\nBye bye");
                return ok;
        }
    }
}