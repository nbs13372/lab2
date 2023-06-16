#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <map>


/*!
	\brief Класс, задающий книгу в библиотеке
*/
class Book
{
public:

	std::string authorName; ///< Имя автора
	std::string bookName; ///< Название книги
	int year; ///< Год публикации
	int pages; ///< Число страниц


	/*!
		Создает объект Book
		\param[in] authorName Имя автора
		\param[in] bookName Название книги
		\param[in] year Год публикации
		\param[in] pages Число страниц
		\return Возвращает объект Book
	*/
	Book(std::string authorName = "", std::string bookName = "", int year = 1970, int pages = 300)
	{
		this->authorName = authorName;
		this->bookName = bookName;
		this->year = year;
		this->pages = pages;
	}

	/*!
		Оператор сравнения меньше
		\param[in] right правое сравниваемое значение
		\return Возвращает true, если левый объект строго меньше, false - иначе
	*/
	bool operator<(Book& right)
	{
		return this->authorName < right.authorName;
	}

	/*!
		Оператор сравнения больше
		\param[in] right правое сравниваемое значение
		\return Возвращает true, если левый объект строго больше, false - иначе
	*/
	bool operator>(Book& right)
	{
		return this->authorName > right.authorName;
	}

	/*!
		Оператор сравнения меньше или равно
		\param[in] right правое сравниваемое значение
		\return Возвращает true, если левый объект меньше или равен, false - иначе
	*/
	bool operator<=(Book& right)
	{
		return this->authorName <= right.authorName;
	}

	/*!
		Оператор сравнения больше или равно
		\param[in] right правое сравниваемое значение
		\return Возвращает true, если левый объект больше или равен, false - иначе
	*/
	bool operator>=(Book& right)
	{
		return this->authorName >= right.authorName;
	}

	bool operator==(const Book& right) const
	{
		return this->authorName == right.authorName;
	}
};


/*!
	\brief Класс, содержащий реализуемые сортировки
*/
template<class T>
class Sorting
{
public:

	/*!
		Оболочка для быстрой сортировки
		\param[in] data Сортируемый массив
		\param[in] n Число элементов в массиве
	*/
	static void quickSort(T* data, int n)
	{
		quickSortHandler(data, 0, n - 1);
	}

private:
	/*!
		Просеивание элементов для создания кучи
		\param[in] data Сортируемый массив
		\param[in] start Индекс начала сортировки
		\param[in] end Индекс конца сортировки
	*/
	static void quickSortHandler(Book* data, int start, int end)
	{
		int i = start, j = end;

		Book p = data[(start + end) / 2];

		while (i < j)
		{
			while (i <= end && data[i] <= p) i++;
			while (j >= start && data[j] >= p) j--;

			if (i < j)
				std::swap(data[i], data[j]);
		}

		if (j > start) quickSortHandler(data, start, j - 1);
		if (i < end) quickSortHandler(data, i + 1, end);
	}
};


/*!
	\brief Класс, содержащий алгоритмы поиска
*/
template<class T>
class Searching
{
public:
	/*!
		Линейный поиск
		\param[in] data Массив, в котором ищутся элементы
		\param[in] n Число элементов в массиве
		\param[in] searching Элемент, который мы ищем
	*/
	static std::vector<int> linear(T data[], int n, T& searching)
	{
		std::vector<int> result;
		for (int i = 0; i < n; i++)
			if (data[i] == searching)
				result.push_back(i);

		return result;
	}

	/*!
		Бинарный поиск
		\param[in] data Массив, в котором ищутся элементы
		\param[in] n Число элементов в массиве
		\param[in] searching Элемент, который мы ищем
	*/
	static std::vector<int> binary(T data[], int n, T& searching)
	{
		int min = 0, max = n - 1;
		std::vector<int> result;
		while (min <= max && !(data[(min + max) / 2] == searching))
		{
			if (data[(min + max) / 2] > searching)
				max = (min + max) / 2 - 1;
			else
				min = (min + max) / 2 + 1;
		}
		if (min <= max)
		{
			int i = (min + max) / 2;
			while (--i >= 0 && data[i] == searching) {}

			while (++i < n && data[i] == searching) result.push_back(i);
		}
		return result;
	}
};


/*!
		Основной метод программы
		\return Код завершения программы
*/
int main()
{
	int sizes[9] = {100, 1000, 5000, 10000, 20000, 40000, 60000, 80000, 100000};

	for (int size : sizes)
	{
		std::ifstream in;
		in.open("data_" + std::to_string(size) + ".txt");
		Book* data = new Book[size];
		std::multimap<std::string, Book> multimap;

		for (int i = 0; i < size; i++)
		{
			std::string authorName, bookName;
			int year, pages;
			in >> authorName >> bookName >> year >> pages;

			data[i] = Book(authorName, bookName, year, pages);
			multimap.insert(std::pair<std::string, Book>(authorName, data[i]));
		}

		in.close();


		Book searching = data[(int)(rand() / RAND_MAX * size)];

		auto begin = std::chrono::high_resolution_clock::now();

		Searching<Book>::linear(data, size, searching);

		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "Linear search " << size << " elements " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " mcs" << std::endl;

		auto sort_begin = std::chrono::high_resolution_clock::now();

		Sorting<Book>::quickSort(data, size);

		auto search_begin = std::chrono::high_resolution_clock::now();

		Searching<Book>::binary(data, size, searching);

		end = std::chrono::high_resolution_clock::now();

		std::cout << "Quick sort " << size << " elements " << std::chrono::duration_cast<std::chrono::microseconds>(search_begin - sort_begin).count() << " mcs" << std::endl;
		std::cout << "Binary search " << size << " elements " << std::chrono::duration_cast<std::chrono::microseconds>(end - search_begin).count() << " mcs" << std::endl;
		std::cout << "Binary search with sorting " << size << " elements " << std::chrono::duration_cast<std::chrono::microseconds>(end - sort_begin).count() << " mcs" << std::endl;

		begin = std::chrono::high_resolution_clock::now();

		multimap.find(searching.authorName);

		end = std::chrono::high_resolution_clock::now();

		std::cout << "Multimap search " << size << " elements " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " mcs" << std::endl << std::endl;
	}
}


//Создание файлов с данными
/*std::string genString()
{
	int i = std::rand() % 50 + 10;

	std::string result = "";

	for (int k = 0; k < i; k++)
	{
		char l = std::rand() % 26 + 0x41;
		result.append(std::string(1, l));
	}

	return result;
}

std::string creatOne()
{
	return genString() + " " + genString() + " " + std::to_string(std::rand() % 50 + 1970) + " " + std::to_string(std::rand() % 999 + 1) + "\n";
}*/
