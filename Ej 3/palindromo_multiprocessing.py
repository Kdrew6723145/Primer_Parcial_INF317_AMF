import multiprocessing

def is_palindrome(word):
    return word == word[::-1]

def check_palindrome(word):
    pool = multiprocessing.Pool()
    n = len(word)
    sub_words = [(word, i, j) for i in range(n) for j in range(i+1, n+1)]
    results = pool.starmap(is_palindrome, sub_words)
    pool.close()
    return any(results)

word = "racecar"
print(check_palindrome(word))