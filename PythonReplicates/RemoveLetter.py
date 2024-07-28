def remove_character(input_string, index):
    if 0 <= index < len(input_string):
        new_string = input_string[:index] + input_string[index+1:]
        return new_string
    else:
        print("Index out of range.")
        return input_string


original_string = "Hello, World!"
index_to_remove = 1
result = remove_character(original_string, index_to_remove)
print(result)
