#include <iostream>
using namespace std;

const int DEFAULT_MEM = 100; // количество элементов массива по умолчанию
const int DOP_MEM = 10; // количество дополнительных элементов, создаваемых при выделении пам€ти 


enum ErrorCode
{
	errorCount_LessZero,           
	errorIndexLessZero,
	errorIndexOutsideArray,
	errorCountLessZero,
};

template <class T> class Array
{
	T* array_;
	int count_, memory_;

public:
	Array(int memory = DEFAULT_MEM);
	Array(T* a, int count);
	Array(const Array<T>& copy);

	~Array() { delete array_; };

	
	int findFirstKey(T key);
	int findMinKey();
	int findMaxKey();

	void input();
	void output();
	
	void addElem(int index, T key);

	void swap(int a, int b);
	void sortingDesc();
	void sortingAsc();

	void shiftRight(int count, int index, T emptyElem);
	void shiftRightSave(int count, int index, T emptyElem);
	void shiftLeft(int count, int index, T emptyElem);
	
	Array<T> operator -(T key);
	void deleteKey(T key);

	void operator -=(int index);
	void deleteIndex(int index);

	Array<T>& operator =(const Array<T>& newArray);

	T& operator [](int index);
	Array operator +(T key);
	void operator +=(T key);

	Array operator +(const Array<T>& Mas);
	void operator +=(const Array<T>& Mas);

	bool operator ==(const Array<T>& Mas);
	bool operator !=(const Array<T>& Mas);

	template <class T> friend ostream& operator <<(ostream& r, Array<T>& Mas);
	template <class T> friend istream& operator >>(istream& r, Array<T>& Mas);
};

//конструктор по умолчанию
template <class T>
Array<T>::Array(int memory)
{
	array_ = new T[memory];
	memory_ = memory;
	count_ = 0;
}

//конструктор с параметрами
template <class T>
Array<T>::Array(T* a, int count)
{
	if (count < 0) throw errorCount_LessZero;

	memory_ = count + DOP_MEM;
	array_ = new T[memory_];
	count_ = count;

	for (int i = 0; i < count_; i++) array_[i] = a[i];
}

//конструктор копировани€
template <class T>
Array<T>::Array(const Array<T>& copy)
{
	memory_ = copy.memory_;
	count_ = copy.count_;
	array_ = new T[memory_];
	for (int i = 0; i < count_; i++) array_[i] = copy.array_[i];
}


//поиск индекса первого вхождени€ элемента
template <class T>
int Array<T>::findFirstKey(T key)
{
	for (int i = 0; i < count_; i++)
		if (key == array_[i]) return i;

	return -1;
}

//поиск минимального и максимального элемента(возвращают -1 если массив пуст)
template <class T>
int Array<T>::findMinKey()
{
	if (count_ == 0) return -1;

	int indexMin = 0;
	for (int i = 1; i < count_; i++)
		if (array_[indexMin] > array_[i]) indexMin = i;
	
	return indexMin;
}

template <class T>
int Array<T>::findMaxKey()
{
	if (count_ == 0) return -1;

	int indexMax = 0;
	for (int i = 1; i < count_; i++)
		if (array_[indexMax] < array_[i]) indexMax = i;

	return indexMax;
}
//ввод-вывод (в консоль)
template <class T>
void  Array<T>::input()
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
		array_ = new T[memory_];
	}

	cout << "\nEnter array: ";
	for (int i = 0; i < count_; i++) cin >> array_[i];
}

template <class T>
void  Array<T>::output()
{
	if (count_ > 0)
	{
		cout << "Array: ";
		for (int i = 0; i < count_; i++) cout << array_[i] << ' ';
		cout << "\n" << count_ << " slots out of " << memory_ << " occupied.\n\n";
	}
	else cout << "Array is empty. " << memory_ << " slots avilable.\n\n";
}

//добавление элемента на заданную позицию
template <class T>
void Array<T>::addElem(int index, T key)
{
	if (index < 0) throw errorIndexLessZero;
	
	if (index < count_)
	{
		this->shiftRightSave(1, index, key);
	}
	else
		if (index < memory_)
		{
			//for (int i = count_; i < index; i++) array_[i] = EMPTY_ELEM;
			count_ = index + 1;
			array_[index] = key;
		}
		else
			if(index >= memory_)
			{
				T* a = new T[index + DOP_MEM];
				for (int i = 0; i < count_; i++) a[i] = array_[i];
				//for (int i = count_; i < index; i++) a[i] = EMPTY_ELEM;
				a[index] = key;
				delete array_;
				array_ = a;
				count_ = index + 1;
				memory_ = index + DOP_MEM + 1;
			}
}

//мен€ютс€ местами 2 элемента массива
template <class T>
void Array<T>::swap(int a, int b)
{
	if ((a < 0) || (a >= count_) || (b < 0) || (b >= count_)) throw errorIndexOutsideArray;

	T c = array_[a];
	array_[a] = array_[b];
	array_[b] = c;
}

//сортировки (по убыванию и возрастанию)
template <class T>
void Array<T>::sortingDesc()
{
	for (int i = count_ - 1; i > 0; i--)
		for (int j = 0; j < i; j++)
			if (array_[j] > array_[j + 1]) swap(j, j + 1);
}

template <class T>
void Array<T>::sortingAsc()
{
	for (int i = 0; i < count_; i++)
		for (int j = count_ - 1; j > i; j--)
			if (array_[j] > array_[j - 1]) swap(j, j - 1);
}

//сдвиг (вправо(+ верси€ сдвига вправо без потери элементов) и влево)
template <class T>
void Array<T>::shiftRight(int count, int index, T emptyElem)
{
	if (count < 0) throw errorCountLessZero;
	if (count > count_ - index) count = count_ - index;

	for (int i = count_ - 1; i >= (index + count); i--) array_[i] = array_[i - count];

	for (int i = 0; i < count; i++) array_[index + i] = emptyElem;
}

template <class T>
void Array<T>::shiftRightSave(int count, int index, T emptyElem)
{
	if (count < 0) throw errorCountLessZero;

	if (count + count_ > memory_)
	{
		memory_ = count_ + count + DOP_MEM;
		count_ += count;
		T* newArr = new T[memory_];
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

template <class T>
void Array<T>::shiftLeft(int count, int index, T emptyElem)
{
	if (count < 0) throw errorCountLessZero;
	if (count > index + 1) count = index + 1;

	for (int i = 0; i <= (index - count); i++) array_[i] = array_[i + count];

	for (int i = 0; i < count; i++) array_[index - i] = emptyElem;
}

//удаление первого вхождени€ ключа
template <class T>
Array<T> Array<T>::operator -(T key)
{
	int index = this->findFirstKey(key);
	Array<T> result(*this);

	if (index != -1)
	{
		for (int i = index; i < count_ - 1; i++) result.array_[i] = result.array_[i + 1];
		result.count_--;
	}
	return result;
}

template <class T>
void Array<T>::deleteKey(T key)
{
	int index = this->findFirstKey(key);

	if (index != -1)
	{
		for (int i = index; i < count_ - 1; i++) array_[i] = array_[i + 1];
		count_--;
	}
}

template <class T>
//удаление элемента по идексу
void Array<T>::operator -=(int index)
{
	if (index < 0) throw errorIndexLessZero;

	if (index < count_)
	{
		for (int i = index; i < count_ - 1; i++) array_[i] = array_[i + 1];
		count_--;
	}
}

template <class T>
void Array<T>::deleteIndex(int index)
{
	if (index < 0) throw errorIndexLessZero;

	if (index < count_)
	{
		for (int i = index; i < count_ - 1; i++) array_[i] = array_[i + 1];
		count_--;
	}
}


//операци€ присвоени€
template <class T>
Array<T>& Array<T>::operator =(const Array& newArray)
{
	if (this != &newArray)
	{
		delete array_;

		memory_ = newArray.memory_;	
		count_ = newArray.count_;	
		array_ = new T[memory_];	
		for (int i = 0; i < count_; i++) array_[i] = newArray.array_[i];
	}
	return *this;
}

// вз€тие ссылки на элемент
template <class T>
T& Array<T>::operator [](int index)
{
	if ((index < 0) || (index >= count_)) throw errorIndexOutsideArray;
	return array_[index];
}

// добавление ключа в начало
template <class T>
Array<T> Array<T>::operator +(T key)
{
	Array result(*this);
	result.shiftRightSave(1, 0, key);
	return result;
}

// добавление ключа в конец
template <class T>
void Array<T>::operator +=(T key)
{
	if (count_ + 1 > memory_)
	{
		T* a = new T[count_ + 1 + DOP_MEM];

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
template <class T>
Array<T> Array<T>::operator +(const Array<T>& Mas)
{
	Array result(*this);
	if (Mas.count_ != 0)
	{	
		result.shiftRightSave(Mas.count_, 0, Mas.array_[0]);	
		for (int i = 1; i < Mas.count_; i++) result.array_[i] = Mas.array_[i];
	}

	return result;
}

// добавление массива в конец
template <class T>
void Array<T>::operator +=(const Array<T>& Mas)
{

	if (count_ + Mas.count_ > memory_)
	{
		T* a = new T[count_ + Mas.count_ + DOP_MEM];

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
template <class T>
bool Array<T>::operator ==(const Array<T>& Mas)
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

template <class T>
bool Array<T>::operator !=(const Array<T>& Mas)
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
template <class T>
ostream& operator <<(ostream& r, Array<T>& Mas)
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

template <class T>
istream& operator >>(istream& r, Array<T>& Mas)
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
		Mas.array_ = new T[Mas.memory_];
	}

	cout << "Enter array: ";
	for (int i = 0; i < Mas.count_; i++) r >> Mas.array_[i];

	return r;
}



int main()
{
	try
	{
		char W[7] = "winter";
		char S[7] = "summer";
		Array<char*> a(5);
		
		a += W;
		a += S;
		
		cout << a;

		a.sortingDesc();
		cout << a;
		/*

		Array<char> w(W, 6);
		Array<char> s(S, 6);
		Array<Array<char>> b(5);

		b += w;
		b += s;

		cout << b;*/
	}
	catch (ErrorCode a)
	{
		return a;
	}
	
	return 0;
}