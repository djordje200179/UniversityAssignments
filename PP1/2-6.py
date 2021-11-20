k = int(input())

raw_input = input()
nums = raw_input.split()
is_valid = True
for i in range(len(nums)):
    nums[i] = int(nums[i])
    
    if nums[i] <= 0:
        is_valid = False
        break

if is_valid:
    i = 0
    while i < len(nums):
        sum = 0

        for j in range(1, nums[i] + 1):
            sum += j * (nums[i] % j == 0)

        if nums[i] == 1 + k * (sum - nums[i] - 1):
            i += 1
        else:
            nums.pop(i)

    print(nums) 