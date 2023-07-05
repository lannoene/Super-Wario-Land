#include <iostream>
#include <cstdlib>

template <typename type> class SquishyArray {
public:
	SquishyArray(int inputArrayLength) {
		array_ptr = (type*)malloc(inputArrayLength * sizeof(type));
		array_length = inputArrayLength;
	}
	~SquishyArray(void) {
		free(array_ptr);
	}
	int length(void) {
		return array_length;
	}
	void array_push(type value) {
		//array_capacity = array_capacity == 0 ? array_capacity << 1 : 1;
		change_array_size(1);
		array_ptr[array_length - 1] = value;
	}
	type* data(void) {
		return array_ptr;
	}
	void array_front(type value) {
		change_array_size(1);
		for (type i = array_length - 1; i > 0; i--) {
			array_ptr[i] = array_ptr[i - 1];
		}
		array_ptr[0] = value;
	}
	void array_splice(int index, int elements) {
		if (index > array_length || index + elements > array_length || index < 0 || index + elements < 0) {
			return;
		}
		for (int i = index; i < array_length; i++) {
			array_ptr[i] = array_ptr[i + elements];
		}
		change_array_size(-elements);
	}
private:
	type* array_ptr;
	size_t array_length;
	size_t array_capacity;
	
	void change_array_size(int newSize) {
		array_length += newSize;
		array_ptr = (type*)realloc(array_ptr, array_length * sizeof(type));
	}
};