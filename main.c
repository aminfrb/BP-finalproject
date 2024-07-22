#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    char *name;
    char *family;
    char *userID;
    char *password;
    char *nationalID;
    char *birthDate;
    char *gender;
    char *role; // admin, student
    int active;
    int deleted;
    int approved;
    int balance;
} User;

typedef struct
{
    int id;
    char *name;
    char *location;
    int capacity;
    char *type; // girlish, boyish
    char *meal; // dinner, lunch, both
    char *lunchTimeLimit;
    char *dinnerTimeLimit;
} Self;

typedef struct
{
    int id;
    char *name;
    char *type; // dessert, food
    int price;
} Food;

typedef struct
{
    int selfID;
    char *date;
    char *type; // lunch, dinner
    int foodID;
    int count;
} MealPlan;

typedef struct
{
    int selfID;
    char *date;
    char *meal;
    int foodID;
    char *userID;
    int canceled;
    int isTaken;
} Reserve;

typedef struct
{
    char date[11];
    char time[6];
} DateTime;

typedef struct
{
    char *title;
    char *content;
    char *date;
} News;

DateTime currentDateTime;
User currentUser;
int userCount = 0;
User *users;
int selfCount = 0;
Self *selfs;
int foodCount = 0;
Food *foods;
int mealPlanCount = 0;
MealPlan *mealPlans;
int reserveCount = 0;
Reserve *reserves;
int newsCount = 0;
News *newsList;

// نمایش کاربر
void displayUser(User user)
{
    printf("{ ");
    printf("name: %s, ", user.name);
    printf("family: %s, ", user.family);
    printf("userID: %s, ", user.userID);
    printf("password: %s, ", user.password);
    printf("nationalID: %s, ", user.nationalID);
    printf("birthDate: %s, ", user.birthDate);
    printf("gender: %s, ", user.gender);
    printf("type: %s, ", user.role);
    printf("approved: %d, ", user.approved);
    printf("deleted: %d, ", user.deleted);
    printf("active: %d ", user.active);
    printf("balance: %d ", user.balance);
    printf("}\n");
}

int compareDates(char *date1, char *date2)
{
    int year1, month1, day1;
    int year2, month2, day2;

    sscanf(date1, "%d-%d-%d", &year1, &month1, &day1);
    sscanf(date2, "%d-%d-%d", &year2, &month2, &day2);

    struct tm tm1 = {0, 0, 0, day1, month1 - 1, year1 - 1900};
    struct tm tm2 = {0, 0, 0, day2, month2 - 1, year2 - 1900};

    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);

    double diff = difftime(t1, t2);

    if (diff == 0)
    {
        return 0;
    }
    else if (diff < 0)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

int compareTimes(char *time1, char *time2)
{
    int result = strcmp(time1, time2);
    if (result < 0)
    {
        // time1 is earlier than time2
        return -1;
    }
    else if (result == 0)
    {
        // time1 is the same as time2
        return 0;
    }
    // time2 is earlier than time1
    return 1;
}

// بررسی وجود فایل
int fileExists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    fclose(file);
    return 0;
}

void loadFile(char *fname, char *data_group)
{
    FILE *fptr;
    fptr = fopen(fname, "r");
    int i = 0;
    char line[10 * 256];
    if (!strcmp(data_group, "USER"))
    {
        int active, deleted, approved;

        while (fgets(line, sizeof(line), fptr))
        {
            char *name;
            char *family;
            char *userID;
            char *password;
            char *nationalID;
            char *birthDate;
            char *gender;
            char *role;
            strtok(line, ":");
            name = strtok(NULL, "|");
            strtok(NULL, ":");
            family = strtok(NULL, "|");
            strtok(NULL, ":");
            userID = strtok(NULL, "|");
            strtok(NULL, ":");
            password = strtok(NULL, "|");
            strtok(NULL, ":");
            nationalID = strtok(NULL, "|");
            strtok(NULL, ":");
            birthDate = strtok(NULL, "|");
            strtok(NULL, ":");
            gender = strtok(NULL, "|");
            strtok(NULL, ":");
            role = strtok(NULL, "|");
            strtok(NULL, ":");
            active = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            deleted = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            approved = atoi(strtok(NULL, "\n"));
            registerUser(name,
                         family,
                         userID,
                         password,
                         nationalID,
                         birthDate,
                         gender,
                         role);
            users[i].approved = approved;
            users[i].deleted = deleted;
            users[i].active = active;
            i++;
        }
    }
    else if (!strcmp(data_group, "SELF"))
    {
        while (fgets(line, sizeof(line), fptr))
        {
            int id;
            char *name;
            char *location;
            int capacity;
            char *type; // girlish, boyish
            char *meal; // dinner, lunch, both
            char *lunchTimeLimit;
            char *dinnerTimeLimit;

            strtok(line, ":");
            id = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            name = strtok(NULL, "|");
            strtok(NULL, ":");
            location = strtok(NULL, "|");
            strtok(NULL, ":");
            capacity = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            type = strtok(NULL, "|");
            strtok(NULL, ":");
            meal = strtok(NULL, "|");
            strtok(NULL, ":");
            lunchTimeLimit = strtok(NULL, "|");
            strtok(NULL, ":");
            dinnerTimeLimit = strtok(NULL, "\n");
            defineSelf(id, name, location, capacity, type, meal, lunchTimeLimit, dinnerTimeLimit);
            i++;
        }
    }
    else if (!strcmp(data_group, "FOOD"))
    {
        while (fgets(line, sizeof(line), fptr))
        {
            int id;
            char *name;
            char *type;
            int price;
            strtok(line, ":");
            id = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            name = strtok(NULL, "|");
            strtok(NULL, ":");
            type = strtok(NULL, "|");
            strtok(NULL, ":");
            price = atoi(strtok(NULL, "\n"));
            defineFood(id, name, type, price);
            i++;
        }
    }
    else if (!strcmp(data_group, "MEAL_PLANS"))
    {
        while (fgets(line, sizeof(line), fptr))
        {
            int selfID;
            char *date;
            char *type; // lunch, dinner
            int foodID;
            int count;
            strtok(line, ":");
            selfID = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            date = strtok(NULL, "|");
            strtok(NULL, ":");
            type = strtok(NULL, "|");
            strtok(NULL, ":");
            foodID = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            count = atoi(strtok(NULL, "\n"));
            defineMealPlan(selfID, date, type, foodID, count);
            i++;
        }
    }
    else if (!strcmp(data_group, "RESERVE"))
    {
        while (fgets(line, sizeof(line), fptr))
        {
            int selfID;
            char *date;
            char *meal; // lunch, dinner
            int foodID;
            char *userID;
            strtok(line, ":");
            selfID = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            date = strtok(NULL, "|");
            strtok(NULL, ":");
            meal = strtok(NULL, "|");
            strtok(NULL, ":");
            foodID = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            userID = strtok(NULL, "\n");
            reserve(selfID, date, meal, foodID, userID);
            i++;
        }
    }
    fclose(fptr);
}

void readCommandsFile(char *fname)
{
    FILE *fptr;
    fptr = fopen(fname, "r");
    if (!fptr)
    {
        printf("oops, file can't be read\n");
        return;
    }

    int i = 0;

    char line[10 * 256];
    int cmd_id;
    char *code, *action, *rest;

    while (fgets(line, sizeof(line), fptr))
    {
        code = strtok(line, "#");
        action = strtok(NULL, "#");
        rest = strtok(NULL, "\n");
        int size_action = strlen(action);
        if (action[size_action - 1] == '\n') {
            action[size_action - 1] = '\0';
        }
        cmd_id = atoi(code);
        // printf("\n(%d), (%s), (%s)\n", cmd_id, action, rest);
        runCommand(cmd_id, action, rest);
        i++;
    }
}

int isEmpty(User user)
{
    return user.name == NULL;
}

char **split(char *str, char *delimiter, int *size)
{
    char **result = (char **)malloc(sizeof(char *) * strlen(str)); // Allocate memory for the result array
    char *token = strtok(str, delimiter);                          // Get the first token
    int i = 0;

    while (token != NULL)
    {
        result[i] = (char *)malloc(sizeof(char) * (strlen(token) + 1)); // Allocate memory for each token
        strcpy(result[i], token);                                       // Copy the token to the result array
        token = strtok(NULL, delimiter);                                // Get the next token
        i++;
    }
    *size = i;
    return result;
}

int contains(char *s1, char *s2)
{
    return strstr(s1, s2) != NULL;
}

int isValid(char *str)
{
    if (str == NULL)
        return 0;
    if (contains(str, ":") || contains(str, "|") || contains(str, "#"))
        return 0;
    return 1;
}

void runCommand(int code, char *action, char *data)
{
    printf("%d#", code);
    if (data == NULL) {
        if (strcmp(action, "vote") != 0 || strcmp(action, "check-news") != 0) {
            printf("invalid\n");
            return;
        }
    }
    if (contains(data, "#"))
    {
        printf("invalid\n");
        return;
    }
    if (!strcmp(action, "login"))
    {
        strtok(data, ":");
        char *userID = strtok(NULL, "|");
        strtok(NULL, ":");
        char *password = strtok(NULL, "\n");
        if (password == NULL || !isValid(userID))
        {
            printf("invalid\n");
            return;
        }
        loginUser(userID, password);
        return;
    }
    else if (!strcmp(action, "register"))
    {
        char *name;
        char *family;
        char *userID;
        char *password;
        char *nationalID;
        char *birthDate;
        char *gender;
        char *role;
        strtok(data, ":");
        name = strtok(NULL, "|");
        strtok(NULL, ":");
        family = strtok(NULL, "|");
        strtok(NULL, ":");
        userID = strtok(NULL, "|");
        strtok(NULL, ":");
        password = strtok(NULL, "|");
        strtok(NULL, ":");
        nationalID = strtok(NULL, "|");
        strtok(NULL, ":");
        birthDate = strtok(NULL, "|");
        strtok(NULL, ":");
        gender = strtok(NULL, "|");
        strtok(NULL, ":");
        role = strtok(NULL, "\n");
        if (!isValid(name) || !isValid(family) ||
            !isValid(userID) || !isValid(nationalID) ||
            password == NULL || !isValid(nationalID) ||
            !isValid(birthDate) || !isValid(gender) ||
            !isValid(role))
        {
            printf("invalid\n");
            return;
        }
        registerUser(name, family, userID, password, nationalID, birthDate, gender, role);
        return;
    }
    else if (!strcmp(action, "logout"))
    {
        strtok(data, ":");
        char *userID = strtok(NULL, "\n");
        if (!isValid(userID))
        {
            printf("invalid\n");
            return;
        }
        logout(userID);
        return;
    }
    else if (!strcmp(action, "change-pass"))
    {
        strtok(data, ":");
        char *userID = strtok(NULL, "|");
        strtok(NULL, ":");
        char *oldPass = strtok(NULL, "|");
        strtok(NULL, ":");
        char *newPass = strtok(NULL, "\n");
        if (!isValid(userID) || !isValid(oldPass) || !isValid(newPass))
        {
            printf("invalid\n");
            return;
        }
        changePassword(userID, oldPass, newPass);
        return;
    }
    else if (!strcmp(action, "deactivate"))
    {
        strtok(data, ":");
        char *userID = strtok(NULL, "\n");
        if (!isValid(userID))
        {
            printf("invalid\n");
            return;
        }
        if (isAdmin(currentUser.role))
            deactivateUser(userID);
        else
            printf("permission-denied\n");
        return;
    }
    else if (!strcmp(action, "approve"))
    {
        if (isAdmin(currentUser.role))
        {
            if (!contains(data, "|"))
            {
                int size1;
                char **result1 = split(data, ":", &size1);
                approveUser(result1[1]);
            }
            else
            {
                int size2;
                char **result2 = split(data, "|", &size2);
                int i;
                for (i = 0; i < size2; i++)
                {
                    if (contains(result2[i], ":"))
                    {
                        int size3;
                        char **result3 = split(result2[i], ":", &size3);
                        approveUser(result3[1]);
                    }
                }
            }
            printf("success\n");
        }
        else
            printf("permission-denied\n");
        return;
    }
    else if (!strcmp(action, "remove-student"))
    {
        strtok(data, ":");
        char *userID = strtok(NULL, "\n");
        if (!isValid(userID))
        {
            printf("invalid\n");
            return;
        }
        if (isAdmin(currentUser.role))
            removeUser(userID, 0);
        else
            printf("permission-denied\n");
        return;
    }
    else if (!strcmp(action, "change-student-pass"))
    {
        strtok(data, ":");
        char *userID = strtok(NULL, "|");
        strtok(NULL, ":");
        char *newPass = strtok(NULL, "\n");
        if (!isValid(userID) || !isValid(newPass))
        {
            printf("invalid\n");
            return;
        }
        if (isAdmin(currentUser.role))
        {
            changeStudentPassword(userID, newPass);
        }
        else
        {
            printf("permission-denied\n");
        }
        return;
    }
    else if (!strcmp(action, "define-self"))
    {
        if (isAdmin(currentUser.role))
        {
            int id;
            char *name;
            char *location;
            int capacity;
            char *type; // girlish, boyish
            char *meal; // dinner, lunch, both
            char *lunchTimeLimit;
            char *dinnerTimeLimit;
            strtok(data, ":");
            id = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            name = strtok(NULL, "|");
            strtok(NULL, ":");
            location = strtok(NULL, "|");
            strtok(NULL, ":");
            capacity = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            type = strtok(NULL, "|");
            strtok(NULL, ":");
            meal = strtok(NULL, "|");
            strtok(NULL, ":");
            lunchTimeLimit = strtok(NULL, "|");
            strtok(NULL, ":");
            dinnerTimeLimit = strtok(NULL, "\n");
            if (id == NULL || !isValid(name) ||
                !isValid(localtime) || capacity == NULL ||
                !isValid(type) || !isValid(meal) ||
                !isValid(lunchTimeLimit) || !isValid(dinnerTimeLimit))
            {
                printf("invalid\n");
                return;
            }
            defineSelf(id, name, location, capacity, type, meal, lunchTimeLimit, dinnerTimeLimit);
        }
        else
        {
            printf("permission-denied\n");
        }
        return;
    }
    else if (!strcmp(action, "define-food"))
    {
        if (isAdmin(currentUser.role))
        {
            int id;
            char *name;
            char *type; // dessert, food
            int price;
            strtok(data, ":");
            id = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            name = strtok(NULL, "|");
            strtok(NULL, ":");
            type = strtok(NULL, "|");
            strtok(NULL, ":");
            price = atoi(strtok(NULL, "\n"));
            if (id==NULL || !isValid(name) || !isValid(type) || price == NULL) {
                printf("invalid\n");
                return;
            }
            defineFood(id, name, type, price);
        }
        else
        {
            printf("permission-denied\n");
        }
        return;
    }
    else if (!strcmp(action, "define-meal-plan"))
    {
        if (isAdmin(currentUser.role))
        {
            int selfID;
            char *date;
            char *type; // lunch, dinner
            int foodID;
            int count;
            strtok(data, ":");
            selfID = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            date = strtok(NULL, "|");
            strtok(NULL, ":");
            type = strtok(NULL, "|");
            strtok(NULL, ":");
            foodID = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            count = atoi(strtok(NULL, "\n"));
            if (selfID == NULL || !isValid(date) ||
             !isValid(type) || foodID == NULL ||
             count == NULL) {
                printf("invalid\n");
             }
            defineMealPlan(selfID, date, type, foodID, count);
        }
        else
        {
            printf("permission-denied\n");
        }
        return;
    }
    else if (!strcmp(action, "reserve"))
    {
        if (!isAdmin(currentUser.role) && !isEmpty(currentUser))
        {
            int selfID;
            char *date;
            char *meal; // lunch, dinner
            int foodID;
            strtok(data, ":");
            selfID = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            date = strtok(NULL, "|");
            strtok(NULL, ":");
            meal = strtok(NULL, "|");
            strtok(NULL, ":");
            foodID = atoi(strtok(NULL, "\n"));
            reserve(selfID, date, meal, foodID, currentUser.userID);
        }
        else
        {
            printf("permission-denied\n");
        }
    }
    else if (!strcmp(action, "take-food"))
    {
        if (!isAdmin(currentUser.role) && !isEmpty(currentUser))
        {
            int selfID;
            char *date;
            char *meal; // lunch, dinner
            strtok(data, ":");
            selfID = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            date = strtok(NULL, "|");
            strtok(NULL, ":");
            meal = strtok(NULL, "\n");
            takeFood(selfID, date, meal, currentUser.userID);
        }
        else
        {
            printf("permission-denied\n");
        }
    }
    else if (!strcmp(action, "charge-account"))
    {
        if (!isEmpty(currentUser))
        {
            int amount;
            strtok(data, ":");
            amount = atoi(strtok(NULL, "|"));
            chargeAccount(amount, currentUser.userID);
        }
        else
        {
            printf("permission-denied\n");
        }
    }
    else if (!strcmp(action, "charge-student-account"))
    {
        if (isAdmin(currentUser.role))
        {
            char *userID;
            int amount;
            strtok(data, ":");
            userID = strtok(NULL, "|");
            strtok(NULL, ":");
            amount = atoi(strtok(NULL, "\n"));
            chargeStudentAccount(userID, amount);
        }
        else
        {
            printf("permission-denied\n");
        }
    }
    else if (!strcmp(action, "send-charge"))
    {
        if (!isEmpty(currentUser))
        {
            char *userID;
            int amount;
            char *name;
            strtok(data, ":");
            userID = strtok(NULL, "|");
            strtok(NULL, ":");
            amount = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            name = strtok(NULL, "\n");
            sendCharge(userID, amount, name);
        }
        else
        {
            printf("permission-denied\n");
        }
    }
    else if (!strcmp(action, "cancel-reserve"))
    {
        if (!isAdmin(currentUser.role) && !isEmpty(currentUser))
        {
            char *date;
            char *meal;
            strtok(data, ":");
            date = strtok(NULL, "|");
            strtok(NULL, ":");
            meal = strtok(NULL, "\n");
            cancelReserve(date, meal);
        }
        else
        {
            printf("permission-denied\n");
        }
    }
    else if (!strcmp(action, "daily-reserve"))
    {
        if (!isAdmin(currentUser.role) && !isEmpty(currentUser))
        {
            int selfID;
            int foodID;
            strtok(data, ":");
            selfID = atoi(strtok(NULL, "|"));
            strtok(NULL, ":");
            foodID = atoi(strtok(NULL, "\n"));
            dailyReserve(selfID, foodID);
        }
        else
        {
            printf("permission-denied\n");
        }
    }
    else if (!strcmp(action, "change-self"))
    {
        if (!isAdmin(currentUser.role) && !isEmpty(currentUser))
        {
            char *date;
            char *meal;
            int newSelfID;
            strtok(data, ":");
            date = strtok(NULL, "|");
            strtok(NULL, ":");
            meal = strtok(NULL, "|");
            strtok(NULL, ":");
            newSelfID = atoi(strtok(NULL, "\n"));
            changeSelf(date, meal, newSelfID);
        }
        else
        {
            printf("permission-denied\n");
        }
    }
    else if (!strcmp(action, "change-datetime"))
    {
        char *date;
        char *time;
        strtok(data, ":");
        date = strtok(NULL, "|");
        strtok(NULL, ":");
        time = strtok(NULL, "\n");
        changeDateTime(date, time);
    }
    else if (!strcmp(action, "vote"))
    {
        if (!isAdmin(currentUser.role) && !isEmpty(currentUser))
        {
            vote();
        }
        else
        {
            printf("permission-denied\n");
        }
    }
    else if (!strcmp(action, "add-poll"))
    {
        if (isAdmin(currentUser.role))
        {
            addPoll();
        }
        else
        {
            printf("permission-denied\n");
        }
    }
    else if (!strcmp(action, "define-agent"))
    {
        if (isAdmin(currentUser.role))
        {
            defineAgent();
        }
        else
        {
            printf("permission-denied\n");
        }
    }
    else if (!strcmp(action, "check-news"))
    {
        checkNews(0);
    }
    else if (!strcmp(action, "add-news"))
    {
        if (isAdmin(currentUser.role))
        {
            char *title;
            char *content;
            char *date;
            strtok(data, ":");
            title = strtok(NULL, "|");
            strtok(NULL, ":");
            content = strtok(NULL, "|");
            strtok(NULL, ":");
            date = strtok(NULL, "\n");
            if (!isValid(date)) {
                printf("invalid\n");
                return;
            }
            addNews(title, content, date);
        }
        else
        {
            printf("permission-denied\n");
        }
    }
    else
    {
        printf("invalid\n");
    }
    return;
}

// بررسی وجود یا عدم وجود کاربر
int checkUser(char *userID, char *password)
{
    int i;
    for (i = 0; i < userCount; i++)
    {
        if (!strcmp(userID, users[i].userID) && !strcmp(password, users[i].password))
        {
            if (users[i].deleted == 1 || users[i].active == 0)
                return 0;
            return 1;
        }
    }
    return 0;
}

int checkMealPlan(int foodID, int selfID, char *date)
{
    int i;
    for (i = 0; i < mealPlanCount; i++)
    {
        if (!strcmp(date, mealPlans[i].date) && foodID == mealPlans[i].foodID && mealPlans[i].selfID == selfID)
            return 1;
    }
    return 0;
}

// بررسی وجود یک شناسه
int checkFood(int id)
{
    int i;
    for (i = 0; i < foodCount; i++)
        if (id == foods[i].id)
            return 1;
    return 0;
}

int checkSelf(int id)
{
    int i;
    for (i = 0; i < selfCount; i++)
        if (id == selfs[i].id)
            return 1;
    return 0;
}

// گرفتن اندیس کاربر
int getUserIndex(char *userID)
{
    if (userID == NULL)
        return -1;

    int i;
    for (i = 0; i < userCount; i++)
    {
        if (!strcmp(userID, users[i].userID))
        {
            if (users[i].deleted == 1 || users[i].active == 0)
                return -1;
            return i;
        }
    }
    return -1;
}

// لاگین کردن کاربر
int loginUser(char *userID, char *password)
{
    int i;
    if (checkUser(userID, password))
    {
        int i = getUserIndex(userID);
        memcpy(&currentUser, &users[i], sizeof(User));
        printf("success\n");
        return 1;
    }
    printf("not-found\n");
    return 0;
}

// خروج از حساب کاربری
void logout(char *userID)
{
    if (!isEmpty(currentUser))
        if (!strcmp(userID, currentUser.userID))
        {
            memset(&currentUser, 0, sizeof(User));
            printf("success\n");
        }
        else
            printf("not-found\n");
    else
    {
        printf("not-found\n");
    }
}

// ثبت نام کاربر جدید
void registerUser(char *name,
                  char *family,
                  char *userID,
                  char *password,
                  char *nationalID,
                  char *birthDate,
                  char *gender,
                  char *role)
{
    userCount++;
    users = (User *)realloc(users, userCount * sizeof(User));
    users[userCount - 1].name = (char *)malloc(strlen(name) + 1);
    strcpy(users[userCount - 1].name, name);
    users[userCount - 1].family = (char *)malloc(strlen(family) + 1);
    strcpy(users[userCount - 1].family, family);
    users[userCount - 1].userID = (char *)malloc(strlen(userID) + 1);
    strcpy(users[userCount - 1].userID, userID);
    users[userCount - 1].password = (char *)malloc(strlen(password) + 1);
    strcpy(users[userCount - 1].password, password);
    users[userCount - 1].nationalID = (char *)malloc(strlen(nationalID) + 1);
    strcpy(users[userCount - 1].nationalID, nationalID);
    users[userCount - 1].birthDate = (char *)malloc(strlen(birthDate) + 1);
    strcpy(users[userCount - 1].birthDate, birthDate);
    users[userCount - 1].gender = (char *)malloc(strlen(gender) + 1);
    strcpy(users[userCount - 1].gender, gender);
    users[userCount - 1].role = (char *)malloc(strlen(role) + 1);
    strcpy(users[userCount - 1].role, role);
    users[userCount - 1].active = 1;
    users[userCount - 1].deleted = 0;
    users[userCount - 1].approved = isAdmin(role);
    users[userCount - 1].balance = 0;

    updateFile("users.txt");

    printf("success\n");
}

void eraseContent(char *fname)
{
    FILE *fp = fopen(fname, "w");
    freopen(fname, "w", fp);
    fclose(fp);
}

void updateFile(char *fname)
{
    // eraseContent(fname);
    FILE *file = fopen(fname, "w");
    freopen(fname, "w", file);
    if (!strcmp(fname, "users.txt"))
    {
        int i;
        for (i = 0; i < userCount; i++)
            fprintf(file, "name:%s|family:%s|user-id:%s|password:%s|national-id-code:%s|birthdate:%s|gender:%s|type:%s|active:%d|deleted:%d|approved:%d|balance:%d\n",
                    users[i].name,
                    users[i].family,
                    users[i].userID,
                    users[i].password,
                    users[i].nationalID,
                    users[i].birthDate,
                    users[i].gender,
                    users[i].role,
                    users[i].active,
                    users[i].deleted,
                    users[i].approved,
                    users[i].balance);
    }
    if (!strcmp(fname, "selfs.txt"))
    {
        int i;
        for (i = 0; i < selfCount; i++)
            fprintf(file, "id:%d|name:%s|location:%s|capacity:%d|type:%s|meal:%s|lunch-time-limit:%s|dinner-time-limit:%s\n",
                    selfs[i].id,
                    selfs[i].name,
                    selfs[i].location,
                    selfs[i].capacity,
                    selfs[i].type,
                    selfs[i].meal,
                    selfs[i].lunchTimeLimit,
                    selfs[i].dinnerTimeLimit);
    }
    if (!strcmp(fname, "foods.txt"))
    {
        int i;
        for (i = 0; i < foodCount; i++)
            fprintf(file, "id:%d|name:%s|type:%s|price:%d\n",
                    foods[i].id,
                    foods[i].name,
                    foods[i].type,
                    foods[i].price);
    }
    if (!strcmp(fname, "meal-plans.txt"))
    {
        int i;
        for (i = 0; i < mealPlanCount; i++)
            fprintf(file, "self-id:%d|date:%s|type:%s|food-id:%d|count:%d\n",
                    mealPlans[i].selfID,
                    mealPlans[i].date,
                    mealPlans[i].type,
                    mealPlans[i].foodID,
                    mealPlans[i].count);
    }
    if (!strcmp(fname, "reserves.txt"))
    {
        int i;
        for (i = 0; i < reserveCount; i++)
            fprintf(file, "self-id:%d|date:%s|meal:%s|food-id:%d|user-id:%s\n",
                    reserves[i].selfID,
                    reserves[i].date,
                    reserves[i].meal,
                    reserves[i].foodID,
                    reserves[i].userID);
    }
    fclose(file);
}

// تغییر رمز عبور
void changePassword(char *userID, char *oldPass, char *newPass)
{
    if (!isEmpty(currentUser) && !strcmp(currentUser.userID, userID))
    {
        int idx;
        idx = getUserIndex(currentUser.userID);
        if (idx == -1)
        {
            printf("not-found\n");
            return;
        }
        if (!strcmp(oldPass, currentUser.password))
        {
            strcpy(users[idx].password, newPass);
            updateFile("users.txt");
            printf("success\n");
        }
        else
            printf("not-found\n");
    }
    else
    {
        printf("not-found\n");
    }
}

// تغییر رمز عبور دانشجو توسط ادمین
void changeStudentPassword(char *userID, char *newPass)
{
    int idx = getUserIndex(userID);
    if (idx == -1)
    {
        printf("not-found\n");
        return;
    }
    strcpy(users[idx].password, newPass);
    updateFile("users.txt");
    printf("success\n");
}

// تایید کاربر توسط مسئول
void approveUser(char *userID)
{
    int idx = getUserIndex(userID);
    if (idx != -1)
    {
        if (users[idx].approved)
            return;
        else
            users[idx].approved = 1;
        updateFile("users.txt");
    }
    else
    {
        printf("not-found\n");
    }
}

// غیرفعال کردن کاربر توسط مسئول
void deactivateUser(char *userID)
{
    int idx = getUserIndex(userID);
    if (idx != -1)
    {
        if (users[idx].active == 0)
            printf("success\n");
        else
            users[idx].active = 0;
        updateFile("users.txt");
        printf("success\n");
    }
    else
    {
        printf("not-found\n");
    }
}

void chargeStudentAccount(char *userID, int amount)
{
    int idx = getUserIndex(userID);
    if (idx != -1)
    {
        users[idx].balance += amount;
        updateFile("users.txt");
        printf("success\n");
    }
    else
    {
        printf("not-found\n");
    }
}

// حذف کاربر
void removeUser(char *userID, int interactive)
{
    int idx = getUserIndex(userID);
    if (idx != -1)
    {
        char ack[3];
        if (users[idx].deleted)
        {
            printf("not-found\n");
            return;
        }
        else
        {
            if (interactive)
            {
                printf("are you sure (y/n) ? ");
                scanf("%s", &ack);
            }
            else
                strcpy(ack, "y");

            if (!strcmp(ack, "y") || !strcmp(ack, "yes"))
                users[idx].deleted = 1;
        }
        updateFile("users.txt");
        printf("success\n");
    }
    else
    {
        printf("not-found\n");
    }
}

void defineSelf(int id, char *name, char *location, int capacity, char *type, char *meal, char *lunchTimeLimit, char *dinnerTimeLimit)
{
    if (!checkSelf(id))
    {
        selfCount++;
        selfs = (Self *)realloc(selfs, selfCount * sizeof(Self));
        selfs[selfCount - 1].id = id;
        selfs[selfCount - 1].name = (char *)malloc(strlen(name) + 1);
        strcpy(selfs[selfCount - 1].name, name);
        selfs[selfCount - 1].location = (char *)malloc(strlen(location) + 1);
        strcpy(selfs[selfCount - 1].location, location);
        selfs[selfCount - 1].capacity = capacity;
        selfs[selfCount - 1].type = (char *)malloc(strlen(type) + 1);
        strcpy(selfs[selfCount - 1].type, type);
        selfs[selfCount - 1].meal = (char *)malloc(strlen(meal) + 1);
        strcpy(selfs[selfCount - 1].meal, meal);
        selfs[selfCount - 1].lunchTimeLimit = (char *)malloc(strlen(lunchTimeLimit) + 1);
        strcpy(selfs[selfCount - 1].lunchTimeLimit, lunchTimeLimit);
        selfs[selfCount - 1].dinnerTimeLimit = (char *)malloc(strlen(dinnerTimeLimit) + 1);
        strcpy(selfs[selfCount - 1].dinnerTimeLimit, dinnerTimeLimit);

        updateFile("selfs.txt");

        printf("success\n");
        return;
    }
    printf("success\n");
}

void defineFood(int id, char *name, char *type, int price)
{
    if (!checkFood(id))
    {
        foodCount++;
        foods = (Food *)realloc(foods, foodCount * sizeof(Food));
        foods[foodCount - 1].id = id;
        foods[foodCount - 1].name = (char *)malloc(strlen(name) + 1);
        strcpy(foods[foodCount - 1].name, name);
        foods[foodCount - 1].type = (char *)malloc(strlen(type) + 1);
        strcpy(foods[foodCount - 1].type, type);
        foods[foodCount - 1].price = price;

        updateFile("foods.txt");

        printf("success\n");
        return;
    }
    printf("success\n");
}

void defineMealPlan(int selfID, char *date, char *type, int foodID, int count)
{
    if (!checkMealPlan(foodID, selfID, date))
    {
        mealPlanCount++;
        mealPlans = (MealPlan *)realloc(mealPlans, mealPlanCount * sizeof(MealPlan));
        mealPlans[mealPlanCount - 1].selfID = selfID;
        mealPlans[mealPlanCount - 1].date = (char *)malloc(strlen(date) + 1);
        strcpy(mealPlans[mealPlanCount - 1].date, date);
        mealPlans[mealPlanCount - 1].type = (char *)malloc(strlen(type) + 1);
        strcpy(mealPlans[mealPlanCount - 1].type, type);
        mealPlans[mealPlanCount - 1].foodID = foodID;
        mealPlans[mealPlanCount - 1].count = count;

        updateFile("meal-plans.txt");
        printf("success\n");
        return;
    }
    printf("success\n");
}

// تشخیص دستور ورودی توسط کاربر
int getCommand(char *cmd)
{
    if (!strcmp(cmd, "register"))
        return 1;
    if (!strcmp(cmd, "login"))
        return 2;
    if (!strcmp(cmd, "change-pass"))
        return 3;
    if (!strcmp(cmd, "approve"))
        return 4;
    if (!strcmp(cmd, "remove-student"))
        return 5;
    if (!strcmp(cmd, "deactivate"))
        return 6;
    if (!strcmp(cmd, "define-self"))
        return 7;
    if (!strcmp(cmd, "logout"))
        return 8;
    if (!strcmp(cmd, "change-student-pass"))
        return 9;
    if (!strcmp(cmd, "define-food"))
        return 10;
    if (!strcmp(cmd, "define-meal-plan"))
        return 11;
    if (!strcmp(cmd, "charge-student-account"))
        return 12;
    if (!strcmp(cmd, "reserve"))
        return 13;
    if (!strcmp(cmd, "take-food"))
        return 14;
    if (!strcmp(cmd, "charge-account"))
        return 15;
    if (!strcmp(cmd, "send-charge"))
        return 16;
    if (!strcmp(cmd, "cancel-reserve"))
        return 17;
    if (!strcmp(cmd, "daily-reserve"))
        return 18;
    if (!strcmp(cmd, "change-self"))
        return 19;
    if (!strcmp(cmd, "add-news"))
        return 20;
    if (!strcmp(cmd, "check-news"))
        return 21;
    if (!strcmp(cmd, "change-datetime"))
        return 22;
    if (!strcmp(cmd, "vote"))
        return 23;
    if (!strcmp(cmd, "add-poll"))
        return 24;
    if (!strcmp(cmd, "define-agent"))
        return 25;
    if (!strcmp(cmd, "exit"))
        return 0;
    if (!strcmp(cmd, "cls"))
        return 100;
    return -1;
}

// مخفی نشان دادن گذرواژه
char *inputPassword()
{
    char ch;
    char *password = malloc(64);
    int i = 0;
    while ((ch = getch()) != 13)
    {
        printf("*");
        password[i] = ch;
        i++;
    }
    password[i] = '\0';
    return password;
}

// چک کردن نقش کاربر کنونی
int isAdmin(char *role)
{
    return role != NULL && !strcmp(role, "admin");
}

void createFile(char *fname)
{
    FILE *fptr;
    fptr = fopen(fname, "w");
    fclose(fptr);
}

void displayFoods()
{
    // TODO
}

int checkReserves(int selfID, char *date, char *meal, int foodID, char *userID)
{
    int i;

    for (i = 0; i < reserveCount; i++)
    {
        if (reserves[i].selfID == selfID &&
            reserves[i].foodID == foodID &&
            !strcmp(reserves[i].date, date) &&
            !strcmp(reserves[i].meal, meal) &&
            !strcmp(reserves[i].userID, userID))
        {
            return 1;
        }
    }
    return 0;
}

void reserve(int selfID, char *date, char *meal, int foodID, char *userID)
{
    // displayUser(currentUser);
    if (!checkReserves(selfID, date, meal, foodID, userID))
    {
        reserveCount++;
        reserves = (Reserve *)realloc(reserves, reserveCount * sizeof(Reserve));
        reserves[reserveCount - 1].selfID = selfID;
        reserves[reserveCount - 1].date = (char *)malloc(strlen(date) + 1);
        strcpy(reserves[reserveCount - 1].date, date);
        reserves[reserveCount - 1].meal = (char *)malloc(strlen(meal) + 1);
        strcpy(reserves[reserveCount - 1].meal, meal);
        reserves[reserveCount - 1].foodID = foodID;
        reserves[reserveCount - 1].userID = (char *)malloc(strlen(userID) + 1);
        strcpy(reserves[reserveCount - 1].userID, userID);

        updateFile("reserves.txt");

        printf("success\n");
    }
    else
    {
        printf("success\n");
    }
}

int getReserveIndex(int selfID, char *date, char *meal, char *userID)
{
    int i;
    for (i = 0; i < reserveCount; i++)
    {
        if (reserves[i].selfID == selfID &&
            !strcmp(reserves[i].date, date) &&
            !strcmp(reserves[i].meal, meal) &&
            !strcmp(reserves[i].userID, userID))
        {
            return i;
        }
    }
    return -1;
}

void takeFood(int selfID, char *date, char *meal, char *userID)
{
    int idx = getReserveIndex(selfID, date, meal, userID);
    if (idx != -1)
    {
        if (!reserves[idx].isTaken && !reserves[idx].canceled)
        {
            reserves[idx].isTaken = 1;
            updateFile("reserves.txt");
            printf("success\n");
        }
        else
        {
            printf("not-found\n");
        }
    }
    else
    {
        printf("permission-denied\n");
    }
}

void chargeAccount(int amount, char *userID)
{
    // TODO
    int idx = getUserIndex(userID);
    if (idx != -1)
    {
        users[idx].balance += amount;
        updateFile("users.txt");
        printf("success\n");
    }
    else
    {
        printf("permission-denied\n");
    }
}

void sendCharge(char *userID, int amount, char *name)
{
    // TODO
    int idx = getUserIndex(userID);
    if (idx != -1)
    {
        if (!strcmp(users[idx].name, name))
        {
            users[idx].balance += amount;
            updateFile("users.txt");
            printf("success\n");
        }
        else
        {
            printf("not-found\n");
        }
    }
    else
    {
        printf("not-found\n");
    }
}

void cancelReserve(char *date, char *meal)
{
    // TODO
    int i;
    for (i = 0; i < reserveCount; i++)
    {
        if (!strcmp(reserves[i].userID, currentUser.userID) && !strcmp(reserves[i].date, date) && !strcmp(reserves[i].meal, meal))
        {
            reserves[i].canceled = 1;
            updateFile("reserves.txt");
            printf("success\n");
            return;
        }
    }
    printf("not-found\n");
}

void dailyReserve(int selfID, int foodID)
{
    // TODO
    reserve(selfID, currentDateTime.date, compareTimes(currentDateTime.time, "1400") < 0 ? "lunch" : "dinner", foodID, currentUser.userID);
}

void changeSelf(char *date, char *meal, int newSelfID)
{
    // TODO
    int i;
    for (i = 0; i < reserveCount; i++)
    {
        if (!strcmp(reserves[i].userID, currentUser.userID) && !strcmp(reserves[i].date, date) && !strcmp(reserves[i].meal, meal))
        {
            reserves[i].selfID = newSelfID;
            updateFile("reserves.txt");
            printf("success\n");
            return;
        }
    }
    printf("not-found\n");
}

void addPoll()
{
    // TODO
    printf("success\n");
}

void addNews(char *title, char *content, char *date)
{
    // TODO
    newsCount++;
    newsList = (News *)realloc(newsList, newsCount * sizeof(News));
    newsList[newsCount - 1].title = (char *)malloc(strlen(title) + 1);
    strcpy(newsList[newsCount - 1].title, title);
    newsList[newsCount - 1].content = (char *)malloc(strlen(content) + 1);
    strcpy(newsList[newsCount - 1].content, content);
    newsList[newsCount - 1].date = (char *)malloc(strlen(date) + 1);
    strcpy(newsList[newsCount - 1].date, date);
    printf("success\n");
}

void displayNews(int index, int it)
{
    if (it)
    {
        printf("\n{\n");
        printf("\ttitle: %s,\n", newsList[index].title);
        printf("\tcontent: %s,\n", newsList[index].content);
        printf("\tdate: %s,\n", newsList[index].date);
        printf("}\n");
    }
}

void checkNews(int interactive)
{
    int i;
    int flag = 0;
    for (i = 0; i < newsCount; i++)
    {
        char *date1 = currentDateTime.date;
        char *date2 = newsList[i].date;
        if (compareDates(date1, date2) <= 0)
        {
            flag = 1;
            displayNews(i, interactive);
        }
    }
    if (!flag)
    {
        printf("not-found\n");
    }
    else
    {
        printf("success\n");
    }
}

void defineAgent()
{
    // TODO
    printf("success\n");
}

void vote()
{
    // TODO
    printf("success\n");
}

void changeDateTime(char *date, char *time)
{
    strcpy(currentDateTime.date, date);
    strcpy(currentDateTime.time, time);
    printf("success\n");
}

void init()
{
    time_t currentTime;
    struct tm *localTime;
    time(&currentTime);
    localTime = localtime(&currentTime);
    char timeString[6], dateString[11];

    strftime(timeString, 6, "%H%M", localTime);
    strftime(dateString, 11, "%Y-%m-%d", localTime);

    strcpy(currentDateTime.date, dateString);
    strcpy(currentDateTime.time, timeString);

    registerUser("Admin", "No1", "admin", "admin", "admin", "1900-01-01", "male", "admin");

    system("cls");
}

int main()
{
    char cmd[8 * 256];
    int quit = 0;

    init();

    // if (fileExists("users.txt"))
    //     loadFile("temp_users.txt", "USER");
    // else
    //     createFile("users.txt");
    // if (fileExists("foods.txt"))
    //     loadFile("foods.txt", "FOOD");
    // else
    //     createFile("foods.txt");
    // if (fileExists("reserves.txt"))
    //     loadFile("reserves.txt", "RESERVE");
    // else
    //     createFile("reserves.txt");
    // if (fileExists("selfs.txt"))
    //     loadFile("selfs.txt", "SELF");
    // else
    //     createFile("selfs.txt");
    // if (fileExists("meal-plans.txt"))
    //     loadFile("meal-plans.txt", "MEAL_PLANS");
    // else
    //     createFile("meal-plans.txt");
    if (fileExists("final_test_case_input.txt"))
        readCommandsFile("final_test_case_input.txt");
    // system("cls");
    while (!quit)
    {
        printf(">>> ");
        scanf("%s", &cmd);
        switch (getCommand(cmd))
        {
        case 1:
        {
            // register
            char name[256];
            char family[256];
            char userID[256];
            char *password;
            char nationalID[256];
            char birthDate[256];
            char gender[256];
            char role[256];
            printf("name: ");
            scanf("%s", &name);
            printf("family: ");
            scanf("%s", &family);
            printf("user ID: ");
            scanf("%s", &userID);
            printf("password: ");
            password = inputPassword();
            printf("\n");
            printf("nationalID: ");
            scanf("%s", &nationalID);
            printf("birthdate: ");
            scanf("%s", &birthDate);
            printf("gender: ");
            scanf("%s", &gender);
            printf("type: ");
            scanf("%s", &role);
            registerUser(name,
                         family,
                         userID,
                         password,
                         nationalID,
                         birthDate,
                         gender,
                         role);
            break;
        }
        case 2:
        {
            // login
            char userID[256];
            char *password;
            printf("user ID: ");
            scanf("%s", &userID);
            printf("password: ");
            password = inputPassword();
            printf("\n");
            loginUser(userID, password);
            break;
        }
        case 3:
        {
            // change password
            char *oldPass;
            char *newPass;
            printf("current password: ");
            oldPass = inputPassword();
            printf("\n");
            printf("new password: ");
            newPass = inputPassword();
            printf("\n");
            changePassword(currentUser.userID, oldPass, newPass);
            break;
        }
        case 9:
        {
            // change-student-password
            if (isAdmin(currentUser.role))
            {
                char userID[256];
                char *newPass;
                printf("user ID: ");
                scanf("%s", &userID);
                printf("new password: ");
                newPass = inputPassword();
                printf("\n");
                changeStudentPassword(userID, newPass);
            }
            else
                printf("permission-denied\n");
            break;
        }
        case 4:
        {
            // approve
            if (isAdmin(currentUser.role))
            {
                char *userID;
                printf("user ID: ");
                scanf("%s", &userID);
                approveUser(userID);
            }
            else
                printf("permission-denied\n");
            break;
        }
        case 5:
        {
            // remove-student
            if (isAdmin(currentUser.role))
            {
                char userID[256];
                printf("user ID: ");
                scanf("%s", &userID);
                removeUser(userID, 1);
            }
            else
                printf("permission-denied\n");
            break;
        }
        case 6:
        {
            // deactivate
            if (isAdmin(currentUser.role))
            {
                char userID[256];
                printf("user ID: ");
                scanf("%s", &userID);
                deactivateUser(userID);
            }
            else
                printf("permission-denied\n");
            break;
        }
        case 7:
        {
            // define-self
            if (isAdmin(currentUser.role))
            {
                int id;
                char name[256];
                char location[256];
                int capacity;
                char type[256]; // girlish, boyish
                char meal[256]; // dinner, lunch, both
                char lunchTimeLimit[256];
                char dinnerTimeLimit[256];
                printf("id: ");
                scanf("%d", &id);
                printf("name: ");
                scanf("%s", &name);
                printf("location: ");
                scanf("%s", &location);
                printf("capacity: ");
                scanf("%d", &capacity);
                printf("type: ");
                scanf("%s", &type);
                printf("meal: ");
                scanf("%s", &meal);
                printf("lunch time limit: ");
                scanf("%s", &lunchTimeLimit);
                printf("dinner time limit: ");
                scanf("%s", &dinnerTimeLimit);
                defineSelf(id, name, location, capacity, type, meal, lunchTimeLimit, dinnerTimeLimit);
            }
            else
            {
                printf("permission-denied\n");
            }
            break;
        }
        case 10:
        {
            // define-food
            if (isAdmin(currentUser.role))
            {
                int id;
                char name[256];
                char type[256]; // dessert, food
                int price;
                printf("id: ");
                scanf("%d", &id);
                printf("name: ");
                scanf("%s", &name);
                printf("type: ");
                scanf("%s", &type);
                printf("price: ");
                scanf("%d", &price);
                defineFood(id, name, type, price);
            }
            else
            {
                printf("permission-denied\n");
            }
            break;
        }
        case 11:
        {
            // define-meal-plan
            if (isAdmin(currentUser.role))
            {
                int selfID;
                char date[256];
                char type[256]; // lunch, dinner
                int foodID;
                int count;

                printf("self-id: ");
                scanf("%d", &selfID);
                printf("date: ");
                scanf("%s", &date);
                printf("type: ");
                scanf("%s", &type);
                printf("food-id: ");
                scanf("%d", &foodID);
                printf("count: ");
                scanf("%d", &count);
                defineMealPlan(selfID, date, type, foodID, count);
            }
            else
            {
                printf("permission-denied\n");
            }
            break;
        }
        case 12:
        {
            // charge-student-account
            if (isAdmin(currentUser.role))
            {
                char userID[256];
                int amount;

                printf("user ID: ");
                scanf("%s", &userID);
                printf("amount: ");
                scanf("%d", &amount);

                chargeStudentAccount(userID, amount);
            }
            else
            {
                printf("permission-denied\n");
            }
            break;
        }
        case 13:
        {
            // reserve
            if (!isAdmin(currentUser.role) && !isEmpty(currentUser))
            {
                int selfID;
                char date[256];
                char meal[256]; // lunch, dinner
                int foodID;

                displayFoods();
                printf("self-id: ");
                scanf("%d", &selfID);
                printf("date: ");
                scanf("%s", &date);
                printf("meal: ");
                scanf("%s", &meal);
                printf("food-id: ");
                scanf("%d", &foodID);
                reserve(selfID, date, meal, foodID, currentUser.userID);
            }
            else
            {
                printf("permission-denied\n");
            }
            break;
        }

        case 14:
        {
            // take-food
            if (!isAdmin(currentUser.role) && !isEmpty(currentUser))
            {
                int selfID;
                char date[256];
                char meal[256]; // lunch, dinner
                int foodID;

                printf("self-id: ");
                scanf("%d", &selfID);
                printf("date: ");
                scanf("%s", &date);
                printf("meal: ");
                scanf("%s", &meal);
                takeFood(selfID, date, meal, currentUser.userID);
            }
            else
            {
                printf("permission-denied\n");
            }
            break;
        }
        case 15:
        {
            // charge-account
            if (!isEmpty(currentUser))
            {
                int amount;
                char cardNumber[256];
                char passCode[256];
                printf("amount: ");
                scanf("%d", &amount);
                printf("card-number: ");
                scanf("%s", &cardNumber);
                printf("pass-code: ");
                scanf("%s", &passCode);
                chargeAccount(amount, currentUser.userID);
            }
            else
            {
                printf("permission-denied\n");
            }
            break;
        }
        case 16:
        {
            // send-charge
            if (!isEmpty(currentUser))
            {
                char amount[256];
                char userID[256];
                char name[256];
                printf("amount: ");
                scanf("%s", &amount);
                printf("user ID: ");
                scanf("%s", &userID);
                printf("name: ");
                scanf("%s", &name);
                sendCharge(userID, atoi(amount), name);
            }
            break;
        }
        case 17:
        {
            // cancel-reserve
            if (!isAdmin(currentUser.role) && !isEmpty(currentUser))
            {
                char date[256];
                char meal[256];

                printf("date: ");
                scanf("%s", &date);
                printf("meal: ");
                scanf("%s", &meal);

                cancelReserve(date, meal);
            }
            else
            {
                printf("permission-denied\n");
            }
            break;
        }
        case 18:
        {
            // daily-reserve
            if (!isAdmin(currentUser.role) && !isEmpty(currentUser))
            {
                int selfID;
                int foodID;
                printf("self ID: ");
                scanf("%d", &selfID);
                printf("food ID: ");
                scanf("%d", &foodID);
                dailyReserve(selfID, foodID);
            }
            else
            {
                printf("permission-denied\n");
            }
            break;
        }
        case 19:
        {
            // change-self
            if (!isAdmin(currentUser.role) && !isEmpty(currentUser))
            {
                char date[256];
                char meal[256];
                int newSelfID;
                printf("date: ");
                scanf("%s", &date);
                printf("meal: ");
                scanf("%s", &meal);
                printf("new self id: ");
                scanf("%d", &newSelfID);
                printf("%s, %s, %d\n", date, meal, newSelfID);
                changeSelf(date, meal, newSelfID);
            }
            else
            {
                printf("permission-denied\n");
            }
            break;
        }
        case 20:
        {
            // add-news
            if (isAdmin(currentUser.role))
            {
                char *title;
                char *content;
                char date[256];
                title = (char *)malloc((256 + 1) * sizeof(char));
                printf("title: ");
                getchar();
                fgets(title, 256 + 1, stdin);
                title[strlen(title) - 1] = '\0';
                content = (char *)malloc((1024 + 1) * sizeof(char));
                printf("content: ");
                getchar();
                fgets(content, 1024 + 1, stdin);
                printf("date: ");
                scanf("%s", &date);

                printf("title: %s\ncontent: %s\ndate: %s\n", title, content, date);
                addNews(title, content, date);
            }
            else
            {
                printf("permission-denied\n");
            }
            break;
        }
        case 21:
        {
            // check-news
            checkNews(1);
            break;
        }
        case 22:
        {
            // change-datetime
            char date[11];
            char time[6];
            printf("date: ");
            scanf("%s", &date);
            printf("time: ");
            scanf("%s", &time);
            changeDateTime(date, time);
            break;
        }
        case 23:
        {
            if (!isAdmin(currentUser.role) && !isEmpty(currentUser))
            {
                vote();
            }
            else
            {
                printf("permission-denied\n");
            }
            break;
        }
        case 24:
        {
            if (isAdmin(currentUser.role))
            {
                addPoll();
            }
            else
            {
                printf("permission-denied\n");
            }
        }
        case 25:
        {
            if (isAdmin(currentUser.role))
            {
                defineAgent();
            }
            else
            {
                printf("permission-denied\n");
            }
        }
        case 8:
        {
            // logout
            logout(currentUser.userID);
            break;
        }
        case 100:
        {
            // clear screen
            system("cls");
            break;
        }
        case 0:
        {
            // exit
            quit = 1;
            break;
        }
        default:
        {
            printf("invalid\n");
        }
        }
    }

    return 0;
}
