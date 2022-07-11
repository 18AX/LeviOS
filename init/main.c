int main(void)
{
    asm volatile("xchgw %bx, %bx");

    for (;;)
    {
        asm volatile("pause");
    }

    return 0;
}