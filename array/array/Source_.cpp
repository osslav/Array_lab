#include <iostream>
#include <ctime>
using namespace std;

const int DEFAULT_MEM = 100; // количество элементов массива по умолчанию
const int DOP_MEM = 10; // количество дополнительных элементов, создаваемых при выделении памяти 
const int MAX_NUMBER = 99; // максимально возможный элемент массива при случайном заполнении
const int EMPTY_ELEM = 0; // число, записываемое в  ячейки, для которых не определено конкретно что в них должно быть;

/* коды исключений:
конструктор с параметрами:
0 - в конструктор введено отрицательное n

addElem :
1 - получена отрицательная позиция;

swap :
2 - первый операнд выходит за границы массива
3 - второй операнд выходит за границы массива

shiftRight :
4 - нельзя сдвинуть на отрицательное число

shiftRightSave :
5 - нельзя сдвинуть на отрицательное число

shiftLeft :
6 - нельзя сдвинуть на отрицательное число 

operator [] :
7 - индекс выходит за границы массива

operator - :
8 - попытка удалить отрицательный индекс

deleteIndex :
9 - попытка удалить отрицательный индекс
*/
class Array
{
	int* array_;
	int count_, memory_;

public:
	Array();
	Array(int* a, int count);
	Array(const Array& copy);

	~Array() { delete array_; };

	int findFirstKey(int key);
	int findMinKey();
	int findMaxKey();

	void input();
	void output();

	void addElem(int index, int key);

	void swap(int a, int b);
	void sortingDesc();
	void sortingAsc();

	void shiftRight(int count, int index, int emptyElem = EMPTY_ELEM);
	void shiftRightSave(int count, int index, int emptyElem = EMPTY_ELEM);
	void shiftLeft(int count, int index, int emptyElem = EMPTY_ELEM);

	Array operator -(int key);
	void deleteKey(int key);

	void operator -=(int index);
	void deleteIndex(int index);

	void operator =(const Array& newArray);

	void randArray(int count);
	void randArrayDesc(int count);
	void randArrayAsc(int count);

	int& operator [](int index);
	Array operator +(int key);
	void operator +=(int key);

	Array operator +(const Array& Mas);
	void operator +=(const Array& Mas);

	bool operator ==(const Array& Mas);
	bool operator !=(const Array& Mas);

	friend ostream& operator <<(ostream& r, Array& Mas);
	friend istream& operator >>(istream& r, Array& Mas);
};

//конструктор по умолчанию
Array::Array()
{
	array_ = new int[DEFAULT_MEM];
	memory_ = DEFAULT_MEM;
	count_ = 0;
}

//конструктор с параметрами
Array::Array(int* a, int count)
{
	if (count < 0) throw 0;

	memory_ = count + DOP_MEM;
	array_ = new int[memory_];
	count_ = count;

	for (int i = 0; i < count_; i++) array_[i] = a[i];
}

//конструктор копирования
Array::Array(const Array& copy)
{
	memory_ = copy.memory_;
	count_ = copy.count_;
	array_ = new int[memory_];
	for (int i = 0; i < count_; i++) array_[i] = copy.array_[i];
}

//поиск индекса первого вхождения числа
int Array::findFirstKey(int key)
{
	for (int i = 0; i < count_; i++)
		if (key == array_[i]) return i;

	return -1;
}

//поиск минимального и максимального элемента(возвращают -1 если массив пуст)
int Array::findMinKey()
{
	if (count_ == 0) return -1;

	int indexMin = 0;
	for (int i = 1; i < count_; i++)
		if (array_[indexMin] > array_[i]) indexMin = i;
	
	return indexMin;
}

int Array::findMaxKey()
{
	if (count_ == 0) return -1;

	int indexMax = 0;
	for (int i = 1; i < count_; i++)
		if (array_[indexMax] < array_[i]) indexMax = i;

	return indexMax;
}
//ввод-вывод (в консоль)
void  Array::input()
{
	cout << "Enter number of elements: ";
	cin >> count_;
	while (count_ < 0)
	{
		cout << "Error. This number must >= 0. Enter here again: ";
		cin >> count_;
	}

	if (count_ > memory_)
	{
		delete array_;
		memory_ = count_ + DOP_MEM;
		array_ = new int[memory_];
	}

	cout << "\nEnter array(enter space between numbers): ";
	for (int i = 0; i < count_; i++) cin >> array_[i];
}

void  Array::output()
{
	if (count_ > 0)
	{
		cout << "Array: ";
		for (int i = 0; i < count_; i++) cout << array_[i] << "  ";
		cout << "\n" << count_ << " slots out of " << memory_ << " occupied.\n\n";
	}
	else cout << "Array is empty. " << memory_ << " slots avilable.\n\n";
}

//добавление элемента на заданную позицию
void Array::addElem(int index, int key)
{
	if (index < 0) throw 1;
	
	if (index < count_)
	{
		this->shiftRightSave(1, index);
		array_[index] = key;
	}
	else
		if (index < memory_)
		{
			for (int i = count_; i < index; i++) array_[i] = EMPTY_ELEM;
			count_ = index + 1;
			array_[index] = key;
		}
		else
			if(index >= memory_)
			{
				int* a = new int[index + DOP_MEM];
				for (int i = 0; i < count_; i++) a[i] = array_[i];
				for (int i = count_; i < index; i++) a[i] = EMPTY_ELEM;
				a[index] = key;
				delete array_;
				array_ = a;
				count_ = index + 1;
				memory_ = index + DOP_MEM + 1;
			}
}

//меняются местами 2 элемента массива
void Array::swap(int a, int b)
{
	if ((a < 0) || (a >= count_)) throw 2;
	if ((b < 0) || (b >= count_)) throw 3;

	int c = array_[a];
	array_[a] = array_[b];
	array_[b] = c;
}

//сортировки (по убыванию и возрастанию)
void Array::sortingDesc()
{
	for (int i = count_ - 1; i > 0; i--)
		for (int j = 0; j < i; j++)
			if (array_[j] > array_[j + 1]) swap(j, j + 1);
}

void Array::sortingAsc()
{
	for (int i = 0; i < count_; i++)
		for (int j = count_ - 1; j > i; j--)
			if (array_[j] > array_[j - 1]) swap(j, j - 1);
}

//сдвиг (вправо(+ версия сдвига вправо без потери элементов) и влево)
void Array::shiftRight(int count, int index, int emptyElem)
{
	if (count < 0) throw 4;
	if (count > count_ - index) count = count_ - index;

	for (int i = count_ - 1; i >= (index + count); i--) array_[i] = array_[i - count];

	for (int i = 0; i < count; i++) array_[index + i] = emptyElem;
}

void Array::shiftRightSave(int count, int index, int emptyElem)
{
	if (count < 0) throw 5;

	if (count + count_ > memory_)
	{
		memory_ = count_ + count + DOP_MEM;
		count_ += count;
		int* newArr = new int[memory_];
		int i = 0;

		for (; i < index; i++) newArr[i] = array_[i];
		for (; i < index + count; i++) newArr[i] = emptyElem;
		for (; i < count_; i++) newArr[i] = array_[i - count];

		delete array_;
		array_ = newArr;
	}
	else
	{
		count_ += count;
		for (int i = count_ - 1; i >= (index + count); i--) array_[i] = array_[i - count];
		for (int i = 0; i < count; i++) array_[index + i] = emptyElem;
	}
}

void Array::shiftLeft(int count, int index, int emptyElem)
{
	if (count < 0) throw 6;
	if (count > index + 1) count = index + 1;

	for (int i = 0; i <= (index - count); i++) array_[i] = array_[i + count];

	for (int i = 0; i < count; i++) array_[index - i] = emptyElem;
}

//удаление первого вхождения ключа
Array Array::operator -(int key)
{
	int index = this->findFirstKey(key);
	Array result(*this);

	if (index != -1)
	{
		for (int i = index; i < count_ - 1; i++) result.array_[i] = result.array_[i + 1];
		result.count_--;
	}
	return result;
}

void Array::deleteKey(int key)
{
	int index = this->findFirstKey(key);

	if (index != -1)
	{
		for (int i = index; i < count_ - 1; i++) array_[i] = array_[i + 1];
		count_--;
	}
}


//удаление элемента по идексу
void Array::operator -=(int index)
{
	if (index < 0) throw 8;

	if (index < count_)
	{
		for (int i = index; i < count_ - 1; i++) array_[i] = array_[i + 1];
		count_--;
	}
}

void Array::deleteIndex(int index)
{
	if (index < 0) throw 9;

	if (index < count_)
	{
		for (int i = index; i < count_ - 1; i++) array_[i] = array_[i + 1];
		count_--;
	}
}


//операция присвоения
void Array::operator =(const Array& newArray)
{
	if (this != &newArray)
	{
		delete array_;

		memory_ = newArray.memory_;	
		count_ = newArray.count_;	
		array_ = new int[memory_];	
		for (int i = 0; i < count_; i++) array_[i] = newArray.array_[i];
	}
}

//заполнение массива случайными числами (полностью случайным, по возрастанию, по убыванию)
void Array::randArray(int count)
{
	if (count < 0) throw 0;

	if (count > memory_)
	{
		delete array_;
		memory_ = count + DOP_MEM;
		array_ = new int[memory_];
	}

	count_ = count;
	for (int i = 0; i < count_; i++) array_[i] = rand() % (MAX_NUMBER + 1);
}

void Array::randArrayDesc(int count)
{
	if (count < 0) throw 0;

	if (count > memory_)
	{
		delete array_;
		memory_ = count + DOP_MEM;
		array_ = new int[memory_];
	}
	count_ = count;

	array_[0] = rand() % (MAX_NUMBER + 1);
	for (int i = 1; i < count_; i++)
	{
		int newElem = rand() % (MAX_NUMBER + 1), j = 0;
		
		while ((j < i) && (array_[j] < newElem)) j++;
		this->shiftRight(1, j, newElem);
	}
}

void Array::randArrayAsc(int count)
{
	if (count < 0) throw 0;

	if (count > memory_)
	{
		delete array_;
		memory_ = count + DOP_MEM;
		array_ = new int[memory_];
	}
	count_ = count;

	array_[0] = rand() % (MAX_NUMBER + 1);
	for (int i = 1; i < count_; i++)
	{
		int newElem = rand() % (MAX_NUMBER + 1), j = 0;

		while ((j < i) && (array_[j] > newElem)) j++;
		this->shiftRight(1, j, newElem);
	}
}


// взятие ссылки на элемент
int& Array::operator [](int index)
{
	if ((index < 0) || (index >= count_)) throw 7;
	return array_[index];
}

// добавление ключа в начало
Array Array::operator +(int key)
{
	Array result(*this);
	result.shiftRightSave(1, 0);
	result.array_[0] = key;
	return result;
}

// добавление ключа в конец
void Array::operator +=(int key)
{
	if (count_ + 1 > memory_)
	{
		int* a = new int[count_ + 1 + DOP_MEM];

		for (int i = 0; i < count_; i++) a[i] = array_[i];
		a[count_] = key;

		delete array_;
		array_ = a;
		memory_ = count_ + 1 + DOP_MEM;
		count_ ++;
	}
	else
	{
		array_[count_] = key;
		count_++;
	}
}


// добавление массива в начало
Array Array::operator +(const Array& Mas)
{
	Array result(*this);
	result.shiftRightSave(Mas.count_, 0);
	for (int i = 0; i < Mas.count_; i++) result.array_[i] = Mas.array_[i];
	return result;
}

// добавление массива в конец
void Array::operator +=(const Array& Mas)
{

	if (count_ + Mas.count_ > memory_)
	{
		int* a = new int[count_ + Mas.count_ + DOP_MEM];

		for (int i = 0; i < count_; i++) a[i] = array_[i];
		for (int i = 0; i < Mas.count_; i++) a[count_ + i] = Mas.array_[i];

		delete array_;
		array_ = a;
		memory_ = count_ + Mas.count_ + DOP_MEM;
		count_ += Mas.count_;
	}
	else
	{
		for (int i = 0; i < Mas.count_; i++) array_[count_ + i] = Mas.array_[i];
		count_ += Mas.count_;
	}
}

//сравнение массивов
bool Array::operator ==(const Array& Mas)
{
	if (count_ == Mas.count_)
	{
		for (int i = 0; i < count_; i++)
			if (array_[i] != Mas.array_[i])
				return false;
		return true;
	}
	return false;
}

bool Array::operator !=(const Array& Mas)
{
	if (count_ == Mas.count_)
	{
		for (int i = 0; i < count_; i++)
			if (array_[i] != Mas.array_[i])
				return true;
		return false;
	}
	return true;
}

//потоковый ввод-вывод
ostream& operator <<(ostream& r, Array& Mas)
{
	if (Mas.count_ > 0)
	{
		cout << "Array: ";
		for (int i = 0; i < Mas.count_; i++) r << Mas.array_[i] << "  ";
		r << "\n" << Mas.count_ << " slots out of " << Mas.memory_ << " occupied.\n\n";
	}
	else r << "Array is empty. " << Mas.memory_ << " slots avilable.\n\n";
	return r;
}

istream& operator >>(istream& r, Array& Mas)
{
	cout << "Enter number of elements: ";
	r >> Mas.count_;
	while (Mas.count_ < 0)
	{
		cout << "Error. This number must >= 0. Enter here again: ";
		r >> Mas.count_;
	}

	if (Mas.count_ > Mas.memory_)
	{
		delete Mas.array_;
		Mas.memory_ = Mas.count_ + DOP_MEM;
		Mas.array_ = new int[Mas.memory_];
	}

	cout << "Enter array(enter space between numbers): ";
	for (int i = 0; i < Mas.count_; i++) r >> Mas.array_[i];

	return r;
}

int main()
{
	srand(time(NULL));
	int a[3] = { 1, 4, 3 };
	Array A(a, 3);

	A.randArrayAsc(20);
	cout << A;

	cout << "\n" << A[A.findMinKey()] << " | " << A[A.findMaxKey()];
	return 0;
}