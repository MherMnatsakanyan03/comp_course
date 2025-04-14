#include <iostream>
#include <cassert>

int n_sum(int n)
{
    return n * (n + 1) / 2;
}

int sum_multiples_of_3_or_5(int n)
{
    int sum = 0;

    int amount_3 = (n - 1) / 3;
    int amount_5 = (n - 1) / 5;

    sum += 3 * n_sum(amount_3);
    sum += 5 * n_sum(amount_5);

    int amount_15 = (n - 1) / 15;

    sum -= 15 * n_sum(amount_15);

    return sum;
}

int main()
{
    {
        int n = 10;
        assert(sum_multiples_of_3_or_5(n) == 23);
    }
    {
        int n = 0;
        assert(sum_multiples_of_3_or_5(n) == 0);
    }
    {
        int n = 1;
        assert(sum_multiples_of_3_or_5(n) == 0);
    }
    {
        int n = 3;
        assert(sum_multiples_of_3_or_5(n) == 0);
    }
    {
        int n = 4;
        assert(sum_multiples_of_3_or_5(n) == 3);
    }
    {
        int n = 5;
        assert(sum_multiples_of_3_or_5(n) == 3);
    }
    {
        int n = 6;
        assert(sum_multiples_of_3_or_5(n) == 8);
    }
    {
        int n = 1000;
        assert(sum_multiples_of_3_or_5(n) == 233168);
    }

    std::cout << "All tests passed successfully!\n";
    return EXIT_SUCCESS;
}