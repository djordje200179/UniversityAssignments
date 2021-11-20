def filter_words(words):
    filtered_words = []

    for word in words:
        nums = []
        temp_word = ""

        for char in word:
            if char.isdigit():
                temp_word += char
            else:
                if len(temp_word) > 0:
                    nums.append(temp_word)
                    temp_word = ""

        if len(temp_word) > 0:
            nums.append(temp_word)

        nums = [int(num) for num in nums]
        if len(nums) == 0:
            filtered_words.append(word)
        else:
            max_num = max(nums)

            filtered_words.append(word.replace(str(max_num), ''))

    return filtered_words

def is_legit(words):
    for word in words:
        if not word.isalnum():
            return False

    return True

def main():
    words = input().split(',')

    if is_legit(words):
        filtered = filter_words(words)
        print(*filtered, sep=' ', end='')

main()