
a = int(input("Enter Denomintator: "))
try:
    result = 6/a
    print(result)
except ZeroDivisionError:
    print("Can't divide by zero")


