#include <iostream>
#include <ctime>
using namespace std;

const int DEFAULT_MEM = 100; // ���������� ��������� ������� �� ���������
const int DOP_MEM = 10; // ���������� �������������� ���������, ����������� ��� ��������� ������ 
const int MAX_NUMBER = 99; // ����������� ��������� ������� ������� ��� ��������� ����������
const int EMPTY_ELEM = 0; // �����, ������������ �  ������, ��� ������� �� ���������� ��������� ��� � ��� ������ ����;

/* ���� ����������:
����������� � �����������:
0 - � ����������� ������� ������������� n

addElem :
1 - �������� ������������� �������;

swap :
2 - ������ ������� ������� �� ������� �������
3 - ������ ������� ������� �� ������� �������

shiftRight :
4 - ������ �������� �� ������������� �����

shiftRightSave :
5 - ������ �������� �� ������������� �����

shiftLeft :
6 - ������ �������� �� ������������� ����� 

operator [] :
7 - ������ ������� �� ������� �������

*/
class c_array
{
	int* A;
	int n, mem;

public:
	c_array();
	c_array(int* a, int count);
	c_array(const c_array& copy);

	~c_array() { delete A; };

	int findFirstKey(int key);

	void input();
	void output();

	void addElem(int index, int key);

	void swap(int a, int b);
	void sortingDesc();
	void sortingAsc();

	void shiftRight(int count, int index);
	void shiftRightSave(int count, int index);
	void shiftLeft(int count, int index);

	c_array& operator -(int key);
	c_array& operator -=(int index);

	void operator =(const c_array& newArray);

	void randArray(int count);
	void randArrayDesc(int count);
	void randArrayAsc(int count);

	int& operator [](int index);
	c_array& operator +(int key);
	c_array& operator +=(int key);

	c_array& operator +(const c_array& Mas);
	c_array& operator +=(const c_array& Mas);

	bool operator ==(const c_array Mas);
	bool operator !=(const c_array Mas);

	friend ostream& operator <<(ostream& r, c_array& Mas);
	friend istream& operator >>(istream& r, c_array& Mas);
};

//����������� �� ���������
c_array::c_array()
{
	A = new int[DEFAULT_MEM];
	mem = DEFAULT_MEM;
	n = 0;
}

//����������� � �����������
c_array::c_array(int* a, int count)
{
	if (count < 0) throw 0;

	mem = count + DOP_MEM;
	A = new int[mem];
	n = count;

	for (int i = 0; i < n; i++) A[i] = a[i];
}

//����������� �����������
c_array::c_array(const c_array& copy)
{
	mem = copy.mem;
	n = copy.n;
	A = new int[mem];
	for (int i = 0; i < n; i++) A[i] = copy.A[i];
}

//����� ������� ������� ��������� �����
int c_array::findFirstKey(int key)
{
	for (int i = 0; i < n; i++)
		if (key == A[i]) return i;

	return -1;
}

//����-����� (� �������)
void  c_array::input()
{
	cout << "Enter number of elements: ";
	cin >> n;
	while (n < 0)
	{
		cout << "Error. This number must >= 0. Enter here again: ";
		cin >> n;
	}

	if (n > mem)
	{
		delete A;
		mem = n + DOP_MEM;
		A = new int[mem];
	}

	cout << "\nEnter array(enter space between numbers): ";
	for (int i = 0; i < n; i++) cin >> A[i];

	return;
}

void  c_array::output()
{
	if (n > 0)
	{
		cout << "Array: ";
		for (int i = 0; i < n; i++) cout << A[i] << "  ";
		cout << "\n" << n << " slots out of " << mem << " occupied.\n\n";
	}
	else cout << "Array is empty. " << mem << " slots avilable.\n\n";
	return;
}

//���������� �������� �� �������� �������
void c_array::addElem(int index, int key)
{
	if (index < 0) throw 1;
	
	if (index < n)
	{
		A[index] = key;
	}
	else
		if (index < mem)
		{
			n = index;
			for (int i = n; i < index; i++) A[i] = EMPTY_ELEM;
			A[index] = key;
		}
		else
			if(index >= mem)
			{
				int* a = new int[index + DOP_MEM];
				for (int i = 0; i < n; i++) a[i] = A[i];
				for (int i = n; i < index; i++) a[i] = EMPTY_ELEM;
				a[index] = key;
				delete A;
				A = a;
				n = index + 1;
				mem = index + DOP_MEM + 1;
			}
	return;
}

//�������� ������� 2 �������� �������
void c_array::swap(int a, int b)
{
	if ((a < 0) || (a >= n)) throw 2;
	if ((b < 0) || (b >= n)) throw 3;

	int c = A[a];
	A[a] = A[b];
	A[b] = c;
	return;
}

//���������� (�� �������� � �����������)
void c_array::sortingDesc()
{
	for (int i = n - 1; i > 0; i--)
		for (int j = 0; j < i; j++)
			if (A[j] > A[j + 1]) swap(j, j + 1);
	return;
}

void c_array::sortingAsc()
{
	for (int i = 0; i < n; i++)
		for (int j = n - 1; j > i; j--)
			if (A[j] > A[j - 1]) swap(j, j - 1);
	return;
}

//����� (������(+ ������ ������ ������ ��� ������ ���������) � �����)
void c_array::shiftRight(int count, int index)
{
	if (count < 0) throw 4;
	if (count > n - index) count = n - index;

	for (int i = n - 1; i >= (index + count); i--) A[i] = A[i - count];

	for (int i = 0; i < count; i++) A[index + i] = EMPTY_ELEM;

	return;
}

void c_array::shiftRightSave(int count, int index)
{
	if (count < 0) throw 5;

	if (count + n > mem)
	{
		mem = n + count + DOP_MEM;
		n += count;
		int* newArr = new int[mem];
		int i = 0;

		for (; i < index; i++) newArr[i] = A[i];
		for (; i < index + count; i++) newArr[i] = EMPTY_ELEM;
		for (; i < n; i++) newArr[i] = A[i - count];

		delete A;
		A = newArr;
	}
	else
	{
		n += count;
		for (int i = n - 1; i >= (index + count); i--) A[i] = A[i - count];
		for (int i = 0; i < count; i++) A[index + i] = EMPTY_ELEM;
	}
	return;
}

void c_array::shiftLeft(int count, int index)
{
	if (count < 0) throw 6;
	if (count > index + 1) count = index + 1;

	for (int i = 0; i <= (index - count); i++) A[i] = A[i + count];

	for (int i = 0; i < count; i++) A[index - i] = EMPTY_ELEM;

	return;
}

//�������� ������� ��������� �����
c_array& c_array::operator -(int key)
{
	int index = this->findFirstKey(key);
	c_array* result = new c_array(A, n);

	if (index != -1)
	{
		for (int i = index; i < n - 1; i++) result->A[i] = result->A[i + 1];
		result->n--;
	}
	return *result;
}

//�������� �������� �� ������
c_array& c_array::operator -=(int index)
{
	c_array* result = new c_array(A, n);

	if (index < 0) throw 0;

	if (index < n)
	{
		for (int i = index; i < n - 1; i++) result->A[i] = result->A[i + 1];
		result->n--;
	}


	return *result;
}

//�������� ����������
void c_array::operator =(const c_array& newArray)
{
	delete A;

	mem = newArray.mem;
	n = newArray.n;
	A = new int[mem];
	for (int i = 0; i < n; i++) A[i] = newArray.A[i];
}

//���������� ������� ���������� ������� (��������� ���������, �� �����������, �� ��������)
void c_array::randArray(int count)
{
	if (count < 0) throw 0;

	if (count > mem)
	{
		delete A;
		mem = count + DOP_MEM;
		A = new int[mem];
	}

	n = count;
	for (int i = 0; i < n; i++) A[i] = rand() % (MAX_NUMBER + 1);
	return;
}

void c_array::randArrayDesc(int count)
{
	if (count < 0) throw 0;

	if (count > mem)
	{
		delete A;
		mem = count + DOP_MEM;
		A = new int[mem];
	}
	n = count;

	A[0] = rand() % ((MAX_NUMBER / count) * 2);
	for (int i = 1; i < n; i++) A[i] = A[i - 1] + (rand() % ((MAX_NUMBER / n) * 2));
	return;
}

void c_array::randArrayAsc(int count)
{
	if (count < 0) throw 0;

	if (count > mem)
	{
		delete A;
		mem = count + DOP_MEM;
		A = new int[mem];
	}
	n = count;

	A[n - 1] = rand() % ((MAX_NUMBER / count) * 2);
	for (int i = n - 2; i >= 0; i--) A[i] = A[i + 1] + (rand() % ((MAX_NUMBER / n) * 2));
	return;
}

// ������ ������ �� �������
int& c_array::operator [](int index)
{
	if ((index < 0) || (index >= n)) throw 7;
	return A[index];
}

// ���������� ����� � ������
c_array& c_array::operator +(int key)
{
	c_array* result = new c_array;
	*result = *this;
	result->shiftRightSave(1, 0);
	result->A[0] = key;
	return *result;
}

// ���������� ����� � �����
c_array& c_array::operator +=(int key)
{
	c_array* result;

	if (n + 1 > mem)
	{
		int* a = new int[n + 1 + DOP_MEM];

		for (int i = 0; i < n; i++) a[i] = A[i];
		a[n + 1] = key;

		result = new c_array;
		result->A = a;
		result->mem = n + 1 + DOP_MEM;
		result->n = n + 1;
	}
	else
	{
		result = new c_array(*this);
		result->A[n] = key;
		result->n = n + 1;
	}
	return *result;
}


// ���������� ������� � ������
c_array& c_array::operator +(const c_array& Mas)
{
	c_array* result = new c_array;
	*result = *this;
	result->shiftRightSave(Mas.n, 0);
	for (int i = 0; i < Mas.n; i++) result->A[i] = Mas.A[i];
	return *result;
}

// ���������� ������� � �����
c_array& c_array::operator +=(const c_array& Mas)
{
	c_array* result;

	if (n + Mas.n > mem)
	{
		int* a = new int[n + Mas.n + DOP_MEM];

		for (int i = 0; i < n; i++) a[i] = A[i];
		for (int i = 0; i < Mas.n; i++) a[n + i] = Mas.A[i];

		result = new c_array;
		result->A = a;
		result->mem = n + Mas.n + DOP_MEM;
		result->n = n + Mas.n;
	}
	else
	{
		result = new c_array(*this);
		for (int i = 0; i < Mas.n; i++) result->A[n + i] = Mas.A[i];
		result->n = n + Mas.n;
	}
	return *result;
}

//��������� ��������
bool c_array::operator ==(const c_array Mas)
{
	if (n = Mas.n)
	{
		for (int i = 0; i < n; i++)
			if (A[i] != Mas.A[i])
				return false;
		return true;
	}
	return false;
}

bool c_array::operator !=(const c_array Mas)
{
	if (n = Mas.n)
	{
		for (int i = 0; i < n; i++)
			if (A[i] != Mas.A[i])
				return true;
		return false;
	}
	return true;
}

//��������� ����-�����
ostream& operator <<(ostream& r, c_array& Mas)
{
	if (Mas.n > 0)
	{
		cout << "Array: ";
		for (int i = 0; i < Mas.n; i++) r << Mas.A[i] << "  ";
		r << "\n" << Mas.n << " slots out of " << Mas.mem << " occupied.\n\n";
	}
	else r << "Array is empty. " << Mas.mem << " slots avilable.\n\n";
	return r;
}

istream& operator >>(istream& r, c_array& Mas)
{
	cout << "Enter number of elements: ";
	r >> Mas.n;
	while (Mas.n < 0)
	{
		cout << "Error. This number must >= 0. Enter here again: ";
		r >> Mas.n;
	}

	if (Mas.n > Mas.mem)
	{
		delete Mas.A;
		Mas.mem = Mas.n + DOP_MEM;
		Mas.A = new int[Mas.mem];
	}

	cout << "Enter array(enter space between numbers): ";
	for (int i = 0; i < Mas.n; i++) r >> Mas.A[i];

	return r;
}

int main()
{
	srand(time(NULL));
	int a[3] = { 1, 4, 3 };
	c_array A(a, 4);
	

	A.randArray(20);

	//cin >> A;
	cout << A;


}