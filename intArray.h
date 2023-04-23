#ifndef INTARRAY_H
#define INTARRAY_H
using namespace std;
#include <cassert>
class IntArray : public exception
{
private:
    int m_length{};//размер мaссива 
    int* m_data{};//сами данные, мы используем указатель для хранения данных, потому что нам нужно, чтобы наш массив мог изменяться 

public:
    
    //Здесь мы добавляем конструкторы, которые позволяют нам создавать внутренние массивы
    IntArray() = default;//конструктор, который создает пустой массив

    IntArray(int length) : m_length{ length }//конструктор, который позволяет нам создавать массив заранее определенного размера  
    {
        try {
            if (length <= 0)
            {
                throw length_error("bad_lenght");
                cout << endl;
            }
        }
        catch (const length_error& le) {
            cerr << le.what() << endl;
            return;
            
        }
        if (length > 0)
            m_data = new int[length] {};//данные равны созданному массиву определенной длины
            m_length = length;
            
        }

    //деструктор, который освобождает любые динамически выделяемые данные
    ~IntArray()
    {
        delete[] m_data;
        // здесь нам не нужно устанавливать m_data равным null или m_length равным 0, поскольку объект в любом случае будет уничтожен сразу после выполнения этой функции
    }
    //эта функция стирает массив и устанавливает длину = 0
    void erase()
    {
        delete[] m_data;
        // Нам нужно убедиться, что мы установили m_data в nullptr здесь, иначе он останется указывающим на освобожденную память!
        m_data = nullptr;
        m_length = 0;
    }
    //перегрузить оператор, чтобы мы могли получить доступ к элементам массива
    int& operator[](int index)
    {
        assert(index >= 0 && index < m_length);
        return m_data[index];
    }
   /* {
        try {
            if (index <= 0 || index > m_length || m_length <= 0) {
                throw out_of_range("bad_range");
            }
            return m_data[index];
        }
        catch (const out_of_range& oor) {
            cout << oor.what() << endl;

        }
    }*/
   
    
    // перераспределение изменяет размер массива. Все существующие элементы будут уничтожены. Эта функция работает быстро.
    void reallocate(int newLength)
    {
        // Сначала мы удаляем все существующие элементы
        erase();

        // Если наш массив сейчас будет пустым, вернитесь сюда
        if (newLength <= 0)
            return;

        // Тогда мы должны выделить новые элементы
        m_data = new int[newLength];
        m_length = newLength;
    }

    // resize изменяет размер массива. Все существующие элементы будут сохранены. Эта функция работает медленно.
    void resize(int newLength)
    {
        try {
            if (m_length <= 0)
            {
                throw length_error("bad_lenght");
            }
        }
        catch (const length_error& len) {
            cout << len.what() << endl;
            return;

        }

        // если массив уже имеет нужную длину, мы закончили
        if (newLength == m_length)
            return;

        // Если мы изменяем размер до пустого массива, сделайте это и верните
        if (newLength <= 0)
        {
            erase();
            return;
        }

        // Теперь мы можем предположить, что newLength - это как минимум 1 элемент. Этот алгоритм
        // работает следующим образом: Сначала мы собираемся выделить новый массив. Затем мы
        // собираемся скопировать элементы из существующего массива в новый массив.
        // Как только это будет сделано, мы можем уничтожить старый массив и заставить m_data
        // указывать на новый массив.

        // Сначала мы должны выделить новый массив
        int* data{ new int[newLength] };

        // Затем мы должны выяснить, сколько элементов скопировать из существующего
        // array в новый массив. Мы хотим скопировать столько элементов, сколько есть
        // в меньшем из двух массивов.
        if (m_length > 0)
        {
            int elementsToCopy{ (newLength > m_length) ? m_length : newLength };//Это используется для копирования не более чем элементов в массиве m_length
            // Теперь скопируйте элементы один за другим
            for (int index{ 0 }; index < elementsToCopy; ++index)
                //здесь мы используем этот "index < elementsToCopy" 
                //потому что первый элемент массива равен [0], 
                //итак, нам нужно копировать до тех пор, пока мы не скопируем их все
                data[index] = m_data[index];
        }

        // Теперь мы можем удалить старый массив, потому что он нам больше не нужен
        delete[] m_data;

        // И вместо этого используйте новый массив! Обратите внимание, что это просто приводит к тому, что m_data указывает
        // на тот же адрес, что и новый массив, который мы динамически выделили. Потому что
        // данные были распределены динамически, они не будут уничтожены, когда выйдут за пределы области видимости.
        m_data = data;
        m_length = newLength;
    }
    // если нам нужно скопировать массив, мы добавляем конструктор копирования и оператор присваивания
    IntArray(const IntArray& a)
    {
        // Установите размер нового массива соответствующим образом
        reallocate(a.getLength());

        // Затем скопируйте элементы
        for (int index{ 0 }; index < m_length; ++index)
            m_data[index] = a.m_data[index];
    }
    //оператор присваивания
    IntArray& operator=(const IntArray& a)
    {
        // Проверка самоназначения
        if (&a == this)
            return *this;

        // Установите размер нового массива соответствующим образом
        reallocate(a.getLength());

        // Затем скопируйте элементы
        for (int index{ 0 }; index < m_length; ++index)
            m_data[index] = a.m_data[index];

        return *this;
    }
    //функции вставки и удаления
    void insertBefore(int value, int index)
    {
        // Проверка на вменяемость значения нашего индекса
        try {
            if (index < 0 || index > m_length || m_length < 0) {
                throw out_of_range("bad_range");
                cout << endl;
            }

            // Сначала создайте новый массив, на один элемент больший, чем старый массив
            int* data{ new int[m_length + 1] };

            // Скопируйте все элементы вплоть до индекса
            for (int before{ 0 }; before < index; ++before)
                data[before] = m_data[before];

            // Вставляем наш новый элемент в новый массив
            data[index] = value;

            // Скопируйте все значения после вставленного элемента
            for (int after{ index }; after < m_length; ++after)
                data[after + 1] = m_data[after];

            // Наконец, удалите старый массив и используйте вместо него новый массив
            delete[] m_data;
            m_data = data;
            ++m_length;
        }
    catch (const out_of_range& oorh) {
    cout << oorh.what() << endl; }
               
    }

    void remove(int index)
    {
        // Проверка на вменяемость значения нашего индекса
        /*assert(index >= 0 && index < m_length);*/
        try {
            if (index <= 0 || index >= m_length || m_length < 0) {
                throw out_of_range("bad_range");
                cout << endl;
                return;
            }
        

        // Если это последний оставшийся элемент в массиве, установите для массива значение empty и извлеките его
        if (m_length == 1)
        {
            erase();
            return;
        }

        // Сначала создайте новый массив, на один элемент меньший, чем старый массив
        int* data{ new int[m_length - 1] };

        // Скопируйте все элементы вплоть до индекса
        for (int before{ 0 }; before < index; ++before)
            data[before] = m_data[before];

        // Скопируйте все значения после удаленного элемента
        for (int after{ index + 1 }; after < m_length; ++after)
            data[after - 1] = m_data[after];

        // Наконец, удалите старый массив и используйте вместо него новый массив
        delete[] m_data;
        m_data = data;
        --m_length;
    }
            catch (const out_of_range& oorg) {
                cout << oorg.what() << endl; }
    }

    // Пара дополнительных функций просто для удобства
    void insertAtBeginning(int value) { insertBefore(value, 0); }
    void insertAtEnd(int value) { insertBefore(value, m_length); }
    int getLength() const { return m_length; }
};
#endif

