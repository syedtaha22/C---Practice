import time

alphabets = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ "
# alphabets = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ?/.>,<;:@'#~[]{=}+-_1234567890!£$%^&*() "
# alphabets +='"'


def remove_character(input_string, index):
    if 0 <= index < len(input_string):
        new_string = input_string[:index] + input_string[index+1:]
        return new_string
    else:
        print("Index out of range.")
        return input_string


def iterator(message):
    result = ""
    count = -1
    for letter in message:
        for alphabet in alphabets:
            count += 1
            result += alphabet
            print(result)
            if alphabet != letter:
                result = remove_character(result, count)
                count -= 1
                time.sleep(0.1)
            else:
                time.sleep(0.1)
                break


message = input("Enter your Message: ")
iterator(message)