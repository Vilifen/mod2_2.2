
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <limits.h>

#include "Calculator.h"

static int ReadInt(const char* prompt, int* value)
{
    char line[128];

    while (1)
    {
        printf("%s", prompt);

        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            return 0;
        }

        char* end;
        errno = 0;
        long number = strtol(line, &end, 10);

        if (end == line || errno == ERANGE ||
            number < INT_MIN || number > INT_MAX)
        {
            printf("Ошибка! Введите целое число.\n");
            continue;
        }

        while (*end == ' ' || *end == '\t' ||
               *end == '\n' || *end == '\r')
        {
            end++;
        }

        if (*end != '\0')
        {
            printf("Ошибка! Введите целое число.\n");
            continue;
        }

        *value = (int)number;
        return 1;
    }
}

static int ReadDouble(const char* prompt, double* value)
{
    char line[128];

    while (1)
    {
        printf("%s", prompt);

        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            return 0;
        }

        char* end;
        errno = 0;
        double number = strtod(line, &end);

        if (end == line || errno == ERANGE || !isfinite(number))
        {
            printf("Ошибка! Введите корректное число.\n");
            continue;
        }

        while (*end == ' ' || *end == '\t' ||
               *end == '\n' || *end == '\r')
        {
            end++;
        }

        if (*end != '\0')
        {
            printf("Ошибка! Введите корректное число.\n");
            continue;
        }

        *value = number;
        return 1;
    }
}

static void Menu(void)
{
    int action;
    double arg_1;
    double arg_2;
    double result;

    printf("\nДоступные действия:\n");
    printf("1 - Сложение\n");
    printf("2 - Вычитание\n");
    printf("3 - Умножение\n");
    printf("4 - Деление\n");
    printf("5 - Максимум\n");
    printf("6 - Минимум\n");
    printf("7 - Выход\n");

    if (!ReadInt("Введите номер действия: ", &action))
    {
        exit(EXIT_SUCCESS);
    }

    if (action == 7)
    {
        printf("Выход из программы.\n");
        exit(EXIT_SUCCESS);
    }

    if (action < 1 || action > 6)
    {
        printf("Некорректный номер действия!\n");
        return;
    }

    /* Деление принимает ровно два аргумента. */
    if (action == 4)
    {
        if (!ReadDouble("Введите первое число: ", &arg_1) ||
            !ReadDouble("Введите второе число: ", &arg_2))
        {
            exit(EXIT_SUCCESS);
        }

        if (Division(arg_1, arg_2, &result))
        {
            printf("Ответ: %.10g\n", result);
        }
        else
        {
            printf("Ошибка: деление на ноль невозможно!\n");
        }

        return;
    }

    /* Для остальных операций количество аргументов задаёт пользователь. */
    int count;

    if (!ReadInt("Введите количество аргументов (не менее 2): ",
                 &count))
    {
        exit(EXIT_SUCCESS);
    }

    if (count < 2)
    {
        printf("Количество аргументов должно быть не меньше 2.\n");
        return;
    }

    double* args = malloc((size_t)count * sizeof(*args));

    if (args == NULL)
    {
        printf("Ошибка: не удалось выделить память.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        char prompt[64];

        snprintf(prompt, sizeof(prompt),
                 "Введите аргумент %d: ", i + 1);

        if (!ReadDouble(prompt, &args[i]))
        {
            free(args);
            exit(EXIT_SUCCESS);
        }
    }

    switch (action)
    {
        case 1:
            result = Addition((size_t)count, args);
            break;

        case 2:
            result = Subtraction((size_t)count, args);
            break;

        case 3:
            result = Multiplication((size_t)count, args);
            break;

        case 5:
            result = Max((size_t)count, args);
            break;

        case 6:
            result = Min((size_t)count, args);
            break;

        default:
            free(args);
            return;
    }

    if (isfinite(result))
    {
        printf("Ответ: %.10g\n", result);
    }
    else
    {
        printf("Ошибка: результат выходит за допустимый диапазон.\n");
    }

    free(args);
}

int main(void)
{
    printf("Задание 2.2: Калькулятор\n");

    while (1)
    {
        Menu();
    }

    return 0;
}
