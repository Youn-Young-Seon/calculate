#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct Animal
{
    char name[30];
    int age;
    int health;
    int food;
    int clean;
} Animal;

void create_animal(Animal *animal)
{
    std::cout << "동물의 이름은? ";
    std::cout.flush();
    std::cin >> animal->name;

    std::cout << "동물의 나이는? ";
    std::cout.flush();
    std::cin >> animal->age;

    animal->health = 100;
    animal->food = 100;
    animal->clean = 100;
}

void play(Animal *animal)
{
    animal->health += 10;
    animal->food -= 20;
    animal->clean -= 30;
}

void one_day_pass(Animal *animal)
{
    animal->health -= 10;
    animal->food -= 30;
    animal->clean -= 20;
}

void show_stat(Animal *animal)
{
    std::cout << animal->name << "의 상태" << std::endl;
    std::cout << "체력: " << animal->health << std::endl;
    std::cout << "배부름: " << animal->food << std::endl;
    std::cout << "청결: " << animal->clean << std::endl;
}

int main()
{
#ifdef _WIN32
    // Windows 콘솔 UTF-8 설정
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    Animal *list[30];
    int animal_num = 0;

    std::cout << "=== 동물 키우기 게임 ===" << std::endl;

    for (;;)
    {
        std::cout << "\n1. 동물 추가하기" << std::endl;
        std::cout << "2. 놀기" << std::endl;
        std::cout << "3. 상태 보기" << std::endl;
        std::cout << "4. 종료" << std::endl;
        std::cout << "선택: ";
        std::cout.flush();

        int input;
        std::cin >> input;

        int play_with;
        switch (input) {
        case 1:
            list[animal_num] = new Animal;
            create_animal(list[animal_num]);

            animal_num++;
            break;

        case 2:
            std::cout << "누구랑 놀게? (번호): ";
            std::cout.flush();
            std::cin >> play_with;

            if (play_with < animal_num) play(list[play_with]);
            break;

        case 3:
            std::cout << "몇 번 동물? (번호): ";
            std::cout.flush();
            std::cin >> play_with;

            if (play_with < animal_num) show_stat(list[play_with]);
            break;

        case 4:
            std::cout << "게임 종료!" << std::endl;
            // 메모리 해제
            for (int i = 0; i < animal_num; i++)
            {
                delete list[i];
            }
            return 0;

        default:
            std::cout << "잘못된 선택입니다." << std::endl;
            break;
        }

        // 하루가 지나감
        for (int i = 0; i < animal_num; i++) {
            one_day_pass(list[i]);
        }
    }

    return 0;
}