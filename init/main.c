__attribute__((naked)) int main(void)
{
    asm volatile("xchgw %bx, %bx");

    return 0;
}