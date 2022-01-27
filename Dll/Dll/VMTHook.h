#pragma once

#include <Windows.h>
#include <memory>
#include <map>

class vmthook
{
public:
	vmthook() {}

	/// <summary>
	/// prepares the vmt hook.
	/// </summary>
	/// <param name="ppclass">assign the pointer to the vtable.</param>
	vmthook(void* ppclass)
	{
		this->base_pointer = (void**)ppclass;

		while (this->base_pointer[this->index_count] != nullptr)
			this->index_count++;

		this->vmt = std::make_unique<void* []>(this->index_count * 4);

		std::memcpy(vmt.get(), this->base_pointer, this->index_count * 4);

		DWORD old_protect;
		VirtualProtect(vmt.get(), this->index_count * 4, PAGE_READWRITE, &old_protect);
	}

	/// <summary>
	/// incase the constructor is not used the appy function is available to apply the hook.
	/// </summary>
	/// <param name="ppclass">assign the pointer to the vtable.</param>
	void apply(void** ppclass)
	{
		this->index_count = 0; this->base_pointer = nullptr;

		this->base_pointer = (void**)ppclass;

		while (this->base_pointer[this->index_count] != nullptr)
			this->index_count++;

		this->vmt = std::make_unique<void* []>(this->index_count * 4);

		std::memcpy(vmt.get(), this->base_pointer, this->index_count * 4);
	}

	/// <summary>
	/// hooks the vtable[index] and replaces it with the custom function.
	/// </summary>
	/// <param name="index">the index that will be replaces.</param>
	/// <param name="function">the function that will replace the vtable[index].</param>
	void hook(int index, void* function)
	{
		DWORD old_protect;
		VirtualProtect(&this->base_pointer[index], sizeof(void*), PAGE_EXECUTE_READWRITE, &old_protect);
		this->base_pointer[index] = function;
		VirtualProtect(&this->base_pointer[index], sizeof(void*), old_protect, &old_protect);
	}

	/// <summary>
	/// this function returns the original function.
	/// </summary>
	/// <param name="index">the index of the vtable that will be returned.</param>
	/// <returns>address.</returns>
	template <typename T>
	T get_original(int index)
	{
		return reinterpret_cast<T>(this->vmt.get()[index]);
	}

	/// <summary>
	/// returns total amount of functions found in the vtable.
	/// </summary>
	/// <returns>an number.</returns>
	int get_total_indexes()
	{
		return this->index_count;
	}

private:
	/// <summary>
	/// will be used to create an copy of the virtual functions.
	/// </summary>
	std::unique_ptr<void*[]> vmt = nullptr;
	/// <summary>
	/// will be used to point to the vtable.
	/// </summary>
	void** base_pointer = nullptr;
	/// <summary>
	/// will be used to count total amount of virtual functions found in the vtable.
	/// </summary>
	int index_count = 0;
};