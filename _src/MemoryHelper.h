#pragma once

namespace MEMORY
{
	template <class T> void SafeDelete(T*& t)
	{
		if(t)
			delete t;
		t = nullptr;
	}
	template <class T> void SafeDeleteArr(T*& t)
	{
		if(t)
			delete[] t;
		t = nullptr;
	}

	namespace LIST
	{
		//Adds 'newT' to the end of the list
		template <class T> void AddToList(T*& t, T* newT, int& nt)
		{
			if(!newT)
				return;

			T* temp = new T[(nt) + 1];
			for(int i = 0; i < nt; i++)
			{
				temp[i] = t[i];
			}

			temp[nt] = *newT;

			SafeDeleteArr(t);
			nt++;

			t = new T[nt];

			for(int i = 0; i < nt; i++)
			{
				t[i] = temp[i];
			}

			SafeDeleteArr(temp);
		}
		//Adds 'newT' to the end of the list
		template <class T> void AddToListArr(T**& t, T* newT, int& nt)
		{
			if(!newT)
				return;

			T** temp = new T*[nt + 1];
			for(int i = 0; i < nt; i++)
			{
				temp[i] = t[i];
				t[i] = nullptr;
			}

			temp[nt] = newT;

			SafeDeleteArr(t);

			nt++;

			t = new T*[nt];

			for(int i = 0; i <  nt; i++)
			{
				t[i] = temp[i];
				temp[i] = nullptr;
			}

			SafeDeleteArr(temp);
		}
		//removes the item in the list and the index 'remove_i'
		template<class T> void RemoveItem(T**& t, int& nt, int remove_i)
		{
			T** temp = new T*[nt - 1];

			for (int i = 0; i < nt; i++)
			{
				if (i < remove_i)
					temp[i] = t[i];
				else
					temp[i - 1] = t[i];
			}

			nt--;

			MEMORY::SafeDeleteArr(t);
		}
		
		template<class T> void RemoveItem(T*& t, int& nt, int remove_i)
		{
			T* temp = new T[nt - 1];

			for (int i = 0; i < nt; i++)
			{
				if (i < remove_i)
					temp[i] = t[i];
				else
					temp[i - 1] = t[i];
			}

			nt--;

			MEMORY::SafeDeleteArr(t);
		}
	}

	namespace DX
	{
		template <class T> void SafeRelease(T*& t)
		{
			if(t)
				t->Release();
			t = nullptr;
		}
	}
}