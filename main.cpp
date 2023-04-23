#include <iostream>
#include "intArray.h"
#include <typeinfo>
#include <exception> 
using namespace std;
int main()
{
    // Объявляем массив из 10 элементов
    IntArray array(50);
  
    // Заполните массив числами от 1 до 10
    for (int i{ 0 }; i < 50; ++i) {
        array[i] = i + 1;   
}
   
    // Измените размер массива до 8 элементов
    array.resize(20);
   
    // Вставьте число 20 перед элементом с индексом 5
    array.insertBefore(20, 5);
    
    // Удалить элемент с индексом 3
    array.remove(7);
   
    // Добавьте 30 и 40 в конец и начало
    array.insertAtEnd(30);
    array.insertAtBeginning(40);
  

    // Еще несколько тестов для обеспечения построения копий / назначения массивов
    // ничего не ломает
    {
        IntArray b{ array };
        b = array;
        b = b;
        array = array;
    }

    // Print out all the numbers
 

    for (int i{ 0 }; i < array.getLength(); ++i)
        cout << array[i] << ' ';

    cout << '\n';
    

    return 0;
}